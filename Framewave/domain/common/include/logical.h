/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "logical_Shift.h"

namespace OPT_LEVEL
{

namespace
{

template< class T>
SYS_INLINE void And_ref( const T *s1, const T *s2, T *d, CH ch)
{
    switch (ch)
    {
    case C4 :
        d[3] = (s1[3] & s2[3]);
    case AC4:
    case C3	:
        d[2] = (s1[2] & s2[2]);
        d[1] = (s1[1] & s2[1]);
    case C1	:
        d[0] = (s1[0] & s2[0]);
    }
}

template< class T, class CH>
SYS_INLINE void And_ref_I( const T *s1, T *sd, CH ch )
{
    switch (ch)
    {
    case C4 :
        sd[3] = (s1[3] & sd[3]);
    case AC4:
    case C3	:
        sd[2] = (s1[2] & sd[2]);
        sd[1] = (s1[1] & sd[1]);
    case C1	:
        sd[0] = (s1[0] & sd[0]);
    }
}

template< class T>
SYS_INLINE void And_ref( const T *s1, const T val, T *d, CH )
{
    d[0] = (s1[0] & val);
}

template< class T, class CH>
SYS_INLINE void And_ref_I( const T val, T *sd, CH  )
{
    sd[0] = (sd[0] & val);
}

template< class T, class CH>
SYS_INLINE void Or_ref( const T *s1, const T *s2, T *d, CH ch )
{
    switch (ch)
    {
    case C4 :
        d[3] = (s1[3] | s2[3]);
    case AC4:
    case C3	:
        d[2] = (s1[2] | s2[2]);
        d[1] = (s1[1] | s2[1]);
    case C1	:
        d[0] = (s1[0] | s2[0]);
    }
}

template< class T, class CH>
SYS_INLINE void Or_ref_I( const T *s1, T *sd, CH ch )
{
    switch (ch)
    {
    case C4 :
        sd[3] = (s1[3] | sd[3]);
    case AC4:
    case C3	:
        sd[2] = (s1[2] | sd[2]);
        sd[1] = (s1[1] | sd[1]);
    case C1	:
        sd[0] = (s1[0] | sd[0]);
    }
}


template< class T>
SYS_INLINE void Or_ref( const T *s1, const T val, T *d, CH )
{
    d[0] = (s1[0] | val);
}

template< class T, class CH>
SYS_INLINE void Or_ref_I( const T val, T *sd, CH  )
{
    sd[0] = (sd[0] | val);
}

template< class T, class CH>
SYS_INLINE void Xor_ref( const T *s1, const T *s2, T *d, CH ch )
{
    switch (ch)
    {
    case C4 :
        d[3] = (s1[3] ^ s2[3]);
    case AC4:
    case C3	:
        d[2] = (s1[2] ^ s2[2]);
        d[1] = (s1[1] ^ s2[1]);
    case C1	:
        d[0] = (s1[0] ^ s2[0]);
    }
}

template< class T, class CH>
SYS_INLINE void Xor_ref_I( const T *s1, T *sd, CH ch )
{
    switch (ch)
    {
    case C4 :
        sd[3] = (s1[3] ^ sd[3]);
    case AC4:
    case C3	:
        sd[2] = (s1[2] ^ sd[2]);
        sd[1] = (s1[1] ^ sd[1]);
    case C1	:
        sd[0] = (s1[0] ^ sd[0]);
    }
}


template< class T>
SYS_INLINE void Xor_ref( const T *s1, const T val, T *d, CH )
{
    d[0] = (s1[0] ^ val);
}

template< class T, class CH>
SYS_INLINE void Xor_ref_I( const T val, T *sd, CH  )
{
    sd[0] = (sd[0] ^ val);
}

template< class T, class CH>
SYS_INLINE void Not_ref( const T *s, T *d, CH ch )
{
    switch (ch)
    {
    case C4 :
        d[3] =  ~s[3];
    case AC4:
    case C3	:
        d[2] =  ~s[2];
        d[1] =  ~s[1];
    case C1	:
        d[0] =  ~s[0];
    }
}

template< class T, class CH>
SYS_INLINE void Not_ref_I( T *sd, CH ch )
{
    switch (ch)
    {
    case C4 :
        sd[3] =  ~sd[3];
    case AC4:
    case C3	:
        sd[2] =  ~sd[2];
        sd[1] =  ~sd[1];
    case C1	:
        sd[0] =  ~sd[0];
    }
}




//Initialization functions, will be called only once for the entire buffer
SYS_INLINE STATIC void Not_8u_AC4_func(AC4_Mask &dat, XMM128& one)
{
    dat.posMask.i = _mm_set1_epi32(0xFFFFFFFF);
    dat.negMask.i = dat.posMask.i;
    //posMask contains the first 3 channels data to be max value
    //the fourth channel value is filled with zero
    dat.posMask.i = _mm_slli_epi32(dat.posMask.i,8);
    dat.posMask.i = _mm_srli_epi32(dat.posMask.i,8);
    //negMask contains the first 3 channels filled with zero
    //the fourth channel value is max value
    dat.negMask.i = _mm_srli_epi32(dat.negMask.i,24);
    dat.negMask.i = _mm_slli_epi32(dat.negMask.i,24);
    one.i = _mm_setzero_si128();
    one.i = _mm_cmpeq_epi32(one.i, one.i);
}

template< class T>
SYS_INLINE STATIC void AllC_C1_func(T v, XMM128& value)
{
    FW_SSE2::Load1(value, v);
}

//Applicable for 8u and 32s types
template< class T>
SYS_INLINE STATIC void AllC_C3_func(T* v, XMM128 value[3])
{
    FW_SSE2::Load2(value[0], v, C3, 0);
    FW_SSE2::Load2(value[1], v, C3, 1);
    FW_SSE2::Load2(value[2], v, C3, 2);
}

SYS_INLINE STATIC void AllC_C3_func(Fw16u* v, XMM128 value[3])
{
    FW_SSE2::Load2(value[0], v, C3, 0);
    FW_SSE2::Load2(value[1], v, C3, 2);
    FW_SSE2::Load2(value[2], v, C3, 1);
}

template< class T>
SYS_INLINE STATIC void AllC_C4_func(T* v, XMM128& value)
{
    FW_SSE2::Load2(value, v, C4, 0);
}

SYS_INLINE STATIC void AllC_8u_AC4_func(AC4_Mask &dat, Fw8u* v, XMM128& value)
{
    dat.posMask.i = _mm_set1_epi32(0xFFFFFFFF);
    dat.negMask.i = dat.posMask.i;
    dat.posMask.i = _mm_slli_epi32(dat.posMask.i,8);
    dat.posMask.i = _mm_srli_epi32(dat.posMask.i,8);
    dat.negMask.i = _mm_srli_epi32(dat.negMask.i,24);
    dat.negMask.i = _mm_slli_epi32(dat.negMask.i,24);
    FW_SSE2::Load2(value, v, AC4, 0);
}

SYS_INLINE STATIC void AllC_16u_AC4_func(AC4_Mask &dat, Fw16u* v, XMM128& value)
{
    dat.posMask.i = _mm_set1_epi32(0xFFFFFFFF);
    dat.negMask.i = dat.posMask.i;
    dat.posMask.i = _mm_slli_epi64(dat.posMask.i,16);
    dat.posMask.i = _mm_srli_epi64(dat.posMask.i,16);
    dat.negMask.i = _mm_srli_epi64(dat.negMask.i,48);
    dat.negMask.i = _mm_slli_epi64(dat.negMask.i,48);
    FW_SSE2::Load2(value, v, AC4, 0);
}

SYS_INLINE STATIC void AllC_32s_AC4_func(AC4_Mask &dat, Fw32s* v, XMM128& value)
{
    dat.posMask.i = _mm_set1_epi32(0xFFFFFFFF);
    dat.negMask.i = dat.posMask.i;
    dat.posMask.i = _mm_slli_si128(dat.posMask.i,4);
    dat.posMask.i = _mm_srli_si128(dat.posMask.i,4);
    dat.negMask.i = _mm_srli_si128(dat.negMask.i,12);
    dat.negMask.i = _mm_slli_si128(dat.negMask.i,12);
    FW_SSE2::Load2(value, v, AC4, 0);
}


SYS_INLINE STATIC void All_8uAC4_func(AC4_Mask &dat)
{
    dat.posMask.i = _mm_set1_epi32(0xFFFFFFFF);
    dat.negMask.i = dat.posMask.i;
    dat.posMask.i = _mm_slli_epi32(dat.posMask.i,8);
    dat.posMask.i = _mm_srli_epi32(dat.posMask.i,8);
    dat.negMask.i = _mm_srli_epi32(dat.negMask.i,24);
    dat.negMask.i = _mm_slli_epi32(dat.negMask.i,24);
}

SYS_INLINE STATIC void All_16uAC4_func(AC4_Mask &dat)
{
    dat.posMask.i = _mm_set1_epi32(0xFFFFFFFF);
    dat.negMask.i = dat.posMask.i;
    dat.posMask.i = _mm_slli_epi64(dat.posMask.i,16);
    dat.posMask.i = _mm_srli_epi64(dat.posMask.i,16);
    dat.negMask.i = _mm_srli_epi64(dat.negMask.i,48);
    dat.negMask.i = _mm_slli_epi64(dat.negMask.i,48);
}

SYS_INLINE STATIC void All_32sAC4_func(AC4_Mask &dat)
{
    dat.posMask.i = _mm_set1_epi32(0xFFFFFFFF);
    dat.negMask.i = dat.posMask.i;
    dat.posMask.i = _mm_slli_si128(dat.posMask.i,4);
    dat.posMask.i = _mm_srli_si128(dat.posMask.i,4);
    dat.negMask.i = _mm_srli_si128(dat.negMask.i,12);
    dat.negMask.i = _mm_slli_si128(dat.negMask.i,12);
}


SYS_INLINE void And_AC4_I_sse2( RegFile & r, const AC4_Mask & data )
{
    XMM128 accumulator;
	//Saving Alpha channel of the destination
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
	//dst = src1 && dst
	r.dst[0].i = _mm_and_si128( r.src1[0].i, r.dst[0].i );
    //clearing the alpha channel from the destination
	r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    //Restoring saved Alpha channel data
	r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void And_AC4_sse2( RegFile & r, const AC4_Mask & data )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_and_si128( r.src1[0].i, r.src2[0].i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void And_C3_I_sse2( RegFile & r )
{
    r.dst[0].i = _mm_and_si128( r.src1[0].i, r.dst[0].i );
    r.dst[1].i = _mm_and_si128( r.src1[1].i, r.dst[1].i );
    r.dst[2].i = _mm_and_si128( r.src1[2].i, r.dst[2].i );
}

SYS_INLINE void And_C3_sse2( RegFile & r )
{
    r.dst[0].i = _mm_and_si128( r.src1[0].i, r.src2[0].i );
    r.dst[1].i = _mm_and_si128( r.src1[1].i, r.src2[1].i );
    r.dst[2].i = _mm_and_si128( r.src1[2].i, r.src2[2].i );
}

SYS_INLINE void Or_AC4_I_sse2( RegFile & r, const AC4_Mask & data )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_or_si128( r.src1[0].i, r.dst[0].i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void Or_AC4_sse2( RegFile & r, const AC4_Mask & data )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_or_si128( r.src1[0].i, r.src2[0].i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void Or_C3_I_sse2( RegFile & r )
{
    r.dst[0].i = _mm_or_si128( r.src1[0].i, r.dst[0].i );
    r.dst[1].i = _mm_or_si128( r.src1[1].i, r.dst[1].i );
    r.dst[2].i = _mm_or_si128( r.src1[2].i, r.dst[2].i );
}

SYS_INLINE void Or_C3_sse2( RegFile & r )
{
    r.dst[0].i = _mm_or_si128( r.src1[0].i, r.src2[0].i );
    r.dst[1].i = _mm_or_si128( r.src1[1].i, r.src2[1].i );
    r.dst[2].i = _mm_or_si128( r.src1[2].i, r.src2[2].i );
}

SYS_INLINE void Xor_AC4_I_sse2( RegFile & r, const AC4_Mask & data )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_xor_si128( r.src1[0].i, r.dst[0].i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void Xor_AC4_sse2( RegFile & r, const AC4_Mask & data )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_xor_si128( r.src1[0].i, r.src2[0].i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void Xor_C3_I_sse2( RegFile & r )
{
    r.dst[0].i = _mm_xor_si128( r.src1[0].i, r.dst[0].i );
    r.dst[1].i = _mm_xor_si128( r.src1[1].i, r.dst[1].i );
    r.dst[2].i = _mm_xor_si128( r.src1[2].i, r.dst[2].i );
}

SYS_INLINE void Xor_C3_sse2( RegFile & r )
{
    r.dst[0].i = _mm_xor_si128( r.src1[0].i, r.src2[0].i );
    r.dst[1].i = _mm_xor_si128( r.src1[1].i, r.src2[1].i );
    r.dst[2].i = _mm_xor_si128( r.src1[2].i, r.src2[2].i );
}


SYS_INLINE void AndC_C3_I_sse2( RegFile & r, const XMM128 value[] )
{
    r.dst[0].i = _mm_and_si128( value[0].i, r.dst[0].i );
    r.dst[1].i = _mm_and_si128( value[1].i, r.dst[1].i );
    r.dst[2].i = _mm_and_si128( value[2].i, r.dst[2].i );
}

SYS_INLINE void AndC_C3_sse2( RegFile & r, const XMM128 value[] )
{
    r.dst[0].i = _mm_and_si128( r.src1[0].i, value[0].i );
    r.dst[1].i = _mm_and_si128( r.src1[1].i, value[1].i );
    r.dst[2].i = _mm_and_si128( r.src1[2].i, value[2].i );
}

SYS_INLINE void AndC_AC4_I_sse2( RegFile & r, const AC4_Mask & data, const XMM128& value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_and_si128(  value.i, r.dst[0].i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void AndC_AC4_sse2( RegFile & r, const AC4_Mask & data, const XMM128& value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_and_si128( r.src1[0].i,  value.i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void OrC_C3_I_sse2( RegFile & r, const XMM128 value[] )
{
    r.dst[0].i = _mm_or_si128( value[0].i, r.dst[0].i );
    r.dst[1].i = _mm_or_si128( value[1].i, r.dst[1].i );
    r.dst[2].i = _mm_or_si128( value[2].i, r.dst[2].i );
}

SYS_INLINE void OrC_C3_sse2( RegFile & r, const XMM128 value[] )
{
    r.dst[0].i = _mm_or_si128( r.src1[0].i, value[0].i );
    r.dst[1].i = _mm_or_si128( r.src1[1].i, value[1].i );
    r.dst[2].i = _mm_or_si128( r.src1[2].i, value[2].i );
}

SYS_INLINE void OrC_AC4_I_sse2( RegFile & r, const AC4_Mask & data, const XMM128& value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_or_si128(  value.i, r.dst[0].i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void OrC_AC4_sse2( RegFile & r, const AC4_Mask & data, const XMM128& value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_or_si128( r.src1[0].i,  value.i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}


SYS_INLINE void XorC_C3_I_sse2( RegFile & r, const XMM128 value[] )
{
    r.dst[0].i = _mm_xor_si128( value[0].i, r.dst[0].i );
    r.dst[1].i = _mm_xor_si128( value[1].i, r.dst[1].i );
    r.dst[2].i = _mm_xor_si128( value[2].i, r.dst[2].i );
}

SYS_INLINE void XorC_C3_sse2( RegFile & r, const XMM128 value[] )
{
    r.dst[0].i = _mm_xor_si128( r.src1[0].i, value[0].i );
    r.dst[1].i = _mm_xor_si128( r.src1[1].i, value[1].i );
    r.dst[2].i = _mm_xor_si128( r.src1[2].i, value[2].i );
}

SYS_INLINE void XorC_AC4_I_sse2( RegFile & r, const AC4_Mask & data, const XMM128& value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_xor_si128(  value.i, r.dst[0].i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void XorC_AC4_sse2( RegFile & r, const AC4_Mask & data, const XMM128& value )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_xor_si128( r.src1[0].i,  value.i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void Not_AC4_I_sse2( RegFile & r, const AC4_Mask & data, const XMM128& one )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_andnot_si128( r.dst[0].i,  one.i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}

SYS_INLINE void Not_AC4_sse2( RegFile & r, const AC4_Mask & data, const XMM128& one )
{
    XMM128 accumulator;
    accumulator.i = _mm_and_si128(r.dst[0].i,data.negMask.i);
    r.dst[0].i = _mm_andnot_si128( r.src1[0].i,  one.i );
    r.dst[0].i = _mm_and_si128(r.dst[0].i,data.posMask.i);
    r.dst[0].i = _mm_or_si128(r.dst[0].i,accumulator.i);
}


//FE_End



namespace Shift_All
{
template< class T>
class RShift_B2_C1 : public fe2< T, C1,  T, C1  >
{
public:
    Fw32u val;
    XMM128 value;

    RShift_B2_C1(Fw32u val_){
        val = val_;
    }
    IV REFR( const T *s, T *d ) const
    {
        Fw32u num_bits =  (sizeof(T) * 8);
        if (val >= num_bits) *d = (T)((*s ) >> num_bits);
        else 	*d = (T)((*s ) >> val);
    }
};

template< class T>
class RShift_B1_C1 : public fe1< T, C1  >
{
public:
    Fw32u val;
    XMM128 value;

    RShift_B1_C1(Fw32u val_){
        val = val_;
    }
    IV REFR( T *d ) const
    {
        Fw32u num_bits =  (sizeof(T) * 8);
        if (val >= num_bits) *d = (T)((*d ) >> num_bits);
        else 	*d = (T)((*d ) >> val);
    }
};

template< class T>
class LShift_B2_C1 : public fe2< T, C1,  T, C1  >
{
public:
    Fw32u val;
    XMM128 value;

    LShift_B2_C1(Fw32u val_){
        val = val_;
    }

    IV REFR( const T *s, T *d ) const
    {
        if ( val >= (sizeof(T) * 8)) *d= 0;
        else *d = (T)(*s << val);
    }
};

template< class T>
class LShift_B1_C1 : public fe1< T, C1  >
{
public:
    Fw32u val;
    XMM128 value;

    LShift_B1_C1(Fw32u val_){
        val = val_;
    }

	IV REFR( T *d ) const
    {
        if ( val >= (sizeof(T) * 8)) *d= 0;
        else *d = (T)(*d << val);
    }
};

template< class T>
class Shift_B2_AC4 : public fe2< T, AC4,  T, AC4  >
{
public:
    Fw32u val[3];

    Shift_B2_AC4(const Fw32u value[3]){
        val[0] = value[0], val[1] = value[1], val[2] = value[2];
    }
};

template< class T>
class Shift_B1_AC4 : public fe1< T, AC4  >
{
public:
    Fw32u val[3];

    Shift_B1_AC4(const Fw32u value[3]){
        val[0] = value[0], val[1] = value[1], val[2] = value[2];
    }
};

template< class T>
class Shift_B2_C3 : public fe2< T, C3,  T, C3  >
{
public:
    Fw32u val[3];

    Shift_B2_C3(const Fw32u value[3]){
        val[0] = value[0], val[1] = value[1], val[2] = value[2];
    }
};

template< class T>
class Shift_B1_C3 : public fe1< T, C3  >
{
public:
    Fw32u val[3];

    Shift_B1_C3(const Fw32u value[3]){
        val[0] = value[0], val[1] = value[1], val[2] = value[2];
    }
};

template< class T>
class Shift_B2_C4 : public fe2< T, C4,  T, C4  >
{
public:
    Fw32u val[4];

    Shift_B2_C4(const Fw32u value[4]){
        val[0] = value[0], val[1] = value[1], val[2] = value[2], val[3] = value[3];
    }
};

template< class T>
class Shift_B1_C4 : public fe1< T, C4  >
{
public:
    Fw32u val[4];

    Shift_B1_C4(const Fw32u value[4]){
        val[0] = value[0], val[1] = value[1], val[2] = value[2], val[3] = value[3];
    }
};

}

namespace RShift_8u
{
class RShift_B2_C1 : public Shift_All::RShift_B2_C1<Fw8u>
{
public:
    FE_SSE2_REF
    RShift_B2_C1(Fw32u value): Shift_All::RShift_B2_C1<Fw8u>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::RShift_8u(reg.src1[0].i, Shift_All::RShift_B2_C1<Fw8u>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class RShift_B1_C1 : public Shift_All::RShift_B1_C1<Fw8u>
{
public:
    FE_SSE2_REF
    RShift_B1_C1(Fw32u value): Shift_All::RShift_B1_C1<Fw8u>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::RShift_8u(reg.dst[0].i, Shift_All::RShift_B1_C1<Fw8u>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class RShift_B2_AC4 : public Shift_All::Shift_B2_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B2_AC4(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8u_AC4_sse2(r, chMask, Shift_All::Shift_B2_AC4<Fw8u>::val);
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        RShift<Fw8u, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw8u>::val);
    }
};

class RShift_B1_AC4 : public Shift_All::Shift_B1_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B1_AC4(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8u_AC4_I_sse2(r, chMask, Shift_All::Shift_B1_AC4<Fw8u>::val);
    }
    IV REFR( Fw8u *sd ) const
    {
        RShift<Fw8u, AC4>(sd, Shift_All::Shift_B1_AC4<Fw8u>::val);
    }
};

class RShift_B2_C3 : public Shift_All::Shift_B2_C3<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B2_C3(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C3_init(chMask, Shift_All::Shift_B2_C3<Fw8u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8u_C3_sse2(r, chMask);
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        RShift<Fw8u, C3>(s, d, Shift_All::Shift_B2_C3<Fw8u>::val);
    }
};

class RShift_B1_C3 : public Shift_All::Shift_B1_C3<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B1_C3(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C3_init(chMask, Shift_All::Shift_B1_C3<Fw8u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8u_C3_I_sse2(r, chMask);
    }
    IV REFR( Fw8u *sd ) const
    {
        RShift<Fw8u, C3>(sd, Shift_All::Shift_B1_C3<Fw8u>::val);
    }
};

class RShift_B2_C4 : public Shift_All::Shift_B2_C4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B2_C4(const Fw32u value[4]): Shift_All::Shift_B2_C4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8u_C4_sse2(r, chMask, Shift_All::Shift_B2_C4<Fw8u>::val);
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        RShift<Fw8u, C4>(s, d, Shift_All::Shift_B2_C4<Fw8u>::val);
    }
};

class RShift_B1_C4 : public Shift_All::Shift_B1_C4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B1_C4(const Fw32u value[4]): Shift_All::Shift_B1_C4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8u_C4_I_sse2(r, chMask, Shift_All::Shift_B1_C4<Fw8u>::val);
    }
    IV REFR( Fw8u *sd ) const
    {
        RShift<Fw8u, C4>(sd, Shift_All::Shift_B1_C4<Fw8u>::val);
    }
};


class RShift_B2_AC4_eq : public Shift_All::Shift_B2_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    AC4_Mask mask;
    RShift_B2_AC4_eq(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_AC4_init(mask, value, Shift_All::Shift_B2_AC4<Fw8u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift8uAC4_eq_sse2(mask, r, value );
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        RShift<Fw8u, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw8u>::val);
    }
};

class RShift_B1_AC4_eq : public Shift_All::Shift_B1_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    AC4_Mask mask;
    RShift_B1_AC4_eq(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_AC4_init(mask, value, Shift_All::Shift_B1_AC4<Fw8u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift8uAC4_eq_I_sse2(mask, r, value );
    }
    IV REFR( Fw8u *sd ) const
    {
        RShift<Fw8u, AC4>(sd, Shift_All::Shift_B1_AC4<Fw8u>::val);
    }
};

class RShift_B2_C3_eq : public Shift_All::Shift_B2_C3<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B2_C3_eq(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw8u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift8uC3_eq_sse2(value, r);
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        RShift<Fw8u, C3>(s, d, Shift_All::Shift_B2_C3<Fw8u>::val);
    }
};

class RShift_B1_C3_eq : public Shift_All::Shift_B1_C3<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B1_C3_eq(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw8u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift8uC3_eq_I_sse2(value, r);
    }
    IV REFR( Fw8u *sd ) const
    {
        RShift<Fw8u, C3>(sd, Shift_All::Shift_B1_C3<Fw8u>::val);
    }
};

class RShift_B2_C4_eq : public Shift_All::Shift_B2_C4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B2_C4_eq(const Fw32u value[3]): Shift_All::Shift_B2_C4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::RShift_8u(reg.src1[0].i, value.i);

    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        RShift<Fw8u, C4>(s, d, Shift_All::Shift_B2_C4<Fw8u>::val);
    }
};

class RShift_B1_C4_eq : public Shift_All::Shift_B1_C4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B1_C4_eq(const Fw32u value[3]): Shift_All::Shift_B1_C4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::RShift_8u(reg.dst[0].i, value.i);
    }
    IV REFR( Fw8u *sd ) const
    {
        RShift<Fw8u, C4>(sd, Shift_All::Shift_B1_C4<Fw8u>::val);
    }
};
}


namespace RShift_8s
{
class RShift_B2_C1 : public Shift_All::RShift_B2_C1<Fw8s>
{
public:
    FE_SSE2_REF
    RShift_B2_C1(Fw32u value): Shift_All::RShift_B2_C1<Fw8s>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::RShift_8s(reg.src1[0].i, Shift_All::RShift_B2_C1<Fw8s>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class RShift_B1_C1 : public Shift_All::RShift_B1_C1<Fw8s>
{
public:
    FE_SSE2_REF
    RShift_B1_C1(Fw32u value): Shift_All::RShift_B1_C1<Fw8s>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::RShift_8s(reg.dst[0].i, Shift_All::RShift_B1_C1<Fw8s>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class RShift_B2_AC4 : public Shift_All::Shift_B2_AC4<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B2_AC4(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw8s>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8s_AC4_sse2(r, chMask, Shift_All::Shift_B2_AC4<Fw8s>::val);
    }
    IV REFR( const Fw8s *s, Fw8s *d ) const
    {
        RShift<Fw8s, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw8s>::val);
    }
};

class RShift_B1_AC4 : public Shift_All::Shift_B1_AC4<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B1_AC4(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw8s>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8s_AC4_I_sse2(r, chMask, Shift_All::Shift_B1_AC4<Fw8s>::val);
    }
    IV REFR( Fw8s *sd ) const
    {
        RShift<Fw8s, AC4>(sd, Shift_All::Shift_B1_AC4<Fw8s>::val);
    }
};

class RShift_B2_C3 : public Shift_All::Shift_B2_C3<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B2_C3(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw8s>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C3_init(chMask, Shift_All::Shift_B2_C3<Fw8s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8s_C3_sse2(r, chMask);
    }
    IV REFR( const Fw8s *s, Fw8s *d ) const
    {
        RShift<Fw8s, C3>(s, d, Shift_All::Shift_B2_C3<Fw8s>::val);
    }
};

class RShift_B1_C3 : public Shift_All::Shift_B1_C3<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B1_C3(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw8s>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C3_init(chMask, Shift_All::Shift_B1_C3<Fw8s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8s_C3_I_sse2(r, chMask);
    }
    IV REFR( Fw8s *sd ) const
    {
        RShift<Fw8s, C3>(sd, Shift_All::Shift_B1_C3<Fw8s>::val);
    }
};

class RShift_B2_C4 : public Shift_All::Shift_B2_C4<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B2_C4(const Fw32u value[4]): Shift_All::Shift_B2_C4<Fw8s>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8s_C4_sse2(r, chMask, Shift_All::Shift_B2_C4<Fw8s>::val);
    }
    IV REFR( const Fw8s *s, Fw8s *d ) const
    {
        RShift<Fw8s, C4>(s, d, Shift_All::Shift_B2_C4<Fw8s>::val);
    }
};

class RShift_B1_C4 : public Shift_All::Shift_B1_C4<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B1_C4(const Fw32u value[4]): Shift_All::Shift_B1_C4<Fw8s>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_8s_C4_I_sse2(r, chMask, Shift_All::Shift_B1_C4<Fw8s>::val);
    }
    IV REFR( Fw8s *sd ) const
    {
        RShift<Fw8s, C4>(sd, Shift_All::Shift_B1_C4<Fw8s>::val);
    }
};


class RShift_B2_AC4_eq : public Shift_All::Shift_B2_AC4<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    AC4_Mask mask;
    RShift_B2_AC4_eq(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw8s>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_AC4_init(mask, value, Shift_All::Shift_B2_AC4<Fw8s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift8sAC4_eq_sse2(mask, r, value );
    }
    IV REFR( const Fw8s *s, Fw8s *d ) const
    {
        RShift<Fw8s, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw8s>::val);
    }
};

class RShift_B1_AC4_eq : public Shift_All::Shift_B1_AC4<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    AC4_Mask mask;
    RShift_B1_AC4_eq(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw8s>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_AC4_init(mask, value, Shift_All::Shift_B1_AC4<Fw8s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift8sAC4_eq_I_sse2(mask, r, value );
    }
    IV REFR( Fw8s *sd ) const
    {
        RShift<Fw8s, AC4>(sd, Shift_All::Shift_B1_AC4<Fw8s>::val);
    }
};

class RShift_B2_C3_eq : public Shift_All::Shift_B2_C3<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B2_C3_eq(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw8s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift8sC3_eq_sse2(value, r);
    }
    IV REFR( const Fw8s *s, Fw8s *d ) const
    {
        RShift<Fw8s, C3>(s, d, Shift_All::Shift_B2_C3<Fw8s>::val);
    }
};

class RShift_B1_C3_eq : public Shift_All::Shift_B1_C3<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B1_C3_eq(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw8s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift8sC3_eq_I_sse2(value, r);
    }
    IV REFR( Fw8s *sd ) const
    {
        RShift<Fw8s, C3>(sd, Shift_All::Shift_B1_C3<Fw8s>::val);
    }
};

class RShift_B2_C4_eq : public Shift_All::Shift_B2_C4<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B2_C4_eq(const Fw32u value[3]): Shift_All::Shift_B2_C4<Fw8s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::RShift_8s(reg.src1[0].i, value.i);

    }
    IV REFR( const Fw8s *s, Fw8s *d ) const
    {
        RShift<Fw8s, C4>(s, d, Shift_All::Shift_B2_C4<Fw8s>::val);
    }
};

class RShift_B1_C4_eq : public Shift_All::Shift_B1_C4<Fw8s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B1_C4_eq(const Fw32u value[3]): Shift_All::Shift_B1_C4<Fw8s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::RShift_8s(reg.dst[0].i, value.i);
    }
    IV REFR( Fw8s *sd ) const
    {
        RShift<Fw8s, C4>(sd, Shift_All::Shift_B1_C4<Fw8s>::val);
    }
};
}



namespace RShift_16u
{
class RShift_B2_C1 : public Shift_All::RShift_B2_C1<Fw16u>
{
public:
    FE_SSE2_REF
    RShift_B2_C1(Fw32u value): Shift_All::RShift_B2_C1<Fw16u>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_srl_epi16 ( reg.src1[0].i, Shift_All::RShift_B2_C1<Fw16u>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class RShift_B1_C1 : public Shift_All::RShift_B1_C1<Fw16u>
{
public:
    FE_SSE2_REF
    RShift_B1_C1(Fw32u value): Shift_All::RShift_B1_C1<Fw16u>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_srl_epi16 ( reg.dst[0].i, Shift_All::RShift_B1_C1<Fw16u>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class RShift_B2_AC4 : public Shift_All::Shift_B2_AC4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B2_AC4(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask, Shift_All::Shift_B2_AC4<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16u_AC4_sse2(r, chMask);
    }
    IV REFR( const Fw16u *s, Fw16u *d ) const
    {
        RShift<Fw16u, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw16u>::val);
    }
};

class RShift_B1_AC4 : public Shift_All::Shift_B1_AC4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B1_AC4(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask, Shift_All::Shift_B1_AC4<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16u_AC4_I_sse2(r, chMask);
    }
    IV REFR( Fw16u *sd ) const
    {
        RShift<Fw16u, AC4>(sd, Shift_All::Shift_B1_AC4<Fw16u>::val);
    }
};

class RShift_B2_C3 : public Shift_All::Shift_B2_C3<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B2_C3(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C3_init(chMask, Shift_All::Shift_B2_C3<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16u_C3_sse2(r, chMask);
    }
    IV REFR( const Fw16u *s, Fw16u *d ) const
    {
        RShift<Fw16u, C3>(s, d, Shift_All::Shift_B2_C3<Fw16u>::val);
    }
};

class RShift_B1_C3 : public Shift_All::Shift_B1_C3<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B1_C3(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C3_init(chMask, Shift_All::Shift_B1_C3<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16u_C3_I_sse2(r, chMask);
    }
    IV REFR( Fw16u *sd ) const
    {
        RShift<Fw16u, C3>(sd, Shift_All::Shift_B1_C3<Fw16u>::val);
    }
};

class RShift_B2_C4 : public Shift_All::Shift_B2_C4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B2_C4(const Fw32u value[4]): Shift_All::Shift_B2_C4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask,Shift_All::Shift_B2_C4<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16u_C4_sse2(r, chMask);
    }
    IV REFR( const Fw16u *s, Fw16u *d ) const
    {
        RShift<Fw16u, C4>(s, d, Shift_All::Shift_B2_C4<Fw16u>::val);
    }
};

class RShift_B1_C4 : public Shift_All::Shift_B1_C4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B1_C4(const Fw32u value[4]): Shift_All::Shift_B1_C4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask, Shift_All::Shift_B1_C4<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16u_C4_I_sse2(r, chMask);
    }
    IV REFR( Fw16u *sd ) const
    {
        RShift<Fw16u, C4>(sd, Shift_All::Shift_B1_C4<Fw16u>::val);
    }
};


class RShift_B2_C3_eq : public Shift_All::Shift_B2_C3<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B2_C3_eq(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw16u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift16uC3_eq_sse2(value, r);
    }
    IV REFR( const Fw16u *s, Fw16u *d ) const
    {
        RShift<Fw16u, C3>(s, d, Shift_All::Shift_B2_C3<Fw16u>::val);
    }
};

class RShift_B1_C3_eq : public Shift_All::Shift_B1_C3<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B1_C3_eq(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw16u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift16uC3_eq_I_sse2(value, r);
    }
    IV REFR( Fw16u *sd ) const
    {
        RShift<Fw16u, C3>(sd, Shift_All::Shift_B1_C3<Fw16u>::val);
    }
};

class RShift_B2_C4_eq : public Shift_All::Shift_B2_C4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B2_C4_eq(const Fw32u value[3]): Shift_All::Shift_B2_C4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(Shift_All::Shift_B2_C4<Fw16u>::val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_srl_epi16 ( reg.src1[0].i, value.i);

    }
    IV REFR( const Fw16u *s, Fw16u *d ) const
    {
        RShift<Fw16u, C4>(s, d, Shift_All::Shift_B2_C4<Fw16u>::val);
    }
};

class RShift_B1_C4_eq : public Shift_All::Shift_B1_C4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B1_C4_eq(const Fw32u value[3]): Shift_All::Shift_B1_C4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(Shift_All::Shift_B1_C4<Fw16u>::val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_srl_epi16 ( reg.dst[0].i, value.i);
    }
    IV REFR( Fw16u *sd ) const
    {
        RShift<Fw16u, C4>(sd, Shift_All::Shift_B1_C4<Fw16u>::val);
    }
};
}








namespace RShift_16s
{
class RShift_B2_C1 : public Shift_All::RShift_B2_C1<Fw16s>
{
public:
    FE_SSE2_REF 
    RShift_B2_C1(Fw32u value): Shift_All::RShift_B2_C1<Fw16s>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sra_epi16 ( reg.src1[0].i, Shift_All::RShift_B2_C1<Fw16s>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class RShift_B1_C1 : public Shift_All::RShift_B1_C1<Fw16s>
{
public:
    FE_SSE2_REF 
    RShift_B1_C1(Fw32u value): Shift_All::RShift_B1_C1<Fw16s>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sra_epi16 ( reg.dst[0].i, Shift_All::RShift_B1_C1<Fw16s>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class RShift_B2_AC4 : public Shift_All::Shift_B2_AC4<Fw16s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B2_AC4(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw16s>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask, Shift_All::Shift_B2_AC4<Fw16s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16s_AC4_sse2(r, chMask);
    }
    IV REFR( const Fw16s *s, Fw16s *d ) const
    {
        RShift<Fw16s, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw16s>::val);
    }
};

class RShift_B1_AC4 : public Shift_All::Shift_B1_AC4<Fw16s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B1_AC4(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw16s>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask, Shift_All::Shift_B1_AC4<Fw16s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16s_AC4_I_sse2(r, chMask);
    }
    IV REFR( Fw16s *sd ) const
    {
        RShift<Fw16s, AC4>(sd, Shift_All::Shift_B1_AC4<Fw16s>::val);
    }
};

class RShift_B2_C3 : public Shift_All::Shift_B2_C3<Fw16s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B2_C3(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw16s>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C3_init(chMask, Shift_All::Shift_B2_C3<Fw16s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16s_C3_sse2(r, chMask);
    }
    IV REFR( const Fw16s *s, Fw16s *d ) const
    {
        RShift<Fw16s, C3>(s, d, Shift_All::Shift_B2_C3<Fw16s>::val);
    }
};

class RShift_B1_C3 : public Shift_All::Shift_B1_C3<Fw16s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B1_C3(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw16s>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C3_init(chMask, Shift_All::Shift_B1_C3<Fw16s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16s_C3_I_sse2(r, chMask);
    }
    IV REFR( Fw16s *sd ) const
    {
        RShift<Fw16s, C3>(sd, Shift_All::Shift_B1_C3<Fw16s>::val);
    }
};

class RShift_B2_C4 : public Shift_All::Shift_B2_C4<Fw16s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B2_C4(const Fw32u value[4]): Shift_All::Shift_B2_C4<Fw16s>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask,Shift_All::Shift_B2_C4<Fw16s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16s_C4_sse2(r, chMask);
    }
    IV REFR( const Fw16s *s, Fw16s *d ) const
    {
        RShift<Fw16s, C4>(s, d, Shift_All::Shift_B2_C4<Fw16s>::val);
    }
};

class RShift_B1_C4 : public Shift_All::Shift_B1_C4<Fw16s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B1_C4(const Fw32u value[4]): Shift_All::Shift_B1_C4<Fw16s>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask, Shift_All::Shift_B1_C4<Fw16s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_16s_C4_I_sse2(r, chMask);
    }
    IV REFR( Fw16s *sd ) const
    {
        RShift<Fw16s, C4>(sd, Shift_All::Shift_B1_C4<Fw16s>::val);
    }
};


class RShift_B2_C3_eq : public Shift_All::Shift_B2_C3<Fw16s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B2_C3_eq(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw16s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift16sC3_eq_sse2(value, r);
    }
    IV REFR( const Fw16s *s, Fw16s *d ) const
    {
        RShift<Fw16s, C3>(s, d, Shift_All::Shift_B2_C3<Fw16s>::val);
    }
};

class RShift_B1_C3_eq : public Shift_All::Shift_B1_C3<Fw16s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B1_C3_eq(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw16s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift16sC3_eq_I_sse2(value, r);
    }
    IV REFR( Fw16s *sd ) const
    {
        RShift<Fw16s, C3>(sd, Shift_All::Shift_B1_C3<Fw16s>::val);
    }
};

class RShift_B2_C4_eq : public Shift_All::Shift_B2_C4<Fw16s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B2_C4_eq(const Fw32u value[3]): Shift_All::Shift_B2_C4<Fw16s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(Shift_All::Shift_B2_C4<Fw16s>::val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sra_epi16 ( reg.src1[0].i, value.i);

    }
    IV REFR( const Fw16s *s, Fw16s *d ) const
    {
        RShift<Fw16s, C4>(s, d, Shift_All::Shift_B2_C4<Fw16s>::val);
    }
};

class RShift_B1_C4_eq : public Shift_All::Shift_B1_C4<Fw16s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B1_C4_eq(const Fw32u value[3]): Shift_All::Shift_B1_C4<Fw16s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(Shift_All::Shift_B1_C4<Fw16s>::val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sra_epi16 ( reg.dst[0].i, value.i);
    }
    IV REFR( Fw16s *sd ) const
    {
        RShift<Fw16s, C4>(sd, Shift_All::Shift_B1_C4<Fw16s>::val);
    }
};
}





namespace RShift_32s
{
class RShift_B2_C1 : public Shift_All::RShift_B2_C1<Fw32s>
{
public:
    FE_SSE2_REF
    RShift_B2_C1(Fw32u value): Shift_All::RShift_B2_C1<Fw32s>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sra_epi32 ( reg.src1[0].i, Shift_All::RShift_B2_C1<Fw32s>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class RShift_B1_C1 : public Shift_All::RShift_B1_C1<Fw32s>
{
public:
    FE_SSE2_REF
    RShift_B1_C1(Fw32u value): Shift_All::RShift_B1_C1<Fw32s>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sra_epi32 ( reg.dst[0].i, Shift_All::RShift_B1_C1<Fw32s>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class RShift_B2_AC4 : public Shift_All::Shift_B2_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B2_AC4(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C4_init(chMask, Shift_All::Shift_B2_AC4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_32s_AC4_sse2(r, chMask);
    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        RShift<Fw32s, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw32s>::val);
    }
};

class RShift_B1_AC4 : public Shift_All::Shift_B1_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B1_AC4(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C4_init(chMask, Shift_All::Shift_B1_AC4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_32s_AC4_I_sse2(r, chMask);
    }
    IV REFR( Fw32s *sd ) const
    {
        RShift<Fw32s, AC4>(sd, Shift_All::Shift_B1_AC4<Fw32s>::val);
    }
};

class RShift_B2_C3 : public Shift_All::Shift_B2_C3<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B2_C3(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C3_init(chMask, Shift_All::Shift_B2_C3<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_32s_C3_sse2(r, chMask);
    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        RShift<Fw32s, C3>(s, d, Shift_All::Shift_B2_C3<Fw32s>::val);
    }
};

class RShift_B1_C3 : public Shift_All::Shift_B1_C3<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    RShift_B1_C3(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C3_init(chMask, Shift_All::Shift_B1_C3<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_32s_C3_I_sse2(r, chMask);
    }
    IV REFR( Fw32s *sd ) const
    {
        RShift<Fw32s, C3>(sd, Shift_All::Shift_B1_C3<Fw32s>::val);
    }
};

class RShift_B2_C4 : public Shift_All::Shift_B2_C4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B2_C4(const Fw32u value[4]): Shift_All::Shift_B2_C4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C4_init(chMask,Shift_All::Shift_B2_C4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_32s_C4_sse2(r, chMask);
    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        RShift<Fw32s, C4>(s, d, Shift_All::Shift_B2_C4<Fw32s>::val);
    }
};

class RShift_B1_C4 : public Shift_All::Shift_B1_C4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    RShift_B1_C4(const Fw32u value[4]): Shift_All::Shift_B1_C4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C4_init(chMask, Shift_All::Shift_B1_C4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift_32s_C4_I_sse2(r, chMask);
    }
    IV REFR( Fw32s *sd ) const
    {
        RShift<Fw32s, C4>(sd, Shift_All::Shift_B1_C4<Fw32s>::val);
    }
};


class RShift_B2_C3_eq : public Shift_All::Shift_B2_C3<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B2_C3_eq(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw32s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift32sC3_eq_sse2(value, r);
    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        RShift<Fw32s, C3>(s, d, Shift_All::Shift_B2_C3<Fw32s>::val);
    }
};

class RShift_B1_C3_eq : public Shift_All::Shift_B1_C3<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B1_C3_eq(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw32s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift32sC3_eq_I_sse2(value, r);
    }
    IV REFR( Fw32s *sd ) const
    {
        RShift<Fw32s, C3>(sd, Shift_All::Shift_B1_C3<Fw32s>::val);
    }
};

class RShift_B2_C4_eq : public Shift_All::Shift_B2_C4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B2_C4_eq(const Fw32u value[3]): Shift_All::Shift_B2_C4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(Shift_All::Shift_B2_C4<Fw32s>::val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sra_epi32 ( reg.src1[0].i, value.i);

    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        RShift<Fw32s, C4>(s, d, Shift_All::Shift_B2_C4<Fw32s>::val);
    }
};

class RShift_B1_C4_eq : public Shift_All::Shift_B1_C4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    RShift_B1_C4_eq(const Fw32u value[3]): Shift_All::Shift_B1_C4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(Shift_All::Shift_B1_C4<Fw32s>::val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sra_epi32 ( reg.dst[0].i, value.i);
    }
    IV REFR( Fw32s *sd ) const
    {
        RShift<Fw32s, C4>(sd, Shift_All::Shift_B1_C4<Fw32s>::val);
    }
};

class RShift_B2_AC4_eq : public Shift_All::Shift_B2_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    AC4_Mask mask;
    RShift_B2_AC4_eq(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_AC4_init(mask, value, Shift_All::Shift_B2_AC4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift32sAC4_eq_sse2(mask, r, value );
    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        RShift<Fw32s, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw32s>::val);
    }
};

class RShift_B1_AC4_eq : public Shift_All::Shift_B1_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    AC4_Mask mask;
    RShift_B1_AC4_eq(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_AC4_init(mask, value, Shift_All::Shift_B1_AC4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        RShift32sAC4_eq_I_sse2(mask, r, value );
    }
    IV REFR( Fw32s *sd ) const
    {
        RShift<Fw32s, AC4>(sd, Shift_All::Shift_B1_AC4<Fw32s>::val);
    }
};
}


namespace LShift_8u
{
class LShift_B2_C1 : public Shift_All::LShift_B2_C1<Fw8u>
{
public:
    FE_SSE2_REF
    LShift_B2_C1(Fw32u value): Shift_All::LShift_B2_C1<Fw8u>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::LShift_8u(reg.src1[0].i, Shift_All::LShift_B2_C1<Fw8u>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class LShift_B1_C1 : public Shift_All::LShift_B1_C1<Fw8u>
{
public:
    FE_SSE2_REF
    LShift_B1_C1(Fw32u value): Shift_All::LShift_B1_C1<Fw8u>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::LShift_8u(reg.dst[0].i, Shift_All::LShift_B1_C1<Fw8u>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class LShift_B2_AC4 : public Shift_All::Shift_B2_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    LShift_B2_AC4(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_8u_AC4_sse2(r, chMask, Shift_All::Shift_B2_AC4<Fw8u>::val);
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        LShift<Fw8u, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw8u>::val);
    }
};

class LShift_B1_AC4 : public Shift_All::Shift_B1_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    LShift_B1_AC4(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_8u_AC4_I_sse2(r, chMask, Shift_All::Shift_B1_AC4<Fw8u>::val);
    }
    IV REFR( Fw8u *sd ) const
    {
        LShift<Fw8u, AC4>(sd, Shift_All::Shift_B1_AC4<Fw8u>::val);
    }
};

class LShift_B2_C3 : public Shift_All::Shift_B2_C3<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    LShift_B2_C3(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C3_init(chMask, Shift_All::Shift_B2_C3<Fw8u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_8u_C3_sse2(r, chMask);
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        LShift<Fw8u, C3>(s, d, Shift_All::Shift_B2_C3<Fw8u>::val);
    }
};

class LShift_B1_C3 : public Shift_All::Shift_B1_C3<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    LShift_B1_C3(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C3_init(chMask, Shift_All::Shift_B1_C3<Fw8u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_8u_C3_I_sse2(r, chMask);
    }
    IV REFR( Fw8u *sd ) const
    {
        LShift<Fw8u, C3>(sd, Shift_All::Shift_B1_C3<Fw8u>::val);
    }
};

class LShift_B2_C4 : public Shift_All::Shift_B2_C4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    LShift_B2_C4(const Fw32u value[4]): Shift_All::Shift_B2_C4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_8u_C4_sse2(r, chMask, Shift_All::Shift_B2_C4<Fw8u>::val);
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        LShift<Fw8u, C4>(s, d, Shift_All::Shift_B2_C4<Fw8u>::val);
    }
};

class LShift_B1_C4 : public Shift_All::Shift_B1_C4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    LShift_B1_C4(const Fw32u value[4]): Shift_All::Shift_B1_C4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_C4_init(chMask);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_8u_C4_I_sse2(r, chMask, Shift_All::Shift_B1_C4<Fw8u>::val);
    }
    IV REFR( Fw8u *sd ) const
    {
        LShift<Fw8u, C4>(sd, Shift_All::Shift_B1_C4<Fw8u>::val);
    }
};


class LShift_B2_AC4_eq : public Shift_All::Shift_B2_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    AC4_Mask mask;
    LShift_B2_AC4_eq(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_AC4_init(mask, value, Shift_All::Shift_B2_AC4<Fw8u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift8uAC4_eq_sse2(mask, r, value );
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        LShift<Fw8u, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw8u>::val);
    }
};

class LShift_B1_AC4_eq : public Shift_All::Shift_B1_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    AC4_Mask mask;
    LShift_B1_AC4_eq(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        Shift_8u_AC4_init(mask, value, Shift_All::Shift_B1_AC4<Fw8u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift8uAC4_eq_I_sse2(mask, r, value );
    }
    IV REFR( Fw8u *sd ) const
    {
        LShift<Fw8u, AC4>(sd, Shift_All::Shift_B1_AC4<Fw8u>::val);
    }
};

class LShift_B2_C3_eq : public Shift_All::Shift_B2_C3<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B2_C3_eq(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw8u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift8uC3_eq_sse2(value, r);
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        LShift<Fw8u, C3>(s, d, Shift_All::Shift_B2_C3<Fw8u>::val);
    }
};

class LShift_B1_C3_eq : public Shift_All::Shift_B1_C3<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B1_C3_eq(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw8u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift8uC3_eq_I_sse2(value, r);
    }
    IV REFR( Fw8u *sd ) const
    {
        LShift<Fw8u, C3>(sd, Shift_All::Shift_B1_C3<Fw8u>::val);
    }
};

class LShift_B2_C4_eq : public Shift_All::Shift_B2_C4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B2_C4_eq(const Fw32u value[3]): Shift_All::Shift_B2_C4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::LShift_8u(reg.src1[0].i, value.i);

    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        LShift<Fw8u, C4>(s, d, Shift_All::Shift_B2_C4<Fw8u>::val);
    }
};

class LShift_B1_C4_eq : public Shift_All::Shift_B1_C4<Fw8u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B1_C4_eq(const Fw32u value[3]): Shift_All::Shift_B1_C4<Fw8u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = CBL_SSE2::LShift_8u(reg.dst[0].i, value.i);
    }
    IV REFR( Fw8u *sd ) const
    {
        LShift<Fw8u, C4>(sd, Shift_All::Shift_B1_C4<Fw8u>::val);
    }
};
}


namespace LShift_16u
{
class LShift_B2_C1 : public Shift_All::LShift_B2_C1<Fw16u>
{
public:
    FE_SSE2_REF
    LShift_B2_C1(Fw32u value): Shift_All::LShift_B2_C1<Fw16u>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sll_epi16 ( reg.src1[0].i, Shift_All::LShift_B2_C1<Fw16u>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class LShift_B1_C1 : public Shift_All::LShift_B1_C1<Fw16u>
{
public:
    FE_SSE2_REF
    LShift_B1_C1(Fw32u value): Shift_All::LShift_B1_C1<Fw16u>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sll_epi16 ( reg.dst[0].i, Shift_All::LShift_B1_C1<Fw16u>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class LShift_B2_AC4 : public Shift_All::Shift_B2_AC4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    LShift_B2_AC4(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask, Shift_All::Shift_B2_AC4<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_16u_AC4_sse2(r, chMask);
    }
    IV REFR( const Fw16u *s, Fw16u *d ) const
    {
        LShift<Fw16u, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw16u>::val);
    }
};

class LShift_B1_AC4 : public Shift_All::Shift_B1_AC4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    LShift_B1_AC4(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask, Shift_All::Shift_B1_AC4<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_16u_AC4_I_sse2(r, chMask);
    }
    IV REFR( Fw16u *sd ) const
    {
        LShift<Fw16u, AC4>(sd, Shift_All::Shift_B1_AC4<Fw16u>::val);
    }
};

class LShift_B2_C3 : public Shift_All::Shift_B2_C3<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    LShift_B2_C3(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C3_init(chMask, Shift_All::Shift_B2_C3<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_16u_C3_sse2(r, chMask);
    }
    IV REFR( const Fw16u *s, Fw16u *d ) const
    {
        LShift<Fw16u, C3>(s, d, Shift_All::Shift_B2_C3<Fw16u>::val);
    }
};

class LShift_B1_C3 : public Shift_All::Shift_B1_C3<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    LShift_B1_C3(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C3_init(chMask, Shift_All::Shift_B1_C3<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_16u_C3_I_sse2(r, chMask);
    }
    IV REFR( Fw16u *sd ) const
    {
        LShift<Fw16u, C3>(sd, Shift_All::Shift_B1_C3<Fw16u>::val);
    }
};

class LShift_B2_C4 : public Shift_All::Shift_B2_C4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    LShift_B2_C4(const Fw32u value[4]): Shift_All::Shift_B2_C4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask,Shift_All::Shift_B2_C4<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_16u_C4_sse2(r, chMask);
    }
    IV REFR( const Fw16u *s, Fw16u *d ) const
    {
        LShift<Fw16u, C4>(s, d, Shift_All::Shift_B2_C4<Fw16u>::val);
    }
};

class LShift_B1_C4 : public Shift_All::Shift_B1_C4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    LShift_B1_C4(const Fw32u value[4]): Shift_All::Shift_B1_C4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        Shift_16u_C4_init(chMask, Shift_All::Shift_B1_C4<Fw16u>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_16u_C4_I_sse2(r, chMask);
    }
    IV REFR( Fw16u *sd ) const
    {
        LShift<Fw16u, C4>(sd, Shift_All::Shift_B1_C4<Fw16u>::val);
    }
};


class LShift_B2_C3_eq : public Shift_All::Shift_B2_C3<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B2_C3_eq(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw16u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift16uC3_eq_sse2(value, r);
    }
    IV REFR( const Fw16u *s, Fw16u *d ) const
    {
        LShift<Fw16u, C3>(s, d, Shift_All::Shift_B2_C3<Fw16u>::val);
    }
};

class LShift_B1_C3_eq : public Shift_All::Shift_B1_C3<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B1_C3_eq(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw16u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift16uC3_eq_I_sse2(value, r);
    }
    IV REFR( Fw16u *sd ) const
    {
        LShift<Fw16u, C3>(sd, Shift_All::Shift_B1_C3<Fw16u>::val);
    }
};

class LShift_B2_C4_eq : public Shift_All::Shift_B2_C4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B2_C4_eq(const Fw32u value[3]): Shift_All::Shift_B2_C4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(Shift_All::Shift_B2_C4<Fw16u>::val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sll_epi16 ( reg.src1[0].i, value.i);

    }
    IV REFR( const Fw16u *s, Fw16u *d ) const
    {
        LShift<Fw16u, C4>(s, d, Shift_All::Shift_B2_C4<Fw16u>::val);
    }
};

class LShift_B1_C4_eq : public Shift_All::Shift_B1_C4<Fw16u>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B1_C4_eq(const Fw32u value[3]): Shift_All::Shift_B1_C4<Fw16u>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(Shift_All::Shift_B1_C4<Fw16u>::val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sll_epi16 ( reg.dst[0].i, value.i);
    }
    IV REFR( Fw16u *sd ) const
    {
        LShift<Fw16u, C4>(sd, Shift_All::Shift_B1_C4<Fw16u>::val);
    }
};
}









namespace LShift_32s
{
class LShift_B2_C1 : public Shift_All::LShift_B2_C1<Fw32s>
{
public:
    FE_SSE2_REF
    LShift_B2_C1(Fw32u value): Shift_All::LShift_B2_C1<Fw32s>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sll_epi32 ( reg.src1[0].i, Shift_All::LShift_B2_C1<Fw32s>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class LShift_B1_C1 : public Shift_All::LShift_B1_C1<Fw32s>
{
public:
    FE_SSE2_REF
    LShift_B1_C1(Fw32u value): Shift_All::LShift_B1_C1<Fw32s>(value) { }
    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sll_epi32 ( reg.dst[0].i, Shift_All::LShift_B1_C1<Fw32s>::value.i);
    }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val, 0, 0, 0);
    }
};

class LShift_B2_AC4 : public Shift_All::Shift_B2_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    LShift_B2_AC4(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C4_init(chMask, Shift_All::Shift_B2_AC4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_32s_AC4_sse2(r, chMask);
    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        LShift<Fw32s, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw32s>::val);
    }
};

class LShift_B1_AC4 : public Shift_All::Shift_B1_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    LShift_B1_AC4(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C4_init(chMask, Shift_All::Shift_B1_AC4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_32s_AC4_I_sse2(r, chMask);
    }
    IV REFR( Fw32s *sd ) const
    {
        LShift<Fw32s, AC4>(sd, Shift_All::Shift_B1_AC4<Fw32s>::val);
    }
};

class LShift_B2_C3 : public Shift_All::Shift_B2_C3<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    LShift_B2_C3(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C3_init(chMask, Shift_All::Shift_B2_C3<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_32s_C3_sse2(r, chMask);
    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        LShift<Fw32s, C3>(s, d, Shift_All::Shift_B2_C3<Fw32s>::val);
    }
};

class LShift_B1_C3 : public Shift_All::Shift_B1_C3<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[4];
    LShift_B1_C3(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C3_init(chMask, Shift_All::Shift_B1_C3<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_32s_C3_I_sse2(r, chMask);
    }
    IV REFR( Fw32s *sd ) const
    {
        LShift<Fw32s, C3>(sd, Shift_All::Shift_B1_C3<Fw32s>::val);
    }
};

class LShift_B2_C4 : public Shift_All::Shift_B2_C4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    LShift_B2_C4(const Fw32u value[4]): Shift_All::Shift_B2_C4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C4_init(chMask,Shift_All::Shift_B2_C4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_32s_C4_sse2(r, chMask);
    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        LShift<Fw32s, C4>(s, d, Shift_All::Shift_B2_C4<Fw32s>::val);
    }
};

class LShift_B1_C4 : public Shift_All::Shift_B1_C4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 chMask[8];
    LShift_B1_C4(const Fw32u value[4]): Shift_All::Shift_B1_C4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_C4_init(chMask, Shift_All::Shift_B1_C4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift_32s_C4_I_sse2(r, chMask);
    }
    IV REFR( Fw32s *sd ) const
    {
        LShift<Fw32s, C4>(sd, Shift_All::Shift_B1_C4<Fw32s>::val);
    }
};


class LShift_B2_C3_eq : public Shift_All::Shift_B2_C3<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B2_C3_eq(const Fw32u value[3]): Shift_All::Shift_B2_C3<Fw32s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift32sC3_eq_sse2(value, r);
    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        LShift<Fw32s, C3>(s, d, Shift_All::Shift_B2_C3<Fw32s>::val);
    }
};

class LShift_B1_C3_eq : public Shift_All::Shift_B1_C3<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B1_C3_eq(const Fw32u value[3]): Shift_All::Shift_B1_C3<Fw32s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift32sC3_eq_I_sse2(value, r);
    }
    IV REFR( Fw32s *sd ) const
    {
        LShift<Fw32s, C3>(sd, Shift_All::Shift_B1_C3<Fw32s>::val);
    }
};

class LShift_B2_C4_eq : public Shift_All::Shift_B2_C4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B2_C4_eq(const Fw32u value[3]): Shift_All::Shift_B2_C4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(Shift_All::Shift_B2_C4<Fw32s>::val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sll_epi32 ( reg.src1[0].i, value.i);

    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        LShift<Fw32s, C4>(s, d, Shift_All::Shift_B2_C4<Fw32s>::val);
    }
};

class LShift_B1_C4_eq : public Shift_All::Shift_B1_C4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    LShift_B1_C4_eq(const Fw32u value[3]): Shift_All::Shift_B1_C4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        value.i= _mm_setr_epi32(Shift_All::Shift_B1_C4<Fw32s>::val[0], 0, 0, 0);
    }

    IV SSE2( RegFile & reg ) const
    {
        reg.dst[0].i = _mm_sll_epi32 ( reg.dst[0].i, value.i);
    }
    IV REFR( Fw32s *sd ) const
    {
        LShift<Fw32s, C4>(sd, Shift_All::Shift_B1_C4<Fw32s>::val);
    }
};

class LShift_B2_AC4_eq : public Shift_All::Shift_B2_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    AC4_Mask mask;
    LShift_B2_AC4_eq(const Fw32u value[3]): Shift_All::Shift_B2_AC4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_AC4_init(mask, value, Shift_All::Shift_B2_AC4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift32sAC4_eq_sse2(mask, r, value );
    }
    IV REFR( const Fw32s *s, Fw32s *d ) const
    {
        LShift<Fw32s, AC4>(s, d, Shift_All::Shift_B2_AC4<Fw32s>::val);
    }
};

class LShift_B1_AC4_eq : public Shift_All::Shift_B1_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    XMM128 value;
    AC4_Mask mask;
    LShift_B1_AC4_eq(const Fw32u value[3]): Shift_All::Shift_B1_AC4<Fw32s>(value) { }
    IV SSE2_Init()
    {
        Shift_32s_AC4_init(mask, value, Shift_All::Shift_B1_AC4<Fw32s>::val);
    }

    IV SSE2( RegFile & r ) const
    {
        LShift32sAC4_eq_I_sse2(mask, r, value );
    }
    IV REFR( Fw32s *sd ) const
    {
        LShift<Fw32s, AC4>(sd, Shift_All::Shift_B1_AC4<Fw32s>::val);
    }
};
}








namespace And_all
{
template< class T, enum CH cs1>
class And_B3 : public fe3< T, cs1,  T, cs1,  T, cs1  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_and_si128( r.src1[0].i, r.src2[0].i );
    }
    IV REFR( const T *s1, const T *s2, T *d ) const
    {
        And_ref( s1, s2, d, cs1 );
    }
};

template< class T>
class And_B3_C3 : public fe3< T, C3,  T, C3,  T, C3  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        And_C3_sse2(r);
    }
    IV REFR( const T *s1, const T *s2, T *d ) const
    {
        And_ref( s1, s2, d, C3 );
    }
};

template< class T, enum CH cs1>
class And_B2 : public fe2< T, cs1,  T, cs1  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_and_si128( r.src1[0].i, r.dst[0].i );
    }
    IV REFR( const T *s1, T *sd ) const
    {
        And_ref_I( s1, sd, cs1 );
    }
};
template< class T>
class And_B2_C3 : public fe2< T, C3,  T, C3  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        And_C3_I_sse2(r);
    }
    IV REFR( const T *s1, T *sd ) const
    {
        And_ref_I( s1, sd, C3 );
    }
};
}
namespace And_8u
{
class And_B3_AC4 : public fe3< Fw8u, AC4,  Fw8u, AC4,  Fw8u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_8uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        And_AC4_sse2(r, data);
    }
    IV REFR( const Fw8u *s1, const Fw8u *s2, Fw8u *d ) const
    {
        And_ref( s1, s2, d, AC4 );
    }
};

class And_B2_AC4 : public fe2< Fw8u, AC4,  Fw8u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_8uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        And_AC4_I_sse2(r, data);
    }
    IV REFR( const Fw8u *s1, Fw8u *sd ) const
    {
        And_ref_I( s1, sd, AC4 );
    }
};
}

namespace And_16u
{

class And_B3_AC4 : public fe3< Fw16u, AC4,  Fw16u, AC4,  Fw16u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_16uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        And_AC4_sse2(r, data);
    }
    IV REFR( const Fw16u *s1, const Fw16u *s2, Fw16u *d ) const
    {
        And_ref( s1, s2, d, AC4 );
    }
};

class And_B2_AC4 : public fe2< Fw16u, AC4,  Fw16u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_16uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        And_AC4_I_sse2(r, data);
    }
    IV REFR( const Fw16u *s1, Fw16u *sd ) const
    {
        And_ref_I( s1, sd, AC4 );
    }
};
}

namespace And_32s
{

class And_B3_AC4 : public fe3< Fw32s, AC4,  Fw32s, AC4,  Fw32s, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_32sAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        And_AC4_sse2(r, data);
    }
    IV REFR( const Fw32s *s1, const Fw32s *s2, Fw32s *d ) const
    {
        And_ref( s1, s2, d, AC4 );
    }
};

class And_B2_AC4 : public fe2< Fw32s, AC4,  Fw32s, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_32sAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        And_AC4_I_sse2(r, data);
    }
    IV REFR( const Fw32s *s1, Fw32s *sd ) const
    {
        And_ref_I( s1, sd, AC4 );
    }
};
}


namespace Or_all
{
template< class T, enum CH cs1>
class Or_B3 : public fe3< T, cs1,  T, cs1,  T, cs1  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_or_si128( r.src1[0].i, r.src2[0].i );
    }
    IV REFR( const T *s1, const T *s2, T *d ) const
    {
        Or_ref( s1, s2, d, cs1 );
    }
};

template< class T>
class Or_B3_C3 : public fe3< T, C3,  T, C3,  T, C3  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        Or_C3_sse2(r);
    }
    IV REFR( const T *s1, const T *s2, T *d ) const
    {
        Or_ref( s1, s2, d, C3 );
    }
};

template< class T, enum CH cs1>
class Or_B2 : public fe2< T, cs1,  T, cs1  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_or_si128( r.src1[0].i, r.dst[0].i );
    }
    IV REFR( const T *s1, T *sd ) const
    {
        Or_ref_I( s1, sd, cs1 );
    }
};
template< class T>
class Or_B2_C3 : public fe2< T, C3,  T, C3  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        Or_C3_I_sse2(r);
    }
    IV REFR( const T *s1, T *sd ) const
    {
        Or_ref_I( s1, sd, C3 );
    }
};
}
namespace Or_8u
{

class Or_B3_AC4 : public fe3< Fw8u, AC4,  Fw8u, AC4,  Fw8u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_8uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Or_AC4_sse2(r, data);
    }
    IV REFR( const Fw8u *s1, const Fw8u *s2, Fw8u *d ) const
    {
        Or_ref( s1, s2, d, AC4 );
    }
};

class Or_B2_AC4 : public fe2< Fw8u, AC4,  Fw8u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_8uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Or_AC4_I_sse2(r, data);
    }
    IV REFR( const Fw8u *s1, Fw8u *sd ) const
    {
        Or_ref_I( s1, sd, AC4 );
    }
};
}

namespace Or_16u
{

class Or_B3_AC4 : public fe3< Fw16u, AC4,  Fw16u, AC4,  Fw16u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_16uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Or_AC4_sse2(r, data);
    }
    IV REFR( const Fw16u *s1, const Fw16u *s2, Fw16u *d ) const
    {
        Or_ref( s1, s2, d, AC4 );
    }
};

class Or_B2_AC4 : public fe2< Fw16u, AC4,  Fw16u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_16uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Or_AC4_I_sse2(r, data);
    }
    IV REFR( const Fw16u *s1, Fw16u *sd ) const
    {
        Or_ref_I( s1, sd, AC4 );
    }
};
}

namespace Or_32s
{

class Or_B3_AC4 : public fe3< Fw32s, AC4,  Fw32s, AC4,  Fw32s, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_32sAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Or_AC4_sse2(r, data);
    }
    IV REFR( const Fw32s *s1, const Fw32s *s2, Fw32s *d ) const
    {
        Or_ref( s1, s2, d, AC4 );
    }
};

class Or_B2_AC4 : public fe2< Fw32s, AC4,  Fw32s, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_32sAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Or_AC4_I_sse2(r, data);
    }
    IV REFR( const Fw32s *s1, Fw32s *sd ) const
    {
        Or_ref_I( s1, sd, AC4 );
    }
};
}

namespace Xor_all
{
template< class T, enum CH cs1>
class Xor_B3 : public fe3< T, cs1,  T, cs1,  T, cs1  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_xor_si128( r.src1[0].i, r.src2[0].i );
    }
    IV REFR( const T *s1, const T *s2, T *d ) const
    {
        Xor_ref( s1, s2, d, cs1 );
    }
};

template< class T>
class Xor_B3_C3 : public fe3< T, C3,  T, C3,  T, C3  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        Xor_C3_sse2(r);
    }
    IV REFR( const T *s1, const T *s2, T *d ) const
    {
        Xor_ref( s1, s2, d, C3 );
    }
};

template< class T, enum CH cs1>
class Xor_B2 : public fe2< T, cs1,  T, cs1  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_xor_si128( r.src1[0].i, r.dst[0].i );
    }
    IV REFR( const T *s1, T *sd ) const
    {
        Xor_ref_I( s1, sd, cs1 );
    }
};
template< class T>
class Xor_B2_C3 : public fe2< T, C3,  T, C3  >
{
public:
    FE_SSE2_REF
    IV SSE2_Init(){}
    IV SSE2( RegFile & r ) const
    {
        Xor_C3_I_sse2(r);
    }
    IV REFR( const T *s1, T *sd ) const
    {
        Xor_ref_I( s1, sd, C3 );
    }
};
}
namespace Xor_8u
{

class Xor_B3_AC4 : public fe3< Fw8u, AC4,  Fw8u, AC4,  Fw8u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_8uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Xor_AC4_sse2(r, data);
    }
    IV REFR( const Fw8u *s1, const Fw8u *s2, Fw8u *d ) const
    {
        Xor_ref( s1, s2, d, AC4 );
    }
};

class Xor_B2_AC4 : public fe2< Fw8u, AC4,  Fw8u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_8uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Xor_AC4_I_sse2(r, data);
    }
    IV REFR( const Fw8u *s1, Fw8u *sd ) const
    {
        Xor_ref_I( s1, sd, AC4 );
    }
};
}

namespace Xor_16u
{

class Xor_B3_AC4 : public fe3< Fw16u, AC4,  Fw16u, AC4,  Fw16u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_16uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Xor_AC4_sse2(r, data);
    }
    IV REFR( const Fw16u *s1, const Fw16u *s2, Fw16u *d ) const
    {
        Xor_ref( s1, s2, d, AC4 );
    }
};

class Xor_B2_AC4 : public fe2< Fw16u, AC4,  Fw16u, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_16uAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Xor_AC4_I_sse2(r, data);
    }
    IV REFR( const Fw16u *s1, Fw16u *sd ) const
    {
        Xor_ref_I( s1, sd, AC4 );
    }
};
}
namespace Xor_32s
{

class Xor_B3_AC4 : public fe3< Fw32s, AC4,  Fw32s, AC4,  Fw32s, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_32sAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Xor_AC4_sse2(r, data);
    }
    IV REFR( const Fw32s *s1, const Fw32s *s2, Fw32s *d ) const
    {
        Xor_ref( s1, s2, d, AC4 );
    }
};

class Xor_B2_AC4 : public fe2< Fw32s, AC4,  Fw32s, AC4  >
{
public:
    FE_SSE2_REF

    AC4_Mask data;
    IV SSE2_Init()
    {
        All_32sAC4_func(data);
    }

    IV SSE2( RegFile & r ) const
    {
        Xor_AC4_I_sse2(r, data);
    }
    IV REFR( const Fw32s *s1, Fw32s *sd ) const
    {
        Xor_ref_I( s1, sd, AC4 );
    }
};
}




namespace AllC
{
template< class T>
class AllC_B2_C1 : public fe2< T,  C1,  T, C1  >
{
public:
    T val;
    XMM128 value;
    AllC_B2_C1(T v):val(v){ }
};
template< class T>
class AllC_B2_C3 : public fe2< T,  C3,  T, C3  >
{
public:
    T val[3];
    XMM128 value[3];
    AllC_B2_C3(const T v[]){
        val[0] = v[0], val[1] = v[1], val[2] =v[2];
    }
};

template< class T>
class AllC_B2_C4 : public fe2< T,  C4,  T, C4  >
{
public:
    T val[4];
    XMM128 value;
    AllC_B2_C4(const T v[]){
        val[0] = v[0], val[1] = v[1], val[2] = v[2], val[3] = v[3];
    }
};

template< class T>
class AllC_B2_AC4 : public fe2< T,  AC4,  T, AC4  >
{
public:
    T val[3];
    XMM128 value;
    AC4_Mask data;
    AllC_B2_AC4(const T v[]){
        val[0] = v[0], val[1] = v[1], val[2] = v[2];
    }
};


template< class T>
class AllC_B1_C1 : public fe1< T,  C1  >
{
public:
    T val;
    XMM128 value;
    AllC_B1_C1(T v):val(v){ }
};
template< class T>
class AllC_B1_C3 : public fe1< T,  C3  >
{
public:
    T val[3];
    XMM128 value[3];
    AllC_B1_C3(const T v[]){
        val[0] = v[0], val[1] = v[1], val[2] =v[2];
    }
};

template< class T>
class AllC_B1_C4 : public fe1< T,  C4  >
{
public:
    T val[4];
    XMM128 value;
    AllC_B1_C4(const T v[]){
        val[0] = v[0], val[1] = v[1], val[2] = v[2], val[3] = v[3];
    }
};

template< class T>
class AllC_B1_AC4 : public fe1< T,  AC4  >
{
public:
    T val[3];
    XMM128 value;
    AC4_Mask data;
    AllC_B1_AC4(const T v[]){
        val[0] = v[0], val[1] = v[1], val[2] = v[2];
    }
};
}
namespace AndC_all
{

template< class T>
class AndC_B2_C1 : public AllC::AllC_B2_C1<T>
{
public:
    FE_SSE2_REF
    AndC_B2_C1(T value): AllC::AllC_B2_C1<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_and_si128( r.src1[0].i, AllC::AllC_B2_C1<T>::value.i );
    }
    IV REFR( const T *s, T *d ) const
    {
        And_ref( s, AllC::AllC_B2_C1<T>::val, d, C1 );
    }
    IV SSE2_Init()
    {
		AllC_C1_func(AllC::AllC_B2_C1<T>::val, AllC::AllC_B2_C1<T>::value);
    }
};
template< class T>
class AndC_B2_C3 : public AllC::AllC_B2_C3<T>
{
public:
    FE_SSE2_REF
    AndC_B2_C3(const T value[]): AllC::AllC_B2_C3<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        AndC_C3_sse2(r, AllC::AllC_B2_C3<T>::value);
    }
    IV REFR( const T *s, T *d ) const
    {
        And_ref( s, AllC::AllC_B2_C3<T>::val, d, C3 );
    }
    IV SSE2_Init()
    {
		AllC_C3_func(AllC::AllC_B2_C3<T>::val, AllC::AllC_B2_C3<T>::value);
    }
};
template< class T>
class AndC_B2_C4 : public AllC::AllC_B2_C4<T>
{
public:
    FE_SSE2_REF
    AndC_B2_C4(const T value[]): AllC::AllC_B2_C4<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_and_si128( r.src1[0].i, AllC::AllC_B2_C4<T>::value.i );
    }
    IV REFR( const T *s, T *d ) const
    {
        And_ref( s, AllC::AllC_B2_C4<T>::val, d, C4 );
    }
    IV SSE2_Init()
    {
        AllC_C4_func(AllC::AllC_B2_C4<T>::val, AllC::AllC_B2_C4<T>::value);
    }
};


template< class T>
class AndC_B1_C1 : public AllC::AllC_B1_C1<T>
{
public:
    FE_SSE2_REF
    AndC_B1_C1(T value): AllC::AllC_B1_C1<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_and_si128( r.dst[0].i, AllC::AllC_B1_C1<T>::value.i );
    }
    IV REFR( T *d ) const
    {
        And_ref_I( AllC::AllC_B1_C1<T>::val, d, C1 );
    }
    IV SSE2_Init()
    {
        AllC_C1_func(AllC::AllC_B1_C1<T>::val, AllC::AllC_B1_C1<T>::value);
    }
};
template< class T>
class AndC_B1_C3 : public AllC::AllC_B1_C3<T>
{
public:
    FE_SSE2_REF
    AndC_B1_C3(const T value[]): AllC::AllC_B1_C3<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        AndC_C3_I_sse2(r, AllC::AllC_B1_C3<T>::value);
    }
    IV REFR( T *d ) const
    {
        And_ref_I( AllC::AllC_B1_C3<T>::val, d, C3 );
    }
    IV SSE2_Init()
    {
        AllC_C3_func(AllC::AllC_B1_C3<T>::val, AllC::AllC_B1_C3<T>::value);
    }
};
template< class T>
class AndC_B1_C4 : public AllC::AllC_B1_C4<T>
{
public:
    FE_SSE2_REF
    AndC_B1_C4(const T value[]): AllC::AllC_B1_C4<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_and_si128( r.dst[0].i, AllC::AllC_B1_C4<T>::value.i );
    }
    IV REFR( T *d ) const
    {
        And_ref_I( AllC::AllC_B1_C4<T>::val, d, C4 );
    }
    IV SSE2_Init()
    {
        AllC_C4_func(AllC::AllC_B1_C4<T>::val, AllC::AllC_B1_C4<T>::value);
    }
};
}
namespace AndC_8u
{

class AndC_B2_AC4 : public AllC::AllC_B2_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    AndC_B2_AC4(const Fw8u value[]): AllC::AllC_B2_AC4<Fw8u>(value){ }
    IV SSE2_Init()
    {
        AllC_8u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        AndC_AC4_sse2(r, data, value);
    }
    IV REFR( const Fw8u *s, Fw8u *sd ) const
    {
        And_ref( s, val, sd, AC4 );
    }
};

class AndC_B1_AC4 : public AllC::AllC_B1_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    AndC_B1_AC4(const Fw8u value[]): AllC::AllC_B1_AC4<Fw8u>(value){ }
    IV SSE2_Init()
    {
        AllC_8u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        AndC_AC4_I_sse2(r, data, value);
    }
    IV REFR( Fw8u *sd ) const
    {
        And_ref_I( val, sd, AC4 );
    }
};
}
namespace AndC_16u
{

class AndC_B2_AC4 : public AllC::AllC_B2_AC4<Fw16u>
{
public:
    FE_SSE2_REF
    AndC_B2_AC4(const Fw16u value[]): AllC::AllC_B2_AC4<Fw16u>(value){ }
    IV SSE2_Init()
    {
        AllC_16u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        AndC_AC4_sse2(r, data, value);
    }
    IV REFR( const Fw16u *s, Fw16u *sd ) const
    {
        And_ref( s, val, sd, AC4 );
    }
};

class AndC_B1_AC4 : public AllC::AllC_B1_AC4<Fw16u>
{
public:
    FE_SSE2_REF
    AndC_B1_AC4(const Fw16u value[]): AllC::AllC_B1_AC4<Fw16u>(value){ }
    IV SSE2_Init()
    {
        AllC_16u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        AndC_AC4_I_sse2(r, data, value);
    }
    IV REFR( Fw16u *sd ) const
    {
        And_ref_I( val, sd, AC4 );
    }
};
}
namespace AndC_32s
{

class AndC_B2_AC4 : public AllC::AllC_B2_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    AndC_B2_AC4(const Fw32s value[]): AllC::AllC_B2_AC4<Fw32s>(value){ }
    IV SSE2_Init()
    {
        AllC_32s_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        AndC_AC4_sse2(r, data, value);
    }
    IV REFR( const Fw32s *s, Fw32s *sd ) const
    {
        And_ref( s, val, sd, AC4 );
    }
};

class AndC_B1_AC4 : public AllC::AllC_B1_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    AndC_B1_AC4(const Fw32s value[]): AllC::AllC_B1_AC4<Fw32s>(value){ }
    IV SSE2_Init()
    {
        AllC_32s_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        AndC_AC4_I_sse2(r, data, value);
    }
    IV REFR( Fw32s *sd ) const
    {
        And_ref_I( val, sd, AC4 );
    }
};
}
namespace OrC_all
{

template< class T>
class OrC_B2_C1 : public AllC::AllC_B2_C1<T>
{
public:
    FE_SSE2_REF
    OrC_B2_C1(T value): AllC::AllC_B2_C1<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_or_si128( r.src1[0].i, AllC::AllC_B2_C1<T>::value.i );
    }
    IV REFR( const T *s, T *d ) const
    {
        Or_ref( s, AllC::AllC_B2_C1<T>::val, d, C1 );
    }
    IV SSE2_Init()
    {
        AllC_C1_func(AllC::AllC_B2_C1<T>::val, AllC::AllC_B2_C1<T>::value);
    }
};
template< class T>
class OrC_B2_C3 : public AllC::AllC_B2_C3<T>
{
public:
    FE_SSE2_REF
    OrC_B2_C3(const T value[]): AllC::AllC_B2_C3<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        OrC_C3_sse2(r, AllC::AllC_B2_C3<T>::value);
    }
    IV REFR( const T *s, T *d ) const
    {
        Or_ref( s, AllC::AllC_B2_C3<T>::val, d, C3 );
    }
    IV SSE2_Init()
    {
        AllC_C3_func(AllC::AllC_B2_C3<T>::val, AllC::AllC_B2_C3<T>::value);
    }
};
template< class T>
class OrC_B2_C4 : public AllC::AllC_B2_C4<T>
{
public:
    FE_SSE2_REF
    OrC_B2_C4(const T value[]): AllC::AllC_B2_C4<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_or_si128( r.src1[0].i, AllC::AllC_B2_C4<T>::value.i );
    }
    IV REFR( const T *s, T *d ) const
    {
        Or_ref( s, AllC::AllC_B2_C4<T>::val, d, C4 );
    }
    IV SSE2_Init()
    {
        AllC_C4_func(AllC::AllC_B2_C4<T>::val, AllC::AllC_B2_C4<T>::value);
    }
};
template< class T>
class OrC_B1_C1 : public AllC::AllC_B1_C1<T>
{
public:
    FE_SSE2_REF
    OrC_B1_C1(T value): AllC::AllC_B1_C1<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_or_si128( r.dst[0].i, AllC::AllC_B1_C1<T>::value.i );
    }
    IV REFR( T *d ) const
    {
        Or_ref_I( AllC::AllC_B1_C1<T>::val, d, C1 );
    }
    IV SSE2_Init()
    {
        AllC_C1_func(AllC::AllC_B1_C1<T>::val, AllC::AllC_B1_C1<T>::value);
    }
};
template< class T>
class OrC_B1_C3 : public AllC::AllC_B1_C3<T>
{
public:
    FE_SSE2_REF
    OrC_B1_C3(const T value[]): AllC::AllC_B1_C3<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        OrC_C3_I_sse2(r, AllC::AllC_B1_C3<T>::value);
    }
    IV REFR( T *d ) const
    {
        Or_ref_I( AllC::AllC_B1_C3<T>::val, d, C3 );
    }
    IV SSE2_Init()
    {
        AllC_C3_func(AllC::AllC_B1_C3<T>::val, AllC::AllC_B1_C3<T>::value);
    }
};
template< class T>
class OrC_B1_C4 : public AllC::AllC_B1_C4<T>
{
public:
    FE_SSE2_REF
    OrC_B1_C4(const T value[]): AllC::AllC_B1_C4<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_or_si128( r.dst[0].i, AllC::AllC_B1_C4<T>::value.i );
    }
    IV REFR( T *d ) const
    {
        Or_ref_I( AllC::AllC_B1_C4<T>::val, d, C4 );
    }
    IV SSE2_Init()
    {
        AllC_C4_func(AllC::AllC_B1_C4<T>::val, AllC::AllC_B1_C4<T>::value);
    }
};
}
namespace OrC_8u
{

class OrC_B2_AC4 : public AllC::AllC_B2_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    OrC_B2_AC4(const Fw8u value[]): AllC::AllC_B2_AC4<Fw8u>(value){ }
    IV SSE2_Init()
    {
        AllC_8u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        OrC_AC4_sse2(r, data, value);
    }
    IV REFR( const Fw8u *s, Fw8u *sd ) const
    {
        Or_ref( s, val, sd, AC4 );
    }
};

class OrC_B1_AC4 : public AllC::AllC_B1_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    OrC_B1_AC4(const Fw8u value[]): AllC::AllC_B1_AC4<Fw8u>(value){ }
    IV SSE2_Init()
    {
        AllC_8u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        OrC_AC4_I_sse2(r, data, value);
    }
    IV REFR( Fw8u *sd ) const
    {
        Or_ref_I( val, sd, AC4 );
    }
};
}
namespace OrC_16u
{
class OrC_B2_AC4 : public AllC::AllC_B2_AC4<Fw16u>
{
public:
    FE_SSE2_REF
    OrC_B2_AC4(const Fw16u value[]): AllC::AllC_B2_AC4<Fw16u>(value){ }
    IV SSE2_Init()
    {
        AllC_16u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        OrC_AC4_sse2(r, data, value);
    }
    IV REFR( const Fw16u *s, Fw16u *sd ) const
    {
        Or_ref( s, val, sd, AC4 );
    }
};

class OrC_B1_AC4 : public AllC::AllC_B1_AC4<Fw16u>
{
public:
    FE_SSE2_REF
    OrC_B1_AC4(const Fw16u value[]): AllC::AllC_B1_AC4<Fw16u>(value){ }
    IV SSE2_Init()
    {
        AllC_16u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        OrC_AC4_I_sse2(r, data, value);
    }
    IV REFR( Fw16u *sd ) const
    {
        Or_ref_I( val, sd, AC4 );
    }
};
}
namespace OrC_32s
{

class OrC_B2_AC4 : public AllC::AllC_B2_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    OrC_B2_AC4(const Fw32s value[]): AllC::AllC_B2_AC4<Fw32s>(value){ }
    IV SSE2_Init()
    {
        AllC_32s_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        OrC_AC4_sse2(r, data, value);
    }
    IV REFR( const Fw32s *s, Fw32s *sd ) const
    {
        Or_ref( s, val, sd, AC4 );
    }
};

class OrC_B1_AC4 : public AllC::AllC_B1_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    OrC_B1_AC4(const Fw32s value[]): AllC::AllC_B1_AC4<Fw32s>(value){ }
    IV SSE2_Init()
    {
        AllC_32s_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        OrC_AC4_I_sse2(r, data, value);
    }
    IV REFR( Fw32s *sd ) const
    {
        Or_ref_I( val, sd, AC4 );
    }
};
}

namespace XorC_all
{

template< class T>
class XorC_B2_C1 : public AllC::AllC_B2_C1<T>
{
public:
    FE_SSE2_REF
    XorC_B2_C1(T value): AllC::AllC_B2_C1<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_xor_si128( r.src1[0].i, AllC::AllC_B2_C1<T>::value.i );
    }
    IV REFR( const T *s, T *d ) const
    {
        Xor_ref( s, AllC::AllC_B2_C1<T>::val, d, C1 );
    }
    IV SSE2_Init()
    {
        AllC_C1_func(AllC::AllC_B2_C1<T>::val, AllC::AllC_B2_C1<T>::value);
    }
};
template< class T>
class XorC_B2_C3 : public AllC::AllC_B2_C3<T>
{
public:
    FE_SSE2_REF
    XorC_B2_C3(const T value[]): AllC::AllC_B2_C3<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        XorC_C3_sse2(r, AllC::AllC_B2_C3<T>::value);
    }
    IV REFR( const T *s, T *d ) const
    {
        Xor_ref( s, AllC::AllC_B2_C3<T>::val, d, C3 );
    }
    IV SSE2_Init()
    {
        AllC_C3_func(AllC::AllC_B2_C3<T>::val, AllC::AllC_B2_C3<T>::value);
    }
};
template< class T>
class XorC_B2_C4 : public AllC::AllC_B2_C4<T>
{
public:
    FE_SSE2_REF
    XorC_B2_C4(const T value[]): AllC::AllC_B2_C4<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_xor_si128( r.src1[0].i, AllC::AllC_B2_C4<T>::value.i );
    }
    IV REFR( const T *s, T *d ) const
    {
        Xor_ref( s, AllC::AllC_B2_C4<T>::val, d, C4 );
    }
    IV SSE2_Init()
    {
        AllC_C4_func(AllC::AllC_B2_C4<T>::val, AllC::AllC_B2_C4<T>::value);
    }
};
template< class T>
class XorC_B1_C1 : public AllC::AllC_B1_C1<T>
{
public:
    FE_SSE2_REF
    XorC_B1_C1(T value): AllC::AllC_B1_C1<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_xor_si128( r.dst[0].i, AllC::AllC_B1_C1<T>::value.i );
    }
    IV REFR( T *d ) const
    {
        Xor_ref_I( AllC::AllC_B1_C1<T>::val, d, C1 );
    }
    IV SSE2_Init()
    {
        AllC_C1_func(AllC::AllC_B1_C1<T>::val, AllC::AllC_B1_C1<T>::value);
    }
};
template< class T>
class XorC_B1_C3 : public AllC::AllC_B1_C3<T>
{
public:
    FE_SSE2_REF
    XorC_B1_C3(const T value[]): AllC::AllC_B1_C3<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        XorC_C3_I_sse2(r, AllC::AllC_B1_C3<T>::value);
    }
    IV REFR( T *d ) const
    {
        Xor_ref_I( AllC::AllC_B1_C3<T>::val, d, C3 );
    }
    IV SSE2_Init()
    {
        AllC_C3_func(AllC::AllC_B1_C3<T>::val, AllC::AllC_B1_C3<T>::value);
    }
};
template< class T>
class XorC_B1_C4 : public AllC::AllC_B1_C4<T>
{
public:
    FE_SSE2_REF
    XorC_B1_C4(const T value[]): AllC::AllC_B1_C4<T>(value){ }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_xor_si128( r.dst[0].i, AllC::AllC_B1_C4<T>::value.i );
    }
    IV REFR( T *d ) const
    {
        Xor_ref_I( AllC::AllC_B1_C4<T>::val, d, C4 );
    }
    IV SSE2_Init()
    {
        AllC_C4_func(AllC::AllC_B1_C4<T>::val, AllC::AllC_B1_C4<T>::value);
    }
};
}
namespace XorC_8u
{

class XorC_B2_AC4 : public AllC::AllC_B2_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    XorC_B2_AC4(const Fw8u value[]): AllC::AllC_B2_AC4<Fw8u>(value){ }
    IV SSE2_Init()
    {
        AllC_8u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        XorC_AC4_sse2(r, data, value);
    }
    IV REFR( const Fw8u *s, Fw8u *sd ) const
    {
        Xor_ref( s, val, sd, AC4 );
    }
};

class XorC_B1_AC4 : public AllC::AllC_B1_AC4<Fw8u>
{
public:
    FE_SSE2_REF
    XorC_B1_AC4(const Fw8u value[]): AllC::AllC_B1_AC4<Fw8u>(value){ }
    IV SSE2_Init()
    {
        AllC_8u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        XorC_AC4_I_sse2(r, data, value);
    }
    IV REFR( Fw8u *sd ) const
    {
        Xor_ref_I( val, sd, AC4 );
    }
};
}
namespace XorC_16u
{

class XorC_B2_AC4 : public AllC::AllC_B2_AC4<Fw16u>
{
public:
    FE_SSE2_REF
    XorC_B2_AC4(const Fw16u value[]): AllC::AllC_B2_AC4<Fw16u>(value){ }
    IV SSE2_Init()
    {
        AllC_16u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        XorC_AC4_sse2(r, data, value);
    }
    IV REFR( const Fw16u *s, Fw16u *sd ) const
    {
        Xor_ref( s, val, sd, AC4 );
    }
};

class XorC_B1_AC4 : public AllC::AllC_B1_AC4<Fw16u>
{
public:
    FE_SSE2_REF
    XorC_B1_AC4(const Fw16u value[]): AllC::AllC_B1_AC4<Fw16u>(value){ }
    IV SSE2_Init()
    {
        AllC_16u_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        XorC_AC4_I_sse2(r, data, value);
    }
    IV REFR( Fw16u *sd ) const
    {
        Xor_ref_I( val, sd, AC4 );
    }
};
}
namespace XorC_32s
{

class XorC_B2_AC4 : public AllC::AllC_B2_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    XorC_B2_AC4(const Fw32s value[]): AllC::AllC_B2_AC4<Fw32s>(value){ }
    IV SSE2_Init()
    {
        AllC_32s_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        XorC_AC4_sse2(r, data, value);
    }
    IV REFR( const Fw32s *s, Fw32s *sd ) const
    {
        Xor_ref( s, val, sd, AC4 );
    }
};

class XorC_B1_AC4 : public AllC::AllC_B1_AC4<Fw32s>
{
public:
    FE_SSE2_REF
    XorC_B1_AC4(const Fw32s value[]): AllC::AllC_B1_AC4<Fw32s>(value){ }
    IV SSE2_Init()
    {
        AllC_32s_AC4_func(data, val, value);
    }

    IV SSE2( RegFile & r ) const
    {
        XorC_AC4_I_sse2(r, data, value);
    }
    IV REFR( Fw32s *sd ) const
    {
        Xor_ref_I( val, sd, AC4 );
    }
};
}

namespace Not_All
{
template< class T, enum CH cs1>
class Not_B2 : public fe2< T, cs1, T, cs1  >
{
    XMM128 one;
public:
    FE_SSE2_REF
    IV SSE2_Init()
    {
        one.i = _mm_setzero_si128();
        one.i = _mm_cmpeq_epi32(one.i, one.i);
    }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_andnot_si128( r.src1[0].i, one.i );
    }
    IV REFR( const T *s, T *d ) const
    {
        Not_ref( s, d, cs1 );
    }
};

template< class T, enum CH cs1>
class Not_B1 : public fe1< T, cs1  >
{
    XMM128 one;
public:
    FE_SSE2_REF
    IV SSE2_Init()
    {
        one.i = _mm_setzero_si128();
        one.i = _mm_cmpeq_epi32(one.i,one.i);
    }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_andnot_si128( r.dst[0].i, one.i );
    }
    IV REFR( T *d ) const
    {
        Not_ref_I( d, cs1 );
    }
};

class Not_B2_C3 : public fe2< Fw8u, C3, Fw8u, C3  >
{
    XMM128 one;
public:
    FE_SSE2_REF
    IV SSE2_Init()
    {
        one.i = _mm_setzero_si128();
        one.i = _mm_cmpeq_epi32(one.i, one.i);
    }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_andnot_si128( r.src1[0].i, one.i );
        r.dst[1].i = _mm_andnot_si128( r.src1[1].i, one.i );
        r.dst[2].i = _mm_andnot_si128( r.src1[2].i, one.i );
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        Not_ref( s, d, C3 );
    }
};

class Not_B1_C3 : public fe1< Fw8u, C3  >
{
    XMM128 one;
public:
    FE_SSE2_REF
    IV SSE2_Init()
    {
        one.i = _mm_setzero_si128();
        one.i = _mm_cmpeq_epi32(one.i,one.i);
    }
    IV SSE2( RegFile & r ) const
    {
        r.dst[0].i = _mm_andnot_si128( r.dst[0].i, one.i );
        r.dst[1].i = _mm_andnot_si128( r.dst[1].i, one.i );
        r.dst[2].i = _mm_andnot_si128( r.dst[2].i, one.i );
    }
    IV REFR( Fw8u *d ) const
    {
        Not_ref_I( d, C3 );
    }
};

class Not_B2_AC4 : public fe2< Fw8u, AC4, Fw8u, AC4  >
{
    XMM128 one;
    AC4_Mask mask;
public:
    FE_SSE2_REF
    IV SSE2_Init()
    {
        Not_8u_AC4_func(mask,one);
    }
    IV SSE2( RegFile & r ) const
    {
        Not_AC4_sse2(r, mask, one);
    }
    IV REFR( const Fw8u *s, Fw8u *d ) const
    {
        Not_ref( s, d, AC4 );
    }
};

class Not_B1_AC4 : public fe1< Fw8u, AC4  >
{
    XMM128 one;
    AC4_Mask mask;
public:
    FE_SSE2_REF
    IV SSE2_Init()
    {
        Not_8u_AC4_func(mask,one);
    }
    IV SSE2( RegFile & r ) const
    {
        Not_AC4_I_sse2(r, mask, one);
    }
    IV REFR( Fw8u *d ) const
    {
        Not_ref_I( d, AC4 );
    }
};
}
}
} // namespace OPT_LEVEL

