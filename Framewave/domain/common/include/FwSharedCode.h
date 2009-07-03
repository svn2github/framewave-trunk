/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FW_SHARED_CODE_H__
#define __FW_SHARED_CODE_H__

#include "fwdev.h"

namespace OPT_LEVEL
{

struct FW_REF
{
    //different in rounding from dtoi
    static inline int float_int ( float x )
    {
        double di, fract = modf( x, &di );
        int i = CBL_LIBRARY::Limits<int>::Sat(di);
        bool even = 0 == i % 2;
        int res;

        if     ( fract >   0.5 ) res = i+1;                             //  1.6-> 2,  2.6-> 3
        else if( fract ==  0.5 ) res = even ? ( i == 0 ? 1 : i ) : i+1; //  1.5-> 2,  2.5-> 2
        else if( fract >=  0.0 ) res = i;                               //  1.4-> 1,  2.4-> 1
        else if( fract >  -0.5 ) res = i;                               // -1.4->-1, -2.4->-2
        else if( fract == -0.5 ) res = even ? i : i-1;                  // -1.5->-2, -2.5->-2
        else                     res = i-1;                             // -1.6->-2, -2.6->-3

       return res;
    }

    static inline int dtoi ( double x )
    {
        if( !SYS_FINITE(x) )
            return CBL_LIBRARY::Limits<Fw32s>::MaxValue();
        double di, fract = modf( x, &di );

        int i = CBL_LIBRARY::Limits<int>::Sat ( di );

        bool even = !(i&0x1);
        int res;

        if     ( fract >   0.5 ) res = i+1;            //  1.6-> 2,  2.6-> 3
        else if( fract ==  0.5 ) res = even ? i : i+1; //  1.5-> 2,  2.5-> 2 
        else if( fract >=  0.0 ) res = i;              //  1.4-> 1,  2.4-> 1
        else if( fract >  -0.5 ) res = i;              // -1.4->-1, -2.4->-2 
        else if( fract == -0.5 ) res = even ? i : i-1; // -1.5->-2, -2.5->-2
        else                     res = i-1;            // -1.6->-2, -2.6->-3

        return res;
    }
    static inline int ftoi ( float f  )
    {
        return dtoi( f );
    }
    static inline Fw64s dtol ( double x )
    {

        if( !SYS_FINITE(x) )
            return CBL_LIBRARY::Limits<Fw64s>::MaxValue();;
        double di, fract = modf( x, &di );
        Fw64s i = CBL_LIBRARY::Limits<Fw64s>::Sat( di );
        bool even = !(i&0x1);
        Fw64s res;

        if     ( fract >   0.5 ) res = i+1;              //  1.6-> 2,  2.6-> 3
        else if( fract ==  0.5 ) res = even ? (i) : i+1; //  1.5-> 2,  2.5-> 2 
        else if( fract >=  0.0 ) res = i;                //  1.4-> 1,  2.4-> 1
        else if( fract >  -0.5 ) res = i;                // -1.4->-1, -2.4->-2 
        else if( fract == -0.5 ) res = even ? i : i-1;   // -1.5->-2, -2.5->-2
        else                     res = i-1;              // -1.6->-2, -2.6->-3

        return res;
    }

//    saturates the value to range between lo and hi inclusive
//    the value to be saturated is of type TV, the result and the range limits are of type TR

    template <typename TR, typename TV>
    static SYS_INLINE TR CLIP(const TR &lo, const TV &val, const TR &hi)
    {
        return ( val < lo ? lo : ( val > hi ? hi : (TR)val ) );
    }

    template< class T >
    static U32 BytesToAlign16( const T * a )    // Number of bytes needed to process before a becomes 16 byte aligned
    {
        size_t addr = (size_t) a;
        return (U32)((0x10 - (0xF & addr)) & 0xF);
    }

//      Return an address plus some number of bytes.

    template <class T>
    SYS_WEAKINLINE static T* Offset( T *address, T_OFFSET byteCount )
    {
        U8 *byteAddress = (U8*)address;
        byteAddress += byteCount;
        return (T*)byteAddress;
    }

    template <class T>
    static bool IsAligned( T *ptrA, int stepA )
    {
        return IS_ALIGNED16_1( ptrA ) && IS_ALIGNED16_1(FW_REF::Offset(ptrA,stepA));
    }

    template <class TA, class TB>
    static bool IsAligned( TA *ptrA, int stepA, TB *ptrB, int stepB )
    {
        return IS_ALIGNED16_1( ptrA ) && IS_ALIGNED16_1(FW_REF::Offset(ptrA,stepA))
            && IS_ALIGNED16_1( ptrB ) && IS_ALIGNED16_1(FW_REF::Offset(ptrB,stepB));
    }

    template <class TA, class TB, class TC>
    static bool IsAligned( TA *ptrA, int stepA, TB *ptrB, int stepB, TC *ptrC, int stepC )
    {
        return IS_ALIGNED16_1( ptrA ) && IS_ALIGNED16_1(FW_REF::Offset(ptrA,stepA))
            && IS_ALIGNED16_1( ptrB ) && IS_ALIGNED16_1(FW_REF::Offset(ptrB,stepB))
            && IS_ALIGNED16_1( ptrC ) && IS_ALIGNED16_1(FW_REF::Offset(ptrC,stepC));
    }

    template <class TA, class TB, class TC, class TD >
    static bool IsAligned( TA *ptrA, int stepA, TB *ptrB, int stepB, TC *ptrC, int stepC, TD *ptrD, int stepD)
    {
        return IS_ALIGNED16_1( ptrA ) && IS_ALIGNED16_1(FW_REF::Offset(ptrA,stepA))
            && IS_ALIGNED16_1( ptrB ) && IS_ALIGNED16_1(FW_REF::Offset(ptrB,stepB))
            && IS_ALIGNED16_1( ptrC ) && IS_ALIGNED16_1(FW_REF::Offset(ptrC,stepC))
            && IS_ALIGNED16_1( ptrD ) && IS_ALIGNED16_1(FW_REF::Offset(ptrD,stepD));
    }

    template <class TA, class TB, class TC, class TD,class TE >
    static bool IsAligned( TA *ptrA, int stepA, TB *ptrB, int stepB, TC *ptrC, int stepC, TD *ptrD, int stepD, TE *ptrE, int stepE)
    {
        return IS_ALIGNED16_1( ptrA ) && IS_ALIGNED16_1(FW_REF::Offset(ptrA,stepA))
            && IS_ALIGNED16_1( ptrB ) && IS_ALIGNED16_1(FW_REF::Offset(ptrB,stepB))
            && IS_ALIGNED16_1( ptrC ) && IS_ALIGNED16_1(FW_REF::Offset(ptrC,stepC))
            && IS_ALIGNED16_1( ptrD ) && IS_ALIGNED16_1(FW_REF::Offset(ptrD,stepD))
            && IS_ALIGNED16_1( ptrE ) && IS_ALIGNED16_1(FW_REF::Offset(ptrE,stepE));
    }

    template <class TA, class TB, class TC, class TD,class TE, class TF >
    static bool IsAligned( TA *ptrA, int stepA, TB *ptrB, int stepB, TC *ptrC, int stepC, TD *ptrD, int stepD,TE *ptrE, int stepE,TF *ptrF, int stepF)
    {
        return IS_ALIGNED16_1( ptrA ) && IS_ALIGNED16_1(FW_REF::Offset(ptrA,stepA))
            && IS_ALIGNED16_1( ptrB ) && IS_ALIGNED16_1(FW_REF::Offset(ptrB,stepB))
            && IS_ALIGNED16_1( ptrC ) && IS_ALIGNED16_1(FW_REF::Offset(ptrC,stepC))
            && IS_ALIGNED16_1( ptrD ) && IS_ALIGNED16_1(FW_REF::Offset(ptrD,stepD))
            && IS_ALIGNED16_1( ptrE ) && IS_ALIGNED16_1(FW_REF::Offset(ptrE,stepE))
            && IS_ALIGNED16_1( ptrF ) && IS_ALIGNED16_1(FW_REF::Offset(ptrF,stepF));
    }

    template <class TA, class TB, class TC, class TD,class TE, class TF,class TG, class TH >
    static bool IsAligned( TA *ptrA, int stepA, TB *ptrB, int stepB, TC *ptrC, int stepC, TD *ptrD, int stepD,TE *ptrE, int stepE,TF *ptrF, int stepF,TG *ptrG, int stepG,TH *ptrH, int stepH)
    {
        return IS_ALIGNED16_1( ptrA ) && IS_ALIGNED16_1(FW_REF::Offset(ptrA,stepA))
            && IS_ALIGNED16_1( ptrB ) && IS_ALIGNED16_1(FW_REF::Offset(ptrB,stepB))
            && IS_ALIGNED16_1( ptrC ) && IS_ALIGNED16_1(FW_REF::Offset(ptrC,stepC))
            && IS_ALIGNED16_1( ptrD ) && IS_ALIGNED16_1(FW_REF::Offset(ptrD,stepD))
            && IS_ALIGNED16_1( ptrE ) && IS_ALIGNED16_1(FW_REF::Offset(ptrE,stepE))
            && IS_ALIGNED16_1( ptrF ) && IS_ALIGNED16_1(FW_REF::Offset(ptrF,stepF))
            && IS_ALIGNED16_1( ptrG ) && IS_ALIGNED16_1(FW_REF::Offset(ptrG,stepG))
            && IS_ALIGNED16_1( ptrH ) && IS_ALIGNED16_1(FW_REF::Offset(ptrH,stepH));
    }

//      Increment an address by some number of bytes.

    template< class T >
    SYS_WEAKINLINE static void Inc( T *&address, T_OFFSET byteOffset )
    {
        address = Offset( address, byteOffset );
    }

    template< class T1, class T2 >
    SYS_WEAKINLINE static void Inc( T1 *&address1, T_OFFSET byteOffset1, T2 *&address2, T_OFFSET byteOffset2 )
    {
        address1 = Offset( address1, byteOffset1 );
        address2 = Offset( address2, byteOffset2 );
    }

    template< class T1, class T2, class T3 >
    SYS_WEAKINLINE static void Inc( T1 *&address1, T_OFFSET byteOffset1, T2 *&address2, T_OFFSET byteOffset2, T3 *&address3, T_OFFSET byteOffset3 )
    {
        address1 = Offset( address1, byteOffset1 );
        address2 = Offset( address2, byteOffset2 );
        address3 = Offset( address3, byteOffset3 );
    }

    template< class T1, class T2, class T3, class T4 >
    SYS_WEAKINLINE static void Inc( T1 *&address1, T_OFFSET byteOffset1, T2 *&address2, T_OFFSET byteOffset2, T3 *&address3, T_OFFSET byteOffset3, T4 *&address4, T_OFFSET byteOffset4 )
    {
        address1 = Offset( address1, byteOffset1 );
        address2 = Offset( address2, byteOffset2 );
        address3 = Offset( address3, byteOffset3 );
        address4 = Offset( address4, byteOffset4 );
    }

    template< class T1, class T2, class T3, class T4,class T5 >
    SYS_WEAKINLINE static void Inc( T1 *&address1, T_OFFSET byteOffset1, T2 *&address2, T_OFFSET byteOffset2, T3 *&address3, T_OFFSET byteOffset3, T4 *&address4, T_OFFSET byteOffset4, T5 *&address5, T_OFFSET byteOffset5 )
    {
        address1 = Offset( address1, byteOffset1 );
        address2 = Offset( address2, byteOffset2 );
        address3 = Offset( address3, byteOffset3 );
        address4 = Offset( address4, byteOffset4 );
        address5 = Offset( address5, byteOffset5 );
    }

    template< class T1, class T2, class T3, class T4,class T5,class T6 >
    SYS_WEAKINLINE static void Inc( T1 *&address1, T_OFFSET byteOffset1, T2 *&address2, T_OFFSET byteOffset2, T3 *&address3, T_OFFSET byteOffset3, T4 *&address4, T_OFFSET byteOffset4,T5 *&address5, T_OFFSET byteOffset5 ,T6 *&address6, T_OFFSET byteOffset6  )
    {
        address1 = Offset( address1, byteOffset1 );
        address2 = Offset( address2, byteOffset2 );
        address3 = Offset( address3, byteOffset3 );
        address4 = Offset( address4, byteOffset4 );
        address5 = Offset( address5, byteOffset5 );
        address6 = Offset( address6, byteOffset6 );
    }

    template< class T1, class T2, class T3, class T4,class T5,class T6,class T7,class T8 >
    SYS_WEAKINLINE static void Inc( T1 *&address1, T_OFFSET byteOffset1, T2 *&address2, T_OFFSET byteOffset2, T3 *&address3, T_OFFSET byteOffset3, T4 *&address4, T_OFFSET byteOffset4,T5 *&address5, T_OFFSET byteOffset5 ,T6 *&address6, T_OFFSET byteOffset6,T7 *&address7, T_OFFSET byteOffset7 ,T8 *&address8, T_OFFSET byteOffset8   )
    {
        address1 = Offset( address1, byteOffset1 );
        address2 = Offset( address2, byteOffset2 );
        address3 = Offset( address3, byteOffset3 );
        address4 = Offset( address4, byteOffset4 );
        address5 = Offset( address5, byteOffset5 );
        address6 = Offset( address6, byteOffset6 );
        address7 = Offset( address7, byteOffset7 );
        address8 = Offset( address8, byteOffset8 );

    }

    //===================================
    //        Parameter Checking
    //===================================
    #define CVP (void *)1
    SYS_INLINE static bool PtrNotOK ( const void *a, const void *b=CVP, const void *c=CVP, const void *d=CVP, const void *e=CVP,const void *f=CVP,const void *g=CVP,const void *h=CVP)
                                                                        { return (a==0 || b==0 || c==0 || d==0 || e==0 || f==0 || g==0 || h==0 ); }
    SYS_INLINE static bool StepNotOK    ( int a, int b=1, int c=1, int d=1, int e=1, int f=1, int g=1, int h=1){ return (a<=0 || b<=0 || c<=0 || d<=0 || e<=0 || f<=0 || g<=0 || h<=0); }
    SYS_INLINE static bool StepZeroNotOK( int a, int b=1, int c=1, int d=1, int e=1, int f=1, int g=1, int h=1){ return (a==0 || b==0 || c==0 || d==0 || e==0 || f==0 || g==0 || h==0); }
    SYS_INLINE static bool RoiNotOK ( FwiSize &roi )                   { return (roi.height <= 0 || roi.width <= 0); }
    SYS_INLINE static bool SizeNotOK( int a )                           { return (a<=0); }


    //=======================================
    //                Conversion
    //=======================================

    // converts from floating-point to fixed-point scaled only 13-bits
    SYS_WEAKINLINE static A16U Float2FixSh13( const double value )
    {
        return (A16U)(8192*value);
    }
    
    SYS_WEAKINLINE static A32U Float2FixSh16_32U( const double value )
    {
        return (A32U)(65536*value);
    }
    
    SYS_WEAKINLINE static A32U Float2Fix_32U( const double value, const int mul )
    {
        return (A32U)(value*mul);
    }

    template <class TS, class TD > 
    SYS_WEAKINLINE static TD GammaCorrect( const TS &v )
    {
        return (TD)((double)v < 4.59 ? 4.5 * (double)v : (1.099 * pow ((double)v/255.0, 0.45) - 0.099)*255);
    }

    template <class TS, class TD > 
    SYS_WEAKINLINE static TD Pack565( const TS &a, const TS &b, const TS &c )
    {
        return (TD)(((U16)(c & 0xF8) << 8) | ((U16)(b & 0xFC) << 3) | ((U16)(a & 0xF8) >> 3));
    }
    template <class TS, class TD > 
    SYS_WEAKINLINE static TD Pack555( const TS &a, const TS &b, const TS &c )
    {
        return (TD)(((U16)(c & 0xF8) << 7) | ((U16)(b & 0xF8) << 2) | ((U16)(a & 0xF8) >> 3));
    }
    template <class TS, class TD > 
    SYS_WEAKINLINE static TD Pack444( const TS &a, const TS &b, const TS &c )
    {
        return (TD)(((U16)(c & 0xF0) << 4) | ((U16)(b & 0xF0)) | ((U16)(a & 0xF0) >> 4));
    }

    template< class TD >
    struct Limits
    {
    };

//   Absolute value of a scalar

    template< class T >
    struct ABS_S
    {
        SYS_INLINE static T ABS( T val )
        {
            return (T)abs(val);
        }
    };

    //=======================================
    //            Type Conversion
    //=======================================

    template<class TD>struct To
    {    
        template<class TS>
        static TD From( TS s      ){ return (TD)s;    }
        static TD From( Fw16sc s ){ return (TD)s.re; }
        static TD From( Fw32sc s ){ return (TD)s.re; }
        static TD From( Fw64sc s ){ return (TD)s.re; }
        static TD From( Fw32fc s ){ return (TD)s.re; }
        static TD From( Fw64fc s ){ return (TD)s.re; }
    };

    //=======================================
    //                Scaling
    //=======================================    
    template <class T>
    SYS_WEAKINLINE static T Scale( const T & val, int scale )
    {
        //if( scale <0 )
        //Ravi-Right shift gives wrong result
        return (T)(CBL_LIBRARY::DoubleToLong( val * pow(2.0,-scale)) );
        //else            return (T)( val >> scale );
    }    

    SYS_WEAKINLINE static Fw32f Scale( Fw32f val, int scale )
    {
        return (Fw32f)( val * pow(2.0,-scale) );
    }

    SYS_WEAKINLINE static Fw64f Scale( Fw64f val, int scale )
    {
        return (Fw64f)( val * pow(2.0,-scale) );
    }

    SYS_WEAKINLINE static Fw16sc Scale( Fw16sc val, int scale )
    {
        val.re = Scale( val.re, scale );
        val.im = Scale( val.im, scale );
        return val;
    }

    SYS_WEAKINLINE static Fw32sc Scale( Fw32sc val, int scale )
    {
        val.re = Scale( val.re, scale );
        val.im = Scale( val.im, scale );
        return val;
    }

    SYS_WEAKINLINE static Fw32fc Scale( Fw32fc val, int scale )
    {
        val.re = Scale( val.re, scale );
        val.im = Scale( val.im, scale );
        return val;
    }

    SYS_WEAKINLINE static Fw64sc Scale( Fw64sc val, int scale )
    {
        val.re = Scale( val.re, scale );
        val.im = Scale( val.im, scale );
        return val;
    }

    SYS_WEAKINLINE static Fw64fc Scale( Fw64fc val, int scale )
    {
        val.re = Scale( val.re, scale );
        val.im = Scale( val.im, scale );
        return val;
    }

    template< class TS, class TD >
    struct Normalize
    {
        static TD LUVNormalizeL ( const double s ){ STATIC_ASSERT(false); return 0; }
        static TD LUVNormalizeU ( const double s ){ STATIC_ASSERT(false); return 0; }
        static TD LUVNormalizeV ( const double s ){ STATIC_ASSERT(false); return 0; }

        static double LUVUnNormalizeL ( const TS s ){ STATIC_ASSERT(false); return 0; }
        static double LUVUnNormalizeU ( const TS s ){ STATIC_ASSERT(false); return 0; }
        static double LUVUnNormalizeV ( const TS s ){ STATIC_ASSERT(false); return 0; }

        static TD LabNormalizeL ( const double s ){ STATIC_ASSERT(false); return 0; }
        static TD LabNormalizea ( const double s ){ STATIC_ASSERT(false); return 0; }
        static TD LabNormalizeb ( const double s ){ STATIC_ASSERT(false); return 0; }

        static double LabUnNormalizeL ( const TS s ){ STATIC_ASSERT(false); return 0; }
        static double LabUnNormalizea ( const TS s ){ STATIC_ASSERT(false); return 0; }
        static double LabUnNormalizeb ( const TS s ){ STATIC_ASSERT(false); return 0; }
        static double Scale ( const double s)       { STATIC_ASSERT(false); return 0; }
        static TD Scale     ( const double s, const double val ){ STATIC_ASSERT(false); return 0; }
        static TD Unscale   ( const double s )      { STATIC_ASSERT(false); return 0; }
    };

};

    //=======================================
    //                Limits
    //=======================================

    #pragma warning( disable: 4018 )                // signed/unsigned mismatch

    template<>
    struct FW_REF::Limits< S8 > : public CBL_LIBRARY::Limits< S8 >
    {
        template < class TS > 
        static S8 Sat( const TS s ){ return (S8) FW_REF::CLIP( MinValue(), s, MaxValue() ); }
    };


    template<>
    struct FW_REF::Limits< U8 > : public CBL_LIBRARY::Limits< U8 >
    {
        template < class TS > static U8 Sat ( const TS s ){ return (U8) FW_REF::CLIP( Limits<U8>::MinValue(), s, Limits<U8>::MaxValue() ); }
        static double Clip ( const double min, const double max, const double val ) { return FW_REF::CLIP (min, val, max); }
    };

    template<>
    struct FW_REF::Limits< S16 > : public CBL_LIBRARY::Limits< S16 >
    {
        template < class TS > static S16 Sat ( const TS s ){ return (S16) FW_REF::CLIP( Limits<S16>::MinValue(), s, Limits<S16>::MaxValue() ); }
        static double Clip ( const double min, const double max, const double val ) { return FW_REF::CLIP (min, val, max); }
    };

    template<>
    struct FW_REF::Limits< U16 > : public CBL_LIBRARY::Limits< U16 >
    {
        template < class TS > static U16 Sat ( const TS s ){ return (U16) FW_REF::CLIP( Limits<U16>::MinValue(), s, Limits<U16>::MaxValue() ); }
        static double Clip ( const double min, const double max, const double val ) { return FW_REF::CLIP (min, val, max); }
    };

    template<>
    struct FW_REF::Limits< S32 > : public CBL_LIBRARY::Limits< S32 >
    {
        template < class TS > static S32 Sat ( const TS s ){ return (S32) FW_REF::CLIP( Limits<S32>::MinValue(), s, Limits<S32>::MaxValue() ); }
    };

    template<>
    struct FW_REF::Limits< long > : public CBL_LIBRARY::Limits< S32 >
    {
        template < class TS > static long Sat ( const TS s ){ return (long)Limits<S32>::Sat (s); }
    };

    template<>
    struct FW_REF::Limits< U32 > : public CBL_LIBRARY::Limits< U32 >
    {
        template < class TS > 
        static U32 Sat ( TS s  ){ return (U32) FW_REF::CLIP( Limits<U32>::MinValue(), s, Limits<U32>::MaxValue() ); }
        static U32 Sat ( S32 s ){ return (s<0) ? 0 : s; }
    };

    template<>
    struct FW_REF::Limits< unsigned long > : public CBL_LIBRARY::Limits< U32 >
    {
        template < class TS > 
        static U32 Sat ( TS s  ){ return (U32) FW_REF::CLIP( Limits<U32>::MinValue(), s, Limits<U32>::MaxValue() ); }
        static U32 Sat ( S32 s ){ return FW_REF::Limits<U32>::Sat (s); }
    };

    template<>
    struct FW_REF::Limits< S64 > : public CBL_LIBRARY::Limits< S64 >
    {
        template < class TS > static S64 Sat ( const TS s ){ return (S64) FW_REF::CLIP( Limits<S64>::MinValue(), s, Limits<S64>::MaxValue() ); }
    };

    template<>
    struct FW_REF::Limits< U64 > : public CBL_LIBRARY::Limits< U64 >
    {
        template < class TS > static U64 Sat ( const TS s ){ return (U64) FW_REF::CLIP( Limits<U64>::MinValue(), s, Limits<U64>::MaxValue() ); }
    };


    template<>
    struct FW_REF::Limits< F32 > : public CBL_LIBRARY::Limits< F32 >
    {
        template < class TS > static F32 Sat ( const TS s ){ return (F32) FW_REF::CLIP( Const::INF_NEG_32F, s, Const::INF_32F ); }
        static double Clip ( const double min, const double max, const double val ) { return FW_REF::CLIP (min, val, max); }
    };

    template<>
    struct FW_REF::Limits< F64 > : public CBL_LIBRARY::Limits< F64 >
    {
        template < class TS > static F64 Sat ( const TS s ){ return (F64) FW_REF::CLIP( Limits<F64>::MinValue(), s, Limits<F64>::MaxValue() ); }
        static double Clip ( const double min, const double max, const double val ) { return FW_REF::CLIP (min, val, max); }
    };


    template<>
    struct FW_REF::Limits< Fw16sc >
    {
        static Fw16sc MinValue(){ Fw16sc a; a.re = Limits<S16>::MinValue(); a.im=a.re; return a; }
        static Fw16sc MaxValue(){ Fw16sc a; a.re = Limits<S16>::MaxValue(); a.im=a.re; return a; }
                    
        template < class TS > 
        static Fw16sc Sat( const TS s )
        { 
            //Ravi-saturation from 32sc to 16sc, assigning to Fw16sc defeats the purpose
            //Fw16sc rVal = s;
            Fw16sc rVal;
            rVal.re = Limits<S16>::Sat( s.re );
            rVal.im = Limits<S16>::Sat( s.im );
            return rVal;
        }
    };

    template<>
    struct FW_REF::Limits< Fw32sc >
    {
        static Fw32sc MinValue(){ Fw32sc a; a.re = Limits<S32>::MinValue(); a.im=a.re; return a; }
        static Fw32sc MaxValue(){ Fw32sc a; a.re = Limits<S32>::MaxValue(); a.im=a.re; return a; }

        template < class TS > 
        static Fw32sc Sat( const TS s )
        { 
            //Ravi-saturation from 64sc to 32sc, assigning to Fw16sc defeats the purpose
            //Fw32sc rVal = s;
            Fw32sc rVal;
            rVal.re = Limits<S32>::Sat( s.re );
            rVal.im = Limits<S32>::Sat( s.im );
            return rVal;
        }
    };

    template<>
    struct FW_REF::Limits< Fw64sc >
    {
        static Fw64sc MinValue(){ Fw64sc a; a.re = Limits<S64>::MinValue(); a.im=a.re; return a; }
        static Fw64sc MaxValue(){ Fw64sc a; a.re = Limits<S64>::MaxValue(); a.im=a.re; return a; }

        template < class TS > 
        static Fw64sc Sat( const TS s )
        { 
            Fw64sc rVal;
            rVal.re = Limits<S64>::Sat( s.re );
            rVal.im = Limits<S64>::Sat( s.im );
            return rVal;
        }
    };

    template<>
    struct FW_REF::Limits< Fw32fc >
    {
        static Fw32fc MinValue(){ Fw32fc a; a.re = Limits<F32>::MinValue(); a.im=a.re; return a; }
        static Fw32fc MaxValue(){ Fw32fc a; a.re = Limits<F32>::MaxValue(); a.im=a.re; return a; }

        template < class TS > 
        static Fw32fc Sat( const TS s )
        {

            Fw32fc rVal;
            rVal.re = Limits<F32>::Sat( s.re );
            rVal.im = Limits<F32>::Sat( s.im );
            return rVal;
        }
    };

    template<>
    struct FW_REF::Limits< Fw64fc >
    {
        static Fw64fc MinValue(){ Fw64fc a; a.re = Limits<F64>::MinValue(); a.im=a.re; return a; }
        static Fw64fc MaxValue(){ Fw64fc a; a.re = Limits<F64>::MaxValue(); a.im=a.re; return a; }

        template < class TS > static Fw64fc Sat( const TS s ){ return (Fw64fc) FW_REF::CLIP( Limits<TS>::MinValue(), s, Limits<TS>::MaxValue() ); }
    };

    #pragma warning( default: 4018 )                // signed/unsigned mismatch

//   Absolute value of a scalar single precision floating point value

    template<>
    struct FW_REF::ABS_S<float>
    {
        SYS_INLINE static float ABS(float val)
        {
            return fabs(val);
        }
    };


//   Absolute value of a scalar double precision floating point value

    template<>
    struct FW_REF::ABS_S<double>
    {
        SYS_INLINE static double ABS(double val)
        {
            return fabs(val);
        }
    };

    template< > struct FW_REF::To< Fw8u > 
    {    
        template<class TS>
        static Fw8u From( TS s     ){ return (Fw8u)s; }
        static Fw8u From( Fw32f f ){ return (Fw8u)Limits<U8>::Sat((S32)CBL_LIBRARY::FloatToInt(f));  }
        static Fw8u From( Fw64f f ){ return (Fw8u)Limits<U8>::Sat((S64)CBL_LIBRARY::DoubleToInt(f)); }
    };
    template< > struct FW_REF::To< Fw8s > 
    {    
        template<class TS>
        static Fw8s From( TS s     ){ return (Fw8s)s; }
        static Fw8s From( Fw32f f ){ return (Fw8s)Limits<S8>::Sat((S32)CBL_LIBRARY::FloatToInt(f));  }
        static Fw8s From( Fw64f f ){ return (Fw8s)Limits<S8>::Sat((S64)CBL_LIBRARY::DoubleToInt(f)); }
    };
    template< > struct FW_REF::To< Fw16u > 
    {    
        template<class TS>
        static Fw16u From( TS s     ){ return (Fw16u)s; }
        static Fw16u From( Fw32f f ){ return (Fw16u)Limits<U16>::Sat((S32)CBL_LIBRARY::FloatToInt(f));  }
        static Fw16u From( Fw64f f ){ return (Fw16u)Limits<U16>::Sat((S64)CBL_LIBRARY::DoubleToInt(f)); }
    };
    template< > struct FW_REF::To< Fw16s > 
    {    
        template<class TS>
        static Fw16s From( TS s     ){ return (Fw16s)s; }
        static Fw16s From( Fw32f f ){ return (Fw16s)Limits<S16>::Sat((S32)CBL_LIBRARY::FloatToInt(f));  }
        static Fw16s From( Fw64f f ){ return (Fw16s)Limits<S16>::Sat((S64)CBL_LIBRARY::DoubleToInt(f)); }
    };
    template< > struct FW_REF::To< Fw32s > 
    {    
        template<class TS>
        static Fw32s From( TS s     ){ return (Fw32s)s; }
        static Fw32s From( Fw32f f ){ return (Fw32s)Limits<S32>::Sat((S32)CBL_LIBRARY::FloatToInt(f));  }
        static Fw32s From( Fw64f f ){ return (Fw32s)Limits<S32>::Sat((S64)CBL_LIBRARY::DoubleToInt(f)); }
    };
    template< > struct FW_REF::To< Fw32u > 
    {    
        template<class TS>
        static Fw32u From( TS s     ){ return (Fw32u)s; }
        static Fw32u From( Fw32f f ){ return (Fw32u)Limits<U32>::Sat((S32)CBL_LIBRARY::FloatToInt(f));  }
        static Fw32u From( Fw64f f ){ return (Fw32u)Limits<U32>::Sat((S64)CBL_LIBRARY::DoubleToInt(f)); }
    };
    template< > struct FW_REF::To<Fw16sc>
    {     
        template<class TS>static Fw16sc From( TS s ){ Fw16sc t; t.re=(Fw16s)s; t.im=(Fw16s)s; return t; }
    };
    template< > struct FW_REF::To<Fw32sc>
    { 
        template<class TS>static Fw32sc From( TS s ){ Fw32sc t; t.re=(Fw32s)s; t.im=(Fw32s)s; return t; }
    };
    template< > struct FW_REF::To<Fw64sc>
    { 
        template<class TS>static Fw64sc From( TS s ){ Fw64sc t; t.re=(Fw64s)s; t.im=(Fw64s)s; return t; }
    };
    template< > struct FW_REF::To<Fw32fc>
    { 
        template<class TS>static Fw32fc From( TS s ){ Fw32fc t; t.re=(Fw32f)s; t.im=(Fw32f)s; return t; }
    };
    template< > struct FW_REF::To<Fw64fc>
    { 
        template<class TS>static Fw64fc From( TS s ){ Fw64fc t; t.re=(Fw64f)s; t.im=(Fw64f)s; return t; }
    };

    template< class TS >
    struct FW_REF::Normalize< TS, U8 >
    {
        static U8 LUVNormalizeL        ( const double L ){ return (U8)(L * 0xFF / 100.0); }
        static U8 LUVNormalizeU        ( const double U ){ return (U8)((U + 134.0) * 0xFF / 354.0); }
        static U8 LUVNormalizeV        ( const double V ){ return (U8)((V + 140.0) * 0xFF / 256.0); }
        static double LUVUnNormalizeL  ( const TS L ){ return ( (double)L * 100.0 / 0xFF); }
        static double LUVUnNormalizeU  ( const TS U ){ return (((double)U * 354.0 / 0xFF) - 134.0); }
        static double LUVUnNormalizeV  ( const TS V ){ return (((double)V * 256.0 / 0xFF) - 140.0); }
        static U8 LabNormalizeL        ( const double L ){ return (U8)(L * 255.0 / 100.0); }
        static U8 LabNormalizea        ( const double a ){ return (U8)(a + 128.0); }
        static U8 LabNormalizeb        ( const double b ){ return (U8)(b + 128.0); }
        static double LabUnNormalizeL  ( const TS L ){ return ((double)L * (100.0 / 0xFF)); }
        static double LabUnNormalizea  ( const TS a ){ return ((double)a - 128.0); }
        static double LabUnNormalizeb  ( const TS b ){ return ((double)b - 128.0); }
        static double Scale            ( const double s){ return (s / (double)Limits<U8>::MaxValue()); }
        static U8 Scale                ( const double s, const double val){ return (U8)(Limits<U8>::Sat(Limits<U8>::MaxValue () * (s / val))); }
        static U8 Unscale              ( const double s){ return (U8)(Limits<U8>::Sat(Limits<U8>::MaxValue () * s)); }
    };

    template< class TS >
    struct FW_REF::Normalize< TS, S16 >
    {
        static S16 LUVNormalizeL        ( const double L ){ return (S16)(L * 0xFFFF / 100.0 -32768); }
        static S16 LUVNormalizeU        ( const double U ){ return (S16)((U + 134.0) * 0xFFFF / 354.0 -32768); }
        static S16 LUVNormalizeV        ( const double V ){ return (S16)((V + 140.0) * 0xFFFF / 256.0 -32768 - 820); }
        static double LUVUnNormalizeL   ( const TS L ){ return ( ((double)L + 32768) * 100.0 /  0xFFFF); } // + 32768 is same as - MIN_16S
        static double LUVUnNormalizeU   ( const TS U ){ return ( (((double)U + 32768) * 354.0 / 0xFFFF) - 134.0); }
        static double LUVUnNormalizeV   ( const TS V ){ return ( (((double)V + 32768) * 256.0 / 0xFFFF) - 140.0); }
        static double Scale             ( const double s){ return (s-Limits<S16>::MinValue())/(Limits<S16>::MaxValue()-Limits<S16>::MinValue()); }
        static S16 Scale                ( const double s, const double val){ return (S16)( (Limits<S16>::MaxValue()-Limits<S16>::MinValue())*(s/val)+Limits<S16>::MinValue() ); }
        static S16 Unscale              ( const double s){ return (S16)Limits<S16>::Sat( s*(Limits<S16>::MaxValue()-Limits<S16>::MinValue()) + Limits<S16>::MinValue()); }
    };

    template< class TS >
    struct FW_REF::Normalize< TS, U16 >
    {
        static U16 LUVNormalizeL        ( const double L ){ return (U16)(L * 0xFFFF / 100.0); }
        static U16 LUVNormalizeU        ( const double U ){ return (U16)((U + 134.0) * 0xFFFF / 354.0); }
        static U16 LUVNormalizeV        ( const double V ){ return (U16)((V + 140.0) * 0xFFFF / 256.0); }
        static double LUVUnNormalizeL   ( const TS L ){ return ((double)L * 100.0 /  0xFFFF); }
        static double LUVUnNormalizeU   ( const TS U ){ return (((double)U * 354.0 / 0xFFFF) - 134.0); }
        static double LUVUnNormalizeV   ( const TS V ){ return (((double)V * 256.0 / 0xFFFF) - 140.0); }
        static U16 LabNormalizeL        ( const double L ){ return (U16)(L * 65535.0 / 100.0); }
        static U16 LabNormalizea        ( const double a ){ return (U16)((a + 128.0)* 0xFF); }
        static U16 LabNormalizeb        ( const double b ){ return (U16)((b + 128.0)* 0xFF); }
        static double LabUnNormalizeL   ( const TS L ){ return ((double)L * 100.0 / 0xFFFF); }
        static double LabUnNormalizea   ( const TS a ){ return ((double)a/0xFF - 128.0); }
        static double LabUnNormalizeb   ( const TS b ){ return ((double)b/0xFF - 128.0); }
        static double Scale             ( const double s){ return (s / (double)Limits<U16>::MaxValue()); }
        static U16 Scale                ( const double s, const double val){ return (U16)(Limits<U16>::MaxValue () * (s / val)); }
        static U16 Unscale              ( const double s){ return (U16)Limits<U16>::Sat(Limits<U16>::MaxValue () * s); }
    };

    template< class TS >
    struct FW_REF::Normalize< TS, U32 >
    {
        static double Scale ( const double s){ return s / (double)Limits<U32>::MaxValue(); }
        static U32 Scale    ( const double s, const double val){ return (U32)(Limits<U32>::MaxValue () * (s / val)); }
        static U32 Unscale  ( const double s){ return (U32)(Limits<U32>::MaxValue () * s); }
    };

    template< class TS >
    struct FW_REF::Normalize< TS, F32 >
    {
        static F32 LUVNormalizeL   ( const double s ){ return (F32)s; }
        static F32 LUVNormalizeU   ( const double s ){ return (F32)s; }
        static F32 LUVNormalizeV   ( const double s ){ return (F32)s; }
        static F32 LUVUnNormalizeL ( const TS s ){ return s; }
        static F32 LUVUnNormalizeU ( const TS s ){ return s; }
        static F32 LUVUnNormalizeV ( const TS s ){ return s; }
        static double Scale ( const double s) { return s; }
        static F32 Scale (const double s, const double val)
        { 
            return (F32)(s/val);
            /*double newVal = s/val;
            return (F32)((newVal>1)?1:((newVal<0)?0:newVal)); */
        }
        static F32 Unscale (const double s){ return (F32)s; }
    };



//=======================================
//                Generic Math
//=======================================

template< class T> SYS_INLINE Fw16sc operator* ( Fw16sc a, const T &b ){ a.re = a.re * b; a.im = a.im * b; return a; }
template< class T> SYS_INLINE Fw32fc operator* ( Fw32fc a, const T &b ){ a.re = a.re * b; a.im = a.im * b; return a; }
template< class T> SYS_INLINE Fw64fc operator* ( Fw64fc a, const T &b ){ a.re = a.re * b; a.im = a.im * b; return a; }
template< class T> SYS_INLINE Fw16sc operator/ ( Fw16sc a, const T &b ){ a.re = a.re / b; a.im = a.im / b; return a; }
template< class T> SYS_INLINE Fw32fc operator/ ( Fw32fc a, const T &b ){ a.re = a.re / b; a.im = a.im / b; return a; }
template< class T> SYS_INLINE Fw64fc operator/ ( Fw64fc a, const T &b ){ a.re = a.re / b; a.im = a.im / b; return a; }
template< class T> SYS_INLINE Fw32sc operator* ( Fw32sc a, const T &b ){ a.re = a.re * b; a.im = a.im * b; return a; }
template< class T> SYS_INLINE Fw32sc operator/ ( Fw32sc a, const T &b ){ a.re = a.re / b; a.im = a.im / b; return a; }
template< class T> SYS_INLINE Fw64sc operator* ( Fw64sc a, const T &b ){ a.re = a.re * b; a.im = a.im * b; return a; }
template< class T> SYS_INLINE Fw64sc operator/ ( Fw64sc a, const T &b ){ a.re = a.re / b; a.im = a.im / b; return a; }

SYS_INLINE bool operator== ( Fw16sc a, const int &b ){ return (a.re==b && a.im==b); }
SYS_INLINE bool operator== ( Fw32sc a, const int &b ){ return (a.re==b && a.im==b); }
SYS_INLINE bool operator== ( Fw32fc a, const int &b ){ return (a.re==b && a.im==b); }
SYS_INLINE bool operator== ( Fw64fc a, const int &b ){ return (a.re==b && a.im==b); }
SYS_INLINE bool operator== ( Fw16sc a, Fw16sc b ){ return (a.re==b.re && a.im==b.im); }
SYS_INLINE bool operator== ( Fw32sc a, Fw32sc b ){ return (a.re==b.re && a.im==b.im); }
SYS_INLINE bool operator== ( Fw32fc a, Fw32fc b ){ return (a.re==b.re && a.im==b.im); }
SYS_INLINE bool operator== ( Fw64fc a, Fw64fc b ){ return (a.re==b.re && a.im==b.im); }
SYS_INLINE bool operator== ( Fw64sc a, Fw64sc b ){ return (a.re==b.re && a.im==b.im); }

SYS_INLINE bool operator!( const Fw16sc &a ){ return !a.re && !a.im; }
SYS_INLINE bool operator!( const Fw32sc &a ){ return !a.re && !a.im; }
SYS_INLINE bool operator!( const Fw32fc &a ){ return !a.re && !a.im; }
SYS_INLINE bool operator!( const Fw64fc &a ){ return !a.re && !a.im; }
SYS_INLINE bool operator!( const Fw64sc &a ){ return !a.re && !a.im; }

SYS_INLINE bool operator <   ( const Fw16sc &a, const Fw16sc &b ){ return (a.re==b.re) ? a.im<b.im : a.re<b.re; }
SYS_INLINE bool operator <   ( const Fw32sc &a, const Fw32sc &b ){ return (a.re==b.re) ? a.im<b.im : a.re<b.re; }
SYS_INLINE bool operator <   ( const Fw32fc &a, const Fw32fc &b ){ return (a.re==b.re) ? a.im<b.im : a.re<b.re; }
SYS_INLINE bool operator <   ( const Fw64fc &a, const Fw64fc &b ){ return (a.re==b.re) ? a.im<b.im : a.re<b.re; }
                                             
SYS_INLINE bool operator >   ( const Fw16sc &a, const Fw16sc &b ){ return (a.re==b.re) ? a.im>b.im : a.re>b.re; }
SYS_INLINE bool operator >   ( const Fw32sc &a, const Fw32sc &b ){ return (a.re==b.re) ? a.im>b.im : a.re>b.re; }
SYS_INLINE bool operator >   ( const Fw32fc &a, const Fw32fc &b ){ return (a.re==b.re) ? a.im>b.im : a.re>b.re; }
SYS_INLINE bool operator >   ( const Fw64fc &a, const Fw64fc &b ){ return (a.re==b.re) ? a.im>b.im : a.re>b.re; }
SYS_INLINE bool operator >   ( const Fw64sc &a, const Fw64sc &b ){ return (a.re==b.re) ? a.im>b.im : a.re>b.re; }

SYS_INLINE Fw16sc operator* ( const Fw16sc &a, const Fw16sc &b )
{ 
    Fw16sc c;
    c.re = FW_REF::Limits<Fw16s>::Sat( (a.re*b.re) - (a.im*b.im) );
    c.im = FW_REF::Limits<Fw16s>::Sat( (a.im*b.re) + (a.re*b.im) );
    return c; 
}
SYS_INLINE Fw32sc operator* ( const Fw32sc &a, const Fw32sc &b )
{
    Fw32sc c;
    c.re = FW_REF::Limits<Fw32s>::Sat( (a.re*b.re) - (a.im*b.im) );
    c.im = FW_REF::Limits<Fw32s>::Sat( (a.im*b.re) + (a.re*b.im) );
    return c; 
}
SYS_INLINE Fw32fc operator* ( const Fw32fc &a, const Fw32fc &b )
{ 
    Fw32fc c;    
    c.re = (a.re*b.re) - (a.im*b.im);
    c.im = (a.im*b.re) + (a.re*b.im);
    return c; 
}
SYS_INLINE Fw64fc operator* ( const Fw64fc &a, const Fw64fc &b )
{ 
    Fw64fc c;    
    c.re = (a.re*b.re) - (a.im*b.im);
    c.im = (a.im*b.re) + (a.re*b.im);
    return c; 
}

SYS_INLINE Fw16sc operator/ ( const Fw16sc &a, const Fw16sc &b )
{ 
    Fw16sc c;    
    c.re = FW_REF::Limits<Fw16s>::Sat( ( a.re*b.re + a.im*b.im ) / ( b.re*b.re + b.im*b.im ) );
    c.im = FW_REF::Limits<Fw16s>::Sat( ( a.im*b.re - a.re*b.im ) / ( b.re*b.re + b.im*b.im ) );
    return c; 
}
SYS_INLINE Fw32sc operator/ ( const Fw32sc &a, const Fw32sc &b )
{ 
    Fw32sc c;    
    c.re = FW_REF::Limits<Fw32s>::Sat( ( a.re*b.re + a.im*b.im ) / ( b.re*b.re + b.im*b.im ) );
    c.im = FW_REF::Limits<Fw32s>::Sat( ( a.im*b.re - a.re*b.im ) / ( b.re*b.re + b.im*b.im ) );
    return c; 
}
SYS_INLINE Fw64sc operator/ ( const Fw64sc &a, const Fw64sc &b )
{ 
    Fw64sc c;    
    c.re = FW_REF::Limits<Fw64s>::Sat( ( a.re*b.re + a.im*b.im ) / ( b.re*b.re + b.im*b.im ) );
    c.im = FW_REF::Limits<Fw64s>::Sat( ( a.im*b.re - a.re*b.im ) / ( b.re*b.re + b.im*b.im ) );
    return c; 
}
SYS_INLINE Fw32fc operator/ ( const Fw32fc &a, const Fw32fc &b )
{ 
    Fw32fc c;    
    c.re = ( a.re*b.re + a.im*b.im ) / ( b.re*b.re + b.im*b.im );
    c.im = ( a.im*b.re - a.re*b.im ) / ( b.re*b.re + b.im*b.im );
    return c;  
}

SYS_INLINE Fw64fc operator/ ( const Fw64fc &a, const Fw64fc &b )
{ 
    Fw64fc c;    
    c.re = ( a.re*b.re + a.im*b.im ) / ( b.re*b.re + b.im*b.im );
    c.im = ( a.im*b.re - a.re*b.im ) / ( b.re*b.re + b.im*b.im );
    return c;  
}

SYS_INLINE Fw16sc operator>>( Fw16sc a, int value ){ a.re >>= value; a.im >>= value; return a; }
SYS_INLINE Fw16sc operator<<( Fw16sc a, int value ){ a.re <<= value; a.im <<= value; return a; }
SYS_INLINE Fw32sc operator>>( Fw32sc a, int value ){ a.re >>= value; a.im >>= value; return a; }
SYS_INLINE Fw32sc operator<<( Fw32sc a, int value ){ a.re <<= value; a.im <<= value; return a; }

SYS_INLINE Fw16sc  operator+ ( Fw16sc a, const Fw16sc &b ) { a.re = a.re + b.re; a.im = a.im + b.im; return a; }
SYS_INLINE Fw16sc  operator- ( Fw16sc a, const Fw16sc &b ) { a.re = a.re - b.re; a.im = a.im - b.im; return a; }
SYS_INLINE Fw32sc  operator+ ( Fw32sc a, const Fw32sc &b ) { a.re +=  b.re; a.im += b.im; return a; }
SYS_INLINE Fw32sc  operator- ( Fw32sc a, const Fw32sc &b ) { a.re -=  b.re; a.im -= b.im; return a; }
SYS_INLINE Fw32fc  operator+ ( Fw32fc a, const Fw32fc &b ) { a.re +=  b.re; a.im += b.im; return a; }
SYS_INLINE Fw32fc  operator- ( Fw32fc a, const Fw32fc &b ) { a.re -=  b.re; a.im -= b.im; return a; }
SYS_INLINE Fw64fc  operator+ ( Fw64fc a, const Fw64fc &b ) { a.re +=  b.re; a.im += b.im; return a; }
SYS_INLINE Fw64fc  operator- ( Fw64fc a, const Fw64fc &b ) { a.re -=  b.re; a.im -= b.im; return a; }
SYS_INLINE Fw64sc  operator+ ( Fw64sc a, const Fw64sc &b ) { a.re +=  b.re; a.im += b.im; return a; }
SYS_INLINE Fw64sc  operator- ( Fw64sc a, const Fw64sc &b ) { a.re -=  b.re; a.im -= b.im; return a; }


SYS_INLINE Fw32sc& operator+= ( Fw32sc &a, const Fw32sc &b ) { a.re += b.re; a.im += b.im; return a; }
SYS_INLINE Fw32fc& operator+= ( Fw32fc &a, const Fw32fc &b ) { a.re += b.re; a.im += b.im; return a; }
SYS_INLINE Fw32fc& operator-= ( Fw32fc &a, const Fw32fc &b ) { a.re -= b.re; a.im -= b.im; return a; }
SYS_INLINE Fw64fc& operator+= ( Fw64fc &a, const Fw64fc &b ) { a.re += b.re; a.im += b.im; return a; }
SYS_INLINE Fw64fc& operator-= ( Fw64fc &a, const Fw64fc &b ) { a.re -= b.re; a.im -= b.im; return a; }
SYS_INLINE Fw64sc& operator+= ( Fw64sc &a, const Fw64sc &b ) { a.re += b.re; a.im += b.im; return a; }
SYS_INLINE Fw64sc& operator-= ( Fw64sc &a, const Fw64sc &b ) { a.re -= b.re; a.im -= b.im; return a; }


SYS_INLINE Fw32fc& operator*= ( Fw32fc &a, const Fw32fc &b ) { Fw32f t = (a.re*b.re) - (a.im*b.im); a.im = (a.im*b.re) + (a.re*b.im); a.re = t; return a; }
SYS_INLINE Fw64fc& operator*= ( Fw64fc &a, const Fw64fc &b ) { Fw64f t = (a.re*b.re) - (a.im*b.im); a.im = (a.im*b.re) + (a.re*b.im); a.re = t; return a; }


} // namespace OPT_LEVEL

#endif // __FW_SHARED_CODE_H__
