/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include "fe.h"

using namespace OPT_LEVEL;

namespace
{

template< class TS, class TD >
SYS_INLINE STATIC void CompareAssign_lt( const TS& s1, const TS& s2, TD& d ) {
    d = (d && s1 <  s2) ? FW_MAX_8U: 0;
}
template< class TS, class TD >
SYS_INLINE STATIC void CompareAssign_gt( const TS& s1, const TS& s2, TD& d ) {
    d = (d && s1 >  s2) ? FW_MAX_8U: 0;
}
template< class TS, class TD >
SYS_INLINE STATIC void CompareAssign_le( const TS& s1, const TS& s2, TD& d ) {
    d = (d && s1 <= s2) ? FW_MAX_8U: 0;
}
template< class TS, class TD >
SYS_INLINE STATIC void CompareAssign_eq( const TS& s1, const TS& s2, TD& d ) {
    d = (d && s1 == s2) ? FW_MAX_8U: 0;
}
template< class TS, class TD >
SYS_INLINE STATIC void CompareAssign_ge( const TS& s1, const TS& s2, TD& d ) {
    d = (d && s1 >= s2) ? FW_MAX_8U: 0;
}

template< class T >
SYS_INLINE STATIC T Absolute(const T& t)
{
    if (t < 0) return (-t);
    else return t;
}

template< class TS, class TD >
SYS_INLINE STATIC void CompareEqualEpsAssign( const TS& s1, const TS& s2, TD& d, const TS& eps)
{
    d = (d && Absolute(s1-s2) <= eps) ? FW_MAX_8U: 0;
}

};


namespace
{


SYS_INLINE static void CmpAssign_sse2_8u_c1_lt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = CBL_SSE2::compare_lt_8u(src1.i, src2.i);
}
SYS_INLINE static void CmpAssign_sse2_8u_c1_gt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = CBL_SSE2::compare_gt_8u(src1.i, src2.i);
}
SYS_INLINE static void CmpAssign_sse2_8u_c1_le(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = CBL_SSE2::compare_le_8u(src1.i, src2.i);
}
SYS_INLINE static void CmpAssign_sse2_8u_c1_eq(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi8(src1.i, src2.i);
}
SYS_INLINE static void CmpAssign_sse2_8u_c1_ge(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = CBL_SSE2::compare_ge_8u(src1.i, src2.i);
}

SYS_INLINE static void CmpAssign_sse2_16s_c1_lt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmplt_epi16(src1.i, src2.i);
    //cmplt returns 16 bit values as result, pack them to 8 bit values
    dst.i = _mm_packs_epi16(dst.i, dst.i);
}
SYS_INLINE static void CmpAssign_sse2_16s_c1_gt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpgt_epi16(src1.i, src2.i);
    dst.i = _mm_packs_epi16(dst.i, dst.i);
}
SYS_INLINE static void CmpAssign_sse2_16s_c1_le(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(_mm_max_epi16(src1.i, src2.i), src2.i);
    dst.i = _mm_packs_epi16(dst.i, dst.i);
}
SYS_INLINE static void CmpAssign_sse2_16s_c1_eq(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(src1.i, src2.i);
    dst.i = _mm_packs_epi16(dst.i, dst.i);
}
SYS_INLINE static void CmpAssign_sse2_16s_c1_ge(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(_mm_min_epi16(src1.i, src2.i), src2.i);
    dst.i = _mm_packs_epi16(dst.i, dst.i);
}

SYS_INLINE static void CmpAssign_sse2_32f_c1_lt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmplt_ps(src1.f, src2.f);
    //result is 8 bit data so the dst is compressed from 32 bits to 8 bits 
    dst.i = _mm_packs_epi16(_mm_packs_epi32(dst.i, dst.i), _mm_packs_epi32(dst.i, dst.i));
}
SYS_INLINE static void CmpAssign_sse2_32f_c1_gt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpgt_ps(src1.f, src2.f);
    dst.i = _mm_packs_epi16(_mm_packs_epi32(dst.i, dst.i), _mm_packs_epi32(dst.i, dst.i));
}
SYS_INLINE static void CmpAssign_sse2_32f_c1_le(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpeq_ps(_mm_max_ps(src1.f, src2.f), src2.f);
    dst.i = _mm_packs_epi16(_mm_packs_epi32(dst.i, dst.i), _mm_packs_epi32(dst.i, dst.i));
}
SYS_INLINE static void CmpAssign_sse2_32f_c1_eq(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpeq_ps(src1.f, src2.f);
    dst.i = _mm_packs_epi16(_mm_packs_epi32(dst.i, dst.i), _mm_packs_epi32(dst.i, dst.i));
}
SYS_INLINE static void CmpAssign_sse2_32f_c1_ge(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpeq_ps(_mm_min_ps(src1.f, src2.f), src2.f);
    dst.i = _mm_packs_epi16(_mm_packs_epi32(dst.i, dst.i), _mm_packs_epi32(dst.i, dst.i));
}

//Only 96 bits of the source is loaded in src1 to accomodate complete pixels for all C3 routines
//For C3, loading 128bits will result in partial pixel being loaded at the msb of the 128 bit reg
SYS_INLINE static void CmpAssign_sse2_8u_c3_lt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    XMM128 max, eq, imResult, dst1, dst2;
    //mask containing first channel value high and other channel vlaue set to zero
    const __m128i mask = _mm_setr_epi32(0xFF0000FF, 0x00FF0000, 0x0000FF00, 0x00000000);

    //src1 less than src2 -> (src2 > src1 && src 2 !=src1)
    max.i = _mm_max_epu8(src1.i, src2.i);
    eq.i = _mm_cmpeq_epi8(src1.i, src2.i);
    imResult.i = _mm_cmpeq_epi8(max.i, src2.i);
    //clear values where src2 is in max and equal to src1
    //resulting in src1 less than src2 is high per element basis
    imResult.i = _mm_xor_si128(imResult.i, eq.i);

    //move 2nd channel and 3rd channel values to the lower position in dst1 and dst2
    dst1.i = _mm_srli_si128(imResult.i, 1);
    dst2.i = _mm_srli_si128(imResult.i, 2);
    //perform "and" operation on 1st, 2nd and 3rd channel result for each pixel
    //The high values resulting are the ones where all three channel data in src1 is less than src2
    imResult.i = _mm_and_si128(imResult.i, dst1.i);
    dst.i = _mm_and_si128(imResult.i, dst2.i);

    //Clear the data in dst except for the first channel and arrange the first channel
    //result in order resulting in one 8 bit value per pixel
    dst.i = _mm_and_si128(dst.i, mask);
    dst1.i = _mm_shufflelo_epi16(dst.i,_MM_SHUFFLE(2,2,3,0));
    dst.i = _mm_srli_si128(dst.i, 2);
    dst.i = _mm_shufflelo_epi16(dst.i,_MM_SHUFFLE(1,1,3,0));
    dst.i = _mm_or_si128(dst.i, dst1.i);
}
SYS_INLINE static void CmpAssign_sse2_8u_c3_gt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    XMM128 imResult, dst1, dst2;
    const __m128i mask = _mm_setr_epi32(0xFF0000FF, 0x00FF0000, 0x0000FF00, 0x00000000);

    //src1 greater than src2 -> (src2 < src1 && src 2 !=src1)
    imResult.i = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_min_epu8(src1.i, src2.i), src2.i),
                            _mm_cmpeq_epi8(src1.i, src2.i) );


    dst1.i = _mm_srli_si128(imResult.i, 1);
    dst2.i = _mm_srli_si128(imResult.i, 2);
    imResult.i = _mm_and_si128(imResult.i, dst1.i);
    dst.i = _mm_and_si128(imResult.i, dst2.i);


    dst.i = _mm_and_si128(dst.i, mask);
    dst1.i = _mm_shufflelo_epi16(dst.i,_MM_SHUFFLE(2,2,3,0));
    dst.i = _mm_srli_si128(dst.i, 2);
    dst.i = _mm_shufflelo_epi16(dst.i,_MM_SHUFFLE(1,1,3,0));
    dst.i = _mm_or_si128(dst.i, dst1.i);
}
SYS_INLINE static void CmpAssign_sse2_8u_c3_le(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    XMM128 imResult, dst1, dst2;
    const __m128i mask = _mm_setr_epi32(0xFF0000FF, 0x00FF0000, 0x0000FF00, 0x00000000);
    //src1 less than equal src2 -> src2 >= src1
    imResult.i = _mm_cmpeq_epi8(_mm_max_epu8(src1.i, src2.i), src2.i);

    dst1.i = _mm_srli_si128(imResult.i, 1);
    dst2.i = _mm_srli_si128(imResult.i, 2);
    imResult.i = _mm_and_si128(imResult.i, dst1.i);
    dst.i = _mm_and_si128(imResult.i, dst2.i);


    dst.i = _mm_and_si128(dst.i, mask);
    dst1.i = _mm_shufflelo_epi16(dst.i,_MM_SHUFFLE(2,2,3,0));
    dst.i = _mm_srli_si128(dst.i, 2);
    dst.i = _mm_shufflelo_epi16(dst.i,_MM_SHUFFLE(1,1,3,0));
    dst.i = _mm_or_si128(dst.i, dst1.i);
}

SYS_INLINE static void CmpAssign_sse2_8u_c3_eq(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    XMM128 imResult, dst1, dst2;
    const __m128i mask = _mm_setr_epi32(0xFF0000FF, 0x00FF0000, 0x0000FF00, 0x00000000);

    imResult.i = _mm_cmpeq_epi8(src1.i, src2.i);

    dst1.i = _mm_srli_si128(imResult.i, 1);
    dst2.i = _mm_srli_si128(imResult.i, 2);
    imResult.i = _mm_and_si128(imResult.i, dst1.i);
    dst.i = _mm_and_si128(imResult.i, dst2.i);

    dst.i = _mm_and_si128(dst.i, mask);
    dst1.i = _mm_shufflelo_epi16(dst.i,_MM_SHUFFLE(2,2,3,0));
    dst.i = _mm_srli_si128(dst.i, 2);
    dst.i = _mm_shufflelo_epi16(dst.i,_MM_SHUFFLE(1,1,3,0));
    dst.i = _mm_or_si128(dst.i, dst1.i);
}

SYS_INLINE static void CmpAssign_sse2_8u_c3_ge(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    XMM128 imResult, dst1, dst2;
    const __m128i mask = _mm_setr_epi32(0xFF0000FF, 0x00FF0000, 0x0000FF00, 0x00000000);

    imResult.i = _mm_cmpeq_epi8(_mm_min_epu8(src1.i, src2.i), src2.i);


    dst1.i = _mm_srli_si128(imResult.i, 1);
    dst2.i = _mm_srli_si128(imResult.i, 2);
    imResult.i = _mm_and_si128(imResult.i, dst1.i);
    dst.i = _mm_and_si128(imResult.i, dst2.i);


    dst.i = _mm_and_si128(dst.i, mask);
    dst1.i = _mm_shufflelo_epi16(dst.i,_MM_SHUFFLE(2,2,3,0));
    dst.i = _mm_srli_si128(dst.i, 2);
    dst.i = _mm_shufflelo_epi16(dst.i,_MM_SHUFFLE(1,1,3,0));
    dst.i = _mm_or_si128(dst.i, dst1.i);
}

SYS_INLINE static void CmpAssign_sse2_16s_c3_lt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmplt_epi16(src1.i, src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    //63 = 111111b, if lower 6 bits(3 channels * 2 bytes(sizeof 16s)) of movemask are high
    //then first pixel result set to FW_MAX_8U else set to 0
    int ele1 = ((movemask & (63))==63)?FW_MAX_8U:0;
    //if 7-12 bits of movemask are high
    //then second pixel result set to (FW_MAX_8U<<8) else set to 0
    int ele2 = (((movemask & (63<<6))==(63<<6))?(FW_MAX_8U<<8):0);
    //"OR" result of 1st and 2nd pixel results
    ele1 = ele1 | ele2;
    //inserting the result into xmm register
    //this gives better performance since avoids round trip to memory
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_c3_gt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpgt_epi16(src1.i, src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (63))==63)?FW_MAX_8U:0;
    int ele2 = (((movemask & (63<<6))==(63<<6))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_c3_le(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(_mm_max_epi16(src1.i, src2.i), src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (63))==63)?FW_MAX_8U:0;
    int ele2 = (((movemask & (63<<6))==(63<<6))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_c3_eq(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(src1.i, src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (63))==63)?FW_MAX_8U:0;
    int ele2 = (((movemask & (63<<6))==(63<<6))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_c3_ge(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(_mm_min_epi16(src1.i, src2.i), src2.i);

    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (63))==63)?FW_MAX_8U:0;
    int ele2 = (((movemask & (63<<6))==(63<<6))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}

SYS_INLINE static void CmpAssign_sse2_32f_c3_lt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmplt_ps(src1.f, src2.f);
    XMM128 dst1, dst2;
    dst1.i = _mm_srli_si128(dst.i, 4);
    dst2.i = _mm_srli_si128(dst.i, 8);
    dst.i = _mm_and_si128(dst.i, dst1.i);
    dst.i = _mm_and_si128(dst.i, dst2.i);

}
SYS_INLINE static void CmpAssign_sse2_32f_c3_gt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpgt_ps(src1.f, src2.f);
    XMM128 dst1, dst2;
    dst1.i = _mm_srli_si128(dst.i, 4);
    dst2.i = _mm_srli_si128(dst.i, 8);
    dst.i = _mm_and_si128(dst.i, dst1.i);
    dst.i = _mm_and_si128(dst.i, dst2.i);
}
SYS_INLINE static void CmpAssign_sse2_32f_c3_le(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpeq_ps(_mm_max_ps(src1.f, src2.f), src2.f);
    XMM128 dst1, dst2;
    dst1.i = _mm_srli_si128(dst.i, 4);
    dst2.i = _mm_srli_si128(dst.i, 8);
    dst.i = _mm_and_si128(dst.i, dst1.i);
    dst.i = _mm_and_si128(dst.i, dst2.i);
}
SYS_INLINE static void CmpAssign_sse2_32f_c3_eq(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpeq_ps(src1.f, src2.f);
    XMM128 dst1, dst2;
    dst1.i = _mm_srli_si128(dst.i, 4);
    dst2.i = _mm_srli_si128(dst.i, 8);
    dst.i = _mm_and_si128(dst.i, dst1.i);
    dst.i = _mm_and_si128(dst.i, dst2.i);
}
SYS_INLINE static void CmpAssign_sse2_32f_c3_ge(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpeq_ps(_mm_min_ps(src1.f, src2.f), src2.f);
    XMM128 dst1, dst2;
    dst1.i = _mm_srli_si128(dst.i, 4);
    dst2.i = _mm_srli_si128(dst.i, 8);
    dst.i = _mm_and_si128(dst.i, dst1.i);
    dst.i = _mm_and_si128(dst.i, dst2.i);
}

SYS_INLINE static void CmpAssign_sse2_8u_c4_lt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    static const __m128i zero = CONST_SETZERO_8I();

    //unsigned operation, (src2 - src1 = 0) -> src1 >= src2
    dst.i = _mm_subs_epu8(src2.i, src1.i);
    dst.i = _mm_cmpeq_epi8(dst.i, zero);
    //not(src1 > = src2) -> src1 < src2
    //If lower 4 bits are set(src1 > = src2), then set 0 else set FW_MAX_8U
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (15)))?0:FW_MAX_8U;
    //Result of second pixel
    int ele2 = (((movemask & (15<<4)))?0:(FW_MAX_8U<<8));
    //Combine result of first 2 pixels
    //insert into location 0 of the destination
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
    //Result of third pixel
    int ele3 = ((movemask & (15<<8)))?0:FW_MAX_8U;
    //Result of fourth pixel
    int ele4 = (((movemask & (15<<12)))?0:(FW_MAX_8U<<8));
    //Combine result of last 2 pixels
    //insert into location 1 of the destination
    ele3 = ele3 | ele4;
    dst.i = _mm_insert_epi16 (dst.i, ele3, 1);

}
SYS_INLINE static void CmpAssign_sse2_8u_c4_gt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    static const __m128i zero = CONST_SETZERO_8I();

    //unsigned operation, (src1 - src2 = 0) -> src2 >= src1
    dst.i = _mm_subs_epu8(src1.i, src2.i);
    dst.i = _mm_cmpeq_epi8(dst.i, zero);

    //not(src2 > = src1) -> src1 > src2
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (15)))?0:FW_MAX_8U;
    int ele2 = (((movemask & (15<<4)))?0:(FW_MAX_8U<<8));
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);

    int ele3 = ((movemask & (15<<8)))?0:FW_MAX_8U;
    int ele4 = (((movemask & (15<<12)))?0:(FW_MAX_8U<<8));
    ele3 = ele3 | ele4;
    dst.i = _mm_insert_epi16 (dst.i, ele3, 1);

}
SYS_INLINE static void CmpAssign_sse2_8u_c4_le(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi8(_mm_max_epu8(src1.i, src2.i), src2.i);

    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (15))==15)?FW_MAX_8U:0;
    int ele2 = (((movemask & (15<<4))==(15<<4))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);

    int ele3 = ((movemask & (15<<8))==(15<<8))?FW_MAX_8U:0;
    int ele4 = (((movemask & (15<<12))==(15<<12))?(FW_MAX_8U<<8):0);
    ele3 = ele3 | ele4;
    dst.i = _mm_insert_epi16 (dst.i, ele3, 1);
}
SYS_INLINE static void CmpAssign_sse2_8u_c4_eq(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi8(src1.i, src2.i);

    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (15))==15)?FW_MAX_8U:0;
    int ele2 = (((movemask & (15<<4))==(15<<4))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);

    int ele3 = ((movemask & (15<<8))==(15<<8))?FW_MAX_8U:0;
    int ele4 = (((movemask & (15<<12))==(15<<12))?(FW_MAX_8U<<8):0);
    ele3 = ele3 | ele4;
    dst.i = _mm_insert_epi16 (dst.i, ele3, 1);
}
SYS_INLINE static void CmpAssign_sse2_8u_c4_ge(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi8(_mm_min_epu8(src1.i, src2.i), src2.i);

    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (15))==15)?FW_MAX_8U:0;
    int ele2 = (((movemask & (15<<4))==(15<<4))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);

    int ele3 = ((movemask & (15<<8))==(15<<8))?FW_MAX_8U:0;
    int ele4 = (((movemask & (15<<12))==(15<<12))?(FW_MAX_8U<<8):0);
    ele3 = ele3 | ele4;
    dst.i = _mm_insert_epi16 (dst.i, ele3, 1);
}

SYS_INLINE static void CmpAssign_sse2_16s_c4_lt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmplt_epi16(src1.i, src2.i);

    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (255))==255)?FW_MAX_8U:0;
    int ele2 = (((movemask & (255<<8))==(255<<8))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_c4_gt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpgt_epi16(src1.i, src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (255))==255)?FW_MAX_8U:0;
    int ele2 = (((movemask & (255<<8))==(255<<8))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_c4_le(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(_mm_max_epi16(src1.i, src2.i), src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (255))==255)?FW_MAX_8U:0;
    int ele2 = (((movemask & (255<<8))==(255<<8))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_c4_eq(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(src1.i, src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (255))==255)?FW_MAX_8U:0;
    int ele2 = (((movemask & (255<<8))==(255<<8))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_c4_ge(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(_mm_min_epi16(src1.i, src2.i), src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (255))==255)?FW_MAX_8U:0;
    int ele2 = (((movemask & (255<<8))==(255<<8))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}

SYS_INLINE static void CmpAssign_sse2_32f_c4_lt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmplt_ps(src1.f, src2.f);
    XMM128 dst1, dst2, dst3;
    dst1.i = _mm_srli_si128(dst.i, 4);
    dst2.i = _mm_srli_si128(dst.i, 8);
    dst3.i = _mm_srli_si128(dst.i, 12);
    dst.i = _mm_and_si128(dst.i, dst1.i);
    dst.i = _mm_and_si128(dst.i, dst2.i);
    dst.i = _mm_and_si128(dst.i, dst3.i);
}
SYS_INLINE static void CmpAssign_sse2_32f_c4_gt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpgt_ps(src1.f, src2.f);
    XMM128 dst1, dst2, dst3;
    dst1.i = _mm_srli_si128(dst.i, 4);
    dst2.i = _mm_srli_si128(dst.i, 8);
    dst3.i = _mm_srli_si128(dst.i, 12);
    dst.i = _mm_and_si128(dst.i, dst1.i);
    dst.i = _mm_and_si128(dst.i, dst2.i);
    dst.i = _mm_and_si128(dst.i, dst3.i);
}
SYS_INLINE static void CmpAssign_sse2_32f_c4_le(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpeq_ps(_mm_max_ps(src1.f, src2.f), src2.f);
    XMM128 dst1, dst2, dst3;
    dst1.i = _mm_srli_si128(dst.i, 4);
    dst2.i = _mm_srli_si128(dst.i, 8);
    dst3.i = _mm_srli_si128(dst.i, 12);
    dst.i = _mm_and_si128(dst.i, dst1.i);
    dst.i = _mm_and_si128(dst.i, dst2.i);
    dst.i = _mm_and_si128(dst.i, dst3.i);
}
SYS_INLINE static void CmpAssign_sse2_32f_c4_eq(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpeq_ps(src1.f, src2.f);
    XMM128 dst1, dst2, dst3;
    dst1.i = _mm_srli_si128(dst.i, 4);
    dst2.i = _mm_srli_si128(dst.i, 8);
    dst3.i = _mm_srli_si128(dst.i, 12);
    dst.i = _mm_and_si128(dst.i, dst1.i);
    dst.i = _mm_and_si128(dst.i, dst2.i);
    dst.i = _mm_and_si128(dst.i, dst3.i);
}
SYS_INLINE static void CmpAssign_sse2_32f_c4_ge(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.f = _mm_cmpeq_ps(_mm_min_ps(src1.f, src2.f), src2.f);
    XMM128 dst1, dst2, dst3;
    dst1.i = _mm_srli_si128(dst.i, 4);
    dst2.i = _mm_srli_si128(dst.i, 8);
    dst3.i = _mm_srli_si128(dst.i, 12);
    dst.i = _mm_and_si128(dst.i, dst1.i);
    dst.i = _mm_and_si128(dst.i, dst2.i);
    dst.i = _mm_and_si128(dst.i, dst3.i);
}

SYS_INLINE static void CmpAssign_sse2_8u_ac4_lt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    XMM128 imResult, dst1, dst2;
    const __m128i mask = _mm_setr_epi32(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
    const __m128i zero = _mm_setzero_si128();
    
    imResult.i = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_max_epu8(src1.i, src2.i), src2.i),
                            _mm_cmpeq_epi8(src1.i, src2.i) );

    dst1.i = _mm_srli_si128(imResult.i, 1);
    dst2.i = _mm_srli_si128(imResult.i, 2);
    imResult.i = _mm_and_si128(imResult.i, dst1.i);
    dst.i = _mm_and_si128(imResult.i, dst2.i);


    dst.i = _mm_and_si128(dst.i, mask);
    dst.i = _mm_packs_epi32(dst.i, zero);
    dst.i = _mm_packus_epi16(dst.i, zero);

}

SYS_INLINE static void CmpAssign_sse2_8u_ac4_gt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    XMM128 imResult, dst1, dst2;
    const __m128i mask = _mm_setr_epi32(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
    const __m128i zero = _mm_setzero_si128();
    
    imResult.i = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_min_epu8(src1.i, src2.i), src2.i),
                            _mm_cmpeq_epi8(src1.i, src2.i) );

    dst1.i = _mm_srli_si128(imResult.i, 1);
    dst2.i = _mm_srli_si128(imResult.i, 2);
    imResult.i = _mm_and_si128(imResult.i, dst1.i);
    dst.i = _mm_and_si128(imResult.i, dst2.i);


    dst.i = _mm_and_si128(dst.i, mask);
    dst.i = _mm_packs_epi32(dst.i, zero);
    dst.i = _mm_packus_epi16(dst.i, zero);
}

SYS_INLINE static void CmpAssign_sse2_8u_ac4_le(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    XMM128 imResult, dst1, dst2;
    const __m128i mask = _mm_setr_epi32(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
    const __m128i zero = _mm_setzero_si128();
    //src1 less than equal src2 -> src2 >= src1
    imResult.i = _mm_cmpeq_epi8(_mm_max_epu8(src1.i, src2.i), src2.i);

    dst1.i = _mm_srli_si128(imResult.i, 1);
    dst2.i = _mm_srli_si128(imResult.i, 2);
    imResult.i = _mm_and_si128(imResult.i, dst1.i);
    dst.i = _mm_and_si128(imResult.i, dst2.i);


    dst.i = _mm_and_si128(dst.i, mask);
    dst.i = _mm_packs_epi32(dst.i, zero);
    dst.i = _mm_packus_epi16(dst.i, zero);
}
SYS_INLINE static void CmpAssign_sse2_8u_ac4_eq(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    XMM128 imResult, dst1, dst2;
    const __m128i mask = _mm_setr_epi32(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
    const __m128i zero = _mm_setzero_si128();
    
    imResult.i = _mm_cmpeq_epi8(src1.i, src2.i);

    dst1.i = _mm_srli_si128(imResult.i, 1);
    dst2.i = _mm_srli_si128(imResult.i, 2);
    imResult.i = _mm_and_si128(imResult.i, dst1.i);
    dst.i = _mm_and_si128(imResult.i, dst2.i);


    dst.i = _mm_and_si128(dst.i, mask);
    dst.i = _mm_packs_epi32(dst.i, zero);
    dst.i = _mm_packus_epi16(dst.i, zero);
}
SYS_INLINE static void CmpAssign_sse2_8u_ac4_ge(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
     XMM128 imResult, dst1, dst2;
    const __m128i mask = _mm_setr_epi32(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
    const __m128i zero = _mm_setzero_si128();
    imResult.i = _mm_cmpeq_epi8(_mm_min_epu8(src1.i, src2.i), src2.i);

    dst1.i = _mm_srli_si128(imResult.i, 1);
    dst2.i = _mm_srli_si128(imResult.i, 2);
    imResult.i = _mm_and_si128(imResult.i, dst1.i);
    dst.i = _mm_and_si128(imResult.i, dst2.i);


    dst.i = _mm_and_si128(dst.i, mask);
    dst.i = _mm_packs_epi32(dst.i, zero);
    dst.i = _mm_packus_epi16(dst.i, zero);
}

SYS_INLINE static void CmpAssign_sse2_16s_ac4_lt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmplt_epi16(src1.i, src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (63))==63)?FW_MAX_8U:0;
    int ele2 = (((movemask & (63<<8))==(63<<8))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_ac4_gt(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpgt_epi16(src1.i, src2.i);

    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (63))==63)?FW_MAX_8U:0;
    int ele2 = (((movemask & (63<<8))==(63<<8))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_ac4_le(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(_mm_max_epi16(src1.i, src2.i), src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (63))==63)?FW_MAX_8U:0;
    int ele2 = (((movemask & (63<<8))==(63<<8))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_ac4_eq(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(src1.i, src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (63))==63)?FW_MAX_8U:0;
    int ele2 = (((movemask & (63<<8))==(63<<8))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}
SYS_INLINE static void CmpAssign_sse2_16s_ac4_ge(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
{
    dst.i = _mm_cmpeq_epi16(_mm_min_epi16(src1.i, src2.i), src2.i);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (63))==63)?FW_MAX_8U:0;
    int ele2 = (((movemask & (63<<8))==(63<<8))?(FW_MAX_8U<<8):0);
    ele1 = ele1 | ele2;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}


SYS_INLINE static void CmpEqEpsAssign_sse2_32f_c1(const XMM128 &src1, const XMM128 &src2, const XMM128 &eps, XMM128 &dst)
{
    //max(src1-src2, src2-src1) = Absolute(src1-src2)
    dst.f = _mm_max_ps(_mm_sub_ps(src2.f, src1.f), _mm_sub_ps(src1.f, src2.f));
    //Absolute(src1-src2) <= eps then result is FW_MAX_8U else result is 0
    //result of _mm_cmpeq_ps is 32 bits wide so pack to convert into 8 bits result
    dst.f = _mm_cmpeq_ps(_mm_max_ps(dst.f, eps.f), eps.f);
    dst.i = _mm_packs_epi32(dst.i, dst.i);
    dst.i = _mm_packs_epi16(dst.i, dst.i);
}


SYS_INLINE static void CmpEqEpsAssign_sse2_32f_c3(const XMM128 &src1, const XMM128 &src2, const XMM128 &eps, XMM128 &dst)
{
    const __m128i mask = _mm_set1_epi32(0x7FFFFFFF);
    dst.f = _mm_sub_ps(src2.f, src1.f);
    dst.i = _mm_and_si128(dst.i, mask);
    dst.f = _mm_cmpeq_ps(_mm_max_ps(dst.f, eps.f), eps.f);
    dst.i = _mm_and_si128(dst.i, _mm_srli_si128(dst.i,4));
    dst.i = _mm_and_si128(dst.i, _mm_srli_si128(dst.i,8));
}


SYS_INLINE static void CmpEqEpsAssign_sse2_32f_c4(const XMM128 &src1, const XMM128 &src2, const XMM128 &eps, XMM128 &dst)
{
    dst.f = _mm_max_ps(_mm_sub_ps(src2.f, src1.f), _mm_sub_ps(src1.f, src2.f));
    dst.f = _mm_cmpeq_ps(_mm_max_ps(dst.f, eps.f), eps.f);
    int movemask = _mm_movemask_epi8 (dst.i);
    int ele1 = ((movemask & (0xFFFF))==0xFFFF)?FW_MAX_8U:0;
    dst.i = _mm_insert_epi16 (dst.i, ele1, 0);
}


template< class TS, class TD, CH cd >
SYS_INLINE STATIC void Compare_lt( const TS * s1, const TS * s2, TD * d)
{

    switch (cd)
    {
    case C4:
        d[0] = ((s1[0] < s2[0])&&(s1[1] < s2[1])&&(s1[2] < s2[2])&&(s1[3] < s2[3]))? FW_MAX_8U : 0;
        break;
    case C3:
    case AC4:
        d[0] = ((s1[0] < s2[0])&&(s1[1] < s2[1])&&(s1[2] < s2[2]))? FW_MAX_8U : 0;
        break;

    case C1:
        d[0] = (s1[0] < s2[0])? FW_MAX_8U : 0;
        break;

    default:
        ASSERT(false);
    }

}
template< class TS, class TD, CH cd >
SYS_INLINE STATIC void Compare_gt( const TS * s1, const TS * s2, TD * d)
{
    switch (cd)
    {
    case C4:
        d[0] = ((s1[0] > s2[0])&&(s1[1] > s2[1])&&(s1[2] > s2[2])&&(s1[3] > s2[3]))? FW_MAX_8U : 0;
        break;
    case C3:
    case AC4:
        d[0] = ((s1[0] > s2[0])&&(s1[1] > s2[1])&&(s1[2] > s2[2]))? FW_MAX_8U : 0;
        break;

    case C1:
        d[0] = (s1[0] > s2[0])? FW_MAX_8U : 0;
        break;

    default:
        ASSERT(false);
    }
}
template< class TS, class TD, CH cd >
SYS_INLINE STATIC void Compare_le( const TS * s1, const TS * s2, TD * d)
{
    switch (cd)
    {
    case C4:
        d[0] = ((s1[0] <= s2[0])&&(s1[1] <= s2[1])&&(s1[2] <= s2[2])&&(s1[3] <= s2[3]))? FW_MAX_8U : 0;
        break;
    case C3:
    case AC4:
        d[0] = ((s1[0] <= s2[0])&&(s1[1] <= s2[1])&&(s1[2] <= s2[2]))? FW_MAX_8U : 0;
        break;

    case C1:
        d[0] = (s1[0] <= s2[0])? FW_MAX_8U : 0;
        break;

    default:
        ASSERT(false);
    }
}
template< class TS, class TD, CH cd >
SYS_INLINE STATIC void Compare_eq( const TS * s1, const TS * s2, TD * d)
{
    switch (cd)
    {
    case C4:
        d[0] = ((s1[0] == s2[0])&&(s1[1] == s2[1])&&(s1[2] == s2[2])&&(s1[3] == s2[3]))? FW_MAX_8U : 0;
        break;
    case C3:
    case AC4:
        d[0] = ((s1[0] == s2[0])&&(s1[1] == s2[1])&&(s1[2] == s2[2]))? FW_MAX_8U : 0;
        break;

    case C1:
        d[0] = (s1[0] == s2[0])? FW_MAX_8U : 0;
        break;

    default:
        ASSERT(false);
    }
}
template< class TS, class TD, CH cd >
SYS_INLINE STATIC void Compare_ge( const TS * s1, const TS * s2, TD * d)
{
    switch (cd)
    {
    case C4:
        d[0] = ((s1[0] >= s2[0])&&(s1[1] >= s2[1])&&(s1[2] >= s2[2])&&(s1[3] >= s2[3]))? FW_MAX_8U : 0;
        break;
    case C3:
    case AC4:
        d[0] = ((s1[0] >= s2[0])&&(s1[1] >= s2[1])&&(s1[2] >= s2[2]))? FW_MAX_8U : 0;
        break;

    case C1:
        d[0] = (s1[0] >= s2[0])? FW_MAX_8U : 0;
        break;

    default:
        ASSERT(false);
    }
}


namespace
{
//Namespace common for all compare routines
namespace Compare_All
{
//B2 - 2 buffers
template< class T>
class CompareC_B2_C1 : public fe2< T, C1,  Fw8u, C1  >
{
public:
    T val;

    CompareC_B2_C1(const T value){
        val = value;
    }
};

template< class T>
class CompareC_B2_AC4 : public fe2< T, AC4,  Fw8u, C1  >
{
public:
    T val[3];

    CompareC_B2_AC4(const T value[3]){
        val[0] = value[0], val[1] = value[1], val[2] = value[2];
    }
};

template< class T>
class CompareC_B2_C3 : public fe2_96< T, C3,  Fw8u, C1  >
{
public:
    T val[3];

    CompareC_B2_C3(const T value[3]){
        val[0] = value[0], val[1] = value[1], val[2] = value[2];
    }
};

template< class T>
class CompareC_B2_C4 : public fe2< T, C4,  Fw8u, C1  >
{
public:
    T val[4];

    CompareC_B2_C4(const T value[4]){
        val[0] = value[0], val[1] = value[1], val[2] = value[2], val[3] = value[3];
    }
};

template< class T, CH cd>
class Compare_B3_lt : public fe3< T, cd, T, cd, Fw8u, C1  >
{
public:
    IV REFR( const T *s1,  const T *s2, Fw8u *d ) const
    {
        Compare_lt<T,Fw8u,cd>(s1, s2, d);
    }
};

template< class T, CH cd>
class Compare_B3_gt : public fe3< T, cd, T, cd, Fw8u, C1  >
{
public:
    IV REFR( const T *s1,  const T *s2, Fw8u *d ) const
    {
        Compare_gt<T,Fw8u,cd>(s1, s2, d);
    }
};

template< class T, CH cd>
class Compare_B3_le : public fe3< T, cd, T, cd, Fw8u, C1  >
{
public:
    IV REFR( const T *s1,  const T *s2, Fw8u *d ) const
    {
        Compare_le<T,Fw8u,cd>(s1, s2, d);
    }
};

template< class T, CH cd>
class Compare_B3_ge : public fe3< T, cd, T, cd, Fw8u, C1  >
{
public:
    IV REFR( const T *s1,  const T *s2, Fw8u *d ) const
    {
        Compare_ge<T,Fw8u,cd>(s1, s2, d);
    }
};

template< class T, CH cd>
class Compare_B3_eq : public fe3< T, cd, T, cd, Fw8u, C1  >
{
public:
    IV REFR( const T *s1,  const T *s2, Fw8u *d ) const
    {
        Compare_eq<T,Fw8u,cd>(s1, s2, d);
    }
};
}

namespace CompareEqualEps_ALL
{
class CompareEqualEpsC_C1 :  public fe2< Fw32f, C1,  Fw8u, C1  >
{
    Fw32f val, eps;
    XMM128 valReg, epsReg;
public:
    FE_SSE2_REF
    CompareEqualEpsC_C1(const Fw32f val, const Fw32f eps):val(val),eps(eps){ }
    IV SSE2_Init()
    {
        epsReg.f = _mm_set1_ps(eps);
        valReg.f = _mm_set1_ps(val);
    }

    IV SSE2( RegFile & r ) const
    {
        CmpEqEpsAssign_sse2_32f_c1(r.src1[0], valReg, epsReg, r.dst[0]);
    }
    IV REFR( const Fw32f *s, Fw8u *d ) const
    {
        d[0] = FW_MAX_8U;
        CompareEqualEpsAssign(*s, val, *d, eps);
    }
};

class CompareEqualEpsC_C3 :  public fe2_96< Fw32f, C3,  Fw8u, C1  >
{
    Fw32f val[3], eps;
    XMM128 valReg, epsReg;
public:
    FE_SSE2_REF
    CompareEqualEpsC_C3(const Fw32f value[3], const Fw32f eps):eps(eps){
        val[0] = value[0], val[1] = value[1], val[2] = value[2];
    }
    IV SSE2_Init()
    {
        epsReg.f = _mm_set1_ps(eps);
        valReg.f = _mm_set_ps(0x00000000, val[2], val[1], val[0]);
    }

    IV SSE2( RegFile & r ) const
    {
        CmpEqEpsAssign_sse2_32f_c3(r.src1[0], valReg, epsReg, r.dst[0]);
    }
    IV REFR( const Fw32f *s, Fw8u *d ) const
    {
        d[0] = FW_MAX_8U;
        CompareEqualEpsAssign(s[0], val[0], d[0], eps);
        CompareEqualEpsAssign(s[1], val[1], d[0], eps);
        CompareEqualEpsAssign(s[2], val[2], d[0], eps);
    }
};

class CompareEqualEpsC_C4 :  public fe2< Fw32f, C4,  Fw8u, C1  >
{
    Fw32f val[4], eps;
    XMM128 valReg, epsReg;
public:
    FE_SSE2_REF
    CompareEqualEpsC_C4(const Fw32f value[4], const Fw32f eps):eps(eps){
        val[0] = value[0], val[1] = value[1], val[2] = value[2], val[3] = value[3];
    }
    IV SSE2_Init()
    {
        epsReg.f = _mm_set1_ps(eps);
        valReg.f = _mm_set_ps(val[3], val[2], val[1], val[0]);
    }

    IV SSE2( RegFile & r ) const
    {
        CmpEqEpsAssign_sse2_32f_c4(r.src1[0], valReg, epsReg, r.dst[0]);
    }
    IV REFR( const Fw32f *s, Fw8u *d ) const
    {
        d[0] = FW_MAX_8U;
        CompareEqualEpsAssign(s[0], val[0], d[0], eps);
        CompareEqualEpsAssign(s[1], val[1], d[0], eps);
        CompareEqualEpsAssign(s[2], val[2], d[0], eps);
        CompareEqualEpsAssign(s[3], val[3], d[03], eps);
    }
};

class CompareEqualEpsC_AC4 :  public fe2< Fw32f, AC4,  Fw8u, C1  >
{
    Fw32f val[3], eps;
    XMM128 valReg, epsReg;
public:
    FE_SSE2_REF
    CompareEqualEpsC_AC4(const Fw32f value[3], const Fw32f eps):eps(eps){
        val[0] = value[0], val[1] = value[1], val[2] = value[3];
    }
    IV SSE2_Init()
    {
        epsReg.f = _mm_set1_ps(eps);
        valReg.f = _mm_set_ps(0x00000000, val[2], val[1], val[0]);
    }

    IV SSE2( RegFile & r ) const
    {
        CmpEqEpsAssign_sse2_32f_c3(r.src1[0], valReg, epsReg, r.dst[0]);
    }
    IV REFR( const Fw32f *s, Fw8u *d ) const
    {
        d[0] = FW_MAX_8U;
        CompareEqualEpsAssign(s[0], val[0], d[0], eps);
        CompareEqualEpsAssign(s[1], val[1], d[0], eps);
        CompareEqualEpsAssign(s[2], val[2], d[0], eps);
    }
};

class CompareEqualEps_C1 :  public fe3< Fw32f, C1, Fw32f, C1, Fw8u, C1  >
{
    Fw32f  eps;
    XMM128  epsReg;
public:
    FE_SSE2_REF
    CompareEqualEps_C1(const Fw32f eps):eps(eps){ }
    IV SSE2_Init()
    {
        epsReg.f = _mm_set1_ps(eps);
    }

    IV SSE2( RegFile & r ) const
    {
        CmpEqEpsAssign_sse2_32f_c1(r.src1[0], r.src2[0], epsReg, r.dst[0]);
    }
    IV REFR( const Fw32f *s1, const Fw32f *s2, Fw8u *d ) const
    {
        d[0] = FW_MAX_8U;
        CompareEqualEpsAssign(*s1, *s2, *d, eps);
    }
};


class CompareEqualEps_C3 :  public fe3_96< Fw32f, C3, Fw32f, C3, Fw8u, C1  >
{
    Fw32f eps;
    XMM128 epsReg;
public:
    FE_SSE2_REF
    CompareEqualEps_C3(const Fw32f eps):eps(eps){ }
    IV SSE2_Init()
    {
        epsReg.f = _mm_set1_ps(eps);
    }

    IV SSE2( RegFile & r ) const
    {
        CmpEqEpsAssign_sse2_32f_c3(r.src1[0], r.src2[0], epsReg, r.dst[0]);
    }
    IV REFR( const Fw32f *s1, const Fw32f *s2, Fw8u *d ) const
    {
        d[0] = FW_MAX_8U;
        CompareEqualEpsAssign(s1[0], s2[0], d[0], eps);
        CompareEqualEpsAssign(s1[1], s2[1], d[0], eps);
        CompareEqualEpsAssign(s1[2], s2[2], d[0], eps);
    }
};

class CompareEqualEps_C4 :  public fe3< Fw32f, C4, Fw32f, C4, Fw8u, C1  >
{
    Fw32f eps;
    XMM128 epsReg;
public:
    FE_SSE2_REF
    CompareEqualEps_C4(const Fw32f eps):eps(eps){ }
    IV SSE2_Init()
    {
        epsReg.f = _mm_set1_ps(eps);
    }

    IV SSE2( RegFile & r ) const
    {
        CmpEqEpsAssign_sse2_32f_c4(r.src1[0], r.src2[0], epsReg, r.dst[0]);
    }
    IV REFR( const Fw32f *s1, const Fw32f *s2, Fw8u *d ) const
    {
        d[0] = FW_MAX_8U;
        CompareEqualEpsAssign(s1[0], s2[0], d[0], eps);
        CompareEqualEpsAssign(s1[1], s2[1], d[0], eps);
        CompareEqualEpsAssign(s1[2], s2[2], d[0], eps);
        CompareEqualEpsAssign(s1[3], s2[3], d[0], eps);

    }
};

class CompareEqualEps_AC4 :  public fe3< Fw32f, AC4, Fw32f, AC4, Fw8u, C1  >
{
    Fw32f eps;
    XMM128 epsReg;
public:
    FE_SSE2_REF
    CompareEqualEps_AC4(const Fw32f eps):eps(eps){ }
    IV SSE2_Init()
    {
        epsReg.f = _mm_set1_ps(eps);
    }

    IV SSE2( RegFile & r ) const
    {
        CmpEqEpsAssign_sse2_32f_c3(r.src1[0], r.src2[0], epsReg, r.dst[0]);
    }
    IV REFR( const Fw32f *s1, const Fw32f *s2, Fw8u *d ) const
    {
        d[0] = FW_MAX_8U;
        CompareEqualEpsAssign(s1[0], s2[0], d[0], eps);
        CompareEqualEpsAssign(s1[1], s2[1], d[0], eps);
        CompareEqualEpsAssign(s1[2], s2[2], d[0], eps);
    }
};
}
namespace Compare_8u
{
class Compare_lt_C1 :  public Compare_All::Compare_B3_lt<Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c1_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_C1 :  public Compare_All::Compare_B3_le<Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c1_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_C1 :  public Compare_All::Compare_B3_gt<Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c1_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_C1 :  public Compare_All::Compare_B3_ge<Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c1_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_C1 :  public Compare_All::Compare_B3_eq<Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c1_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};


class Compare_lt_AC4 :  public Compare_All::Compare_B3_lt<Fw8u, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_ac4_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_AC4 :  public Compare_All::Compare_B3_le<Fw8u, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_ac4_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_AC4 :  public Compare_All::Compare_B3_gt<Fw8u, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_ac4_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_AC4 :  public Compare_All::Compare_B3_ge<Fw8u, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_ac4_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_AC4 :  public Compare_All::Compare_B3_eq<Fw8u, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_ac4_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};


class Compare_lt_C3 :  public fe3_96<Fw8u, C3, Fw8u, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw8u *s1,  const Fw8u *s2, Fw8u *d ) const
    {
        Compare_lt<Fw8u,Fw8u,C3>(s1, s2, d);
    }


    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c3_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_C3 :  public fe3_96<Fw8u, C3, Fw8u, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw8u *s1,  const Fw8u *s2, Fw8u *d ) const
    {
        Compare_le<Fw8u,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c3_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_C3 :  public fe3_96<Fw8u, C3, Fw8u, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw8u *s1,  const Fw8u *s2, Fw8u *d ) const
    {
        Compare_gt<Fw8u,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c3_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_C3 :  public fe3_96<Fw8u, C3, Fw8u, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw8u *s1,  const Fw8u *s2, Fw8u *d ) const
    {
        Compare_ge<Fw8u,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c3_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_C3 :  public fe3_96<Fw8u, C3, Fw8u, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw8u *s1,  const Fw8u *s2, Fw8u *d ) const
    {
        Compare_eq<Fw8u,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c3_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_lt_C4 :  public Compare_All::Compare_B3_lt<Fw8u, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c4_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_C4 :  public Compare_All::Compare_B3_le<Fw8u, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c4_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_C4 :  public Compare_All::Compare_B3_gt<Fw8u, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c4_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_C4 :  public Compare_All::Compare_B3_ge<Fw8u, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c4_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_C4 :  public Compare_All::Compare_B3_eq<Fw8u, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c4_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};
}

namespace Compare_16s
{
class Compare_lt_C1 :  public Compare_All::Compare_B3_lt<Fw16s, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c1_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_C1 :  public Compare_All::Compare_B3_le<Fw16s, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c1_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_C1 :  public Compare_All::Compare_B3_gt<Fw16s, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c1_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_C1 :  public Compare_All::Compare_B3_ge<Fw16s, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c1_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_C1 :  public Compare_All::Compare_B3_eq<Fw16s, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c1_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_lt_AC4 :  public Compare_All::Compare_B3_lt<Fw16s, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_ac4_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_AC4 :  public Compare_All::Compare_B3_le<Fw16s, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_ac4_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_AC4 :  public Compare_All::Compare_B3_gt<Fw16s, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_ac4_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_AC4 :  public Compare_All::Compare_B3_ge<Fw16s, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_ac4_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_AC4 :  public Compare_All::Compare_B3_eq<Fw16s, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_ac4_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_lt_C3 :  public fe3_96<Fw16s, C3, Fw16s, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw16s *s1,  const Fw16s *s2, Fw8u *d ) const
    {
        Compare_lt<Fw16s,Fw8u,C3>(s1, s2, d);
    }


    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c3_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_C3 :  public fe3_96<Fw16s, C3, Fw16s, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw16s *s1,  const Fw16s *s2, Fw8u *d ) const
    {
        Compare_le<Fw16s,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c3_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_C3 :  public fe3_96<Fw16s, C3, Fw16s, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw16s *s1,  const Fw16s *s2, Fw8u *d ) const
    {
        Compare_gt<Fw16s,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c3_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_C3 :  public fe3_96<Fw16s, C3, Fw16s, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw16s *s1,  const Fw16s *s2, Fw8u *d ) const
    {
        Compare_ge<Fw16s,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c3_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_C3 :  public fe3_96<Fw16s, C3, Fw16s, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw16s *s1,  const Fw16s *s2, Fw8u *d ) const
    {
        Compare_eq<Fw16s,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c3_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_lt_C4 :  public Compare_All::Compare_B3_lt<Fw16s, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c4_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_C4 :  public Compare_All::Compare_B3_le<Fw16s, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c4_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_C4 :  public Compare_All::Compare_B3_gt<Fw16s, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c4_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_C4 :  public Compare_All::Compare_B3_ge<Fw16s, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c4_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_C4 :  public Compare_All::Compare_B3_eq<Fw16s, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c4_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};
}
namespace Compare_32f
{
class Compare_lt_C1 :  public Compare_All::Compare_B3_lt<Fw32f, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c1_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_C1 :  public Compare_All::Compare_B3_le<Fw32f, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c1_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_C1 :  public Compare_All::Compare_B3_gt<Fw32f, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c1_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_C1 :  public Compare_All::Compare_B3_ge<Fw32f, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c1_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_C1 :  public Compare_All::Compare_B3_eq<Fw32f, C1>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c1_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_lt_AC4 :  public Compare_All::Compare_B3_lt<Fw32f, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_AC4 :  public Compare_All::Compare_B3_le<Fw32f, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_AC4 :  public Compare_All::Compare_B3_gt<Fw32f, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_AC4 :  public Compare_All::Compare_B3_ge<Fw32f, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_AC4 :  public Compare_All::Compare_B3_eq<Fw32f, AC4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_lt_C3 :  public fe3_96<Fw32f, C3, Fw32f, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw32f *s1,  const Fw32f *s2, Fw8u *d ) const
    {
        Compare_lt<Fw32f,Fw8u,C3>(s1, s2, d);
    }


    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_C3 :  public fe3_96<Fw32f, C3, Fw32f, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw32f *s1,  const Fw32f *s2, Fw8u *d ) const
    {
        Compare_le<Fw32f,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_C3 :  public fe3_96<Fw32f, C3, Fw32f, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw32f *s1,  const Fw32f *s2, Fw8u *d ) const
    {
        Compare_gt<Fw32f,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_C3 :  public fe3_96<Fw32f, C3, Fw32f, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw32f *s1,  const Fw32f *s2, Fw8u *d ) const
    {
        Compare_ge<Fw32f,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_C3 :  public fe3_96<Fw32f, C3, Fw32f, C3, Fw8u, C1>
{

public:
    FE_SSE2_REF

    IV REFR( const Fw32f *s1,  const Fw32f *s2, Fw8u *d ) const
    {
        Compare_eq<Fw32f,Fw8u,C3>(s1, s2, d);
    }

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};


class Compare_lt_C4 :  public Compare_All::Compare_B3_lt<Fw32f, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c4_lt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_le_C4 :  public Compare_All::Compare_B3_le<Fw32f, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c4_le(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_gt_C4 :  public Compare_All::Compare_B3_gt<Fw32f, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c4_gt(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_ge_C4 :  public Compare_All::Compare_B3_ge<Fw32f, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c4_ge(r.src1[0], r.src2[0], r.dst[0]);
    }
};

class Compare_eq_C4 :  public Compare_All::Compare_B3_eq<Fw32f, C4>
{

public:
    FE_SSE2_REF

    IV SSE2_Init(){ }

    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c4_eq(r.src1[0], r.src2[0], r.dst[0]);
    }
};
}




namespace CompareC_8u
{
class CompareC_lt_C1 :  public Compare_All::CompareC_B2_C1<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x16_8u( value.i, Compare_All::CompareC_B2_C1<Fw8u>::val );
    }
    CompareC_lt_C1(Fw8u value): Compare_All::CompareC_B2_C1<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw8u,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw8u>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c1_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_C1 :  public Compare_All::CompareC_B2_C1<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x16_8u( value.i, Compare_All::CompareC_B2_C1<Fw8u>::val );
    }
    CompareC_le_C1(Fw8u value): Compare_All::CompareC_B2_C1<Fw8u>(value){ }
    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_le<Fw8u,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw8u>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c1_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_C1 :  public Compare_All::CompareC_B2_C1<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x16_8u( value.i, Compare_All::CompareC_B2_C1<Fw8u>::val );
    }
    CompareC_gt_C1(Fw8u value): Compare_All::CompareC_B2_C1<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw8u,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw8u>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c1_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_C1 :  public Compare_All::CompareC_B2_C1<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x16_8u( value.i, Compare_All::CompareC_B2_C1<Fw8u>::val );
    }
    CompareC_ge_C1(Fw8u value): Compare_All::CompareC_B2_C1<Fw8u>(value){ }
    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw8u,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw8u>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c1_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_C1 :  public Compare_All::CompareC_B2_C1<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x16_8u( value.i, Compare_All::CompareC_B2_C1<Fw8u>::val );
    }
    CompareC_eq_C1(Fw8u value): Compare_All::CompareC_B2_C1<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw8u,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw8u>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c1_eq(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_lt_AC4 :  public Compare_All::CompareC_B2_AC4<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw8u>::val, AC4, 0);
    }
    CompareC_lt_AC4(const Fw8u value[]): Compare_All::CompareC_B2_AC4<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw8u,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_ac4_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_AC4 :  public Compare_All::CompareC_B2_AC4<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw8u>::val, AC4, 0);
    }
    CompareC_le_AC4(const Fw8u value[]): Compare_All::CompareC_B2_AC4<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_le<Fw8u,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_ac4_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_AC4 :  public Compare_All::CompareC_B2_AC4<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw8u>::val, AC4, 0);
    }
    CompareC_gt_AC4(const Fw8u value[]): Compare_All::CompareC_B2_AC4<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw8u,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_ac4_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_AC4 :  public Compare_All::CompareC_B2_AC4<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw8u>::val, AC4, 0);
    }
    CompareC_ge_AC4(const Fw8u value[]): Compare_All::CompareC_B2_AC4<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw8u,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_ac4_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_AC4 :  public Compare_All::CompareC_B2_AC4<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw8u>::val, AC4, 0);
    }
    CompareC_eq_AC4(const Fw8u value[]): Compare_All::CompareC_B2_AC4<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw8u,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_ac4_eq(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_lt_C3 :  public Compare_All::CompareC_B2_C3<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw8u>::val, C3, 0);
    }
    CompareC_lt_C3(const Fw8u value[]): Compare_All::CompareC_B2_C3<Fw8u>(value){ }
    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw8u,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c3_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_C3 :  public Compare_All::CompareC_B2_C3<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw8u>::val, C3, 0);
    }
    CompareC_le_C3(const Fw8u value[]): Compare_All::CompareC_B2_C3<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_le<Fw8u,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c3_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_C3 :  public Compare_All::CompareC_B2_C3<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw8u>::val, C3, 0);
    }
    CompareC_gt_C3(const Fw8u value[]): Compare_All::CompareC_B2_C3<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw8u,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c3_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_C3 :  public Compare_All::CompareC_B2_C3<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw8u>::val, C3, 0);
    }
    CompareC_ge_C3(const Fw8u value[]): Compare_All::CompareC_B2_C3<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw8u,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c3_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_C3 :  public Compare_All::CompareC_B2_C3<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw8u>::val, C3, 0);
    }
    CompareC_eq_C3(const Fw8u value[]): Compare_All::CompareC_B2_C3<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw8u,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c3_eq(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_lt_C4 :  public Compare_All::CompareC_B2_C4<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw8u>::val, C4, 0);
    }
    CompareC_lt_C4(const Fw8u value[]): Compare_All::CompareC_B2_C4<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw8u,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c4_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_C4 :  public Compare_All::CompareC_B2_C4<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw8u>::val, C4, 0);
    }
    CompareC_le_C4(const Fw8u value[]): Compare_All::CompareC_B2_C4<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_le<Fw8u,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c4_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_C4 :  public Compare_All::CompareC_B2_C4<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw8u>::val, C4, 0);
    }
    CompareC_gt_C4(const Fw8u value[]): Compare_All::CompareC_B2_C4<Fw8u>(value){ }
    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw8u,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c4_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_C4 :  public Compare_All::CompareC_B2_C4<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw8u>::val, C4, 0);
    }
    CompareC_ge_C4(const Fw8u value[]): Compare_All::CompareC_B2_C4<Fw8u>(value){ }

    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw8u,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c4_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_C4 :  public Compare_All::CompareC_B2_C4<Fw8u>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw8u>::val, C4, 0);
    }
    CompareC_eq_C4(const Fw8u value[]): Compare_All::CompareC_B2_C4<Fw8u>(value){ }
    IV REFR( const Fw8u *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw8u,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw8u>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_8u_c4_eq(r.src1[0], value, r.dst[0]);
    }
};
}

namespace CompareC_16s
{
class CompareC_lt_C1 :  public Compare_All::CompareC_B2_C1<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x8_16s( value.i, Compare_All::CompareC_B2_C1<Fw16s>::val );
    }
    CompareC_lt_C1(Fw16s value): Compare_All::CompareC_B2_C1<Fw16s>(value){ }
    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw16s,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw16s>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c1_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_C1 :  public Compare_All::CompareC_B2_C1<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x8_16s( value.i, Compare_All::CompareC_B2_C1<Fw16s>::val );
    }
    CompareC_le_C1(Fw16s value): Compare_All::CompareC_B2_C1<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_le<Fw16s,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw16s>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c1_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_C1 :  public Compare_All::CompareC_B2_C1<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x8_16s( value.i, Compare_All::CompareC_B2_C1<Fw16s>::val );
    }
    CompareC_gt_C1(Fw16s value): Compare_All::CompareC_B2_C1<Fw16s>(value){ }
    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw16s,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw16s>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c1_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_C1 :  public Compare_All::CompareC_B2_C1<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x8_16s( value.i, Compare_All::CompareC_B2_C1<Fw16s>::val );
    }
    CompareC_ge_C1(Fw16s value): Compare_All::CompareC_B2_C1<Fw16s>(value){ }
    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw16s,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw16s>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c1_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_C1 :  public Compare_All::CompareC_B2_C1<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x8_16s( value.i, Compare_All::CompareC_B2_C1<Fw16s>::val );
    }
    CompareC_eq_C1(Fw16s value): Compare_All::CompareC_B2_C1<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw16s,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw16s>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c1_eq(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_lt_AC4 :  public Compare_All::CompareC_B2_AC4<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw16s>::val, AC4, 0);
    }
    CompareC_lt_AC4(const Fw16s value[]): Compare_All::CompareC_B2_AC4<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw16s,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_ac4_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_AC4 :  public Compare_All::CompareC_B2_AC4<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw16s>::val, AC4, 0);
    }
    CompareC_le_AC4(const Fw16s value[]): Compare_All::CompareC_B2_AC4<Fw16s>(value){ }
    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_le<Fw16s,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_ac4_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_AC4 :  public Compare_All::CompareC_B2_AC4<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw16s>::val, AC4, 0);
    }
    CompareC_gt_AC4(const Fw16s value[]): Compare_All::CompareC_B2_AC4<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw16s,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_ac4_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_AC4 :  public Compare_All::CompareC_B2_AC4<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw16s>::val, AC4, 0);
    }
    CompareC_ge_AC4(const Fw16s value[]): Compare_All::CompareC_B2_AC4<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw16s,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_ac4_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_AC4 :  public Compare_All::CompareC_B2_AC4<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw16s>::val, AC4, 0);
    }
    CompareC_eq_AC4(const Fw16s value[]): Compare_All::CompareC_B2_AC4<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw16s,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_ac4_eq(r.src1[0], value, r.dst[0]);
    }
};


class CompareC_lt_C3 :  public Compare_All::CompareC_B2_C3<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw16s>::val, C3, 0);
    }
    CompareC_lt_C3(const Fw16s value[]): Compare_All::CompareC_B2_C3<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw16s,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c3_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_C3 :  public Compare_All::CompareC_B2_C3<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw16s>::val, C3, 0);
    }
    CompareC_le_C3(const Fw16s value[]): Compare_All::CompareC_B2_C3<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_le<Fw16s,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c3_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_C3 :  public Compare_All::CompareC_B2_C3<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw16s>::val, C3, 0);
    }
    CompareC_gt_C3(const Fw16s value[]): Compare_All::CompareC_B2_C3<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw16s,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c3_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_C3 :  public Compare_All::CompareC_B2_C3<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw16s>::val, C3, 0);
    }
    CompareC_ge_C3(const Fw16s value[]): Compare_All::CompareC_B2_C3<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw16s,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c3_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_C3 :  public Compare_All::CompareC_B2_C3<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw16s>::val, C3, 0);
    }
    CompareC_eq_C3(const Fw16s value[]): Compare_All::CompareC_B2_C3<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw16s,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c3_eq(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_lt_C4 :  public Compare_All::CompareC_B2_C4<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw16s>::val, C4, 0);
    }
    CompareC_lt_C4(const Fw16s value[]): Compare_All::CompareC_B2_C4<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw16s,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c4_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_C4 :  public Compare_All::CompareC_B2_C4<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw16s>::val, C4, 0);
    }
    CompareC_le_C4(const Fw16s value[]): Compare_All::CompareC_B2_C4<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_le<Fw16s,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c4_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_C4 :  public Compare_All::CompareC_B2_C4<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw16s>::val, C4, 0);
    }
    CompareC_gt_C4(const Fw16s value[]): Compare_All::CompareC_B2_C4<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw16s,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c4_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_C4 :  public Compare_All::CompareC_B2_C4<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw16s>::val, C4, 0);
    }
    CompareC_ge_C4(const Fw16s value[]): Compare_All::CompareC_B2_C4<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw16s,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c4_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_C4 :  public Compare_All::CompareC_B2_C4<Fw16s>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw16s>::val, C4, 0);
    }
    CompareC_eq_C4(const Fw16s value[]): Compare_All::CompareC_B2_C4<Fw16s>(value){ }

    IV REFR( const Fw16s *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw16s,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw16s>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_16s_c4_eq(r.src1[0], value, r.dst[0]);
    }
};
}


namespace CompareC_32f
{
class CompareC_lt_C1 :  public Compare_All::CompareC_B2_C1<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x4_32f( value.f, Compare_All::CompareC_B2_C1<Fw32f>::val );
    }
    CompareC_lt_C1(Fw32f value): Compare_All::CompareC_B2_C1<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw32f,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw32f>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c1_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_C1 :  public Compare_All::CompareC_B2_C1<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x4_32f( value.f, Compare_All::CompareC_B2_C1<Fw32f>::val );
    }
    CompareC_le_C1(Fw32f value): Compare_All::CompareC_B2_C1<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_le<Fw32f,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw32f>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c1_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_C1 :  public Compare_All::CompareC_B2_C1<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x4_32f( value.f, Compare_All::CompareC_B2_C1<Fw32f>::val );
    }
    CompareC_gt_C1(Fw32f value): Compare_All::CompareC_B2_C1<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw32f,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw32f>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c1_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_C1 :  public Compare_All::CompareC_B2_C1<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x4_32f( value.f, Compare_All::CompareC_B2_C1<Fw32f>::val );
    }
    CompareC_ge_C1(Fw32f value): Compare_All::CompareC_B2_C1<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw32f,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw32f>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c1_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_C1 :  public Compare_All::CompareC_B2_C1<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        CBL_SSE2::Load_1x4_32f( value.f, Compare_All::CompareC_B2_C1<Fw32f>::val );
    }
    CompareC_eq_C1(Fw32f value): Compare_All::CompareC_B2_C1<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw32f,Fw8u,C1>(s1, &(Compare_All::CompareC_B2_C1<Fw32f>::val), d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c1_eq(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_lt_AC4 :  public Compare_All::CompareC_B2_AC4<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw32f>::val, AC4, 0);
    }
    CompareC_lt_AC4(const Fw32f value[]): Compare_All::CompareC_B2_AC4<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw32f,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_AC4 :  public Compare_All::CompareC_B2_AC4<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw32f>::val, AC4, 0);
    }
    CompareC_le_AC4(const Fw32f value[]): Compare_All::CompareC_B2_AC4<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_le<Fw32f,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_AC4 :  public Compare_All::CompareC_B2_AC4<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw32f>::val, AC4, 0);
    }
    CompareC_gt_AC4(const Fw32f value[]): Compare_All::CompareC_B2_AC4<Fw32f>(value){ }
    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw32f,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_AC4 :  public Compare_All::CompareC_B2_AC4<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw32f>::val, AC4, 0);
    }
    CompareC_ge_AC4(const Fw32f value[]): Compare_All::CompareC_B2_AC4<Fw32f>(value){ }
    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw32f,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_AC4 :  public Compare_All::CompareC_B2_AC4<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_AC4<Fw32f>::val, AC4, 0);
    }
    CompareC_eq_AC4(const Fw32f value[]): Compare_All::CompareC_B2_AC4<Fw32f>(value){ }
    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw32f,Fw8u,AC4>(s1, Compare_All::CompareC_B2_AC4<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_eq(r.src1[0], value, r.dst[0]);
    }
};


class CompareC_lt_C3 :  public Compare_All::CompareC_B2_C3<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw32f>::val, C3, 0);
    }
    CompareC_lt_C3(const Fw32f value[]): Compare_All::CompareC_B2_C3<Fw32f>(value){ }
    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw32f,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_C3 :  public Compare_All::CompareC_B2_C3<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw32f>::val, C3, 0);
    }
    CompareC_le_C3(const Fw32f value[]): Compare_All::CompareC_B2_C3<Fw32f>(value){ }
    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_le<Fw32f,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_C3 :  public Compare_All::CompareC_B2_C3<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw32f>::val, C3, 0);
    }
    CompareC_gt_C3(const Fw32f value[]): Compare_All::CompareC_B2_C3<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw32f,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_C3 :  public Compare_All::CompareC_B2_C3<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw32f>::val, C3, 0);
    }
    CompareC_ge_C3(const Fw32f value[]): Compare_All::CompareC_B2_C3<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw32f,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_C3 :  public Compare_All::CompareC_B2_C3<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C3<Fw32f>::val, C3, 0);
    }
    CompareC_eq_C3(const Fw32f value[]): Compare_All::CompareC_B2_C3<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw32f,Fw8u,C3>(s1, Compare_All::CompareC_B2_C3<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c3_eq(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_lt_C4 :  public Compare_All::CompareC_B2_C4<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw32f>::val, C4, 0);
    }
    CompareC_lt_C4(const Fw32f value[]): Compare_All::CompareC_B2_C4<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_lt<Fw32f,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c4_lt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_le_C4 :  public Compare_All::CompareC_B2_C4<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw32f>::val, C4, 0);
    }
    CompareC_le_C4(const Fw32f value[]): Compare_All::CompareC_B2_C4<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_le<Fw32f,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c4_le(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_gt_C4 :  public Compare_All::CompareC_B2_C4<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw32f>::val, C4, 0);
    }
    CompareC_gt_C4(const Fw32f value[]): Compare_All::CompareC_B2_C4<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_gt<Fw32f,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c4_gt(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_ge_C4 :  public Compare_All::CompareC_B2_C4<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw32f>::val, C4, 0);
    }
    CompareC_ge_C4(const Fw32f value[]): Compare_All::CompareC_B2_C4<Fw32f>(value){ }

    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_ge<Fw32f,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c4_ge(r.src1[0], value, r.dst[0]);
    }
};

class CompareC_eq_C4 :  public Compare_All::CompareC_B2_C4<Fw32f>
{

public:
    FE_SSE2_REF
    XMM128 value;
    IV SSE2_Init(){
        FW_SSE2::Load2(value, Compare_All::CompareC_B2_C4<Fw32f>::val, C4, 0);
    }
    CompareC_eq_C4(const Fw32f value[]): Compare_All::CompareC_B2_C4<Fw32f>(value){ }
    IV REFR( const Fw32f *s1,  Fw8u *d ) const
    {
        Compare_eq<Fw32f,Fw8u,C4>(s1, Compare_All::CompareC_B2_C4<Fw32f>::val, d);
    }
    IV SSE2( RegFile & r ) const
    {
        CmpAssign_sse2_32f_c4_eq(r.src1[0], value, r.dst[0]);
    }
};
}

}
};

//---------------------------------------------------------------------------------------------------//
//--------------------------------------iCompare_----------------------------------------------------//
//---------------------------------------------------------------------------------------------------//

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_8u_C1R)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2, 
                                                     int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, 
                                                     FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_8u::Compare_lt_C1 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_lt_C1 >   ( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                              dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_8u::Compare_gt_C1 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_gt_C1 >   ( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                              dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_8u::Compare_le_C1 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_le_C1 >   ( dat, pSrc1, src1Step, pSrc2, src2Step, 
                                                              pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_8u::Compare_eq_C1 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_eq_C1 >   ( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                              dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_8u::Compare_ge_C1 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_ge_C1 >   ( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                              dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_16s_C1R)( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2, 
                                                      int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, 
                                                      FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_16s::Compare_lt_C1 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_lt_C1 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                            dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_16s::Compare_gt_C1 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_gt_C1 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                            dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_16s::Compare_le_C1 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_le_C1 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                            dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_16s::Compare_eq_C1 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_eq_C1 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                            dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_16s::Compare_ge_C1 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_ge_C1 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                            dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_32f_C1R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2,
                                                      int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                      FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_32f::Compare_lt_C1 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_lt_C1 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                            dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_32f::Compare_gt_C1 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_gt_C1 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                            dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_32f::Compare_le_C1 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_le_C1 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                            dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_32f::Compare_eq_C1 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_eq_C1 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                            dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_32f::Compare_ge_C1 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_ge_C1 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}



FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_8u_C3R)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2, 
                                                     int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, 
                                                     FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_8u::Compare_lt_C3 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_lt_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                           dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_8u::Compare_gt_C3 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_gt_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                           dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_8u::Compare_le_C3 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_le_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                           dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_8u::Compare_eq_C3 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_eq_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                           dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_8u::Compare_ge_C3 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_ge_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                           dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;

    }
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_16s_C3R)( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2,
                                                       int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                       FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_16s::Compare_lt_C3 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_lt_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_16s::Compare_gt_C3 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_gt_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_16s::Compare_le_C3 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_le_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_16s::Compare_eq_C3 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_eq_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_16s::Compare_ge_C3 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_ge_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_32f_C3R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2,
                                                      int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                      FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_32f::Compare_lt_C3 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_lt_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_32f::Compare_gt_C3 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_gt_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_32f::Compare_le_C3 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_le_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_32f::Compare_eq_C3 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_eq_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_32f::Compare_ge_C3 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_ge_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_8u_C4R)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,
                                                     int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                     FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_8u::Compare_lt_C4 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_lt_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                           dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_8u::Compare_gt_C4 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_gt_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                           dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_8u::Compare_le_C4 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_le_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                           dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_8u::Compare_eq_C4 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_eq_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, 
                                                           dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_8u::Compare_ge_C4 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_ge_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                           dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_16s_C4R)( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2,
                                                      int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                      FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_16s::Compare_lt_C4 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_lt_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_16s::Compare_gt_C4 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_gt_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_16s::Compare_le_C4 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_le_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_16s::Compare_eq_C4 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_eq_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_16s::Compare_ge_C4 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_ge_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_32f_C4R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2,
                                                      int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                      FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_32f::Compare_lt_C4 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_lt_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_32f::Compare_gt_C4 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_gt_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_32f::Compare_le_C4 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_le_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_32f::Compare_eq_C4 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_eq_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_32f::Compare_ge_C4 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_ge_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_8u_AC4R)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,
                                                       int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                       FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_8u::Compare_lt_AC4 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_lt_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_8u::Compare_gt_AC4 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_gt_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_8u::Compare_le_AC4 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_le_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_8u::Compare_eq_AC4 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_eq_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_8u::Compare_ge_AC4 dat;
        return OPT_LEVEL::fe< Compare_8u::Compare_ge_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                            dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_16s_AC4R)( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2,
                                                        int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                        FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_16s::Compare_lt_AC4 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_lt_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                             dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_16s::Compare_gt_AC4 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_gt_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                             dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_16s::Compare_le_AC4 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_le_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                             dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_16s::Compare_eq_AC4 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_eq_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                             dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_16s::Compare_ge_AC4 dat;
        return OPT_LEVEL::fe< Compare_16s::Compare_ge_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                             dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompare_32f_AC4R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2,
                                                       int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                       FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        Compare_32f::Compare_lt_AC4 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_lt_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                             dstStep, roiSize );
    }

    case fwCmpGreater:		{
        Compare_32f::Compare_gt_AC4 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_gt_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                             dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        Compare_32f::Compare_le_AC4 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_le_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                             dstStep, roiSize );
    }

    case fwCmpEq:			{
        Compare_32f::Compare_eq_AC4 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_eq_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                             dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        Compare_32f::Compare_ge_AC4 dat;
        return OPT_LEVEL::fe< Compare_32f::Compare_ge_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst,
                                                             dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}



//---------------------------------------------------------------------------------------------------//
//--------------------------------------iCompareC_---------------------------------------------------//
//---------------------------------------------------------------------------------------------------//

// 1-channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u value, Fw8u *pDst,
                                                      int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_8u::CompareC_lt_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_lt_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_8u::CompareC_gt_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_gt_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_8u::CompareC_le_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_le_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_8u::CompareC_eq_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_eq_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_8u::CompareC_ge_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_ge_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s value, 
                                                        Fw8u *pDst, int dstStep, FwiSize roiSize, 
                                                        FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_16s::CompareC_lt_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_lt_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_16s::CompareC_gt_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_gt_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_16s::CompareC_le_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_le_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_16s::CompareC_eq_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_eq_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_16s::CompareC_ge_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_ge_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f value, 
                                                       Fw8u *pDst, int dstStep, FwiSize roiSize, 
                                                       FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_32f::CompareC_lt_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_lt_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_32f::CompareC_gt_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_gt_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_32f::CompareC_le_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_le_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_32f::CompareC_eq_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_eq_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_32f::CompareC_ge_C1 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_ge_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

// multi-channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_8u_C3R)( const Fw8u *pSrc, int srcStep, const Fw8u value[3],
                                                      Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                      FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_8u::CompareC_lt_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_lt_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_8u::CompareC_gt_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_gt_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_8u::CompareC_le_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_le_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_8u::CompareC_eq_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_eq_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_8u::CompareC_ge_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_ge_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_16s_C3R)( const Fw16s *pSrc, int srcStep, const Fw16s value[3],
                                                       Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                       FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_16s::CompareC_lt_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_lt_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_16s::CompareC_gt_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_gt_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_16s::CompareC_le_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_le_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_16s::CompareC_eq_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_eq_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_16s::CompareC_ge_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_ge_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_32f_C3R)( const Fw32f *pSrc, int srcStep, const Fw32f value[3],
                                                       Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                       FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_32f::CompareC_lt_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_lt_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_32f::CompareC_gt_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_gt_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_32f::CompareC_le_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_le_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_32f::CompareC_eq_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_eq_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_32f::CompareC_ge_C3 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_ge_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_8u_AC4R)( const Fw8u *pSrc, int srcStep, const Fw8u value[3],
                                                       Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                       FwCmpOp fwCmpOp )
{

    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_8u::CompareC_lt_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_lt_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_8u::CompareC_gt_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_gt_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_8u::CompareC_le_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_le_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_8u::CompareC_eq_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_eq_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_8u::CompareC_ge_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_ge_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_16s_AC4R)( const Fw16s *pSrc, int srcStep, const Fw16s value[3],
                                                        Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                        FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_16s::CompareC_lt_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_lt_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_16s::CompareC_gt_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_gt_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_16s::CompareC_le_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_le_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_16s::CompareC_eq_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_eq_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_16s::CompareC_ge_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_ge_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_32f_AC4R)( const Fw32f *pSrc, int srcStep, const Fw32f value[3],
                                                        Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                        FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_32f::CompareC_lt_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_lt_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_32f::CompareC_gt_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_gt_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_32f::CompareC_le_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_le_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_32f::CompareC_eq_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_eq_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_32f::CompareC_ge_AC4 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_ge_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_8u_C4R)( const Fw8u *pSrc, int srcStep, const Fw8u value[4],
                                                      Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                      FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_8u::CompareC_lt_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_lt_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_8u::CompareC_gt_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_gt_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_8u::CompareC_le_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_le_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_8u::CompareC_eq_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_eq_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_8u::CompareC_ge_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_8u::CompareC_ge_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_16s_C4R)( const Fw16s *pSrc, int srcStep, const Fw16s value[4],
                                                       Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                       FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_16s::CompareC_lt_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_lt_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_16s::CompareC_gt_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_gt_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_16s::CompareC_le_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_le_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_16s::CompareC_eq_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_eq_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_16s::CompareC_ge_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_16s::CompareC_ge_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareC_32f_C4R)( const Fw32f *pSrc, int srcStep, const Fw32f value[4],
                                                       Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                       FwCmpOp fwCmpOp )
{
    switch (fwCmpOp)
    {
    case fwCmpLess:		{
        CompareC_32f::CompareC_lt_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_lt_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreater:		{
        CompareC_32f::CompareC_gt_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_gt_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpLessEq:		{
        CompareC_32f::CompareC_le_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_le_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpEq:			{
        CompareC_32f::CompareC_eq_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_eq_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    case fwCmpGreaterEq:	{
        CompareC_32f::CompareC_ge_C4 dat(value);
        return OPT_LEVEL::fe< CompareC_32f::CompareC_ge_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
    }

    default:
        ASSERT(false);
        return fwStsNoErr;
    }
}



//---------------------------------------------------------------------------------------------------//
//--------------------------------------iCompareEqualEps_--------------------------------------------//
//---------------------------------------------------------------------------------------------------//

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEps_32f_C1R)( const Fw32f *pSrc1, int src1Step,
                                                              const Fw32f *pSrc2, int src2Step,
                                                              Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                              Fw32f eps)
{
    if (eps < 0) return fwStsEpsValErr;
    CompareEqualEps_ALL::CompareEqualEps_C1 dat(eps);
    return OPT_LEVEL::fe< CompareEqualEps_ALL::CompareEqualEps_C1 >( dat, pSrc1, src1Step, pSrc2, src2Step,
                                                                    pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEps_32f_C3R)( const Fw32f *pSrc1, int src1Step,
                                                              const Fw32f *pSrc2, int src2Step,
                                                              Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                              Fw32f eps)
{
    if (eps < 0) return fwStsEpsValErr;
    CompareEqualEps_ALL::CompareEqualEps_C3 dat(eps);
    return OPT_LEVEL::fe< CompareEqualEps_ALL::CompareEqualEps_C3 >( dat, pSrc1, src1Step, pSrc2, src2Step,
                                                                    pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEps_32f_C4R)( const Fw32f *pSrc1, int src1Step,
                                                              const Fw32f *pSrc2, int src2Step,
                                                              Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                              Fw32f eps)
{
    if (eps < 0) return fwStsEpsValErr;
    CompareEqualEps_ALL::CompareEqualEps_C4 dat(eps);
    return OPT_LEVEL::fe< CompareEqualEps_ALL::CompareEqualEps_C4 >( dat, pSrc1, src1Step, pSrc2, src2Step,
                                                                    pDst, dstStep, roiSize );

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEps_32f_AC4R)( const Fw32f *pSrc1, int src1Step,
                                                               const Fw32f *pSrc2, int src2Step,
                                                               Fw8u *pDst, int dstStep, FwiSize roiSize,
                                                               Fw32f eps)
{
    if (eps < 0) return fwStsEpsValErr;
    CompareEqualEps_ALL::CompareEqualEps_AC4 dat(eps);
    return OPT_LEVEL::fe< CompareEqualEps_ALL::CompareEqualEps_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step,
                                                                     pDst, dstStep, roiSize );

}

//---------------------------------------------------------------------------------------------------//
//--------------------------------------iCompareEqualEpsC_-------------------------------------------//
//---------------------------------------------------------------------------------------------------//

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEpsC_32f_C1R)( const Fw32f *pSrc, int srcStep,
                                                               Fw32f value, Fw8u *pDst, int dstStep,
                                                               FwiSize roiSize, Fw32f eps)
{
    if (eps < 0) return fwStsEpsValErr;
    CompareEqualEps_ALL::CompareEqualEpsC_C1 dat(value, eps);
    return OPT_LEVEL::fe< CompareEqualEps_ALL::CompareEqualEpsC_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEpsC_32f_C3R)( const Fw32f *pSrc, int srcStep,
                                                               const Fw32f value[3], Fw8u *pDst,
                                                               int dstStep, FwiSize roiSize, Fw32f eps)
{
    if (eps < 0) return fwStsEpsValErr;
    CompareEqualEps_ALL::CompareEqualEpsC_C3 dat(value, eps);
    return OPT_LEVEL::fe< CompareEqualEps_ALL::CompareEqualEpsC_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEpsC_32f_AC4R)( const Fw32f *pSrc, int srcStep,
                                                                const Fw32f value[3], Fw8u *pDst,
                                                                int dstStep, FwiSize roiSize, Fw32f eps)
{
    if (eps < 0) return fwStsEpsValErr;
    CompareEqualEps_ALL::CompareEqualEpsC_AC4 dat(value, eps);
    return OPT_LEVEL::fe< CompareEqualEps_ALL::CompareEqualEpsC_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEpsC_32f_C4R)( const Fw32f *pSrc, int srcStep,
                                                               const Fw32f value[4], Fw8u *pDst,
                                                               int dstStep, FwiSize roiSize, Fw32f eps)
{
    if (eps < 0) return fwStsEpsValErr;
    CompareEqualEps_ALL::CompareEqualEpsC_C4 dat(value, eps);
    return OPT_LEVEL::fe< CompareEqualEps_ALL::CompareEqualEpsC_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize );
}

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H
