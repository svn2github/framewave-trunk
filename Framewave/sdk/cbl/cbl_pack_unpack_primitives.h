/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_PACKUNPACK_PRIMITIVES_H__
#define __CBL_PACKUNPACK_PRIMITIVES_H__

/// @cond OPT_DETAILS
namespace CBL_Library_Internal
{
    CBL_INLINE long RoundValue( double x )
    {
        if( x < 0 )
        {
            return static_cast<long>( x - 0.5 );
        }
        else
        {
            return static_cast<long>( x + 0.5 );
        }
    }

    template< typename TS, typename TD >
    CBL_INLINE TD SaturateValue( TS value )
    {
        if( value < Limits<TD>::MinValue() )
        {
            return Limits<TD>::MinValue();
        }
        else
        {
            if( value > Limits<TD>::MaxValue() )
            {
                return Limits<TD>::MaxValue();
            }
            else
            {
                return static_cast<TD>(value);
            }
        }
    }

    template< typename TD >
    CBL_INLINE TD SaturateFPValue( F64 value )
    {
        if( value < Limits<TD>::MinValue() )
        {
            return Limits<TD>::MinValue();
        }
        else
        {
            if( value > Limits<TD>::MaxValue() )
            {
                return Limits<TD>::MaxValue();
            }
            else
            {
                return static_cast<TD>(RoundValue(value));
            }
        }
    }
}
/// @endcond

/*! 
    result.s32[0] = Saturate(srcLo.f64[0]);
    result.s32[1] = Saturate(srcLo.f64[1]);
    result.s32[2] = Saturate(srcHi.f64[0]);
    result.s32[3] = Saturate(srcHi.f64[1]);

    where the Saturate function caps the value at min and max 32-bit signed integer value
*/
CBL_INLINE __m128i CBL_REF::Pack64FTo32S( const __m128d & srcLo, const __m128d & srcHi )
{
    const XMM128 & sl = reinterpret_cast<const XMM128 &>( srcLo );
    const XMM128 & sh = reinterpret_cast<const XMM128 &>( srcHi );
    XMM128 result;

    result.s32[0] = CBL_Library_Internal::SaturateFPValue<S32>(sl.f64[0]);
    result.s32[1] = CBL_Library_Internal::SaturateFPValue<S32>(sl.f64[1]);
    result.s32[2] = CBL_Library_Internal::SaturateFPValue<S32>(sh.f64[0]);
    result.s32[3] = CBL_Library_Internal::SaturateFPValue<S32>(sh.f64[1]);

    return result.i;
}

/*! 
    result.s16[0] = Saturate(srcLo.f64[0]);
    result.s16[1] = Saturate(srcLo.f64[1]);

    where the Saturate function caps the value at min and max 16-bit signed integer value
*/
CBL_INLINE __m128i CBL_REF::Pack64FTo16S( const __m128d & src )
{
    const XMM128 & s = reinterpret_cast<const XMM128 &>( src );
    XMM128 result;

    result.s16[0] = CBL_Library_Internal::SaturateFPValue<S16>(s.f64[0]);
    result.s16[1] = CBL_Library_Internal::SaturateFPValue<S16>(s.f64[1]);

    return result.i;
}

/*! 
    result.s16[0] = Saturate(srcLo.f64[0]);
    result.s16[1] = Saturate(srcLo.f64[1]);

    where the Saturate function caps the value at max 16-bit signed integer value
*/
CBL_INLINE __m128i CBL_REF::Pack64FTo16S_nonegative( const __m128d & src )
{
    const XMM128 & s = reinterpret_cast<const XMM128 &>( src );
    XMM128 result;

    if( s.f64[0] > Limits<S16>::MaxValue() )
    {
        result.s16[0] = Limits<S16>::MaxValue();
    }
    else
    {
        result.s16[0] = static_cast<S16>(CBL_Library_Internal::RoundValue(s.f64[0]));
    }

    if( s.f64[1] > Limits<S16>::MaxValue() )
    {
        result.s16[1] = Limits<S16>::MaxValue();
    }
    else
    {
        result.s16[1] = static_cast<S16>(CBL_Library_Internal::RoundValue(s.f64[1]));
    }

    return result.i;
}

/*!
    srcDstLo.s64[0] = srcDstLo.s32[0];
    srcDstLo.s64[1] = srcDstLo.s32[1];
       dstHi.s64[0] = srcDstLo.s32[2];
       dstHi.s64[1] = srcDstLo.s32[3];
*/
CBL_INLINE void CBL_REF::Unpack32STo64S( __m128i & srcDstLo, __m128i & dstHi )
{
    XMM128 & sdl = reinterpret_cast<XMM128&>( srcDstLo );
    XMM128 & dh = reinterpret_cast<XMM128&>( dstHi );

    dh.s64[1] = sdl.s32[3];
    dh.s64[0] = sdl.s32[2];
    sdl.s64[1] = sdl.s32[1];
    sdl.s64[0] = sdl.s32[0];
}

/*!
    srcDstLo.u64[0] = srcDstLo.u32[0];
    srcDstLo.u64[1] = srcDstLo.u32[1];
       dstHi.u64[0] = srcDstLo.u32[2];
       dstHi.u64[1] = srcDstLo.u32[3];
*/
CBL_INLINE void CBL_REF::Unpack32UTo64U( __m128i & srcDstLo, __m128i & dstHi )
{
    XMM128 & sdl = reinterpret_cast<XMM128&>( srcDstLo );
    XMM128 & dh = reinterpret_cast<XMM128&>( dstHi );

    dh.u64[1] = sdl.u32[3];
    dh.u64[0] = sdl.u32[2];
    sdl.u64[1] = sdl.u32[1];
    sdl.u64[0] = sdl.u32[0];
}

/*!
    DstLo.f64[0] = src.s32[0];
    DstLo.f64[1] = src.s32[1];
    dstHi.f64[0] = src.s32[2];
    dstHi.f64[1] = src.s32[3];
*/
CBL_INLINE void CBL_REF::Unpack32STo64F( const __m128i & src, __m128d & dstLo, __m128d & dstHi )
{
    const XMM128 & s = reinterpret_cast<const XMM128&>( src );
    XMM128 & dl = reinterpret_cast<XMM128&>( dstLo );
    XMM128 & dh = reinterpret_cast<XMM128&>( dstHi );
    dh.f64[1] = s.s32[3];
    dh.f64[0] = s.s32[2];
    dl.f64[1] = s.s32[1];
    dl.f64[0] = s.s32[0];
}

/*!
    srcDstLo.s32[0] = srcDstLo.s16[0];
    srcDstLo.s32[1] = srcDstLo.s16[1];
    srcDstLo.s32[2] = srcDstLo.s16[2];
    srcDstLo.s32[3] = srcDstLo.s16[3];
       dstHi.s32[0] = srcDstLo.s16[4];
       dstHi.s32[1] = srcDstLo.s16[5];
       dstHi.s32[2] = srcDstLo.s16[6];
       dstHi.s32[3] = srcDstLo.s16[7];
*/
CBL_INLINE void CBL_REF::Unpack16STo32S( __m128i & srcDstLo, __m128i & dstHi )
{
    XMM128 & sdl = reinterpret_cast<XMM128&>( srcDstLo );
    XMM128 & dh = reinterpret_cast<XMM128&>( dstHi );

    for( int n = 3; n >= 0; n-- )
    {
        dh.s32[n] = sdl.s16[4+n];
    }
    for( int n = 3; n >= 0; n-- )
    {
        sdl.s32[n] = sdl.s16[n];
    }
}

/*!
    srcDstLo.u32[0] = srcDstLo.u16[0];
    srcDstLo.u32[1] = srcDstLo.u16[1];
    srcDstLo.u32[2] = srcDstLo.u16[2];
    srcDstLo.u32[3] = srcDstLo.u16[3];
       dstHi.u32[0] = srcDstLo.u16[4];
       dstHi.u32[1] = srcDstLo.u16[5];
       dstHi.u32[2] = srcDstLo.u16[6];
       dstHi.u32[3] = srcDstLo.u16[7];
*/
CBL_INLINE void CBL_REF::Unpack16UTo32U( __m128i &srcDstLo, __m128i & dstHi )
{
    XMM128 & sdl = reinterpret_cast<XMM128&>( srcDstLo );
    XMM128 & dh = reinterpret_cast<XMM128&>( dstHi );

    for( int n = 3; n >= 0; n-- )
    {
        dh.u32[n] = sdl.u16[4+n];
    }
    for( int n = 3; n >= 0; n-- )
    {
        sdl.u32[n] = sdl.u16[n];
    }
}

/*!
    srcDstLo.s16[0] = srcDstLo.s8[0];
    srcDstLo.s16[1] = srcDstLo.s8[1];
    srcDstLo.s16[2] = srcDstLo.s8[2];
    srcDstLo.s16[3] = srcDstLo.s8[3];
    srcDstLo.s16[4] = srcDstLo.s8[4];
    srcDstLo.s16[5] = srcDstLo.s8[5];
    srcDstLo.s16[6] = srcDstLo.s8[6];
    srcDstLo.s16[7] = srcDstLo.s8[7];
       dstHi.s16[0] = srcDstLo.s8[0];
       dstHi.s16[1] = srcDstLo.s8[1];
       dstHi.s16[2] = srcDstLo.s8[2];
       dstHi.s16[3] = srcDstLo.s8[3];
       dstHi.s16[4] = srcDstLo.s8[4];
       dstHi.s16[5] = srcDstLo.s8[5];
       dstHi.s16[6] = srcDstLo.s8[6];
       dstHi.s16[7] = srcDstLo.s8[7];
*/
CBL_INLINE void CBL_REF::Unpack8UTo16U( __m128i & srcDstLo, __m128i & dstHi )
{
    XMM128 & sdl = reinterpret_cast<XMM128&>( srcDstLo );
    XMM128 & dh = reinterpret_cast<XMM128&>( dstHi );

    for( int n = 7; n >= 0; n-- )
    {
        dh.u16[n] = sdl.u8[8+n];
    }
    for( int n = 7; n >= 0; n-- )
    {
        sdl.u16[n] = sdl.u8[n];
    }
}

/*!
    srcDstLo.u16[0] = srcDstLo.u8[0];
    srcDstLo.u16[1] = srcDstLo.u8[1];
    srcDstLo.u16[2] = srcDstLo.u8[2];
    srcDstLo.u16[3] = srcDstLo.u8[3];
    srcDstLo.u16[4] = srcDstLo.u8[4];
    srcDstLo.u16[5] = srcDstLo.u8[5];
    srcDstLo.u16[6] = srcDstLo.u8[6];
    srcDstLo.u16[7] = srcDstLo.u8[7];
       dstHi.u16[0] = srcDstLo.u8[0];
       dstHi.u16[1] = srcDstLo.u8[1];
       dstHi.u16[2] = srcDstLo.u8[2];
       dstHi.u16[3] = srcDstLo.u8[3];
       dstHi.u16[4] = srcDstLo.u8[4];
       dstHi.u16[5] = srcDstLo.u8[5];
       dstHi.u16[6] = srcDstLo.u8[6];
       dstHi.u16[7] = srcDstLo.u8[7];
*/
CBL_INLINE void CBL_REF::Unpack8STo16S( __m128i & srcDstLo, __m128i & dstHi )
{
    XMM128 & sdl = reinterpret_cast<XMM128&>( srcDstLo );
    XMM128 & dh = reinterpret_cast<XMM128&>( dstHi );

    for( int n = 7; n >= 0; n-- )
    {
        dh.s16[n] = sdl.s8[8+n];
    }
    for( int n = 7; n >= 0; n-- )
    {
        sdl.s16[n] = sdl.s8[n];
    }
}


/// new

CBL_INLINE  void CBL_REF::Unpack8UTo16S( __m128i &srcDstLo, __m128i &dstHi )
{
	
    XMM128 & sdl = reinterpret_cast<XMM128&>( srcDstLo );
    XMM128 & dh = reinterpret_cast<XMM128&>( dstHi );
	
	XMM128 temp;

	temp.u16[0] = 0;

	for (int i = 0; i < 8 ; i++)
	{
		dh.s16[i] = sdl.u8[i+8];
		temp.s16[i] = sdl.u8[i];
	}
	sdl = temp;
}

CBL_INLINE  __m128i CBL_REF::Pack64FTo32SLo( const __m128d &srcLo, const __m128i &src )
{

    const XMM128 & lo = reinterpret_cast<const XMM128&>( srcLo );
    const XMM128 & hi = reinterpret_cast<const XMM128&>( src );

	XMM128 res;
	res.s32[0] = Limits<S32>::Sat(lo.f64[0]); 
	res.s32[1] = Limits<S32>::Sat(lo.f64[1]);
	res.u64[1] = hi.u64[1];

	return res.i;
}

CBL_INLINE  __m128i CBL_REF::Pack32FTo16S(const __m128 &src1,const __m128 &src2)
{
    const XMM128 & lo = reinterpret_cast<const XMM128&>( src1 );
    const  XMM128 & hi = reinterpret_cast<const XMM128&>( src2 );

	XMM128 res;
	res.s16[0] = Limits<S16>::Sat(lo.f32[0]); 
	res.s16[1] = Limits<S16>::Sat(lo.f32[1]);
	res.s16[2] = Limits<S16>::Sat(lo.f32[2]); 
	res.s16[3] = Limits<S16>::Sat(lo.f32[3]);

	res.s16[4] = Limits<S16>::Sat(hi.f32[0]); 
	res.s16[5] = Limits<S16>::Sat(hi.f32[1]);
	res.s16[6] = Limits<S16>::Sat(hi.f32[2]); 
	res.s16[7] = Limits<S16>::Sat(hi.f32[3]);

	return res.i;
}


CBL_INLINE  __m128i CBL_REF::Pack64STo32S( const __m128i &srcLo, const __m128i &srcHi )
{

   const  XMM128 & lo = reinterpret_cast<const XMM128&>( srcLo );
   const XMM128 & hi = reinterpret_cast<const XMM128&>( srcHi );

	XMM128 res;
	res.s32[0] = Limits<S32>::Sat(lo.s64[0]); 
	res.s32[1] = Limits<S32>::Sat(lo.s64[1]);
	res.s32[2] = Limits<S32>::Sat(hi.s64[0]); 
	res.s32[3] = Limits<S32>::Sat(hi.s64[1]);

	return res.i;
}

CBL_INLINE  __m128 CBL_REF::Pack64STo32F( const __m128i &srcLo, const __m128i &srcHi )
{
	XMM128 slo,shi,dest;
	slo.i = srcLo;
	shi.i = srcHi;
	
	dest.f32[0] = (F32) slo.s64[0];
	dest.f32[1] = (F32) slo.s64[1];
	dest.f32[2] = (F32) shi.s64[0];
	dest.f32[3] = (F32) shi.s64[1];

	return dest.f;
}


#endif // __CBL_PACKUNPACK_PRIMITIVES_H__
