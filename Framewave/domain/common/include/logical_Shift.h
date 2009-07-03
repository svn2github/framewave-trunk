/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fe.h"

namespace OPT_LEVEL
{
struct AC4_Mask
{
    XMM128 posMask, negMask;
};

//Initialization funtions for Shift routines
SYS_INLINE STATIC void Shift_8u_C4_init(XMM128 chMask[])
{
    chMask[0].i= _mm_set1_epi32(0x000000FF);
    chMask[1].i= _mm_set1_epi32(0x0000FF00);
    chMask[2].i= _mm_set1_epi32(0x00FF0000);
    chMask[3].i= _mm_set1_epi32(0xFF000000);
}

SYS_INLINE STATIC void Shift_8u_C3_init(XMM128 chMask[], const Fw32u value[])
{
    chMask[0].i= _mm_setr_epi32(0xFF0000FF, 0x00FF0000, 0x0000FF00, 0xFF0000FF);
    chMask[1].i = _mm_shuffle_epi32( chMask[0].i, _MM_SHUFFLE( 2,1,0,2 ) );
    chMask[2].i = _mm_shuffle_epi32( chMask[0].i, _MM_SHUFFLE( 1,0,2,1 ) );
    chMask[3].i = _mm_cvtsi32_si128    ( *(((S32 *)value)+2)        );            // upper 32-bit
    chMask[3].i = _mm_slli_si128       ( chMask[3].i, 8             );            // shift by 8 bytes (64-bits)
    chMask[3].f = _mm_loadl_pi         ( chMask[3].f, (__m64 *)value);            // lower 64-bit
}

SYS_INLINE STATIC void Shift_8u_AC4_init(AC4_Mask &mask, XMM128 &value, Fw32u v[])
{
    value.i= _mm_setr_epi32(v[0], 0, 0, 0);

    mask.posMask.i = _mm_set1_epi32(0xFFFFFFFF);
    mask.negMask   = mask.posMask;
    mask.posMask.i = _mm_slli_epi32(mask.posMask.i,8);
    mask.posMask.i = _mm_srli_epi32(mask.posMask.i,8);
    mask.negMask.i = _mm_srli_epi32(mask.negMask.i,24);
    mask.negMask.i = _mm_slli_epi32(mask.negMask.i,24);
}

SYS_INLINE STATIC void Shift_16u_C4_init(XMM128 chMask[], const Fw32u value[])
{
    chMask[0].i = _mm_setzero_si128();
    chMask[1].i = _mm_setzero_si128();
    chMask[2].i = _mm_setzero_si128();
    chMask[3].i = _mm_setzero_si128();
    chMask[0].u32[0]= value[0];
    chMask[1].u32[0]= value[1];
    chMask[2].u32[0]= value[2];
    chMask[3].u32[0]= value[3];
    chMask[4].i= _mm_setr_epi32(0x0000FFFF, 0, 0x0000FFFF, 0);
    chMask[5].i= _mm_setr_epi32(0xFFFF0000, 0, 0xFFFF0000, 0);
    chMask[6].i= _mm_setr_epi32(0, 0x0000FFFF, 0, 0x0000FFFF);
    chMask[7].i= _mm_setr_epi32(0, 0xFFFF0000, 0, 0xFFFF0000);
}

SYS_INLINE STATIC void Shift_16u_C3_init(XMM128 chMask[], const Fw32u value[])
{
    chMask[0].i= _mm_setr_epi32(0x0000FFFF, 0xFFFF0000, 0x00000000, 0x0000FFFF);
    chMask[1].i= _mm_setr_epi32(0xFFFF0000, 0x00000000, 0x0000FFFF, 0xFFFF0000);
    chMask[2].i= _mm_setr_epi32(0x00000000, 0x0000FFFF, 0xFFFF0000, 0x00000000);
    chMask[3].i= _mm_setr_epi32(value[0], value[1], value[2], 0x00000000);
}

SYS_INLINE STATIC void Shift_32s_C4_init(XMM128 chMask[], const Fw32u value[])
{
    chMask[0].i = _mm_setzero_si128();
    chMask[1].i = _mm_setzero_si128();
    chMask[2].i = _mm_setzero_si128();
    chMask[3].i = _mm_setzero_si128();
    chMask[0].u32[0]= value[0];
    chMask[1].u32[0]= value[1];
    chMask[2].u32[0]= value[2];
    chMask[3].u32[0]= value[3];
    chMask[4].i= _mm_setr_epi32(0xFFFFFFFF, 0, 0, 0);
    chMask[5].i= _mm_setr_epi32(0, 0xFFFFFFFF, 0, 0);
    chMask[6].i= _mm_setr_epi32(0, 0, 0xFFFFFFFF, 0);
    chMask[7].i= _mm_setr_epi32(0, 0, 0, 0xFFFFFFFF);
}

SYS_INLINE STATIC void Shift_32s_AC4_init(AC4_Mask &mask, XMM128 &value, Fw32u v[])
{
    value.i = _mm_setzero_si128();
    value.s32[0] = v[0];
    mask.posMask.i = _mm_setzero_si128();
    mask.posMask.i = _mm_cmpeq_epi32(mask.posMask.i,mask.posMask.i);
    mask.negMask.i = mask.posMask.i;
    mask.posMask.i = _mm_slli_si128 (mask.posMask.i,4);
    mask.posMask.i = _mm_srli_si128(mask.posMask.i,4);
    mask.negMask.i = _mm_srli_si128(mask.negMask.i,12);
    mask.negMask.i = _mm_slli_si128(mask.negMask.i,12);
}

SYS_INLINE STATIC void Shift_32s_C3_init(XMM128 chMask[], const Fw32u value[])
{
    chMask[0].i= _mm_setr_epi32(0xFFFFFFFF, 0, 0, 0xFFFFFFFF);
    chMask[1].i= _mm_setr_epi32(0, 0xFFFFFFFF, 0, 0);
    chMask[2].i= _mm_setr_epi32(0, 0, 0xFFFFFFFF, 0);
    chMask[3].i= _mm_setr_epi32(value[0], value[1], value[2], 0x00000000);
}

template< class T >
SYS_INLINE STATIC void RShift_CH(const T * s, T * d, const Fw32u* value  )
{
    Fw32u num_bits =  (sizeof(T) * 8);
    if (*value >= num_bits) *d = (T)((*s ) >> num_bits);
    else *d = (T)((*s ) >> *value);
}

template< class T , CH cd>
SYS_INLINE STATIC void RShift( const T * s, T * d, const Fw32u value[] )
{
    switch ( cd )
    {
    case C4:
        RShift_CH( s+3, d+3, value+3 );
    case C3:
    case AC4:
        RShift_CH( s+2, d+2, value+2 );
        RShift_CH( s+1, d+1, value+1 );
    case C1:
        RShift_CH( s+0, d+0, value+0 );
    }
}

template< class T , CH cd>
SYS_INLINE STATIC void RShift( T * d, const Fw32u value[] )
{
    switch ( cd )
    {
    case C4:
        RShift_CH( d+3, d+3, value+3 );
    case C3:
    case AC4:
        RShift_CH( d+2, d+2, value+2 );
        RShift_CH( d+1, d+1, value+1 );
    case C1:
        RShift_CH( d+0, d+0, value+0 );
    }
}


template< class T >
SYS_INLINE STATIC void LShift_CH(const T * s, T * d, const Fw32u* value  )
{
    if (*value >= (sizeof(T) * 8)) *d= 0;
    else *d = (T)(*s << *value);
}

template< class T , CH cd>
SYS_INLINE STATIC void LShift( const T * s, T * d, const Fw32u value[] )
{
    switch ( cd )
    {
    case C4:
        LShift_CH( s+3, d+3, value+3 );
    case C3:
    case AC4:
        LShift_CH( s+2, d+2, value+2 );
        LShift_CH( s+1, d+1, value+1 );
    case C1:
        LShift_CH( s+0, d+0, value+0 );
    }
}

template< class T >
SYS_INLINE STATIC void LShift_CH(T * d, const Fw32u* value  )
{
    if (*value >= (sizeof(T) * 8)) *d= 0;
    else *d = (T)(*d << *value);
}

template< class T, CH cd >
SYS_INLINE STATIC void LShift( T * d, const Fw32u value[])
{
    switch ( cd )
    {
    case C4:
        LShift_CH( d+3, value+3  );
    case C3:
    case AC4:
        LShift_CH( d+2, value+2  );
        LShift_CH( d+1, value+1  );
    case C1:
        LShift_CH( d+0, value+0  );
    }
}


SYS_INLINE STATIC void RShift_8u_AC4_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u value[])
{
    XMM128 dst1, dst2, dst3, dst4;
    dst4.i = reg.dst[0].i;
    //separating each channel data into separate registers
    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);
    //Saving the Alpha channel data from destination
    dst4.i = _mm_and_si128(dst4.i, chMask[3].i);
    //align second channel data on 16bits boundary
    dst2.i    = _mm_srli_epi16 ( dst2.i, 8);
    XMM128 iv1, iv2, iv3;
    iv1.i = iv2.i = iv3.i = _mm_setzero_si128();
    //set the constant value in the lower 32 bits of the register
    iv1.u32[0] = (U32)(*(value+0));
    iv2.u32[0] = (U32)(*(value+1));
    iv3.u32[0] = (U32)(*(value+2));
    //shifting first channel data by a 32 bit const in iv1
    dst1.i    = _mm_srl_epi16 ( dst1.i, iv1.i);
    //shifting second channel data by a 32 bit const in iv2
    dst2.i    = _mm_srl_epi16 ( dst2.i, iv2.i);
    //shifting third channel data by a 32 bit const in iv3
    dst3.i    = _mm_srl_epi16 ( dst3.i, iv3.i);
    //Shift the second channel data to its original location
    dst2.i    = _mm_slli_epi16 ( dst2.i, 8);
    //Synthesizing data of all the four channels
    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);
}

SYS_INLINE STATIC void RShift_8u_AC4_I_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u value[])
{
    XMM128 dst1, dst2, dst3, dst4;
    dst4.i = reg.dst[0].i;
    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);
    dst4.i = _mm_and_si128(dst4.i, chMask[3].i);
    dst2.i    = _mm_srli_epi16 ( dst2.i, 8);
    
    XMM128 iv1, iv2, iv3;
    iv1.i = iv2.i = iv3.i = _mm_setzero_si128();
    iv1.u32[0] = (U32)(*(value+0));
    iv2.u32[0] = (U32)(*(value+1));
    iv3.u32[0] = (U32)(*(value+2));
    dst1.i = _mm_srl_epi16 ( dst1.i, iv1.i);
    dst2.i = _mm_srl_epi16 ( dst2.i, iv2.i);
    dst3.i = _mm_srl_epi16 ( dst3.i, iv3.i);
    dst2.i = _mm_slli_epi16 ( dst2.i, 8);
    
    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift8uC3_shift_unit(XMM128& dst, XMM128& value)
{
    XMM128 tmpDst;
    CBL_SSE2::Unpack8UTo16U(dst.i, tmpDst.i);
    dst.i    = _mm_srl_epi16 ( dst.i, value.i);
    tmpDst.i = _mm_srl_epi16 ( tmpDst.i, value.i);
    FW_SSE2::pack16STo8U(dst.i, tmpDst.i);
}

SYS_INLINE STATIC void RShift_8u_C3_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, val0, val1, val2;

    val0.i = _mm_slli_si128 ( chMask[3].i, 12);
    val0.i = _mm_srli_si128 ( val0.i, 12);

    val1.i = _mm_slli_si128 ( chMask[3].i, 8);
    val1.i = _mm_srli_si128 ( val1.i, 12);

    val2.i = _mm_srli_si128 (  chMask[3].i, 8);

    //First Register
    //separating each channel data into separate registers
    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);
    //shifting first channel data
    RShift8uC3_shift_unit(dst1, val0);
    //shifting second channel data
    RShift8uC3_shift_unit(dst2, val1);
    //shifting third channel data
    RShift8uC3_shift_unit(dst3, val2);

    //Synthesizing data of all the four channels
    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.src1[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[1].i, chMask[2].i);

    RShift8uC3_shift_unit(dst1, val1);
    RShift8uC3_shift_unit(dst2, val2);
    RShift8uC3_shift_unit(dst3, val0);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);

    //Third Register
    dst1.i = _mm_and_si128(reg.src1[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[2].i, chMask[2].i);

    RShift8uC3_shift_unit(dst1, val2);
    RShift8uC3_shift_unit(dst2, val0);
    RShift8uC3_shift_unit(dst3, val1);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);
}


SYS_INLINE STATIC void RShift_8u_C3_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, val0, val1, val2;

    val0.i = _mm_slli_si128 ( chMask[3].i, 12);
    val0.i = _mm_srli_si128 ( val0.i, 12);

    val1.i = _mm_slli_si128 ( chMask[3].i, 8);
    val1.i = _mm_srli_si128 ( val1.i, 12);

    val2.i = _mm_srli_si128 (  chMask[3].i, 8);

    //First Register
    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);

    RShift8uC3_shift_unit(dst1, val0);
    RShift8uC3_shift_unit(dst2, val1);
    RShift8uC3_shift_unit(dst3, val2);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.dst[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[1].i, chMask[2].i);

    RShift8uC3_shift_unit(dst1, val1);
    RShift8uC3_shift_unit(dst2, val2);
    RShift8uC3_shift_unit(dst3, val0);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.dst[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[2].i, chMask[2].i);

    RShift8uC3_shift_unit(dst1, val2);

    RShift8uC3_shift_unit(dst2, val0);

    RShift8uC3_shift_unit(dst3, val1);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);

}


SYS_INLINE STATIC void RShift_8u_C4_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u value[])
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);
    dst4.i = _mm_and_si128(reg.src1[0].i, chMask[3].i);

    dst2.i = _mm_srli_epi16 ( dst2.i, 8);
    dst4.i = _mm_srli_epi16 ( dst4.i, 8);

    XMM128 iv1, iv2, iv3, iv4;
    iv1.i = iv2.i = iv3.i = iv4.i = _mm_setzero_si128();
    //iv1.u32[0] = (U32)(*(value+0));
    //iv2.u32[0] = (U32)(*(value+1));
    //iv3.u32[0] = (U32)(*(value+2));
    //iv4.u32[0] = (U32)(*(value+3));
    iv1.i = _mm_loadu_si128((__m128i*)value);
    iv4.i = iv3.i = iv2.i = iv1.i;
    iv4.i = _mm_srli_si128(iv4.i, 12);

    iv3.i = _mm_slli_si128(iv3.i, 4);
    iv3.i = _mm_srli_si128(iv3.i, 12);

    iv2.i = _mm_slli_si128(iv2.i, 8);
    iv2.i = _mm_srli_si128(iv2.i, 12);

    iv1.i = _mm_slli_si128(iv1.i, 12);
    iv1.i = _mm_srli_si128(iv1.i, 12);

    dst1.i = _mm_srl_epi16 ( dst1.i, iv1.i);
    dst2.i = _mm_srl_epi16 ( dst2.i, iv2.i);
    dst3.i = _mm_srl_epi16 ( dst3.i, iv3.i);
    dst4.i = _mm_srl_epi16 ( dst4.i, iv4.i);

    dst2.i = _mm_slli_epi16 ( dst2.i, 8);
    dst4.i = _mm_slli_epi16 ( dst4.i, 8);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);
}

SYS_INLINE STATIC void RShift_8u_C4_I_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u value[])
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[3].i);

    dst2.i = _mm_srli_epi16 ( dst2.i, 8);
    dst4.i = _mm_srli_epi16 ( dst4.i, 8);

    XMM128 iv1, iv2, iv3, iv4;
    iv1.i = iv2.i = iv3.i = iv4.i = _mm_setzero_si128();

    /*iv1.u32[0] = (U32)(*(value+0));
    iv2.u32[0] = (U32)(*(value+1));
    iv3.u32[0] = (U32)(*(value+2));
    iv4.u32[0] = (U32)(*(value+3));*/
    iv1.i = _mm_loadu_si128((__m128i*)value);
    iv4.i = iv3.i = iv2.i = iv1.i;
    iv4.i = _mm_srli_si128(iv4.i, 12);

    iv3.i = _mm_slli_si128(iv3.i, 4);
    iv3.i = _mm_srli_si128(iv3.i, 12);

    iv2.i = _mm_slli_si128(iv2.i, 8);
    iv2.i = _mm_srli_si128(iv2.i, 12);

    iv1.i = _mm_slli_si128(iv1.i, 12);
    iv1.i = _mm_srli_si128(iv1.i, 12);

    dst1.i = _mm_srl_epi16 ( dst1.i, iv1.i);
    dst2.i = _mm_srl_epi16 ( dst2.i, iv2.i);
    dst3.i = _mm_srl_epi16 ( dst3.i, iv3.i);
    dst4.i = _mm_srl_epi16 ( dst4.i, iv4.i);

    dst2.i = _mm_slli_epi16 ( dst2.i, 8);
    dst4.i = _mm_slli_epi16 ( dst4.i, 8);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift8uC3_eq_sse2(const XMM128 &value, RegFile &reg )
{
    //shifting data of all channels with the same constant value
    reg.dst[0].i = CBL_SSE2::RShift_8u(reg.src1[0].i, value.i);
    reg.dst[1].i = CBL_SSE2::RShift_8u(reg.src1[1].i, value.i);
    reg.dst[2].i = CBL_SSE2::RShift_8u(reg.src1[2].i, value.i);
}

SYS_INLINE STATIC void RShift8uC3_eq_I_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = CBL_SSE2::RShift_8u(reg.dst[0].i, value.i);
    reg.dst[1].i = CBL_SSE2::RShift_8u(reg.dst[1].i, value.i);
    reg.dst[2].i = CBL_SSE2::RShift_8u(reg.dst[2].i, value.i);
}

SYS_INLINE STATIC void RShift8uAC4_eq_sse2(const AC4_Mask &mask, RegFile &reg, const XMM128 &value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(reg.dst[0].i,mask.negMask.i);
    reg.dst[0].i = CBL_SSE2::RShift_8u(reg.src1[0].i, value.i);
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,mask.posMask.i);
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i,accumulator.i);
}

SYS_INLINE STATIC void RShift8uAC4_eq_I_sse2(const AC4_Mask &mask, RegFile &reg, const XMM128 &value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(reg.dst[0].i,mask.negMask.i);
    reg.dst[0].i = CBL_SSE2::RShift_8u(reg.dst[0].i, value.i);
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,mask.posMask.i);
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i,accumulator.i);
}

SYS_INLINE STATIC void RShift8sC3_shift_unit(XMM128& dst, XMM128& value)
{
    dst.i = CBL_SSE2::RShift_8s(dst.i, value.i);
}

SYS_INLINE STATIC void RShift_8s_C3_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, val0, val1, val2;

    val0.i = _mm_slli_si128 ( chMask[3].i, 12);
    val0.i = _mm_srli_si128 ( val0.i, 12);

    val1.i = _mm_slli_si128 ( chMask[3].i, 8);
    val1.i = _mm_srli_si128 ( val1.i, 12);

    val2.i = _mm_srli_si128 (  chMask[3].i, 8);

    //First Register
    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);

    RShift8sC3_shift_unit(dst1, val0);

    RShift8sC3_shift_unit(dst2, val1);

    RShift8sC3_shift_unit(dst3, val2);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);

    //Second Register
    dst1.i = _mm_and_si128(reg.src1[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[1].i, chMask[2].i);

    RShift8sC3_shift_unit(dst1, val1);

    RShift8sC3_shift_unit(dst2, val2);

    RShift8sC3_shift_unit(dst3, val0);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);

    //Third Register
    dst1.i = _mm_and_si128(reg.src1[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[2].i, chMask[2].i);

    RShift8sC3_shift_unit(dst1, val2);

    RShift8sC3_shift_unit(dst2, val0);

    RShift8sC3_shift_unit(dst3, val1);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);

}

SYS_INLINE STATIC void RShift_8s_C3_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, val0, val1, val2;

    val0.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    val0.i = _mm_srli_si128 ( val0.i, 12);

    val1.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    val1.i = _mm_srli_si128 ( val1.i, 12);

    val2.i = _mm_srli_si128 (  chMask[3].i, 8);//val 2

    //First Register
    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);

    RShift8sC3_shift_unit(dst1, val0);
    RShift8sC3_shift_unit(dst2, val1);
    RShift8sC3_shift_unit(dst3, val2);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.dst[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[1].i, chMask[2].i);

    RShift8sC3_shift_unit(dst1, val1);
    RShift8sC3_shift_unit(dst2, val2);
    RShift8sC3_shift_unit(dst3, val0);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.dst[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[2].i, chMask[2].i);

    RShift8sC3_shift_unit(dst1, val2);
    RShift8sC3_shift_unit(dst2, val0);
    RShift8sC3_shift_unit(dst3, val1);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);

}

SYS_INLINE STATIC void RShift_8s_AC4_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u* value )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);
    dst4.i = reg.dst[0].i;

    dst2.i = _mm_srai_epi16 ( dst2.i, 8);

    dst1.i = _mm_slli_epi16 ( dst1.i, 8);
    dst3.i = _mm_slli_epi16 ( dst3.i, 8);

    dst1.i = _mm_srai_epi16 ( dst1.i, 8);//sign extension
    dst3.i = _mm_srai_epi16 ( dst3.i, 8);

    XMM128 iv1, iv2, iv3;
    iv1.i = iv2.i = iv3.i = _mm_setzero_si128();
    iv1.u32[0] = (U32)(*(value+0));
    iv2.u32[0] = (U32)(*(value+1));
    iv3.u32[0] = (U32)(*(value+2));

    dst1.i = _mm_sra_epi16 ( dst1.i, iv1.i);
    dst2.i = _mm_sra_epi16 ( dst2.i, iv2.i);
    dst3.i = _mm_sra_epi16 ( dst3.i, iv3.i);

    dst2.i = _mm_slli_epi16 ( dst2.i, 8);

    dst1.i = _mm_and_si128(dst1.i, chMask[0].i);
    dst2.i = _mm_and_si128(dst2.i, chMask[1].i);
    dst3.i = _mm_and_si128(dst3.i, chMask[2].i);
    dst4.i = _mm_and_si128(dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);

    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);
}

SYS_INLINE STATIC void RShift_8s_AC4_I_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u* value )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);
    dst4.i = reg.dst[0].i;

    dst2.i = _mm_srai_epi16 ( dst2.i, 8);

    dst1.i = _mm_slli_epi16 ( dst1.i, 8);
    dst3.i = _mm_slli_epi16 ( dst3.i, 8);

    dst1.i = _mm_srai_epi16 ( dst1.i, 8);//sign extension
    dst3.i = _mm_srai_epi16 ( dst3.i, 8);

    XMM128 iv1, iv2, iv3;
    iv1.i = iv2.i = iv3.i = _mm_setzero_si128();
    iv1.u32[0] = (U32)(*(value+0));
    iv2.u32[0] = (U32)(*(value+1));
    iv3.u32[0] = (U32)(*(value+2));

    dst1.i = _mm_sra_epi16 ( dst1.i, iv1.i);
    dst2.i = _mm_sra_epi16 ( dst2.i, iv2.i);
    dst3.i = _mm_sra_epi16 ( dst3.i, iv3.i);
    dst2.i = _mm_slli_epi16 ( dst2.i, 8);

    dst1.i = _mm_and_si128(dst1.i, chMask[0].i);
    dst2.i = _mm_and_si128(dst2.i, chMask[1].i);
    dst3.i = _mm_and_si128(dst3.i, chMask[2].i);
    dst4.i = _mm_and_si128(dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);

    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);
}

SYS_INLINE STATIC void RShift_8s_C4_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u *value )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);
    dst4.i = _mm_and_si128(reg.src1[0].i, chMask[3].i);

    dst2.i = _mm_srai_epi16 ( dst2.i, 8);
    dst4.i = _mm_srai_epi16 ( dst4.i, 8);

    dst1.i = _mm_slli_epi16 ( dst1.i, 8);
    dst3.i = _mm_slli_epi16 ( dst3.i, 8);

    dst1.i = _mm_srai_epi16 ( dst1.i, 8);//sign extension
    dst3.i = _mm_srai_epi16 ( dst3.i, 8);

    XMM128 iv1, iv2, iv3, iv4;
    iv1.i = iv2.i = iv3.i = iv4.i = _mm_setzero_si128();

    /*iv1.u32[0] = (U32)(*(value+0));
    iv2.u32[0] = (U32)(*(value+1));
    iv3.u32[0] = (U32)(*(value+2));
    iv4.u32[0] = (U32)(*(value+3));*/
    iv1.i = _mm_loadu_si128((__m128i*)value);
    iv4.i = iv3.i = iv2.i = iv1.i;
    iv4.i = _mm_srli_si128(iv4.i, 12);

    iv3.i = _mm_slli_si128(iv3.i, 4);
    iv3.i = _mm_srli_si128(iv3.i, 12);

    iv2.i = _mm_slli_si128(iv2.i, 8);
    iv2.i = _mm_srli_si128(iv2.i, 12);

    iv1.i = _mm_slli_si128(iv1.i, 12);
    iv1.i = _mm_srli_si128(iv1.i, 12);


    dst1.i = _mm_sra_epi16 ( dst1.i, iv1.i);

    dst2.i = _mm_sra_epi16 ( dst2.i, iv2.i);

    dst3.i = _mm_sra_epi16 ( dst3.i, iv3.i);

    dst4.i = _mm_sra_epi16 ( dst4.i, iv4.i);

    dst2.i = _mm_slli_epi16 ( dst2.i, 8);
    dst4.i = _mm_slli_epi16 ( dst4.i, 8);

    dst1.i = _mm_and_si128(dst1.i, chMask[0].i);
    dst2.i = _mm_and_si128(dst2.i, chMask[1].i);
    dst3.i = _mm_and_si128(dst3.i, chMask[2].i);
    dst4.i = _mm_and_si128(dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);

    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift_8s_C4_I_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u *value )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[3].i);

    dst2.i = _mm_srai_epi16 ( dst2.i, 8);
    dst4.i = _mm_srai_epi16 ( dst4.i, 8);

    dst1.i = _mm_slli_epi16 ( dst1.i, 8);
    dst3.i = _mm_slli_epi16 ( dst3.i, 8);

    dst1.i = _mm_srai_epi16 ( dst1.i, 8);//sign extension
    dst3.i = _mm_srai_epi16 ( dst3.i, 8);

    XMM128 iv1, iv2, iv3, iv4;
    iv1.i = iv2.i = iv3.i = iv4.i = _mm_setzero_si128();
    //iv1.u32[0] = (U32)(*(value+0));
    //iv2.u32[0] = (U32)(*(value+1));
    //iv3.u32[0] = (U32)(*(value+2));
    //iv4.u32[0] = (U32)(*(value+3));

    iv1.i = _mm_loadu_si128((__m128i*)value);
    iv4.i = iv3.i = iv2.i = iv1.i;
    iv4.i = _mm_srli_si128(iv4.i, 12);

    iv3.i = _mm_slli_si128(iv3.i, 4);
    iv3.i = _mm_srli_si128(iv3.i, 12);

    iv2.i = _mm_slli_si128(iv2.i, 8);
    iv2.i = _mm_srli_si128(iv2.i, 12);

    iv1.i = _mm_slli_si128(iv1.i, 12);
    iv1.i = _mm_srli_si128(iv1.i, 12);

    dst1.i = _mm_sra_epi16 ( dst1.i, iv1.i);

    dst2.i = _mm_sra_epi16 ( dst2.i, iv2.i);

    dst3.i = _mm_sra_epi16 ( dst3.i, iv3.i);

    dst4.i = _mm_sra_epi16 ( dst4.i, iv4.i);

    dst2.i = _mm_slli_epi16 ( dst2.i, 8);
    dst4.i = _mm_slli_epi16 ( dst4.i, 8);

    dst1.i = _mm_and_si128(dst1.i, chMask[0].i);
    dst2.i = _mm_and_si128(dst2.i, chMask[1].i);
    dst3.i = _mm_and_si128(dst3.i, chMask[2].i);
    dst4.i = _mm_and_si128(dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);

    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}


SYS_INLINE STATIC void RShift8sC3_eq_sse2(const XMM128 &value, RegFile &reg )
{
    //shifting data of all channels with the same constant value
    reg.dst[0].i = CBL_SSE2::RShift_8s(reg.src1[0].i, value.i);
    reg.dst[1].i = CBL_SSE2::RShift_8s(reg.src1[1].i, value.i);
    reg.dst[2].i = CBL_SSE2::RShift_8s(reg.src1[2].i, value.i);
}

SYS_INLINE STATIC void RShift8sC3_eq_I_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = CBL_SSE2::RShift_8s(reg.dst[0].i, value.i);
    reg.dst[1].i = CBL_SSE2::RShift_8s(reg.dst[1].i, value.i);
    reg.dst[2].i = CBL_SSE2::RShift_8s(reg.dst[2].i, value.i);
}

SYS_INLINE STATIC void RShift8sAC4_eq_sse2(const AC4_Mask &mask, RegFile &reg, const XMM128 &value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(reg.dst[0].i,mask.negMask.i);
    reg.dst[0].i = CBL_SSE2::RShift_8s(reg.src1[0].i, value.i);
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,mask.posMask.i);
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i,accumulator.i);
}

SYS_INLINE STATIC void RShift8sAC4_eq_I_sse2(const AC4_Mask &mask, RegFile &reg, const XMM128 &value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(reg.dst[0].i,mask.negMask.i);
    reg.dst[0].i = CBL_SSE2::RShift_8s(reg.dst[0].i, value.i);
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,mask.posMask.i);
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i,accumulator.i);
}

//Rshift_16u

SYS_INLINE STATIC void RShift_16u_C3_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, accumulator;

    //First Register
    //separating each channel data into separate registers
    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    //shifting first channel data
    dst1.i = _mm_srl_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    //shifting second channel data
    dst2.i = _mm_srl_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    //shifting third channel data
    dst3.i = _mm_srl_epi16 ( dst3.i, accumulator.i);

    //Synthesizing data of all the three channels
    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.src1[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[1].i, chMask[2].i);

    accumulator.i = _mm_srli_si128 ( chMask[3].i, 8);//val 2

    dst1.i = _mm_srl_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_srl_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_srl_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.src1[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[2].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_srl_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val2

    dst2.i = _mm_srl_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_srl_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);

}

SYS_INLINE STATIC void RShift_16u_C3_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, accumulator;

    //First Register
    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_srl_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_srl_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_srl_epi16 ( dst3.i, accumulator.i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i    = _mm_or_si128(dst3.i, dst1.i);

    //Second Register
    dst1.i = _mm_and_si128(reg.dst[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[1].i, chMask[2].i);

    accumulator.i = _mm_srli_si128 ( chMask[3].i, 8);//val 2

    dst1.i = _mm_srl_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_srl_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_srl_epi16 ( dst3.i, accumulator.i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i    = _mm_or_si128(dst3.i, dst1.i);

    //Third Register
    dst1.i = _mm_and_si128(reg.dst[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[2].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_srl_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val2

    dst2.i = _mm_srl_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_srl_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);

}

SYS_INLINE STATIC void RShift_16u_AC4_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    //separating each channel data into separate registers
    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[6].i);
    //Saving the Alpha channel data from destination
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    //shifting first channel data
    dst1.i = _mm_srl_epi16 ( dst1.i, chMask[0].i);
    //shifting second channel data
    dst2.i = _mm_srl_epi16 ( dst2.i, chMask[1].i);
    //shifting third channel data
    dst3.i = _mm_srl_epi16 ( dst3.i, chMask[2].i);

    //Synthesizing data of all the four channels
    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift_16u_AC4_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_srl_epi16 ( dst1.i, chMask[0].i);
    dst2.i = _mm_srl_epi16 ( dst2.i, chMask[1].i);
    dst3.i = _mm_srl_epi16 ( dst3.i, chMask[2].i);
    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift_16u_C4_sse2(RegFile &reg, const XMM128 chMask[] )
{

    XMM128 dst1, dst2, dst3, dst4;

    //separating each channel data into separate registers
    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.src1[0].i, chMask[7].i);
    //shifting first channel data
    dst1.i = _mm_srl_epi16 ( dst1.i, chMask[0].i);
    //shifting second channel data
    dst2.i = _mm_srl_epi16 ( dst2.i, chMask[1].i);
    //shifting third channel data
    dst3.i = _mm_srl_epi16 ( dst3.i, chMask[2].i);
    //shifting fourth channel data
    dst4.i = _mm_srl_epi16 ( dst4.i, chMask[3].i);
    //Synthesizing data of all the four channels
    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift_16u_C4_I_sse2(RegFile &reg, const XMM128 chMask[] )
{

    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_srl_epi16 ( dst1.i, chMask[0].i);
    dst2.i = _mm_srl_epi16 ( dst2.i, chMask[1].i);
    dst3.i = _mm_srl_epi16 ( dst3.i, chMask[2].i);
    dst4.i = _mm_srl_epi16 ( dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift16uC3_eq_sse2(const XMM128 &value, RegFile &reg )
{
    //shifting data of all channels with the same constant value
    reg.dst[0].i = _mm_srl_epi16 ( reg.src1[0].i, value.i);
    reg.dst[1].i = _mm_srl_epi16 ( reg.src1[1].i, value.i);
    reg.dst[2].i = _mm_srl_epi16 ( reg.src1[2].i, value.i);
}

SYS_INLINE STATIC void RShift16uC3_eq_I_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = _mm_srl_epi16 ( reg.dst[0].i, value.i);
    reg.dst[1].i = _mm_srl_epi16 ( reg.dst[1].i, value.i);
    reg.dst[2].i = _mm_srl_epi16 ( reg.dst[2].i, value.i);
}


//16s


SYS_INLINE STATIC void RShift_16s_C3_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, accumulator;

    //First Register
    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sra_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sra_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sra_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.src1[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[1].i, chMask[2].i);


    accumulator.i = _mm_srli_si128 ( chMask[3].i, 8);//val 2

    dst1.i = _mm_sra_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sra_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_sra_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.src1[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[2].i, chMask[2].i);


    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sra_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val2

    dst2.i = _mm_sra_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_sra_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);

}

SYS_INLINE STATIC void RShift_16s_C3_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, accumulator;

    //First Register
    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sra_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sra_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sra_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.dst[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[1].i, chMask[2].i);


    accumulator.i = _mm_srli_si128 ( chMask[3].i, 8);//val 2

    dst1.i = _mm_sra_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sra_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_sra_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.dst[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[2].i, chMask[2].i);


    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sra_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val2

    dst2.i = _mm_sra_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_sra_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);

}

SYS_INLINE STATIC void RShift_16s_AC4_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sra_epi16 ( dst1.i, chMask[0].i);

    dst2.i = _mm_sra_epi16 ( dst2.i, chMask[1].i);

    dst3.i = _mm_sra_epi16 ( dst3.i, chMask[2].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift_16s_AC4_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sra_epi16 ( dst1.i, chMask[0].i);
    dst2.i = _mm_sra_epi16 ( dst2.i, chMask[1].i);
    dst3.i = _mm_sra_epi16 ( dst3.i, chMask[2].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift_16s_C4_sse2(RegFile &reg, const XMM128 chMask[] )
{

    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.src1[0].i, chMask[7].i);

    dst1.i = _mm_sra_epi16 ( dst1.i, chMask[0].i);
    dst2.i = _mm_sra_epi16 ( dst2.i, chMask[1].i);
    dst3.i = _mm_sra_epi16 ( dst3.i, chMask[2].i);
    dst4.i = _mm_sra_epi16 ( dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift_16s_C4_I_sse2(RegFile &reg, const XMM128 chMask[] )
{

    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sra_epi16 ( dst1.i, chMask[0].i);
    dst2.i = _mm_sra_epi16 ( dst2.i, chMask[1].i);
    dst3.i = _mm_sra_epi16 ( dst3.i, chMask[2].i);
    dst4.i = _mm_sra_epi16 ( dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift16sC3_eq_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = _mm_sra_epi16 ( reg.src1[0].i, value.i);
    reg.dst[1].i = _mm_sra_epi16 ( reg.src1[1].i, value.i);
    reg.dst[2].i = _mm_sra_epi16 ( reg.src1[2].i, value.i);
}

SYS_INLINE STATIC void RShift16sC3_eq_I_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = _mm_sra_epi16 ( reg.dst[0].i, value.i);
    reg.dst[1].i = _mm_sra_epi16 ( reg.dst[1].i, value.i);
    reg.dst[2].i = _mm_sra_epi16 ( reg.dst[2].i, value.i);
}

SYS_INLINE STATIC void RShift32sC3_eq_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = _mm_sra_epi32 ( reg.src1[0].i,value.i);
    reg.dst[1].i = _mm_sra_epi32 ( reg.src1[1].i,value.i);
    reg.dst[2].i = _mm_sra_epi32 ( reg.src1[2].i,value.i);
}

SYS_INLINE STATIC void RShift32sC3_eq_I_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = _mm_sra_epi32 ( reg.dst[0].i,value.i);
    reg.dst[1].i = _mm_sra_epi32 ( reg.dst[1].i,value.i);
    reg.dst[2].i = _mm_sra_epi32 ( reg.dst[2].i,value.i);
}


SYS_INLINE STATIC void RShift32sAC4_eq_sse2(const AC4_Mask &mask, RegFile &reg , const XMM128 &value)
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(reg.dst[0].i,mask.negMask.i);
    reg.dst[0].i = _mm_sra_epi32 ( reg.src1[0].i,value.i);
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,mask.posMask.i);
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i,accumulator.i);
}

SYS_INLINE STATIC void RShift32sAC4_eq_I_sse2(const AC4_Mask &mask, RegFile &reg , const XMM128 &value)
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(reg.dst[0].i,mask.negMask.i);
    reg.dst[0].i = _mm_sra_epi32 ( reg.dst[0].i,value.i);
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,mask.posMask.i);
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i,accumulator.i);
}



SYS_INLINE STATIC void RShift_32s_C3_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, accumulator;

    //First Register
    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sra_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sra_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sra_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.src1[1].i, chMask[2].i);
    dst2.i = _mm_and_si128(reg.src1[1].i, chMask[0].i);
    dst3.i = _mm_and_si128(reg.src1[1].i, chMask[1].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sra_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sra_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sra_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.src1[2].i, chMask[1].i);
    dst2.i = _mm_and_si128(reg.src1[2].i, chMask[2].i);
    dst3.i = _mm_and_si128(reg.src1[2].i, chMask[0].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sra_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sra_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sra_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);
}

SYS_INLINE STATIC void RShift_32s_C3_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, accumulator;

    //First Register
    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sra_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sra_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sra_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.dst[1].i, chMask[2].i);
    dst2.i = _mm_and_si128(reg.dst[1].i, chMask[0].i);
    dst3.i = _mm_and_si128(reg.dst[1].i, chMask[1].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sra_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sra_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sra_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.dst[2].i, chMask[1].i);
    dst2.i = _mm_and_si128(reg.dst[2].i, chMask[2].i);
    dst3.i = _mm_and_si128(reg.dst[2].i, chMask[0].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sra_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sra_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sra_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);
}

SYS_INLINE STATIC void RShift_32s_AC4_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sra_epi32 ( dst1.i, chMask[0].i);
    dst2.i = _mm_sra_epi32 ( dst2.i, chMask[1].i);
    dst3.i = _mm_sra_epi32 ( dst3.i, chMask[2].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);
}

SYS_INLINE STATIC void RShift_32s_AC4_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sra_epi32 ( dst1.i, chMask[0].i);
    dst2.i = _mm_sra_epi32 ( dst2.i, chMask[1].i);
    dst3.i = _mm_sra_epi32 ( dst3.i, chMask[2].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);
}

SYS_INLINE STATIC void RShift_32s_C4_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.src1[0].i, chMask[7].i);

    dst1.i = _mm_sra_epi32 ( dst1.i, chMask[0].i);
    dst2.i = _mm_sra_epi32 ( dst2.i, chMask[1].i);
    dst3.i = _mm_sra_epi32 ( dst3.i, chMask[2].i);
    dst4.i = _mm_sra_epi32 ( dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void RShift_32s_C4_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sra_epi32 ( dst1.i, chMask[0].i);
    dst2.i = _mm_sra_epi32 ( dst2.i, chMask[1].i);
    dst3.i = _mm_sra_epi32 ( dst3.i, chMask[2].i);
    dst4.i = _mm_sra_epi32 ( dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}





SYS_INLINE STATIC void LShift_8u_AC4_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u value[])
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[3].i);

    dst1.i = _mm_slli_epi16 ( dst1.i, 8);
    dst3.i = _mm_slli_epi16 ( dst3.i, 8);

    XMM128 iv1, iv2, iv3;
    iv1.i = iv2.i = iv3.i = _mm_setzero_si128();
    iv1.u32[0] = (U32)(*(value+0));
    iv2.u32[0] = (U32)(*(value+1));
    iv3.u32[0] = (U32)(*(value+2));

    dst1.i = _mm_sll_epi16 ( dst1.i, iv1.i);
    dst2.i = _mm_sll_epi16 ( dst2.i, iv2.i);
    dst3.i = _mm_sll_epi16 ( dst3.i, iv3.i);

    dst1.i = _mm_srli_epi16 ( dst1.i, 8);
    dst3.i = _mm_srli_epi16 ( dst3.i, 8);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void LShift_8u_AC4_I_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u value[])
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[3].i);

    dst1.i = _mm_slli_epi16 ( dst1.i, 8);
    dst3.i = _mm_slli_epi16 ( dst3.i, 8);

    XMM128 iv1, iv2, iv3;
    iv1.i = iv2.i = iv3.i = _mm_setzero_si128();
    iv1.u32[0] = (U32)(*(value+0));
    iv2.u32[0] = (U32)(*(value+1));
    iv3.u32[0] = (U32)(*(value+2));


    dst1.i = _mm_sll_epi16 ( dst1.i, iv1.i);
    dst2.i = _mm_sll_epi16 ( dst2.i, iv2.i);
    dst3.i = _mm_sll_epi16 ( dst3.i, iv3.i);

    dst1.i = _mm_srli_epi16 ( dst1.i, 8);
    dst3.i = _mm_srli_epi16 ( dst3.i, 8);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);


}

SYS_INLINE STATIC void LShift8uC3_shift_unit(XMM128& dst, XMM128& value)
{
    XMM128 tmpDst;
    CBL_SSE2::Unpack8UTo16U(dst.i, tmpDst.i);
    dst.i    = _mm_srl_epi16 ( dst.i, value.i);
    tmpDst.i = _mm_srl_epi16 ( tmpDst.i, value.i);
    FW_SSE2::pack16STo8U(dst.i, tmpDst.i);
}

SYS_INLINE STATIC void LShift_8u_C3_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst1_2, dst2_2, dst3_2, accumulator;
    XMM128 mask;

    mask.i = _mm_setzero_si128();
    mask.i = _mm_cmpeq_epi32(mask.i,mask.i);
    mask.i = _mm_slli_epi16 ( mask.i, 8);
    mask.i = _mm_srli_epi16 ( mask.i, 8);

    //First Register

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);

    CBL_SSE2::Unpack8UTo16U(dst1.i, dst1_2.i);
    CBL_SSE2::Unpack8UTo16U(dst2.i, dst2_2.i);
    CBL_SSE2::Unpack8UTo16U(dst3.i, dst3_2.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i   = _mm_sll_epi16 ( dst1.i, accumulator.i);
    dst1.i   = _mm_and_si128 ( dst1.i, mask.i);
    dst1_2.i = _mm_sll_epi16 ( dst1_2.i, accumulator.i);
    dst1_2.i = _mm_and_si128 ( dst1_2.i, mask.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i   = _mm_sll_epi16 ( dst2.i, accumulator.i);
    dst2.i   = _mm_and_si128 ( dst2.i, mask.i);
    dst2_2.i = _mm_sll_epi16 ( dst2_2.i, accumulator.i);
    dst2_2.i = _mm_and_si128 ( dst2_2.i, mask.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val 2

    dst3.i   = _mm_sll_epi16 ( dst3.i, accumulator.i);
    dst3.i   = _mm_and_si128 ( dst3.i, mask.i);
    dst3_2.i = _mm_sll_epi16 ( dst3_2.i, accumulator.i);
    dst3_2.i = _mm_and_si128 ( dst3_2.i, mask.i);

    FW_SSE2::pack16STo8U(dst1.i, dst1_2.i);
    FW_SSE2::pack16STo8U(dst2.i, dst2_2.i);
    FW_SSE2::pack16STo8U(dst3.i, dst3_2.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.src1[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[1].i, chMask[2].i);

    CBL_SSE2::Unpack8UTo16U(dst1.i, dst1_2.i);
    CBL_SSE2::Unpack8UTo16U(dst2.i, dst2_2.i);
    CBL_SSE2::Unpack8UTo16U(dst3.i, dst3_2.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i   = _mm_sll_epi16 ( dst1.i, accumulator.i);
    dst1.i   = _mm_and_si128 ( dst1.i, mask.i);
    dst1_2.i = _mm_sll_epi16 ( dst1_2.i, accumulator.i);
    dst1_2.i = _mm_and_si128 ( dst1_2.i, mask.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val 2

    dst2.i   = _mm_sll_epi16 ( dst2.i, accumulator.i);
    dst2.i   = _mm_and_si128 ( dst2.i, mask.i);
    dst2_2.i = _mm_sll_epi16 ( dst2_2.i, accumulator.i);
    dst2_2.i = _mm_and_si128 ( dst2_2.i, mask.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i   = _mm_sll_epi16 ( dst3.i, accumulator.i);
    dst3.i   = _mm_and_si128 ( dst3.i, mask.i);
    dst3_2.i = _mm_sll_epi16 ( dst3_2.i, accumulator.i);
    dst3_2.i = _mm_and_si128 ( dst3_2.i, mask.i);


    FW_SSE2::pack16STo8U(dst1.i, dst1_2.i);
    FW_SSE2::pack16STo8U(dst2.i, dst2_2.i);
    FW_SSE2::pack16STo8U(dst3.i, dst3_2.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.src1[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[2].i, chMask[2].i);

    CBL_SSE2::Unpack8UTo16U(dst1.i, dst1_2.i);
    CBL_SSE2::Unpack8UTo16U(dst2.i, dst2_2.i);
    CBL_SSE2::Unpack8UTo16U(dst3.i, dst3_2.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val 2

    dst1.i   = _mm_sll_epi16 ( dst1.i, accumulator.i);
    dst1.i   = _mm_and_si128 ( dst1.i, mask.i);
    dst1_2.i = _mm_sll_epi16 ( dst1_2.i, accumulator.i);
    dst1_2.i = _mm_and_si128 ( dst1_2.i, mask.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i   = _mm_sll_epi16 ( dst2.i, accumulator.i);
    dst2.i   = _mm_and_si128 ( dst2.i, mask.i);
    dst2_2.i = _mm_sll_epi16 ( dst2_2.i, accumulator.i);
    dst2_2.i = _mm_and_si128 ( dst2_2.i, mask.i);


    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i   = _mm_sll_epi16 ( dst3.i, accumulator.i);
    dst3.i   = _mm_and_si128 ( dst3.i, mask.i);
    dst3_2.i = _mm_sll_epi16 ( dst3_2.i, accumulator.i);
    dst3_2.i = _mm_and_si128 ( dst3_2.i, mask.i);


    FW_SSE2::pack16STo8U(dst1.i, dst1_2.i);
    FW_SSE2::pack16STo8U(dst2.i, dst2_2.i);
    FW_SSE2::pack16STo8U(dst3.i, dst3_2.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);

}


SYS_INLINE STATIC void LShift_8u_C3_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst1_2, dst2_2, dst3_2, accumulator;
    XMM128 mask;

    mask.i = _mm_setzero_si128();
    mask.i = _mm_cmpeq_epi32(mask.i,mask.i);
    mask.i = _mm_slli_epi16 ( mask.i, 8);
    mask.i = _mm_srli_epi16 ( mask.i, 8);

    //First Register

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);

    CBL_SSE2::Unpack8UTo16U(dst1.i, dst1_2.i);
    CBL_SSE2::Unpack8UTo16U(dst2.i, dst2_2.i);
    CBL_SSE2::Unpack8UTo16U(dst3.i, dst3_2.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i   = _mm_sll_epi16 ( dst1.i, accumulator.i);
    dst1.i   = _mm_and_si128 ( dst1.i, mask.i);
    dst1_2.i = _mm_sll_epi16 ( dst1_2.i, accumulator.i);
    dst1_2.i = _mm_and_si128 ( dst1_2.i, mask.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i   = _mm_sll_epi16 ( dst2.i, accumulator.i);
    dst2.i   = _mm_and_si128 ( dst2.i, mask.i);
    dst2_2.i = _mm_sll_epi16 ( dst2_2.i, accumulator.i);
    dst2_2.i = _mm_and_si128 ( dst2_2.i, mask.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val 2

    dst3.i   = _mm_sll_epi16 ( dst3.i, accumulator.i);
    dst3.i   = _mm_and_si128 ( dst3.i, mask.i);
    dst3_2.i = _mm_sll_epi16 ( dst3_2.i, accumulator.i);
    dst3_2.i = _mm_and_si128 ( dst3_2.i, mask.i);

    FW_SSE2::pack16STo8U(dst1.i, dst1_2.i);
    FW_SSE2::pack16STo8U(dst2.i, dst2_2.i);
    FW_SSE2::pack16STo8U(dst3.i, dst3_2.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.dst[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[1].i, chMask[2].i);

    CBL_SSE2::Unpack8UTo16U(dst1.i, dst1_2.i);
    CBL_SSE2::Unpack8UTo16U(dst2.i, dst2_2.i);
    CBL_SSE2::Unpack8UTo16U(dst3.i, dst3_2.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i   = _mm_sll_epi16 ( dst1.i, accumulator.i);
    dst1.i   = _mm_and_si128 ( dst1.i, mask.i);
    dst1_2.i = _mm_sll_epi16 ( dst1_2.i, accumulator.i);
    dst1_2.i = _mm_and_si128 ( dst1_2.i, mask.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val 2

    dst2.i   = _mm_sll_epi16 ( dst2.i, accumulator.i);
    dst2.i   = _mm_and_si128 ( dst2.i, mask.i);
    dst2_2.i = _mm_sll_epi16 ( dst2_2.i, accumulator.i);
    dst2_2.i = _mm_and_si128 ( dst2_2.i, mask.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i   = _mm_sll_epi16 ( dst3.i, accumulator.i);
    dst3.i   = _mm_and_si128 ( dst3.i, mask.i);
    dst3_2.i = _mm_sll_epi16 ( dst3_2.i, accumulator.i);
    dst3_2.i = _mm_and_si128 ( dst3_2.i, mask.i);


    FW_SSE2::pack16STo8U(dst1.i, dst1_2.i);
    FW_SSE2::pack16STo8U(dst2.i, dst2_2.i);
    FW_SSE2::pack16STo8U(dst3.i, dst3_2.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.dst[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[2].i, chMask[2].i);

    CBL_SSE2::Unpack8UTo16U(dst1.i, dst1_2.i);
    CBL_SSE2::Unpack8UTo16U(dst2.i, dst2_2.i);
    CBL_SSE2::Unpack8UTo16U(dst3.i, dst3_2.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val 2

    dst1.i   = _mm_sll_epi16 ( dst1.i, accumulator.i);
    dst1.i   = _mm_and_si128 ( dst1.i, mask.i);
    dst1_2.i = _mm_sll_epi16 ( dst1_2.i, accumulator.i);
    dst1_2.i = _mm_and_si128 ( dst1_2.i, mask.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i   = _mm_sll_epi16 ( dst2.i, accumulator.i);
    dst2.i   = _mm_and_si128 ( dst2.i, mask.i);
    dst2_2.i = _mm_sll_epi16 ( dst2_2.i, accumulator.i);
    dst2_2.i = _mm_and_si128 ( dst2_2.i, mask.i);


    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i   = _mm_sll_epi16 ( dst3.i, accumulator.i);
    dst3.i   = _mm_and_si128 ( dst3.i, mask.i);
    dst3_2.i = _mm_sll_epi16 ( dst3_2.i, accumulator.i);
    dst3_2.i = _mm_and_si128 ( dst3_2.i, mask.i);


    FW_SSE2::pack16STo8U(dst1.i, dst1_2.i);
    FW_SSE2::pack16STo8U(dst2.i, dst2_2.i);
    FW_SSE2::pack16STo8U(dst3.i, dst3_2.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);

}


SYS_INLINE STATIC void LShift_8u_C4_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u value[])
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);
    dst4.i = _mm_and_si128(reg.src1[0].i, chMask[3].i);

    dst1.i    = _mm_slli_epi16 ( dst1.i, 8);
    dst3.i    = _mm_slli_epi16 ( dst3.i, 8);

    XMM128 iv1, iv2, iv3, iv4;

    iv1.i = iv2.i = iv3.i = iv4.i = _mm_setzero_si128();

    //iv1.u32[0] = (U32)(*(value+0));
    //iv2.u32[0] = (U32)(*(value+1));
    //iv3.u32[0] = (U32)(*(value+2));
    //iv4.u32[0] = (U32)(*(value+3));
    iv1.i = _mm_loadu_si128((__m128i*)value);
    iv4.i = iv3.i = iv2.i = iv1.i;
    iv4.i = _mm_srli_si128(iv4.i, 12);

    iv3.i = _mm_slli_si128(iv3.i, 4);
    iv3.i = _mm_srli_si128(iv3.i, 12);

    iv2.i = _mm_slli_si128(iv2.i, 8);
    iv2.i = _mm_srli_si128(iv2.i, 12);

    iv1.i = _mm_slli_si128(iv1.i, 12);
    iv1.i = _mm_srli_si128(iv1.i, 12);

    dst1.i = _mm_sll_epi16 ( dst1.i, iv1.i);

    dst2.i = _mm_sll_epi16 ( dst2.i, iv2.i);

    dst3.i = _mm_sll_epi16 ( dst3.i, iv3.i);

    dst4.i = _mm_sll_epi16 ( dst4.i, iv4.i);

    dst1.i = _mm_srli_epi16 ( dst1.i, 8);
    dst3.i = _mm_srli_epi16 ( dst3.i, 8);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);
}

SYS_INLINE STATIC void LShift_8u_C4_I_sse2(RegFile &reg, const XMM128 chMask[], const Fw32u value[])
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[3].i);

    dst1.i    = _mm_slli_epi16 ( dst1.i, 8);
    dst3.i    = _mm_slli_epi16 ( dst3.i, 8);

    XMM128 iv1, iv2, iv3, iv4;
    iv1.i = iv2.i = iv3.i = iv4.i = _mm_setzero_si128();

    //iv1.u32[0] = (U32)(*(value+0));
    //iv2.u32[0] = (U32)(*(value+1));
    //iv3.u32[0] = (U32)(*(value+2));
    //iv4.u32[0] = (U32)(*(value+3));
    iv1.i = _mm_loadu_si128((__m128i*)value);
    iv4.i = iv3.i = iv2.i = iv1.i;
    iv4.i = _mm_srli_si128(iv4.i, 12);

    iv3.i = _mm_slli_si128(iv3.i, 4);
    iv3.i = _mm_srli_si128(iv3.i, 12);

    iv2.i = _mm_slli_si128(iv2.i, 8);
    iv2.i = _mm_srli_si128(iv2.i, 12);

    iv1.i = _mm_slli_si128(iv1.i, 12);
    iv1.i = _mm_srli_si128(iv1.i, 12);

    dst1.i = _mm_sll_epi16 ( dst1.i, iv1.i);

    dst2.i = _mm_sll_epi16 ( dst2.i, iv2.i);

    dst3.i = _mm_sll_epi16 ( dst3.i, iv3.i);

    dst4.i = _mm_sll_epi16 ( dst4.i, iv4.i);

    dst1.i    = _mm_srli_epi16 ( dst1.i, 8);
    dst3.i    = _mm_srli_epi16 ( dst3.i, 8);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);
}

SYS_INLINE STATIC void LShift8uC3_eq_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = CBL_SSE2::LShift_8u(reg.src1[0].i, value.i);
    reg.dst[1].i = CBL_SSE2::LShift_8u(reg.src1[1].i, value.i);
    reg.dst[2].i = CBL_SSE2::LShift_8u(reg.src1[2].i, value.i);
}

SYS_INLINE STATIC void LShift8uC3_eq_I_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = CBL_SSE2::LShift_8u(reg.dst[0].i, value.i);
    reg.dst[1].i = CBL_SSE2::LShift_8u(reg.dst[1].i, value.i);
    reg.dst[2].i = CBL_SSE2::LShift_8u(reg.dst[2].i, value.i);
}

SYS_INLINE STATIC void LShift8uAC4_eq_sse2(const AC4_Mask &mask, RegFile &reg, const XMM128 &value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(reg.dst[0].i,mask.negMask.i);
    reg.dst[0].i = CBL_SSE2::LShift_8u(reg.src1[0].i, value.i);
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,mask.posMask.i);
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i,accumulator.i);
}

SYS_INLINE STATIC void LShift8uAC4_eq_I_sse2(const AC4_Mask &mask, RegFile &reg, const XMM128 &value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(reg.dst[0].i,mask.negMask.i);
    reg.dst[0].i = CBL_SSE2::LShift_8u(reg.dst[0].i, value.i);
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,mask.posMask.i);
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i,accumulator.i);
}



//Rshift_16u

SYS_INLINE STATIC void LShift_16u_C3_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, accumulator;

    //First Register
    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sll_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sll_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sll_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.src1[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[1].i, chMask[2].i);


    accumulator.i = _mm_srli_si128 ( chMask[3].i, 8);//val 2

    dst1.i = _mm_sll_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sll_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_sll_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.src1[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[2].i, chMask[2].i);


    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sll_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val2

    dst2.i = _mm_sll_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_sll_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);

}

SYS_INLINE STATIC void LShift_16u_C3_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, accumulator;

    //First Register
    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sll_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sll_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sll_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.dst[1].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[1].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[1].i, chMask[2].i);


    accumulator.i = _mm_srli_si128 ( chMask[3].i, 8);//val 2

    dst1.i = _mm_sll_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sll_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val 1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_sll_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.dst[2].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[2].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[2].i, chMask[2].i);


    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);//val1
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sll_epi16 ( dst1.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);//val2

    dst2.i = _mm_sll_epi16 ( dst2.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);//val 0
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst3.i = _mm_sll_epi16 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);
}

SYS_INLINE STATIC void LShift_16u_AC4_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sll_epi16 ( dst1.i, chMask[0].i);

    dst2.i = _mm_sll_epi16 ( dst2.i, chMask[1].i);

    dst3.i = _mm_sll_epi16 ( dst3.i, chMask[2].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void LShift_16u_AC4_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sll_epi16 ( dst1.i, chMask[0].i);

    dst2.i = _mm_sll_epi16 ( dst2.i, chMask[1].i);

    dst3.i = _mm_sll_epi16 ( dst3.i, chMask[2].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void LShift_16u_C4_sse2(RegFile &reg, const XMM128 chMask[] )
{

    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.src1[0].i, chMask[7].i);

    dst1.i = _mm_sll_epi16 ( dst1.i, chMask[0].i);

    dst2.i = _mm_sll_epi16 ( dst2.i, chMask[1].i);

    dst3.i = _mm_sll_epi16 ( dst3.i, chMask[2].i);

    dst4.i = _mm_sll_epi16 ( dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void LShift_16u_C4_I_sse2(RegFile &reg, const XMM128 chMask[] )
{

    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sll_epi16 ( dst1.i, chMask[0].i);

    dst2.i = _mm_sll_epi16 ( dst2.i, chMask[1].i);

    dst3.i = _mm_sll_epi16 ( dst3.i, chMask[2].i);

    dst4.i = _mm_sll_epi16 ( dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);


}

SYS_INLINE STATIC void LShift16uC3_eq_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = _mm_sll_epi16 ( reg.src1[0].i, value.i);
    reg.dst[1].i = _mm_sll_epi16 ( reg.src1[1].i, value.i);
    reg.dst[2].i = _mm_sll_epi16 ( reg.src1[2].i, value.i);
}

SYS_INLINE STATIC void LShift16uC3_eq_I_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = _mm_sll_epi16 ( reg.dst[0].i, value.i);
    reg.dst[1].i = _mm_sll_epi16 ( reg.dst[1].i, value.i);
    reg.dst[2].i = _mm_sll_epi16 ( reg.dst[2].i, value.i);
}


//LShift 32s

SYS_INLINE STATIC void LShift32sC3_eq_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = _mm_sll_epi32 ( reg.src1[0].i,value.i);
    reg.dst[1].i = _mm_sll_epi32 ( reg.src1[1].i,value.i);
    reg.dst[2].i = _mm_sll_epi32 ( reg.src1[2].i,value.i);
}

SYS_INLINE STATIC void LShift32sC3_eq_I_sse2(const XMM128 &value, RegFile &reg )
{
    reg.dst[0].i = _mm_sll_epi32 ( reg.dst[0].i,value.i);
    reg.dst[1].i = _mm_sll_epi32 ( reg.dst[1].i,value.i);
    reg.dst[2].i = _mm_sll_epi32 ( reg.dst[2].i,value.i);
}


SYS_INLINE STATIC void LShift32sAC4_eq_sse2(const AC4_Mask &mask, RegFile &reg , const XMM128 &value)
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(reg.dst[0].i,mask.negMask.i);
    reg.dst[0].i = _mm_sll_epi32 ( reg.src1[0].i,value.i);
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,mask.posMask.i);
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i,accumulator.i);
}

SYS_INLINE STATIC void LShift32sAC4_eq_I_sse2(const AC4_Mask &mask, RegFile &reg , const XMM128 &value)
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(reg.dst[0].i,mask.negMask.i);
    reg.dst[0].i = _mm_sll_epi32 ( reg.dst[0].i,value.i);
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,mask.posMask.i);
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i,accumulator.i);
}



SYS_INLINE STATIC void LShift_32s_C3_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, accumulator;

    //First Register
    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sll_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sll_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sll_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.src1[1].i, chMask[2].i);
    dst2.i = _mm_and_si128(reg.src1[1].i, chMask[0].i);
    dst3.i = _mm_and_si128(reg.src1[1].i, chMask[1].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sll_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sll_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sll_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.src1[2].i, chMask[1].i);
    dst2.i = _mm_and_si128(reg.src1[2].i, chMask[2].i);
    dst3.i = _mm_and_si128(reg.src1[2].i, chMask[0].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sll_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sll_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sll_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);
}

SYS_INLINE STATIC void LShift_32s_C3_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, accumulator;

    //First Register
    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[0].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[1].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[2].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sll_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sll_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sll_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[0].i = _mm_or_si128(dst3.i, dst1.i);


    //Second Register
    dst1.i = _mm_and_si128(reg.dst[1].i, chMask[2].i);
    dst2.i = _mm_and_si128(reg.dst[1].i, chMask[0].i);
    dst3.i = _mm_and_si128(reg.dst[1].i, chMask[1].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sll_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sll_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sll_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[1].i = _mm_or_si128(dst3.i, dst1.i);


    //Third Register
    dst1.i = _mm_and_si128(reg.dst[2].i, chMask[1].i);
    dst2.i = _mm_and_si128(reg.dst[2].i, chMask[2].i);
    dst3.i = _mm_and_si128(reg.dst[2].i, chMask[0].i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 12);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst1.i = _mm_sll_epi32 ( dst1.i, accumulator.i);

    accumulator.i = _mm_slli_si128 ( chMask[3].i, 8);
    accumulator.i = _mm_srli_si128 ( accumulator.i, 12);

    dst2.i = _mm_sll_epi32 ( dst2.i, accumulator.i);

    accumulator.i = _mm_srli_si128 (  chMask[3].i, 8);

    dst3.i = _mm_sll_epi32 ( dst3.i, accumulator.i);

    dst1.i       = _mm_or_si128(dst1.i, dst2.i);
    reg.dst[2].i = _mm_or_si128(dst3.i, dst1.i);
}

SYS_INLINE STATIC void LShift_32s_AC4_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sll_epi32 ( dst1.i, chMask[0].i);

    dst2.i = _mm_sll_epi32 ( dst2.i, chMask[1].i);

    dst3.i = _mm_sll_epi32 ( dst3.i, chMask[2].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);
}

SYS_INLINE STATIC void LShift_32s_AC4_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sll_epi32 ( dst1.i, chMask[0].i);

    dst2.i = _mm_sll_epi32 ( dst2.i, chMask[1].i);

    dst3.i = _mm_sll_epi32 ( dst3.i, chMask[2].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);
}

SYS_INLINE STATIC void LShift_32s_C4_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.src1[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.src1[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.src1[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.src1[0].i, chMask[7].i);

    dst1.i = _mm_sll_epi32 ( dst1.i, chMask[0].i);

    dst2.i = _mm_sll_epi32 ( dst2.i, chMask[1].i);

    dst3.i = _mm_sll_epi32 ( dst3.i, chMask[2].i);

    dst4.i = _mm_sll_epi32 ( dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}

SYS_INLINE STATIC void LShift_32s_C4_I_sse2(RegFile &reg, const XMM128 chMask[] )
{
    XMM128 dst1, dst2, dst3, dst4;

    dst1.i = _mm_and_si128(reg.dst[0].i, chMask[4].i);
    dst2.i = _mm_and_si128(reg.dst[0].i, chMask[5].i);
    dst3.i = _mm_and_si128(reg.dst[0].i, chMask[6].i);
    dst4.i = _mm_and_si128(reg.dst[0].i, chMask[7].i);

    dst1.i = _mm_sll_epi32 ( dst1.i, chMask[0].i);

    dst2.i = _mm_sll_epi32 ( dst2.i, chMask[1].i);

    dst3.i = _mm_sll_epi32 ( dst3.i, chMask[2].i);

    dst4.i = _mm_sll_epi32 ( dst4.i, chMask[3].i);

    dst1.i = _mm_or_si128(dst1.i, dst2.i);
    dst3.i = _mm_or_si128(dst3.i, dst4.i);
    reg.dst[0].i = _mm_or_si128( dst1.i, dst3.i);

}


}//OPT_LEVEL
