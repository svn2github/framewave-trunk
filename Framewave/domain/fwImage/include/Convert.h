/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __CONVERT_H__
#define __CONVERT_H__

#include "fe.h"

namespace OPT_LEVEL
{

/////////////////////////////////////// REF FUNCTIONS ////////////////////////////////////////////

template<class TS, class TD, CH CS>
ISV Convert(const TS * s, TD * d)
    {
    for(int i=0;i<CS;i++)
        d[i] = FW_REF::To< TD >::From( s[i] );;
    }

template< class TS, class TD, CH CS >
ISV ConvertDown( const TS *s, TD *d )
    {
    for(int i=0;i<CS;i++)
        d[i] = FW_REF::Limits<TD>::Sat( s[i] );
    }

/////////////////////////////////////// SSE2 SETUP FUNCTIONS ///////////////////////////////////////

ISV Convert_zero_SETUP_I(__m128i &zero)
    {
    zero = _mm_setzero_si128();
    }

ISV ConvertTo8u_SETUP_I(__m128i &mask)
    {  
    mask = _mm_setzero_si128();									//set to zeros
    mask = _mm_cmpeq_epi32(mask,mask);							//set to ones
    mask = _mm_srli_epi32(mask, 8);		
    }

/////////////////////////////////////// SSE2 FUNCTIONS  ////////////////////////////////////////////

ISV Convert_8u16_C1R_SSE2_I(RegFile &reg, const __m128i &zero)
    {
    reg.dst[0].i = _mm_unpacklo_epi8(reg.src1[0].i, zero);
    }

ISV Convert_8u16_C3R_SSE2_I(RegFile &reg, const __m128i &zero)
    {
    reg.dst[0].i = _mm_unpacklo_epi8(reg.src1[0].i, zero);
    reg.dst[1].i = _mm_unpackhi_epi8(reg.src1[0].i, zero);
    reg.dst[2].i = _mm_unpacklo_epi8(reg.src1[1].i, zero);
    }

ISV Convert_8u32s_C1R_SSE2_I(RegFile &reg, const __m128i &zero)
    {
    reg.dst[0].i = _mm_unpacklo_epi8(reg.src1[0].i, zero);
    reg.dst[0].i = _mm_unpacklo_epi8(reg.dst[0].i,  zero);
    }

ISV Convert_8u32s_C3R_SSE2_I(RegFile &reg, const __m128i &zero)
    {
    reg.dst[1].i  = _mm_unpacklo_epi8(reg.src1[0].i, zero);
    reg.dst[0].i  = _mm_unpacklo_epi8(reg.dst[1].i, zero);
    reg.dst[1].i  = _mm_unpackhi_epi8(reg.dst[1].i, zero);
    reg.dst[2].i  = _mm_unpackhi_epi8(reg.src1[0].i, zero);
    reg.dst[2].i  = _mm_unpacklo_epi8(reg.dst[2].i, zero);
    }


template<IsAlign ia, class TS, CH CD>
ISV Convert_8u32s_C14R_Custom_SSE2(const TS *s1, A32S *d, U32 &pixCount)
    {
    A32S *end	=  d + pixCount* ChannelCount(CD);
    XMM128 src, dst, accumulator;    
    const __m128i zero = _mm_setzero_si128();

     for( ; d <= end-16; d+=16, s1+=16)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&src, (const void*)s1);

        accumulator.i = _mm_unpacklo_epi8(src.i, zero);
        dst.i = _mm_unpacklo_epi16(accumulator.i, zero);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)d);

        accumulator.i = _mm_unpackhi_epi16(accumulator.i, zero);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&accumulator, (void*)(d+4));

        accumulator.i = _mm_unpackhi_epi8(src.i, zero);
        dst.i = _mm_unpacklo_epi16(accumulator.i, zero);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)(d+8));

        accumulator.i = _mm_unpackhi_epi16(accumulator.i, zero);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&accumulator, (void*)(d+12));

        }		
        for( ; d < end; d+=4, s1+=4)
        {
        LoadStoreModules::LOAD<4, DT_SSE2, ia, STREAM_FLSE>(&src, (const void*)s1);

        accumulator.i = _mm_unpacklo_epi8(src.i, zero);
        dst.i = _mm_unpacklo_epi16(accumulator.i, zero);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)d);
        }

    }


template<IsAlign ia, class TS, CH CD>
ISV Convert_8s32s_C14R_Custom_SSE2(const TS *s1, A32S *d, U32 &pixCount)
    {
    A32S *end	=  d + pixCount* ChannelCount(CD);
    XMM128 src, dst, accumulator;    
    const __m128i zero = _mm_setzero_si128();

    for( ; d <= end-16; d+=16, s1+=16)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&src, (const void*)s1);

        accumulator.i = _mm_unpacklo_epi8(zero, src.i);

        dst.i = _mm_unpacklo_epi16(zero, accumulator.i);
        dst.i = _mm_srai_epi32(dst.i, 24);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)d);

        accumulator.i = _mm_unpackhi_epi16(zero, accumulator.i);
        dst.i = _mm_srai_epi32(accumulator.i, 24);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)(d+4));

        accumulator.i = _mm_unpackhi_epi8(zero, src.i);

        dst.i = _mm_unpacklo_epi16(zero, accumulator.i);
        dst.i = _mm_srai_epi32(dst.i, 24);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)(d+8));

        accumulator.i = _mm_unpackhi_epi16(zero, accumulator.i);
        dst.i = _mm_srai_epi32(accumulator.i, 24);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)(d+12));

        }		
        for( ; d < end; d+=4, s1+=4)
        {
        LoadStoreModules::LOAD<4, DT_SSE2, ia, STREAM_FLSE>(&src, (const void*)s1);

        accumulator.i = _mm_unpacklo_epi8(zero, src.i);

        dst.i = _mm_unpacklo_epi16(zero, accumulator.i);
        dst.i = _mm_srai_epi32(dst.i, 24);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)d);
        }

    }


ISV Convert_8u32f_C1R_SSE2_I(RegFile &reg, const __m128i &zero)
    {
    reg.dst[0].i = _mm_unpacklo_epi8(reg.src1[0].i, zero);	    //unpack to 16 bit (don't sign extend)
    reg.dst[0].i = _mm_unpacklo_epi16(reg.dst[0].i, zero);	    //unpack to 32 bit
    reg.dst[0].f = _mm_cvtepi32_ps(reg.dst[0].i);
    }

template<IsAlign ia, class TS, CH CD>
ISV Convert_8u32f_C14R_Custom_SSE2(const TS *s1, A32F *d, U32 &pixCount)
    {
    A32F *end	=  d + pixCount* ChannelCount(CD);
    XMM128 src, dst, accumulator;    
    const __m128i zero = _mm_setzero_si128();

    for( ; d <= end-16; d+=16, s1+=16)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&src, (const void*)s1);

        accumulator.i = _mm_unpacklo_epi8(src.i, zero);

        dst.i = _mm_unpacklo_epi16(accumulator.i, zero);
        dst.f = _mm_cvtepi32_ps(dst.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)d);

        accumulator.i = _mm_unpackhi_epi16(accumulator.i, zero);
        dst.f = _mm_cvtepi32_ps(accumulator.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)(d+4));

        accumulator.i = _mm_unpackhi_epi8(src.i, zero);

        dst.i = _mm_unpacklo_epi16(accumulator.i, zero);
        dst.f = _mm_cvtepi32_ps(dst.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)(d+8));

        accumulator.i = _mm_unpackhi_epi16(accumulator.i, zero);
         dst.f = _mm_cvtepi32_ps(accumulator.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)(d+12));

        }		
        for( ; d < end; d+=4, s1+=4)
        {
        LoadStoreModules::LOAD<4, DT_SSE2, ia, STREAM_FLSE>(&src, (const void*)s1);

        accumulator.i = _mm_unpacklo_epi8(src.i, zero);

        dst.i = _mm_unpacklo_epi16(accumulator.i, zero);
         dst.f = _mm_cvtepi32_ps(dst.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)d);
        }

    }

template<IsAlign ia, class TS, CH CD>
ISV Convert_8s32f_C14R_Custom_SSE2(const TS *s1, A32F *d, U32 &pixCount)
    {
    A32F *end	=  d + pixCount* ChannelCount(CD);
    XMM128 src, dst, accumulator;    
    const __m128i zero = _mm_setzero_si128();

    for( ; d <= end-16; d+=16, s1+=16)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&src, (const void*)s1);

        accumulator.i = _mm_unpacklo_epi8(src.i, zero);

        dst.i = _mm_unpacklo_epi16(accumulator.i, zero);
        dst.i = _mm_slli_epi32(dst.i, 24);
        dst.i = _mm_srai_epi32(dst.i, 24);
        dst.f = _mm_cvtepi32_ps(dst.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)d);

        accumulator.i = _mm_unpackhi_epi16(accumulator.i, zero);
        accumulator.i = _mm_slli_epi32(accumulator.i, 24);
        accumulator.i = _mm_srai_epi32(accumulator.i, 24);
        dst.f = _mm_cvtepi32_ps(accumulator.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)(d+4));

        accumulator.i = _mm_unpackhi_epi8(src.i, zero);

        dst.i = _mm_unpacklo_epi16(accumulator.i, zero);
        dst.i = _mm_slli_epi32(dst.i, 24);
        dst.i = _mm_srai_epi32(dst.i, 24);
        dst.f = _mm_cvtepi32_ps(dst.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)(d+8));

        accumulator.i = _mm_unpackhi_epi16(accumulator.i, zero);
        accumulator.i = _mm_slli_epi32(accumulator.i, 24);
        accumulator.i = _mm_srai_epi32(accumulator.i, 24);
        dst.f = _mm_cvtepi32_ps(accumulator.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)(d+12));

        }		
        for( ; d < end; d+=4, s1+=4)
        {
        LoadStoreModules::LOAD<4, DT_SSE2, ia, STREAM_FLSE>(&src, (const void*)s1);

        accumulator.i = _mm_unpacklo_epi8(src.i, zero);

        dst.i = _mm_unpacklo_epi16(accumulator.i, zero);
        dst.i = _mm_slli_epi32(dst.i, 24);
        dst.i = _mm_srai_epi32(dst.i, 24);
        dst.f = _mm_cvtepi32_ps(dst.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst, (void*)d);
        }

    }


ISV Convert_16u32s_C1R_SSE2_I(RegFile &reg, const __m128i &zero)
    {
    reg.dst[0].i = _mm_unpacklo_epi16(reg.src1[0].i, zero);
    }

ISV Convert_16u32s_C3R_SSE2_I(RegFile &reg, const __m128i &zero)
    {
    reg.dst[0].i = _mm_unpacklo_epi16(reg.src1[0].i, zero);
    reg.dst[1].i = _mm_unpackhi_epi16(reg.src1[0].i, zero);
    reg.dst[2].i = _mm_unpacklo_epi16(reg.src1[1].i, zero);
    }

ISV Convert_16u32f_C1R_SSE2_I(RegFile &reg, const __m128i &zero)
    {
    reg.dst[0].i = _mm_unpacklo_epi16(reg.src1[0].i, zero);
    reg.dst[0].f = _mm_cvtepi32_ps(reg.dst[0].i);
    }

ISV Convert_16u32f_C3R_SSE2_I(RegFile &reg, const __m128i &zero)
    {
    reg.dst[0].i = _mm_unpacklo_epi16(reg.src1[0].i, zero);
    reg.dst[0].f = _mm_cvtepi32_ps(reg.dst[0].i);
    reg.dst[1].i = _mm_unpackhi_epi16(reg.src1[0].i, zero);
    reg.dst[1].f = _mm_cvtepi32_ps(reg.dst[1].i);
    reg.dst[2].i = _mm_unpacklo_epi16(reg.src1[1].i, zero);
    reg.dst[2].f = _mm_cvtepi32_ps(reg.dst[2].i);
    }

ISV Convert_16s32f_C1R_SSE2_I(RegFile &reg, const __m128i &zero)
    {
    reg.dst[0].i = _mm_unpacklo_epi16(zero, reg.src1[0].i);
    reg.dst[0].i = _mm_srai_epi32(reg.dst[0].i, 16);
    reg.dst[0].f = _mm_cvtepi32_ps(reg.dst[0].i);
    }

ISV Convert_16s32f_C3R_SSE2_I(RegFile &reg, const __m128i &zero)
    {
    reg.dst[0].i = _mm_unpacklo_epi16(zero, reg.src1[0].i);
    reg.dst[0].i = _mm_srai_epi32(reg.dst[0].i, 16);
    reg.dst[0].f = _mm_cvtepi32_ps(reg.dst[0].i);
    reg.dst[1].i = _mm_unpackhi_epi16(zero, reg.src1[0].i);
    reg.dst[1].i = _mm_srai_epi32(reg.dst[1].i, 16);
    reg.dst[1].f = _mm_cvtepi32_ps(reg.dst[1].i);
    reg.dst[2].i = _mm_unpacklo_epi16(zero, reg.src1[1].i);
    reg.dst[2].i = _mm_srai_epi32(reg.dst[2].i, 16);
    reg.dst[2].f = _mm_cvtepi32_ps(reg.dst[2].i);	
    }

// 16S8U C1R, C4R
template<IsAlign ia>
IS __m128i convert16s_8u_Helper(__m128i *src1, __m128i *src2)
    {
    XMM128 reg1, reg2;
    
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg1, (const void*)src1);
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg2, (const void*)src2);

    return _mm_packus_epi16 (reg1.i,reg2.i);
    }

template <class TS, class TD, CH CS, IsAlign ia>
ISV Convert_16s8u_Custom_SSE2_I(const TS *s, TD *d, U32 &pixCount)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;
    XMM128 temp,temp1;

    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(CS));

    for( ; (src+3) < end; src+= 4, dst+=2)
        {		
        temp.i = convert16s_8u_Helper<ia>(src, src+1);
        temp1.i = convert16s_8u_Helper<ia>(src+2, src+3);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&temp, (void*)dst);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&temp1, (void*)(dst+1));
        }

    for(; (src+1) < end; src+=2, dst++)
        {
        temp.i = convert16s_8u_Helper<ia>(src, src+1);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&temp, (void*)dst);
        }

    TD *ds = (TD*) dst;
    TS *sc = (TS*) src;
    const TS *scEnd = s + pixCount * ChannelCount(CS);

    for( ; sc < scEnd; sc+=ChannelCount(CS), ds+=ChannelCount(CS))
        {
        ConvertDown<TS,TD,CS>(sc,ds);
        }	
    }

// 16S8U C3R
template <class TS, class TD, CH CS, IsAlign ia>
ISV Convert_16s8u_C3R_Custom_SSE2_I(const TS *s, TD *d, U32 &pixCount)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;
    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(CS));
    XMM128 temp;    

    for(; (src+1) < end; src+=2, dst++)
        {
        temp.i = convert16s_8u_Helper<ia>(src, src+1);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&temp, (void*)dst);
        }

    TD *ds = (TD*) dst;
    TS *sc = (TS*) src;
    const TS *scEnd = s + pixCount * ChannelCount(CS);

    for( ; sc < scEnd; sc++, ds++)     // processing for the last register as channel C1
        {
        ConvertDown<TS,TD,C1>(sc,ds);
        }	
    }

// 16S8U AC4R
template <class TS, class TD, CH CS, IsAlign ia>
ISV Convert_16s8uAC4_Custom_SSE2_I(const TS *s, TD *d, U32 &pixCount, const __m128i &mask)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;
    XMM128 dstReg1,dstReg2;    

    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(CS));

    for( ; (src+3) < end; src+= 4, dst+=2)
        {
        dstReg1.i = convert16s_8u_Helper<ia>(src, src+1);
        dstReg2.i = convert16s_8u_Helper<ia>(src+2, src+3);
        dstReg1.i = _mm_and_si128 (dstReg1.i,mask);
        dstReg2.i = _mm_and_si128 (dstReg2.i,mask);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg1, (void*)dst);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg2, (void*)(dst+1));
        }

    for(; (src+1) < end; src+=2, dst++)
        {
        dstReg1.i = convert16s_8u_Helper<ia>(src, src+1);
        dstReg1.i = _mm_and_si128 (dstReg1.i,mask);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg1, (void*)dst);
        }

    TD *ds = (TD*) dst;
    TS *sc = (TS*) src;
    const TS *scEnd = s + pixCount * ChannelCount(CS);

    for( ; sc < scEnd; sc+=ChannelCount(CS), ds+=ChannelCount(CS))
        {
        ConvertDown<TS,TD,C3>(sc,ds);
        }	
    }

// 16U8U C1R, C4R
template<IsAlign ia>
ISV convert16u8u_Helper(__m128i *src1, __m128i *src2, __m128i *src3, __m128i *src4,__m128i &zeroTemp, __m128i &dst1,__m128i &dst2)
    {
    XMM128 reg1, reg2, reg3, reg4;    

    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg1, (const void*)src1);
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg2, (const void*)src2);
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg3, (const void*)src3);
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg4, (const void*)src4);

    __m128i mask1 = _mm_cmplt_epi16(reg1.i, zeroTemp);
    __m128i mask2 = _mm_cmplt_epi16(reg2.i, zeroTemp);
    __m128i mask3 = _mm_cmplt_epi16(reg3.i, zeroTemp);
    __m128i mask4 = _mm_cmplt_epi16(reg4.i, zeroTemp);

    dst1 = _mm_packus_epi16(reg1.i, reg2.i);
    dst2 = _mm_packus_epi16(reg3.i, reg4.i);

    __m128i mskTemp = _mm_packs_epi16 (mask1, mask2);
    __m128i mskTemp1 = _mm_packs_epi16 (mask3, mask4);

    dst1 = _mm_or_si128(dst1,mskTemp);
    dst2 = _mm_or_si128(dst2,mskTemp1);
    }

template<IsAlign ia>
ISV convert16u8u_Helper(__m128i *src1, __m128i *src2, __m128i &zeroTemp,__m128i &dst)
    {
    XMM128 reg1, reg2;    

    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg1, (const void*)src1);
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg2, (const void*)src2);
    __m128i mask1 = _mm_cmplt_epi16(reg1.i, zeroTemp);
    __m128i mask2 = _mm_cmplt_epi16(reg2.i, zeroTemp);
    dst = _mm_packus_epi16(reg1.i, reg2.i);
    __m128i mskTemp = _mm_packs_epi16 (mask1, mask2);
    dst = _mm_or_si128(dst,mskTemp);
    }

template <class TS, class TD, CH CS, IsAlign ia>
ISV Convert_16u8u_Custom_SSE2_I (const TS *s, TD *d, U32 &pixCount)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;
    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(CS));
    __m128i zero = _mm_setzero_si128();
    XMM128 dstReg1, dstReg2;
    
    dstReg1.i = _mm_setzero_si128();
    dstReg2.i = _mm_setzero_si128();

    for( ; (src+3) < end; src+= 4, dst+=2)
        {
        convert16u8u_Helper<ia>(src, src+1, src+2, src+3,zero,dstReg1.i, dstReg2.i);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg1, (void*)dst);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg2, (void*)(dst+1));
        }

    for(; (src+1) < end; src+=2, dst++)
        {
        convert16u8u_Helper<ia>(src, src+1,zero,dstReg1.i);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg1, (void*)dst);
        }

    TD *ds = (TD*) dst;
    TS *sc = (TS*) src;
    const TS *scEnd = s + pixCount * ChannelCount(CS);

    for( ; sc < scEnd; sc+=ChannelCount(CS), ds+=ChannelCount(CS))
        {
        ConvertDown<TS,TD,CS>(sc,ds);
        }	
    }

// 16U8U C3R
template <class TS, class TD, CH CS, IsAlign ia>
ISV Convert_16u8u_C3R_Custom_SSE2_I (const TS *s, TD *d, U32 &pixCount)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;
    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(CS));
    __m128i zero = _mm_setzero_si128();
    XMM128 dstReg1;    

    dstReg1.i = _mm_setzero_si128();

    for(; (src+1) < end; src+=2, dst++)
        {
        convert16u8u_Helper<ia>(src, src+1,zero,dstReg1.i);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg1, (void*)dst);
        }

    TD *ds = (TD*) dst;
    TS *sc = (TS*) src;
    const TS *scEnd = s + pixCount * ChannelCount(CS);

    for( ; sc < scEnd; sc++, ds++)    // processing the third register as C1R
        {
        ConvertDown<TS,TD,C1>(sc,ds);
        }	
    }

// 16U8U AC4R
template <class TS, class TD, CH CS, IsAlign ia>
SYS_INLINE STATIC void Convert_16u8uAC4_Custom_SSE2_I (const TS *s, TD *d, U32 &pixCount, const __m128i &mask)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;
    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(CS));
    __m128i zero = _mm_setzero_si128();
    XMM128 dstReg1, dstReg2;
    
    dstReg1.i = _mm_setzero_si128();
    dstReg2.i = _mm_setzero_si128();

    for( ; (src+3) < end; src+= 4, dst+=2)
        {
        convert16u8u_Helper<ia>(src, src+1, src+2, src+3,zero,dstReg1.i, dstReg2.i);
        dstReg1.i = _mm_and_si128 (dstReg1.i,mask);
        dstReg2.i = _mm_and_si128 (dstReg2.i,mask);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg1, (void*)dst);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg2, (void*)(dst+1));
        }

    for(; (src+1) < end; src+=2, dst++)
        {
        convert16u8u_Helper<ia>(src, src+1,zero,dstReg1.i);
        dstReg1.i = _mm_and_si128 (dstReg1.i,mask);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg1, (void*)dst);
        }

    TD *ds = (TD*) dst;
    TS *sc = (TS*) src;
    const TS *scEnd = s + pixCount * ChannelCount(CS);
    for( ; sc < scEnd; sc+=ChannelCount(CS), ds+=ChannelCount(CS))
        {
        ConvertDown<TS,TD,C3>(sc,ds);
        }
    }

// 32S8U C1R, C3R, C4R
template<IsAlign ia>
ISV convert_32s8u_Helper(__m128i *src1, __m128i *src2, __m128i *src3, __m128i *src4, __m128i &dst)
    {
    XMM128 reg1, reg2, reg3, reg4;   

    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg1, (const void*)src1);
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg2, (const void*)src2);
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg3, (const void*)src3);
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg4, (const void*)src4);

    __m128i temp = _mm_packs_epi32 (reg1.i,reg2.i);
    __m128i temp1 = _mm_packs_epi32 (reg3.i,reg4.i);

    dst = _mm_packus_epi16(temp, temp1);
    }

template <class TS, class TD, CH CS, IsAlign ia>
ISV Convert_32s8u_Custom_SSE2_I (const TS *s, TD *d, U32 &pixCount)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;
    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(CS));
    XMM128 dstReg;
    
    dstReg.i = _mm_setzero_si128();
    for( ; (src+3) < end; src+= 4, dst+=1)
        {
        convert_32s8u_Helper<ia>(src,src+1,src+2,src+3,dstReg.i);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg, (void*)dst);
        }	

    TD *ds = (TD*) dst;
    TS *sc = (TS*) src;
    const TS *srcEnd = s + pixCount * ChannelCount(CS);
#define TMP_INC (ChannelCount(CS) - 1)
    for( ; (sc + TMP_INC) < srcEnd; sc+=ChannelCount(CS), ds+=ChannelCount(CS))
        {
        ConvertDown<TS,TD,CS>(sc,ds);
        }
#undef TMP_INC
    for(; sc < srcEnd; sc+=1, ds+=1)
        {
        *ds = FW_REF::Limits<TD>::Sat( *sc);
        }	
    }

// 32S8U AC4R
template <class TS, class TD, CH CS, IsAlign ia>
ISV Convert_32s8uAC4_Custom_SSE2_I (const TS *s, TD *d, U32 &pixCount, const __m128i &mask)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;
    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(CS));
    XMM128 dstReg;
    
    dstReg.i = _mm_setzero_si128();

    for( ; (src+3) < end; src+= 4, dst+=1)
        {
        convert_32s8u_Helper<ia>(src,src+1,src+2,src+3,dstReg.i);
        dstReg.i = _mm_and_si128 (dstReg.i,mask);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg, (void*)dst);
        }	

    TD *ds = (TD*) dst;
    TS *sc = (TS*) src;
    const TS *scEnd = s + pixCount * ChannelCount(CS);

    int nChannels = ChannelCount(CS);
    int nCmp = nChannels-1;

    for( ; (sc + nCmp) < scEnd; sc+=nChannels, ds+=nChannels)
        {
        ConvertDown<TS,TD,C3>(sc,ds);
        }
    }

// 32S8S C1R, C3R, C4R
template<IsAlign ia>
ISV convert_32s8s_Helper(__m128i *src1, __m128i *src2, __m128i *src3, __m128i *src4, __m128i &dst)
    {
    XMM128 reg1, reg2, reg3, reg4;    

    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg1, (const void*)src1);
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg2, (const void*)src2);
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg3, (const void*)src3);
    LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&reg4, (const void*)src4);
    
    __m128i temp = _mm_packs_epi32 (reg1.i,reg2.i);
    __m128i temp1 = _mm_packs_epi32 (reg3.i,reg4.i);

    dst =_mm_packs_epi16(temp, temp1);
    }

template <class TS, class TD, CH CS, IsAlign ia>
ISV Convert_32s8s_Custom_SSE2_I (const TS *s, TD *d, U32 &pixCount)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;
    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(CS));
    XMM128 dstReg;    

    dstReg.i = _mm_setzero_si128();
    for( ; (src+3) < end; src+= 4, dst+=1)
        {		
        convert_32s8s_Helper<ia>(src,src+1,src+2,src+3,dstReg.i);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg, (void*)dst);
        }

    TD *ds = (TD*) dst;
    TS *sc = (TS*) src;
    const TS *srcEnd	= s + pixCount * ChannelCount(CS);

#define TMP_INC (ChannelCount(CS) - 1)
    for( ; (sc + TMP_INC) < srcEnd; sc+=ChannelCount(CS), ds+=ChannelCount(CS))
        {
        ConvertDown<TS,TD,CS>(sc,ds);
        }
#undef TMP_INC

    for(; sc < srcEnd; sc+=1, ds+=1)
        {
        *ds = FW_REF::Limits<TD>::Sat( *sc);
        }
    }

// 32S8S AC4R
template <class TS, class TD, CH CS, IsAlign ia>
ISV Convert_32s8sAC4_Custom_SSE2_I (const TS *s, TD *d, U32 &pixCount, const __m128i &mask)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;

    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(CS));
    XMM128 dstReg;
    

    dstReg.i = _mm_setzero_si128();

    for( ; (src+3) < end; src+= 4, dst+=1)
        {
        convert_32s8s_Helper<ia>(src, src+1, src+2,src+3,dstReg.i);
        dstReg.i = _mm_and_si128 (dstReg.i,mask);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dstReg, (void*)dst);
        }

    TD *ds = (TD*) dst;
    TS *sc = (TS*) src;
    const TS *scEnd = s + pixCount * ChannelCount(CS);
    int nChannels = ChannelCount(CS);
    int nCmp = nChannels-1;

    for( ; (sc + nCmp) < scEnd; sc+=nChannels, ds+=nChannels)
        {
        ConvertDown<TS,TD,C3>(sc,ds);
        }
    }

}; // OPT_LEVEL

#endif



