/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_COMMON_PRIMITIVES_H__
#define __CBL_COMMON_PRIMITIVES_H__

// Bitness

#if		defined( WIN64 ) || defined( _WIN64 ) || \
		defined( LIN64 ) || defined( _LIN64 ) || \
        defined( SOL64 ) || defined( _SOL64 ) || \
        defined( MAC64 ) || defined( _MAC64 )

#define CBL_SYS_64
#define CBL_XMM_REG_COUNT 16

#elif	defined( WIN32 ) || defined( _WIN32 ) || \
		defined( LIN32 ) || defined( _LIN32 ) || \
        defined( SOL32 ) || defined( _SOL32 ) || \
        defined( MAC32 ) || defined( _MAC32 )

#define CBL_SYS_32
#define CBL_XMM_REG_COUNT 8

#endif

// Compiler

#if		defined( _MSC_VER )

 #define CBL_MSVC

#elif	defined( __GNUC__ )

 #define CBL_GNUC

#elif   defined( __SUNPRO_CC )

 #define CBL_SUNC

#endif



//=======================================
//				Types
//=======================================
typedef signed char			S8;	
typedef unsigned char		U8;
typedef unsigned short		U16;
typedef unsigned int		U32;
typedef unsigned long long	U64;
typedef signed short		S16;
typedef signed int			S32;
typedef signed long long	S64;
typedef float				F32;
typedef double				F64;



#if defined( CBL_SYS_64 )
#define __cbl_offsetof_(s,m)   (size_t)( (ptrdiff_t)&(((s *)0)->m) )
#else
#define __cbl_offsetof_(s,m)   (size_t)&(((s *)0)->m)
#endif

#if defined( CBL_MSVC )
#define CBL_FORCEALIGN_16	__declspec(align(16))
#elif defined( CBL_GNUC )
#define CBL_FORCEALIGN_16	__attribute__((aligned(16)))
#elif defined( CBL_SUNC )
#define CBL_FORCEALIGN_16 
#endif

#if defined( CBL_MSVC )
#define CBL_INLINE			__forceinline
#elif defined( CBL_GNUC )
#define CBL_INLINE          __inline__
#elif defined( CBL_SUNC )
#define CBL_INLINE          inline
#endif

#ifdef CBL_REF_ONLY

#ifdef CBL_MSVC
	typedef union __m64
	{
	    unsigned __int64     m64_u64;
    } __m64;
	typedef union __m128 {
	     float               m128_f32[4];
	} __m128;
	typedef union __m128i {
	    __int8               m128i_i8[16];
	} __m128i;
	typedef struct __m128d {
	    double               m128d_f64[2];
	} __m128d;
#else // !CBL_MSVC
	typedef union __m64 {
	    int                  m64_i32[2];
	} __m64;
	typedef union __m128 {
	     float               m128_f32[4];
	} __m128;
	typedef union __m128i {
	    long long            m128i_i64[2];
	} __m128i;
	typedef struct __m128d {
	    double               m128d_f64[2];
	} __m128d;
#endif // CBL_MSVC

#else // !CBL_REF_ONLY
#ifndef BLD_REF_ONLY
#ifdef CBL_MSVC
#include <intrin.h>
#else // !CBL_MSVC
#include <pmmintrin.h>
#endif // CBL_MSVC
#endif // BLD_REF_ONLY
#endif // CBL_REF_ONLY

#if defined (BLD_REF_ONLY)

#ifdef CBL_FORCEALIGN_16
#undef CBL_FORCEALIGN_16
#endif

#define CBL_FORCEALIGN_16

#endif

union CBL_FORCEALIGN_16 XMM128
{
    __m128	f;
    __m128d	d;
    __m128i	i;
    __m64	m64[2];	
    double	f64[2];
    U64		u64[2];
    S64		s64[2];
    float	f32[4];	
    S32		s32[4];
    U32		u32[4];
    S16		s16[8];
    U16		u16[8];
    U8		u8 [16];
    S8      s8 [16];  
};

#if defined( CBL_MSVC )

namespace CBL_LIBRARY
{
    static union {
        // if you get a compiler error in the lines below, then the declaration of the
        // __m128 __m128i or __m128d must have changed.  The CONST_SET*** macros below
        // assume specific order of members of the __m128* unions.  You need to update
        // the macros accordingly.

        S32 __cbl_m128_check_[__cbl_offsetof_( __m128, m128_f32 )==0];
        S32 __cbl_m128i_check_[__cbl_offsetof_( __m128i, m128i_i8 )==0];
        S32 __cbl_m128d_check_[__cbl_offsetof_( __m128d, m128d_f64 )==0];
    };
} // namespace CBL_LIBRARY

#define CONST_SETR_8I( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p ) \
    { (unsigned __int8)(a), (unsigned __int8)(b), (unsigned __int8)(c), (unsigned __int8)(d), \
      (unsigned __int8)(e), (unsigned __int8)(f), (unsigned __int8)(g), (unsigned __int8)(h), \
      (unsigned __int8)(i), (unsigned __int8)(j), (unsigned __int8)(k), (unsigned __int8)(l), \
      (unsigned __int8)(m), (unsigned __int8)(n), (unsigned __int8)(o), (unsigned __int8)(p) }

#define CONST_SET_8I( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p ) \
    CONST_SETR_8I( (p), (o), (n), (m), (l), (k), (j), (i), (h), (g), (f), (e), (d), (c), (b), (a) )

#define CONST_SET1_8I( x ) \
    CONST_SET_8I( (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x) )

#define CONST_SETZERO_8I() \
    CONST_SET1_8I( 0 )

#define __CNST16I28I_( x ) \
    ((unsigned __int8)((x) & 0xFF)), ((unsigned __int8)(((x) >> 8) & 0xFF))

#define CONST_SETR_16I( a, b, c, d, e, f, g, h ) \
    { __CNST16I28I_((a)), __CNST16I28I_((b)), __CNST16I28I_((c)), __CNST16I28I_((d)), __CNST16I28I_((e)), __CNST16I28I_((f)), __CNST16I28I_((g)), __CNST16I28I_((h)) }

#define CONST_SET_16I( a, b, c, d, e, f, g, h ) \
    CONST_SETR_16I( (h), (g), (f), (e), (d), (c), (b), (a) )

#define CONST_SET1_16I( x ) \
    CONST_SET_16I( (x), (x), (x), (x), (x), (x), (x), (x) )

#define CONST_SETZERO_16I() \
    CONST_SETZERO_8I()

#define __CNST32I28I_( x ) \
    ((unsigned __int8)((x) & 0xFF)), ((unsigned __int8)(((x) >> 8) & 0xFF)), ((unsigned __int8)(((x) >> 16) & 0xFF)), ((unsigned __int8)(((x) >> 24) & 0xFF))

#define CONST_SETR_32I( a, b, c, d ) \
    { __CNST32I28I_((a)), __CNST32I28I_((b)), __CNST32I28I_((c)), __CNST32I28I_((d)) }

#define CONST_SET_32I( a, b, c, d ) \
    CONST_SETR_32I( (d), (c), (b), (a) )

#define CONST_SET1_32I( x ) \
    CONST_SET_32I( (x), (x), (x), (x) )

#define CONST_SETZERO_32I() \
    CONST_SETZERO_8I()

#define __CNST64I28I_( x ) \
    ((unsigned __int8)((x) & 0xFF)), ((unsigned __int8)(((x) >> 8) & 0xFF)), ((unsigned __int8)(((x) >> 16) & 0xFF)), ((unsigned __int8)(((x) >> 24) & 0xFF)), ((unsigned __int8)(((x) >> 32) & 0xFF)), ((unsigned __int8)(((x) >> 40) & 0xFF)), ((unsigned __int8)(((x) >> 48) & 0xFF)), ((unsigned __int8)(((x) >> 56) & 0xFF))

#define CONST_SETR_64I( a, b ) \
    { __CNST64I28I_((a)), __CNST64I28I_((b)) }

#define CONST_SET_64I( a, b ) \
    CONST_SETR_64I( (b), (a) )

#define CONST_SET1_64I( x ) \
    CONST_SET_64I( (x), (x) )

#define CONST_SETZERO_I() \
	{ 0 }

#define CONST_SETZERO_64I() \
    CONST_SETZERO_8I()

#define CONST_SETR_32F( a, b, c, d ) \
    { (a), (b), (c), (d) }

#define CONST_SET_32F( a, b, c, d ) \
    CONST_SETR_32F( (d), (c), (b), (a) )

#define CONST_SET1_32F( x ) \
    CONST_SET_32F( (x), (x), (x), (x) )

#define CONST_SETZERO_32F() \
    CONST_SET1_32F( 0 )

#define CONST_SETR_64F( a, b ) \
    { (a), (b) }

#define CONST_SET_64F( a, b ) \
    CONST_SETR_64F( (b), (a) )

#define CONST_SET1_64F( x ) \
    CONST_SET_64F( (x), (x) )

#define CONST_SETZERO_64F() \
    CONST_SET1_64F( 0 )

#elif defined( CBL_GNUC ) || defined ( CBL_SUNC )

#define __CNST8TO64_( a, b, c, d, e, f, g, h ) \
	( (((h)&0xff)<<56) | (((g)&0xff)<<48) | (((f)&0xff)<<40) | (((e)&0xff)<<32) | \
 	(((d)&0xff)<<24) | (((c)&0xff)<<16) | (((b)&0xff)<<8) | ((a)&0xff) )

#define CONST_SETR_8I( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p ) \
    { __CNST8TO64_( (S64)(a), (S64)(b), (S64)(c), (S64)(d), \
                 (S64)(e), (S64)(f), (S64)(g), (S64)(h) ), \
      __CNST8TO64_( (S64)(i), (S64)(j), (S64)(k), (S64)(l), \
                 (S64)(m), (S64)(n), (S64)(o), (S64)(p) ) }

#define CONST_SET_8I( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p ) \
    CONST_SETR_8I( (p), (o), (n), (m), (l), (k), (j), (i), (h), (g), (f), (e), (d), (c), (b), (a) )

#define CONST_SET1_8I( x ) \
    CONST_SET_8I( (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x), (x) )

#define CONST_SETZERO_8I() \
    { 0, 0 }

// 16 bit integer types

#define __CNST16TO64_( a, b, c, d ) \
	( ((d)<<48) | (((c)&0xFFFF)<<32) | (((b)&0xFFFF)<<16) | ((a)&0xFFFF) )

#define CONST_SETR_16I( a, b, c, d, e, f, g, h ) \
    { __CNST16TO64_( (U64)(a), (U64)(b), \
                     (U64)(c), (U64)(d) ), \
      __CNST16TO64_( (U64)(e), (U64)(f), \
                     (U64)(g), (U64)(h) ) }

#define CONST_SET_16I( a, b, c, d, e, f, g, h ) \
    CONST_SETR_16I( (h), (g), (f), (e), (d), (c), (b), (a) )

#define CONST_SET1_16I( x ) \
    CONST_SET_16I( (x), (x), (x), (x), (x), (x), (x), (x) )

#define CONST_SETZERO_16I() \
    CONST_SETZERO_8I()

// 32 bit integer types

#define __CNST32TO64_( a, b ) \
	( ((b)<<32) | ((a) & 0xFFFFFFFF) )

#define CONST_SETR_32I( a, b, c, d ) \
    { __CNST32TO64_( (U64)(a), (U64)(b) ), \
      __CNST32TO64_( (U64)(c), (U64)(d) ) }

#define CONST_SET_32I( a, b, c, d ) \
    CONST_SETR_32I( (d), (c), (b), (a) )

#define CONST_SET1_32I( x ) \
    CONST_SET_32I( (x), (x), (x), (x) )

#define CONST_SETZERO_32I() \
    CONST_SETZERO_8I()

// 64 bit integer types

#define CONST_SETR_64I( a, b ) \
    { (a), (b) }

#define CONST_SET_64I( a, b ) \
    CONST_SETR_64I( (b), (a) )

#define CONST_SET1_64I( x ) \
    CONST_SET_64I( (x), (x) )

#define CONST_SETZERO_64I() \
    CONST_SETZERO_8I()


// 32 bit single precision floating point types

#define CONST_SETR_32F( a, b, c, d ) \
    { (a), (b), (c), (d) }

#define CONST_SET_32F( a, b, c, d ) \
    CONST_SETR_32F( (d), (c), (b), (a) )

#define CONST_SET1_32F( x ) \
    CONST_SET_32F( (x), (x), (x), (x) )

#define CONST_SETZERO_32F() \
    CONST_SET1_32F( 0 )

// 64 bit double precision floating point types
#define CONST_SETR_64F( a, b ) \
    { (a), (b) }

#define CONST_SET_64F( a, b ) \
    CONST_SETR_64F( (b), (a) )

#define CONST_SET1_64F( x ) \
    CONST_SET_64F( (x), (x) )

#define CONST_SETZERO_64F() \
    CONST_SET1_64F( 0 )

#endif

#define CBL_SHUF_IMM( d, c, b, a ) ( a | (b << 2) | (c<<4) | (d<<6) )

#define CBL_S8_MIN -128
#define CBL_S8_MAX 0x7F
#define CBL_U8_MIN 0x0
#define CBL_U8_MAX 0xFF
#define CBL_S16_MIN -32768
#define CBL_S16_MAX 0x7FFF
#define CBL_U16_MIN 0x0
#define CBL_U16_MAX 0xFFFF
#define CBL_S32_MIN 0x80000000l
#define CBL_S32_MAX 0x7FFFFFFFl
#define CBL_U32_MIN 0x0
#define CBL_U32_MAX 0xFFFFFFFFl
#define CBL_S64_MIN 0x8000000000000000ll
#define CBL_S64_MAX 0x7FFFFFFFFFFFFFFFll
#define CBL_U64_MIN 0x0
#define CBL_U64_MAX 0xFFFFFFFFFFFFFFFFull
#define CBL_F32_MIN -3.402823466e+38F
#define CBL_F32_MAX 3.402823466e+38F
#define CBL_F64_MIN -1.7976931348623158e+308
#define CBL_F64_MAX 1.7976931348623158e+308

namespace CBL_LIBRARY
{

/// @cond OPT_DETAILS
    namespace CBL_Library_Internal
    {
        const __m128i __g_min_unsigned_vector = CONST_SET1_8I( CBL_U8_MIN );
        const __m128i __g_max_unsigned_vector = CONST_SET1_8I( CBL_U8_MAX );
        const __m128i __g_min_S8_vector  = CONST_SET1_8I(  CBL_S8_MIN );
        const __m128i __g_max_S8_vector  = CONST_SET1_8I(  CBL_S8_MAX );
        const __m128i __g_min_S16_vector = CONST_SET1_16I( CBL_S16_MIN );
        const __m128i __g_max_S16_vector = CONST_SET1_16I( CBL_S16_MAX );
        const __m128i __g_min_S32_vector = CONST_SET1_32I( CBL_S32_MIN );
        const __m128i __g_max_S32_vector = CONST_SET1_32I( CBL_S32_MAX );
        const __m128i __g_min_S64_vector = CONST_SET1_64I( CBL_S64_MIN );
        const __m128i __g_max_S64_vector = CONST_SET1_64I( CBL_S64_MAX );
        const __m128  __g_min_F32_vector = CONST_SET1_32F( CBL_F32_MIN );
	    const __m128  __g_max_F32_vector = CONST_SET1_32F( CBL_F32_MAX );
        const __m128d __g_min_F64_vector = CONST_SET1_64F( CBL_F64_MIN );
        const __m128d __g_max_F64_vector = CONST_SET1_64F( CBL_F64_MAX );
    }
/// @endcond

    template <typename TC, typename TV>
    CBL_INLINE TC Saturate(const TC &min, const TV &val, const TC &max)
    {
        if( val < min )
        {
            return min;
        }
        else
        {
            if( val > max )
            {
                return max;
            }
            else
            {
                return (TC)val;
            }
        }
    }

	template< class TD >
	struct Limits
	{
		static void MinValue(){ }	// Should never run this code!!!
		static void MaxValue(){ }	// Should never run this code!!!
        static void MinVector(){ } // Should never run this code!!!
		static void MaxVector(){ } // Should never run this code!!!
	};

	template<>
	struct Limits< U8 >
	{
		static U8 MinValue(){ return CBL_U8_MIN; }
		static U8 MaxValue(){ return CBL_U8_MAX; }
        static __m128i MinVector(){ return CBL_Library_Internal::__g_min_unsigned_vector; }
		static __m128i MaxVector(){ return CBL_Library_Internal::__g_max_unsigned_vector; }
		template < class TS > 
		static U8 Sat( const TS val ){ return (U8)Saturate( MinValue(), val, MaxValue() ); }
	};

	template<>
	struct Limits< U16 >
	{
		static U16 MinValue(){ return CBL_U16_MIN; }
		static U16 MaxValue(){ return CBL_U16_MAX; }
        static __m128i MinVector(){ return CBL_Library_Internal::__g_min_unsigned_vector; }
		static __m128i MaxVector(){ return CBL_Library_Internal::__g_max_unsigned_vector; }
		template < class TS > 
		static U16 Sat( const TS val ){ return (U16) Saturate( MinValue(), val, MaxValue() ); }
	};

	template<>
	struct Limits< U32 >
	{
		static U32 MinValue(){ return CBL_U32_MIN; }
		static U32 MaxValue(){ return CBL_U32_MAX; }
        static __m128i MinVector(){ return CBL_Library_Internal::__g_min_unsigned_vector; }
		static __m128i MaxVector(){ return CBL_Library_Internal::__g_max_unsigned_vector; }
		template < class TS > 
		static U32 Sat( const TS val ){ return (U32) Saturate( MinValue(), val, MaxValue() ); }
	};

	template<>
	struct Limits< U64 >
	{
		static U64 MinValue(){ return CBL_U64_MIN; }
		static U64 MaxValue(){ return CBL_U64_MAX; }
        static __m128i MinVector(){ return CBL_Library_Internal::__g_min_unsigned_vector; }
		static __m128i MaxVector(){ return CBL_Library_Internal::__g_max_unsigned_vector; }
		template < class TS > 
		static U64 Sat( const TS val ){ return (U64) Saturate( MinValue(), val, MaxValue() ); }
	};

	template<>
	struct Limits< S8 >
	{
		static S8 MinValue(){ return CBL_S8_MIN; }
		static S8 MaxValue(){ return CBL_S8_MAX; }
        static __m128i MinVector(){ return CBL_Library_Internal::__g_min_S8_vector; }
		static __m128i MaxVector(){ return CBL_Library_Internal::__g_max_S8_vector; }
		template < class TS > 
		static S8 Sat( const TS val ){ return (S8) Saturate( MinValue(), val, MaxValue() ); }
	};

	template<>
	struct Limits< S16 >
	{
		static S16 MinValue(){ return CBL_S16_MIN; }
		static S16 MaxValue(){ return CBL_S16_MAX; }
        static __m128i MinVector(){ return CBL_Library_Internal::__g_min_S16_vector; }
		static __m128i MaxVector(){ return CBL_Library_Internal::__g_max_S16_vector; }
		template < class TS > 
		static S16 Sat( const TS val ){ return (S16) Saturate( MinValue(), val, MaxValue() ); }
	};

	template<>
	struct Limits< S32 >
	{
		static S32 MinValue(){ return CBL_S32_MIN; }
		static S32 MaxValue(){ return CBL_S32_MAX; }
        static __m128i MinVector(){ return CBL_Library_Internal::__g_min_S32_vector; }
		static __m128i MaxVector(){ return CBL_Library_Internal::__g_max_S32_vector; }
		template < class TS > 
		static S32 Sat( const TS val ){ return (S32) Saturate( MinValue(), val, MaxValue() ); }
	};

	template<>
	struct Limits< S64 >
	{
		static S64 MinValue(){ return CBL_S64_MIN; }
		static S64 MaxValue(){ return CBL_S64_MAX; }
        static __m128i MinVector(){ return CBL_Library_Internal::__g_min_S64_vector; }
		static __m128i MaxVector(){ return CBL_Library_Internal::__g_max_S64_vector; }
		template < class TS > 
		static S64 Sat( const TS val ){ return (S64) Saturate( MinValue(), val, MaxValue() ); }
	};


	template<>
	struct Limits< F32 >
	{
		static F32 MinValue(){ return CBL_F32_MIN; }
		static F32 MaxValue(){ return CBL_F32_MAX; }
        static __m128 MinVector(){ return CBL_Library_Internal::__g_min_F32_vector; }
		static __m128 MaxVector(){ return CBL_Library_Internal::__g_max_F32_vector; }
		template < class TS > 
		static F32 Sat( const TS val ){ return (F32) Saturate( MinValue(), val, MaxValue() ); }
	};

	template<>
	struct Limits< F64 >
	{
		static F64 MinValue(){ return CBL_F64_MIN; }
		static F64 MaxValue(){ return CBL_F64_MAX; }
        static __m128d MinVector(){ return CBL_Library_Internal::__g_min_F64_vector; }
		static __m128d MaxVector(){ return CBL_Library_Internal::__g_max_F64_vector; }
		template < class TS > 
		static F64 Sat( const TS val ){ return (F64) Saturate( MinValue(), val, MaxValue() ); }
	};

/// @cond OPT_DETAILS
    namespace CBL_Library_Internal
    {
	    CBL_INLINE static void add64to128(const S64 &src1,const S64 &src2,U64 &destLo,S64 &destHi)
	    {

		    U64 tt1 = (U32) src1; 
		    U64 tt2 = (U32) src2; 
		    destLo =  tt1 + tt2;
		    tt1 =  (destLo) >> 1;

		    S32 res1 = (S32)tt1;
		    res1 = (U32) res1 >> 31;

		    S64 tv1 = (src1 >> 32);
		    S64 tv2 = (src2 >> 32);
		    S64 temp1 =	tv1 + 	tv2 + res1;

		    destLo = (destLo & 0x00000000FFFFFFFFll) | (temp1 << 32);
		    destHi = (temp1 >> 32);
	    }

	    CBL_INLINE static S64 addSat64to128(const S64 &src1,const S64 &src2)
	    {	
		    U64 reslo;
		    S64 reshi;

		    const S64 min = CBL_S64_MIN;
		    const U64 max = CBL_S64_MAX;
    		
		    add64to128(src1,src2,reslo,reshi);

		    if((reslo >  max) &&  (reshi >= 0))
		    {
					    return max;
		    }
		    else
		    if( (reshi < 0) && (reslo < (U64) min ))
		    {
					    return min;
		    }
		    return reslo;
	    }
    } // namespace CBL_Library_Internal
/// @endcond

    // Extreme cases: The functions returns CBL_S32_MIN for -INF and CBL_S32_MAX for +INF and NAN.
    CBL_INLINE static S32 DoubleToInt(F64 x)
    {

	    F64 di, fract = modf( x, &di );

	    if( di <= (F64)((S32)CBL_S32_MIN))
		    return CBL_S32_MIN;
	    else
	    if( di >= (F64)((S32)CBL_S32_MAX))
		    return CBL_S32_MAX;

	    int i = (S32)di;
	    bool even = !(i&0x1);
	    int res;

	    if     ( fract >   0.5 ) res = i+1;									//  1.6-> 2,  2.6-> 3			
	    else if( fract ==  0.5 ) res = even ? i : i+1;						//  1.5-> 2,  2.5-> 2 
	    else if( fract >=  0.0 ) res = i;			 						//  1.4-> 1,  2.4-> 1
	    else if( fract >  -0.5 ) res = i;									// -1.4->-1, -2.4->-2 
	    else if( fract == -0.5 ) res = even ? i : i-1;						// -1.5->-2, -2.5->-2
	    else					 res = i-1;									// -1.6->-2, -2.6->-3

	    return res;	
    }
	    // Extreme cases: The functions returns CBL_S64_MIN for -INF and CBL_S64_MAX for +INF and NAN.
    CBL_INLINE static S64 DoubleToLong(F64 x)
    {

	    F64 di, fract = modf( x, &di );

	    if( di <= (F64)((S64)CBL_S64_MIN))
		    return CBL_S64_MIN;
	    else
	    if( di >= (F64)((S64)CBL_S64_MAX))
		    return CBL_S64_MAX;

	    S64 i = (S64)di;
	    bool even = !(i&0x1);
	    S64 res;

	    if     ( fract >   0.5 ) res = i+1;									//  1.6-> 2,  2.6-> 3			
	    else if( fract ==  0.5 ) res = even ? i : i+1;						//  1.5-> 2,  2.5-> 2 
	    else if( fract >=  0.0 ) res = i;			 						//  1.4-> 1,  2.4-> 1
	    else if( fract >  -0.5 ) res = i;									// -1.4->-1, -2.4->-2 
	    else if( fract == -0.5 ) res = even ? i : i-1;						// -1.5->-2, -2.5->-2
	    else					 res = i-1;									// -1.6->-2, -2.6->-3

	    return res;	
    }

    CBL_INLINE static S32 FloatToInt(F32 par)
    {
	    return DoubleToInt(par);   
    }

} // namespace CBL_LIBRARY

#endif // __CBL_COMMON_PRIMITIVES_H__
