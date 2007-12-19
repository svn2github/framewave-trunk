/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FOREACH_H__
#define __FOREACH_H__

#include "fwdev.h"
#include "FwSharedCode_SSE2.h"

namespace OPT_LEVEL
{

struct SYS_FORCEALIGN_16 FnDat
{
    #define MAX_ITEMS 4
    #define XMM_MAX_ITEMS 8
    void * data[MAX_ITEMS];

    XMM128 xmmData [XMM_MAX_ITEMS];
    XMM128 xmmData2[XMM_MAX_ITEMS];
//  XMM128 xmmData3[XMM_MAX_ITEMS];
    int regCount;
    int numThreads;
    
    template< class T > 
    SYS_INLINE const T & Get( U32 i ) const { ASSERT(i<MAX_ITEMS); return *( (T*) data[i] ); }
    SYS_INLINE void Set( U32 i, void * a ){ ASSERT(i<MAX_ITEMS); data[i] = a; }

    FnDat()                                        {                                             }
    FnDat( void *a )                               { data[0]=a;                                  }
    FnDat( void *a, void *b )                      { data[0]=a, data[1]=b;                       }
    FnDat( void *a, void *b, void *c )             { data[0]=a, data[1]=b, data[2]=c;            }
    FnDat( void *a, void *b, void *c, void *d )    { data[0]=a, data[1]=b, data[2]=c, data[3]=d; }
};

static FnDat g_dat; // default FnDat

/*
    TODO:
    Use an enumeration to store settings for tuning foreach. 

    These are features that we would like to be able to specify:

    - Align 96 ( Load half registers )   default is 128.
    - No threading                       default is threading on.
    - No store dest                      default is load and store. 
    - No load dest                       defualt is load and store.
    
    These features are runtime detected and set by the foreach code. 

    - some buffers NOT aligned            default is all aligned.    
*/
enum RegSetting
{
    ALIGN128,        // regular load
    ALIGN96         // fractional load
};

enum MemAlign
{
    MA_FLSE,        // not aligned
    MA_TRUE         // aligned
};

//
// RegBuffer
//
struct RegBuffer
{
    // DATA
    XMM128 dat[4];                              // Registers to contain pixel data.
    U32 regCount;                               // number of xmm registers.
    U32 count;                                  // number of BYTE units. 1 xmm register = 16 BYTE units.
    U32 elements;                               // Number of elements in buffer per SSE iteration.
                                                // Since we can span registers, the loader only cares about elements.
        
    // ACCESSORS
    SYS_INLINE U32 RegCount() const { return regCount; }
    SYS_INLINE U32    Count() const { return count;    }
    SYS_INLINE U32 Elements() const { return elements; }

    SYS_INLINE XMM128 & operator[]( U32 index )
    { 
        ASSERT( index*16 < count );                // Ensure access is valid
        return dat[index]; 
    }
};

#pragma warning( disable: 4328 )    //TODO

template< MemAlign ma >
struct Move
{
    SYS_INLINE static void Load( RegBuffer &rb, __m128i* mem )    // Unaligned load
    {        
        switch( rb.count )
        {
        case 64:    rb.dat[3].i = _mm_loadu_si128( mem + 3 );                                // Falls through
        case 48:    rb.dat[2].i = _mm_loadu_si128( mem + 2 );
        case 32:    rb.dat[1].i = _mm_loadu_si128( mem + 1 );
        case 16:    rb.dat[0].i = _mm_loadu_si128( mem + 0 );        break;

        case 24:    CBL_SSE2::Load_192(rb.dat[0].i, rb.dat[1].i, mem);    break;
        case 12:    CBL_SSE2::Load_96 (rb.dat[0].i, mem);                 break;
        case  8:    CBL_SSE2::Load_64 (rb.dat[0].i, mem);                 break;
        case  4:    CBL_SSE2::Load_32 (rb.dat[0].i, mem);                 break;
        case  3:    CBL_SSE2::Load_24 (rb.dat[0].i, mem);                 break;
        case  2:    CBL_SSE2::Load_16 (rb.dat[0].i, mem);                 break;
        case  1:    CBL_SSE2::Load_8  (rb.dat[0].i, mem);                 break;
        default:    ASSERT(false);
        }
    }

    SYS_INLINE static void Store( RegBuffer &rb, __m128i* mem )
    {
        switch( rb.count )
        {
        case 64:    _mm_storeu_si128( mem + 3, rb.dat[3].i );                                // Falls through
        case 48:    _mm_storeu_si128( mem + 2, rb.dat[2].i );
        case 32:    _mm_storeu_si128( mem + 1, rb.dat[1].i );
        case 16:    _mm_storeu_si128( mem + 0, rb.dat[0].i );             break;

        case 24:    CBL_SSE2::Store_192(mem, rb.dat[0].i, rb.dat[1].i);   break;
        case 12:    CBL_SSE2::Store_96 (mem, rb.dat[0].i);                break;
        case  8:    CBL_SSE2::Store_64 (mem, rb.dat[0].i);                break;
        case  4:    CBL_SSE2::Store_32 (mem, rb.dat[0].i);                break;
        case  3:    CBL_SSE2::Store_24 (mem, rb.dat[0].i);                break;
        case  2:    CBL_SSE2::Store_16 (mem, rb.dat[0].i);                break;
        case  1:    CBL_SSE2::Store_8  (mem, rb.dat[0].i);                break;
        default:    ASSERT(false);
        }
    }
};

template<>
struct Move<MA_TRUE>
{
    SYS_INLINE static void Load( RegBuffer &rb, __m128i* mem )    // Aligned load
    {        
        switch( rb.count )
        {
        case 64:    rb.dat[3].i = _mm_load_si128( mem + 3 );                                // Falls through
        case 48:    rb.dat[2].i = _mm_load_si128( mem + 2 );
        case 32:    rb.dat[1].i = _mm_load_si128( mem + 1 );
        case 16:    rb.dat[0].i = _mm_load_si128( mem + 0 );              break;

        case 24:    CBL_SSE2::Load_192(rb.dat[0].i, rb.dat[1].i, mem);    break;
        case 12:    CBL_SSE2::Load_96 (rb.dat[0].i, mem);                 break;
        case  8:    CBL_SSE2::Load_64 (rb.dat[0].i, mem);                 break;
        case  4:    CBL_SSE2::Load_32 (rb.dat[0].i, mem);                 break;
        case  3:    CBL_SSE2::Load_24 (rb.dat[0].i, mem);                 break;
        case  2:    CBL_SSE2::Load_16 (rb.dat[0].i, mem);                 break;
        case  1:    CBL_SSE2::Load_8  (rb.dat[0].i, mem);                 break;
        default:    ASSERT(false);
        }
    }

    SYS_INLINE static void Store( RegBuffer &rb, __m128i* mem )
    {
        switch( rb.count )
        {
        case 64:    _mm_store_si128( mem + 3, rb.dat[3].i );                                // Falls through
        case 48:    _mm_store_si128( mem + 2, rb.dat[2].i );
        case 32:    _mm_store_si128( mem + 1, rb.dat[1].i );
        case 16:    _mm_store_si128( mem + 0, rb.dat[0].i );              break;

        case 24:    CBL_SSE2::Store_192(mem, rb.dat[0].i, rb.dat[1].i);   break;
        case 12:    CBL_SSE2::Store_96 (mem, rb.dat[0].i);                break;
        case  8:    CBL_SSE2::Store_64 (mem, rb.dat[0].i);                break;
        case  4:    CBL_SSE2::Store_32 (mem, rb.dat[0].i);                break;
        case  3:    CBL_SSE2::Store_24 (mem, rb.dat[0].i);                break;
        case  2:    CBL_SSE2::Store_16 (mem, rb.dat[0].i);                break;
        case  1:    CBL_SSE2::Store_8  (mem, rb.dat[0].i);                break;
        default:    ASSERT(false);
        }
    }
};

#pragma warning( default: 4328 )

template< class T, CH c >
SYS_INLINE static void Init( RegBuffer & rb, U32 pixels )
{
    rb.elements = pixels * ChannelCount(c);
    rb.count = rb.elements * sizeof(T);
    rb.regCount = (rb.count+15) >> 4;
}

template< RegSetting rs, class T, CH c >
SYS_INLINE static U32 CalcPixels()
{
    switch(ChannelCount(c))
    {
    case 1:                      return ( (U32)16 / (U32)sizeof(T) );                                                      // C1
    case 3:        switch( rs ) 
                {
                case ALIGN128:   return ( (U32)16 / (U32)sizeof(T) );                                                      // C3 regular load
                case ALIGN96:    return ( (U32)12 * (MAX((U32)4, (U32)sizeof(T)) / (U32)4) / ((U32)3 * (U32)sizeof(T)) );  // C3 fractional load
                default:         assert (false);
                                 return 0;
                }        
    case 4:                      return ( (U32)16 * (MAX((U32)4, (U32)sizeof(T)) / (U32)4) / ((U32)4 * (U32)sizeof(T)) );  // C4, AC4
    default:                     assert (false);
                                 return 0;
    }
}

struct RegDescriptor
{
    RegBuffer dst;            // Possibly 4 regs in each buffer.
    RegBuffer src1;
    RegBuffer src2;
    RegBuffer src3;
    
    U32 px_ref_count;         // Number of pixels to process in reference code.
    U32 pixels;               // Number of pixels to process per SSE iteration.
    int threadNum;
    U32 startIndex;
};

template< RegSetting rs, int regCount, class TD, CH cd >
SYS_INLINE static void Init( RegDescriptor & reg, int pixCount)
{
    switch(regCount)
    {    
    case 2 :
    case 3 :
    case 4 :
    case 5 :
    case 6 :    reg.pixels = (regCount * 16)/(sizeof(TD)*ChannelCount(cd));
                reg.px_ref_count = pixCount % reg.pixels;
                break;
    default:    reg.pixels = MIN ( (CalcPixels< rs, TD, cd >()), (U32)16 );
                reg.px_ref_count = pixCount % reg.pixels;
    }
    Init<TD,cd>( reg.dst, reg.pixels );
}    

template< RegSetting rs, int regCount, class TS, CH cs, class TD, CH cd >
SYS_INLINE static void Init( RegDescriptor & reg, int pixCount )
{
    //reg.pixels = MIN ( (CalcPixels< rs, TD, cd >()), (CalcPixels< rs, TS, cs >()) );
    //reg.px_ref_count = pixCount % reg.pixels;
    switch(regCount)
    {    
    case 3 :
    case 4 :
    case 5 :
    case 6 :    reg.pixels = (regCount * 16)/(sizeof(TD)*ChannelCount(cd));
                reg.px_ref_count = pixCount % reg.pixels;
                break;
    default:    reg.pixels = MIN ( (CalcPixels< rs, TD, cd >()), (U32)16 );
                reg.px_ref_count = pixCount % reg.pixels;
    }
    Init<TD,cd>( reg.dst, reg.pixels );
    Init<TS,cs>( reg.src1, reg.pixels );
}    

template<  RegSetting rs, int regCount, class TS1, CH cs1, class TS2, CH cs2, class TD, CH cd >
SYS_INLINE static void Init( RegDescriptor & reg, int pixCount )
{
    //reg.pixels = MIN ( (CalcPixels< rs, TD, cd >()), MIN( (CalcPixels< rs, TS1, cs1 >()),
                                                //          (CalcPixels< rs, TS2, cs2 >()) ) );
    //reg.px_ref_count = pixCount % reg.pixels;
    switch(regCount)
    {    
    case 3 :
    case 4 :
    case 5 :
    case 6 :    reg.pixels = (regCount * 16)/(sizeof(TD)*ChannelCount(cd));
                reg.px_ref_count = pixCount % reg.pixels;
                break;
    default:    reg.pixels = MIN ( (CalcPixels< rs, TD, cd >()), (U32)16 );
                reg.px_ref_count = pixCount % reg.pixels;
    }
    Init<TD, cd> ( reg.dst , reg.pixels );
    Init<TS1,cs1>( reg.src1, reg.pixels );
    Init<TS2,cs2>( reg.src2, reg.pixels );
}    

template<  RegSetting rs, int regCount, class TS1,CH cs1, class TS2,CH cs2, class TS3,CH cs3, class TD, CH cd >
SYS_INLINE static void Init( RegDescriptor & reg, int pixCount )
{
    //reg.pixels = MIN ( (CalcPixels< rs, TD, cd >()), MIN( (CalcPixels< rs, TS1, cs1 >()),
                                                //     MIN( (CalcPixels< rs, TS2, cs2 >()),
                                                //          (CalcPixels< rs, TS3, cs3 >()) ) ) );
    //reg.px_ref_count = pixCount % reg.pixels;
    switch(regCount)
    {    
    case 3 :
    case 4 :
    case 5 :
    case 6 :    reg.pixels = (regCount * 16)/(sizeof(TD)*ChannelCount(cd));
                reg.px_ref_count = pixCount % reg.pixels;
                break;
    default:    reg.pixels = MIN ( (CalcPixels< rs, TD, cd >()), (U32)16 );
                reg.px_ref_count = pixCount % reg.pixels;
    }
    Init<TD, cd> ( reg.dst , reg.pixels );
    Init<TS1,cs1>( reg.src1, reg.pixels );
    Init<TS2,cs2>( reg.src2, reg.pixels );
    Init<TS3,cs3>( reg.src3, reg.pixels );
}

//
// Macro definitions
//

#define PIX_FN1 void (*pix_ref )( FnDat &,       TD*, CH , int threadNum )

#define PIX_FN2 void (*pix_ref )( FnDat &, const TS*, CH ,                \
                                                 TD*, CH , int threadNum )

#define PIX_FN3 void (*pix_ref )( FnDat &, const TS1*, CH  ,              \
                                                 const TS2*, CH ,         \
                                                       TD* , CH , int threadNum )

#define PIX_FN4 void (*pix_ref )( FnDat &, const TS1*, CH  ,              \
                                                 const TS2*, CH ,         \
                                                 const TS3*, CH ,         \
                                                        TD*, CH , int threadNum )

#define PIX_SSE void (*pix_sse )( FnDat &dat, RegDescriptor &reg )

#define PIX_SSE_SETUP void (*pix_sse_setup)( FnDat &dat )

#define PIX_SSE_CLEANUP void (*pix_sse_cleanup)( FnDat &dat )


// HEADs 2D SSE2 threadee

#define HEAD1_2D_SSE2_T    template< RegSetting rs, int regCount,         \
                                     class TD, CH cd,                     \
                                     PIX_FN1, PIX_SSE, PIX_SSE_SETUP >    

#define HEAD2_2D_SSE2_T    template< RegSetting rs, int regCount,         \
                                     class TS, CH cs,                     \
                                     class TD, CH cd,                     \
                                     PIX_FN2, PIX_SSE, PIX_SSE_SETUP >    

#define HEAD3_2D_SSE2_T    template< RegSetting rs, int regCount,         \
                                     class TS1, CH cs1,                   \
                                     class TS2, CH cs2,                   \
                                     class TD, CH cd,                     \
                                     PIX_FN3, PIX_SSE, PIX_SSE_SETUP >    

#define HEAD4_2D_SSE2_T    template< RegSetting rs, int regCount,         \
                                     class TS1, CH cs1,                   \
                                     class TS2, CH cs2,                   \
                                     class TS3, CH cs3,                   \
                                     class TD, CH cd,                     \
                                     PIX_FN4, PIX_SSE, PIX_SSE_SETUP >    

// HEADs 2D SSE2 threader

#define HEAD1_2D_SSE2    template< RegSetting rs, int regCount,                     \
                                   class TD, CH cd,                                 \
                                   PIX_FN1, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >

#define HEAD2_2D_SSE2    template< RegSetting rs, int regCount,                     \
                                   class TS, CH cs,                                 \
                                   class TD, CH cd,                                 \
                                   PIX_FN2, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >

#define HEAD3_2D_SSE2    template< RegSetting rs, int regCount,                     \
                                   class TS1, CH cs1,                               \
                                   class TS2, CH cs2,                               \
                                   class TD , CH cd ,                               \
                                   PIX_FN3, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >

#define HEAD4_2D_SSE2    template< RegSetting rs, int regCount,                     \
                                   class TS1, CH cs1,                               \
                                   class TS2, CH cs2,                               \
                                   class TS3, CH cs3,                               \
                                   class TD , CH cd ,                               \
                                   PIX_FN4, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >

// HEADs 1D SSE2 threadee

#define HEAD1_1D_SSE2_T    template< int regCount,                                  \
                                  class TD, CH cd,                                  \
                                  PIX_FN1, PIX_SSE >

#define HEAD2_1D_SSE2_T    template< int regCount,                                  \
                                  class TS, CH cs,                                  \
                                  class TD, CH cd,                                  \
                                  PIX_FN2, PIX_SSE >

#define HEAD3_1D_SSE2_T    template< int regCount,                                  \
                                  class TS1, CH cs1,                                \
                                  class TS2, CH cs2,                                \
                                  class TD , CH cd ,                                \
                                  PIX_FN3, PIX_SSE >

#define HEAD4_1D_SSE2_T    template< int regCount,                                  \
                                  class TS1, CH cs1,                                \
                                  class TS2, CH cs2,                                \
                                  class TS3, CH cs3,                                \
                                  class TD , CH cd ,                                \
                                  PIX_FN4, PIX_SSE >


// HEADs 1D SSE2 threader
#define HEAD1_1D_SSE2_Tr    template< int regCount, class TD, CH cd,                \
                                  PIX_FN1, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >

#define HEAD2_1D_SSE2_Tr    template< int regCount, class TS, CH cs,                \
                                  class TD, CH cd,                                  \
                                  PIX_FN2, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >

#define HEAD3_1D_SSE2_Tr    template< int regCount, class TS1, CH cs1,              \
                                  class TS2, CH cs2,                                \
                                  class TD , CH cd,                                 \
                                  PIX_FN3, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >

#define HEAD4_1D_SSE2_Tr    template< int regCount, class TS1, CH cs1,              \
                                  class TS2, CH cs2,                                \
                                  class TS3, CH cs3,                                \
                                  class TD , CH cd,                                 \
                                  PIX_FN4, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >


// Internal 1D DEV

#define HEAD1_REF        template<class TD, CH cd, PIX_FN1 >

#define HEAD2_REF        template< class TS, CH cs,                                 \
                                   class TD, CH cd, PIX_FN2 >

#define HEAD3_REF        template< class TS1, CH cs1,                               \
                                   class TS2, CH cs2,                               \
                                   class TD , CH cd , PIX_FN3 >

#define HEAD4_REF        template< class TS1, CH cs1,                               \
                                   class TS2, CH cs2,                               \
                                   class TS3, CH cs3,                               \
                                   class TD , CH cd , PIX_FN4 >

// Internal 1D SSE2

#define HEAD1_1D_SSE2    template< MemAlign ma, class TD, CH cd,                    \
                                  PIX_FN1, PIX_SSE >

#define HEAD2_1D_SSE2    template< MemAlign ma, class TS, CH cs,                    \
                                   class TD, CH cd,                                 \
                                   PIX_FN2, PIX_SSE >

#define HEAD3_1D_SSE2    template< MemAlign ma, class TS1, CH cs1,                  \
                                   class TS2, CH cs2,                               \
                                   class TD, CH cd,                                 \
                                   PIX_FN3, PIX_SSE >

#define HEAD4_1D_SSE2    template< MemAlign ma, class TS1, CH cs1,                  \
                                   class TS2, CH cs2,                               \
                                   class TS3, CH cs3,                               \
                                   class TD, CH cd,                                 \
                                   PIX_FN4, PIX_SSE >

// External 1D

#define HEAD1_1D_EXT     template< class TD, CH cd, int regCount,                   \
                                   PIX_FN1, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >

#define HEAD2_1D_EXT    template< class TS, CH cs, int regCount,                    \
                                  class TD, CH cd,                                  \
                                  PIX_FN2, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >

#define HEAD3_1D_EXT     template< class TS1, CH cs1, int regCount,                 \
                                   class TS2, CH cs2,                               \
                                   class TD , CH cd ,                               \
                                   PIX_FN3, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >

#define HEAD4_1D_EXT     template< class TS1, CH cs1, int regCount,                 \
                                   class TS2, CH cs2,                               \
                                   class TS3, CH cs3,                               \
                                   class TD , CH cd ,                               \
                                   PIX_FN4, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >

// External 2D

#define HEAD1_2D_EXT     template< RegSetting rs, int regCount,                     \
                                  class TD, CH cd,                                  \
                                  PIX_FN1, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >\

#define HEAD2_2D_EXT     template< RegSetting rs, int regCount,                     \
                                  class TS, CH cs,                                  \
                                  class TD, CH cd,                                  \
                                  PIX_FN2, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >\

#define HEAD3_2D_EXT     template< RegSetting rs, int regCount,                     \
                                  class TS1, CH cs1,                                \
                                  class TS2, CH cs2,                                \
                                  class TD , CH cd ,                                \
                                  PIX_FN3, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >\

#define HEAD4_2D_EXT     template< RegSetting rs, int regCount,                     \
                                  class TS1, CH cs1,                                \
                                  class TS2, CH cs2,                                \
                                  class TS3, CH cs3,                                \
                                  class TD , CH cd ,                                \
                                  PIX_FN4, PIX_SSE, PIX_SSE_SETUP, PIX_SSE_CLEANUP >\

// Default function pointers

namespace
{
STATIC void NO_SSE2( FnDat &, RegDescriptor & ) {}
STATIC void NO_SSE2_SETUP( FnDat & ) {}
STATIC void NO_SSE2_CLEANUP( FnDat & ) {}
};

typedef void (*PxCleanup)(FnDat &);

// FnArgs for thread functions

template < class TD >
struct FnArgs1
{
    FnDat *dat; FwiSize roi;
    TD *d; int dStep;
    int threadNum;
    U32 startIndex;
};

template < class TS, class TD >
struct FnArgs2
{
    FnDat *dat; FwiSize roi;
    const TS *s; int sStep;
    TD *d;       int dStep;
    int threadNum;
    U32 startIndex;
    
};

template < class TS1, class TS2, class TD >
struct FnArgs3
{
    FnDat *dat; FwiSize roi;
    const TS1 *s1; int s1Step;
    const TS2 *s2; int s2Step;
    TD *d;         int dStep;
    int threadNum;
    U32 startIndex;
    
};

template < class TS1, class TS2, class TS3, class TD >
struct FnArgs4
{
    FnDat *dat; FwiSize roi;
    const TS1 *s1; int s1Step;
    const TS2 *s2; int s2Step;
    const TS3 *s3; int s3Step;
    TD *d;         int dStep;
    int threadNum;
    U32 startIndex;

};

// FnArgs for 1D thread functions

template < class TD >
struct FnArgs1_1D
{
    FnDat *dat;    int len;
    TD *d;                
    int threadNum;
    U32 startIndex;
};

template < class TS, class TD >
struct FnArgs2_1D
{
    FnDat *dat;
    const TS *s;
    TD *d;
    int len;
    int threadNum;
    U32 startIndex;
};

template < class TS1, class TS2, class TD >
struct FnArgs3_1D
{
    FnDat *dat;
    const TS1 *s1;
    const TS2 *s2;
    TD *d;
    int len;
    int threadNum;
    U32 startIndex;
};

template < class TS1, class TS2, class TS3, class TD >
struct FnArgs4_1D
{
    FnDat *dat;
    const TS1 *s1;
    const TS2 *s2;
    const TS3 *s3;
    TD *d;
    int len;
    int threadNum;
    U32 startIndex;
};


//
// 2D MultiThread Calculation
//

template < class TD > SYS_INLINE static
void SetThreadData(FnArgs1<TD> * args, const U32 &numThreads, FnDat &dat, TD *d, int dStep, FwiSize &roi)
{
    U32 height = roi.height / numThreads;    // height per thread
    for(U32 i=0; i<numThreads; ++i)                
    {
        // intialize fnArgs
        args[i].dat        = &dat;
        args[i].roi.width  = roi.width;
        args[i].startIndex = i * (roi.width * height);
        args[i].d          = FW_REF::Offset(d, i*dStep*height);
        args[i].dStep      = dStep;
        args[i].threadNum  = i;
        if( i < numThreads-1 ) args[i].roi.height = height;                                // all but last thread
        else                   args[i].roi.height = roi.height - (numThreads-1)*height;    // last thread
    }
}

template < class TS, class TD > SYS_INLINE static
void SetThreadData(FnArgs2<TS,TD> * args, const U32 &numThreads, FnDat &dat, const TS *s, int sStep,
                                                                                   TD *d, int dStep, FwiSize &roi )
{
    U32 height = roi.height / numThreads;    // height per thread
    for(U32 i=0; i<numThreads; ++i)
    {
        // intialize fnArgs
        args[i].dat        = &dat;
        args[i].roi.width  = roi.width;
        args[i].startIndex  = i * (roi.width * height);
        args[i].d          = FW_REF::Offset(d, i*dStep*height);  args[i].dStep = dStep;
        args[i].s          = FW_REF::Offset(s, i*sStep*height);  args[i].sStep = sStep;
        args[i].threadNum  = i;
        if( i < numThreads-1 )  args[i].roi.height = height;                                // all but last thread
        else                    args[i].roi.height = roi.height - (numThreads-1)*height;    // last thread
    }
}

template < class TS1, class TS2, class TD > SYS_INLINE static
void SetThreadData(FnArgs3<TS1,TS2,TD> * args, const U32 &numThreads, FnDat &dat, const TS1 *s1, int s1Step,
                                                                                  const TS2 *s2, int s2Step,
                                                                                        TD  *d , int dStep , FwiSize &roi )
{
    U32 height = roi.height / numThreads;    // height per thread
    for(U32 i=0; i<numThreads; ++i)                
    {
        // intialize fnArgs
        args[i].dat         = &dat;
        args[i].roi.width   = roi.width;
        args[i].startIndex  = i * (roi.width * height);
        args[i].d           = FW_REF::Offset(d,  i* dStep*height);  args[i].dStep  = dStep ;
        args[i].s1          = FW_REF::Offset(s1, i*s1Step*height);  args[i].s1Step = s1Step;
        args[i].s2          = FW_REF::Offset(s2, i*s2Step*height);  args[i].s2Step = s2Step;
        args[i].threadNum   = i;
        if( i < numThreads-1 )  args[i].roi.height = height;                                // all but last thread
        else                    args[i].roi.height = roi.height - (numThreads-1)*height;    // last thread
    }
}

template < class TS1, class TS2, class TS3, class TD > SYS_INLINE static
void SetThreadData(FnArgs4<TS1,TS2,TS3,TD> * args, const U32 &numThreads, FnDat &dat, const TS1 *s1, int s1Step,
                                                                                      const TS2 *s2, int s2Step,
                                                                                      const TS3 *s3, int s3Step,
                                                                                            TD  *d , int dStep , FwiSize &roi )
{
    U32 height = roi.height / numThreads;    // height per thread
    for(U32 i=0; i<numThreads; ++i)                
    {
        // intialize fnArgs
        args[i].dat        = &dat;
        args[i].roi.width  = roi.width;
        args[i].startIndex = i * (roi.width * height);
        args[i].d          = FW_REF::Offset(d,  i* dStep*height);  args[i].dStep  = dStep ;
        args[i].s1         = FW_REF::Offset(s1, i*s1Step*height);  args[i].s1Step = s1Step;
        args[i].s2         = FW_REF::Offset(s2, i*s2Step*height);  args[i].s2Step = s2Step;
        args[i].s3         = FW_REF::Offset(s3, i*s3Step*height);  args[i].s3Step = s3Step;
        args[i].threadNum  = i;
        if( i < numThreads-1 )  args[i].roi.height = height;                                // all but last thread
        else                    args[i].roi.height = roi.height - (numThreads-1)*height;    // last thread
    }
}


//
// 1D MultiThread Calculation
//

template < class TD > SYS_INLINE static
void SetThreadData_1D(FnArgs1_1D<TD> * args, const U32 &numThreads, FnDat &dat, TD * d, int len)
{
    U32 width = len / numThreads;    // width per thread
    for(U32 i=0; i<numThreads; ++i)                
    {
        // intialize fnArgs
        args[i].dat             = &dat;
        if( i < numThreads-1 )  args[i].len = width;                                 // all but last thread
        else                    args[i].len = len - (numThreads-1)*width;            // last thread
        args[i].startIndex      = i * width;
        args[i].d               = d + i*width;
        args[i].threadNum       = i;
    }
}

template < class TS, class TD > SYS_INLINE static
void SetThreadData_1D(FnArgs2_1D<TS,TD> * args, const U32 &numThreads, FnDat &dat, const TS * s, TD * d, int len )
{
    U32 width = len / numThreads;    // width per thread
    for(U32 i=0; i<numThreads; ++i)                
    {
        // intialize fnArgs
        args[i].dat             = &dat;
        if( i < numThreads-1 )  args[i].len = width;                                 // all but last thread
        else                    args[i].len = len - (numThreads-1)*width;            // last thread
        args[i].startIndex      = i * width;
        args[i].d               = d + i*width;
        args[i].s               = s + i*width;
        args[i].threadNum       = i;
    }
}

template < class TS1, class TS2, class TD > SYS_INLINE static
void SetThreadData_1D(FnArgs3_1D<TS1,TS2,TD> * args, const U32 &numThreads, FnDat &dat, const TS1 *s1, 
                                                                                        const TS2 *s2, 
                                                                                              TD  *d , int len )
{
    U32 width = len / numThreads;    // width per thread
    for(U32 i=0; i<numThreads; ++i)
    {
        // intialize fnArgs
        args[i].dat             = &dat;
        if( i < numThreads-1 )  args[i].len = width;                                 // all but last thread
        else                    args[i].len = len - (numThreads-1)*width;            // last thread
        args[i].startIndex      = i * width;
        args[i].d               = d  + i*width;
        args[i].s1              = s1 + i*width;
        args[i].s2              = s2 + i*width;
        args[i].threadNum       = i;
    }
}

template < class TS1, class TS2,class TS3, class TD > SYS_INLINE static
void SetThreadData_1D(FnArgs4_1D<TS1,TS2,TS3,TD> * args, const U32 &numThreads, FnDat &dat, const TS1 *s1, 
                                                                                            const TS2 *s2, 
                                                                                            const TS3 *s3, 
                                                                                                  TD  *d , int len )
{
    U32 width = len / numThreads;    // width per thread
    for(U32 i=0; i<numThreads; ++i)                
    {
        // intialize fnArgs
        args[i].dat             = &dat;
        if( i < numThreads-1 )  args[i].len = width;                                 // all but last thread
        else                    args[i].len = len - (numThreads-1)*width;            // last thread
        args[i].startIndex      = i * width;
        args[i].d               = d  + i*width;
        args[i].s1              = s1 + i*width;
        args[i].s2              = s2 + i*width;
        args[i].s3              = s3 + i*width;
        args[i].threadNum       = i;
    }
}



//
// 1D Reference
//

HEAD1_REF SYS_INLINE static void
foreach_1D_REF( FnDat &dat, TD *d, int pixCount, int threadNum=0 )
{
    TD * end = d + (pixCount * ChannelCount(cd));
    for( ;d<end; d += ChannelCount(cd) )
        pix_ref( dat, d,cd, threadNum );                                // Process 1 pixel
}

HEAD2_REF SYS_INLINE static void
foreach_1D_REF( FnDat &dat, const TS *s, TD *d, int pixCount, int threadNum=0 )
{
    TD * end = d + (pixCount * ChannelCount(cd));
    for( ;d<end; s += ChannelCount(cs),
                 d += ChannelCount(cd) )
        pix_ref( dat, s,cs, d,cd,threadNum );                           // Process 1 pixel
}



//
// 2D Reference
//

HEAD1_REF static void
foreach_2D_REF( FnDat &dat, TD* &d, int &dStep, FwiSize &roi )
{
    TD *end = FW_REF::Offset( d, roi.height * dStep );
    for( ;d!=end; FW_REF::Inc( d,dStep ) )
    foreach_1D_REF<TD,cd,pix_ref>( dat,d,roi.width );                   // Process 1 row
}

HEAD2_REF static void
foreach_2D_REF( FnDat &dat, const TS* &s, int &sStep,
                                  TD* &d, int &dStep, FwiSize &roi )
{
    TD *end = FW_REF::Offset( d, roi.height * dStep );
    for( ;d!=end; FW_REF::Inc( s,sStep ),
                  FW_REF::Inc( d,dStep ) )
    foreach_1D_REF<TS,cs,TD,cd,pix_ref>( dat,s,d,roi.width );           // Process 1 row
}



//
// 1D SSE2
//

HEAD1_1D_SSE2 SYS_INLINE static void
foreach_1D_SSE2( RegDescriptor &reg, FnDat &data, TD *d, int pixCount )
{    FnDat dat = data;
    TD * end = d + (pixCount - reg.px_ref_count) * ChannelCount(cd);
    for( ;d<end;  d += reg.dst.Elements())
    {
        Move<ma>::Load( reg.dst , (__m128i*) d );            // VC8 does not generate code if we don't read from the destination.// TODO: verify for gcc.
        pix_sse( dat, reg );
      //Move<ma>::Store( reg.dst, (__m128i*) d );
    }    
    foreach_1D_REF<TD,cd,pix_ref>( dat, d, reg.px_ref_count, reg.threadNum );

    data = dat;
}

HEAD2_1D_SSE2 SYS_INLINE static void
foreach_1D_SSE2( RegDescriptor &reg, FnDat &data, const TS *s, TD *d, int pixCount )
{    
    FnDat dat = data;
    TD * end = d + (pixCount - reg.px_ref_count) * ChannelCount(cd);
    for( ;d<end;  s += reg.src1.Elements(), 
                  d += reg.dst. Elements() )
    {
        Move<ma>::Load( reg.src1, (__m128i*) s );
        Move<ma>::Load( reg.dst , (__m128i*) d );            // VC8 does not generate code if we don't read from the destination.// TODO: verify for gcc.
        pix_sse( dat, reg );
      //Move<ma>::Store( reg.dst, (__m128i*) d );
    }    
    foreach_1D_REF<TS,cs,TD,cd,pix_ref>( dat, s, d, reg.px_ref_count, reg.threadNum );

    data = dat;
}



//
// 2D SSE2 threadee
//

HEAD1_2D_SSE2_T SYS_INLINE static void
foreach_2D_SSE2_NT( RegDescriptor &reg, FnDat &dat, TD * d, int dStep, FwiSize &roi  )
{
    //FnDat dat = data;
    //RegDescriptor reg;
    Init<rs,regCount,TD,cd>( reg, roi.width );

    TD *end = FW_REF::Offset( d, roi.height * dStep );
    
    bool isAligned = IS_ALIGNED16_1(d) && IS_ALIGNED16_1(FW_REF::Offset(d,dStep));
    
    if(isAligned) for( ;d!=end; FW_REF::Inc( d,dStep ) )    foreach_1D_SSE2<MA_TRUE,TD,cd,pix_ref,pix_sse>( reg,dat,d,roi.width );
    else          for( ;d!=end; FW_REF::Inc( d,dStep ) )    foreach_1D_SSE2<MA_FLSE,TD,cd,pix_ref,pix_sse>( reg,dat,d,roi.width );
}

HEAD1_2D_SSE2_T SYS_INLINE static void
foreach_2D_SSE2_t( void *fnArgs )
{
    FnArgs1<TD> *args = (FnArgs1<TD> *)fnArgs;
    FwiSize &roi   = args->roi;
    TD* &d     = args->d;
    int &dStep = args->dStep;

    FnDat &dat = *(args->dat);
    RegDescriptor reg;
    reg.threadNum  = args->threadNum;
    reg.startIndex = args->startIndex;
    foreach_2D_SSE2_NT<rs, regCount, TD,cd,pix_ref,pix_sse,pix_sse_setup>( reg, dat, d, dStep, roi  );
}

HEAD2_2D_SSE2_T SYS_INLINE static void
foreach_2D_SSE2_NT( RegDescriptor &reg, FnDat &dat, const TS *s, int sStep, TD *d, int dStep, FwiSize &roi )
{
    //RegDescriptor reg;
    Init<rs,TS,cs,TD,cd>( reg, roi.width );

    TD *end = FW_REF::Offset( d, roi.height * dStep );

    bool isAligned = IS_ALIGNED16_2(d,s) && IS_ALIGNED16_2(FW_REF::Offset(d,dStep),FW_REF::Offset(s,sStep));

    if(isAligned)    for( ;d!=end; FW_REF::Inc( s,sStep ),
                                   FW_REF::Inc( d,dStep ) )  foreach_1D_SSE2<MA_TRUE,TS,cs,TD,cd,pix_ref,pix_sse>( reg,dat,s,d,roi.width );
    else             for( ;d!=end; FW_REF::Inc( s,sStep ),
                                   FW_REF::Inc( d,dStep ) )  foreach_1D_SSE2<MA_FLSE,TS,cs,TD,cd,pix_ref,pix_sse>( reg,dat,s,d,roi.width );
}

HEAD2_2D_SSE2_T SYS_INLINE static void
foreach_2D_SSE2_t( void *fnArgs )
{
    FnArgs2<TS,TD> *args = (FnArgs2<TS,TD> *)fnArgs;        
    FwiSize &roi = args->roi;
    TD* &d = args->d;            int &dStep = args->dStep;
    const TS* &s = args->s;      int &sStep = args->sStep;

    FnDat &dat = *(args->dat);
    RegDescriptor reg;
    Init<rs,regCount,TS,cs,TD,cd>( reg, roi.width );
    reg.threadNum  = args->threadNum;
    reg.startIndex = args->startIndex;
    foreach_2D_SSE2_NT<rs, regCount, TS,cs,TD,cd,pix_ref,pix_sse,pix_sse_setup>( reg, dat, s, sStep, d, dStep,  roi );
}
    
//
// 2D SSE2 threader
//

HEAD1_2D_SSE2 SYS_NOINLINE static void 
foreach_2D_SSE2( FnDat &dat, TD * d, int dStep, FwiSize &roi, unsigned int min_bytes_per_thread )
{
    unsigned int total_bytes = roi.width * roi.height * ChannelCount(cd) * sizeof(TD);
    
    if (min_bytes_per_thread*2 >= total_bytes)
    {    
        RegDescriptor reg;
        reg.startIndex = 0;
        reg.threadNum  = 0;
        dat.numThreads = 1;
        if(pix_sse_setup != NO_SSE2_SETUP) pix_sse_setup(dat);
        foreach_2D_SSE2_NT<rs, regCount, TD,cd,pix_ref,pix_sse,pix_sse_setup>( reg, dat, d, dStep, roi  );
    }
    else
    {

    U32 numThreads = FW_SSE2::getThreadNum(total_bytes, min_bytes_per_thread);
  //U32 numThreads = ThreadPool::ThreadCount(); 
    FnArgs1<TD> *args = (FnArgs1<TD> *) alloca( numThreads * sizeof(FnArgs1<TD>) );
    dat.numThreads    = numThreads;
    SetThreadData(args,numThreads,dat,d,dStep,roi);
    if(pix_sse_setup != NO_SSE2_SETUP) pix_sse_setup(dat);
    for(U32 i=0; i<numThreads-1; ++i)
        ThreadPool::Run(foreach_2D_SSE2_t<rs,regCount, TD,cd,pix_ref,pix_sse,pix_sse_setup>,&args[i]);

    foreach_2D_SSE2_t<rs,regCount,TD,cd,pix_ref,pix_sse,pix_sse_setup>(&args[numThreads-1]);
    
    ThreadPool::Wait();
    }

    PxCleanup cleanup = pix_sse_cleanup;
    if(cleanup != NO_SSE2_CLEANUP) pix_sse_cleanup(dat);
}

HEAD2_2D_SSE2 SYS_NOINLINE static void
foreach_2D_SSE2( FnDat &dat, const TS *s, int sStep,
                                   TD *d, int dStep, FwiSize &roi, unsigned int min_bytes_per_thread )
{
    unsigned int total_bytes = roi.width * roi.height * ChannelCount(cd) * sizeof(TD);
    
    if (min_bytes_per_thread*2 >= total_bytes)
    {    
        RegDescriptor reg;
        reg.startIndex = 0;
        reg.threadNum  = 0;
        dat.numThreads = 1;
        if(pix_sse_setup != NO_SSE2_SETUP) pix_sse_setup(dat);
        foreach_2D_SSE2_NT<rs, regCount, TS,cs,TD,cd,pix_ref,pix_sse,pix_sse_setup>( reg, dat, s, sStep, d, dStep, roi );

      //return foreach_2D_SSE2_NT<rs, regCount, TS1,cs1,TS2,cs2,TD,cd,pix_ref,pix_sse,pix_sse_setup>( reg, dat, s1,s1Step,s2,s2Step, d, dStep, roi);
    }
    else
    {
    U32 numThreads = FW_SSE2::getThreadNum(total_bytes, min_bytes_per_thread);
    dat.numThreads = numThreads;
    FnArgs2<TS,TD> *args = (FnArgs2<TS,TD> *) alloca( numThreads * sizeof(FnArgs2<TS,TD>) );
    SetThreadData(args,numThreads,dat,s,sStep,d,dStep,roi);
    if(pix_sse_setup != NO_SSE2_SETUP) pix_sse_setup(dat);
    for(U32 i=0; i<numThreads-1; ++i)
        ThreadPool::Run(foreach_2D_SSE2_t<rs,regCount, TS,cs,TD,cd,pix_ref,pix_sse,pix_sse_setup>,&args[i]);
        
    foreach_2D_SSE2_t<rs,regCount, TS,cs,TD,cd,pix_ref,pix_sse,pix_sse_setup>(&args[numThreads-1]);
    
    ThreadPool::Wait();
    }
    PxCleanup cleanup = pix_sse_cleanup;
    if(cleanup != NO_SSE2_CLEANUP) pix_sse_cleanup(dat);
}


//
// 1D SSE2 threadee
//

HEAD1_1D_SSE2_T SYS_INLINE static void
foreach_1D_SSE2_NT( RegDescriptor &reg, FnDat& dat, TD* d, int len )
{
    
    Init<ALIGN128,regCount,TD,cd>( reg, len );
    
    bool isAligned = IS_ALIGNED16_1(d);

    if(isAligned)  foreach_1D_SSE2<MA_TRUE,TD,cd,pix_ref,pix_sse>( reg,dat,d,len );
    else           foreach_1D_SSE2<MA_FLSE,TD,cd,pix_ref,pix_sse>( reg,dat,d,len );

}

HEAD1_1D_SSE2_T SYS_INLINE static void
foreach_1D_SSE2_t( void *fnArgs )
{
    FnArgs1_1D<TD> *args = (FnArgs1_1D<TD> *)fnArgs;
    int &len   = args->len;
    TD*      &d     = args->d;
    FnDat &dat = *(args->dat);
    RegDescriptor reg;
    reg.threadNum = args->threadNum;
    reg.startIndex = args->startIndex;
    foreach_1D_SSE2_NT<regCount, TD,cd,pix_ref,pix_sse>( reg,dat,d,len );

}

HEAD2_1D_SSE2_T SYS_INLINE static void
foreach_1D_SSE2_NT( RegDescriptor &reg, FnDat& dat, const TS* s, TD* d, int len )
{
    Init<ALIGN128,regCount,TS,cs,TD,cd>( reg, len );
    bool isAligned = IS_ALIGNED16_2(d,s);

    if(isAligned)   foreach_1D_SSE2<MA_TRUE,TS,cs,TD,cd,pix_ref,pix_sse>( reg,dat,s,d,len );
    else            foreach_1D_SSE2<MA_FLSE,TS,cs,TD,cd,pix_ref,pix_sse>( reg,dat,s,d,len );
}

HEAD2_1D_SSE2_T SYS_INLINE static void
foreach_1D_SSE2_t( void *fnArgs )
{
    FnArgs2_1D<TS,TD> *args = (FnArgs2_1D<TS,TD> *)fnArgs;
    int &len = args->len;
    TD* &d = args->d;
    const TS* &s = args->s;
    FnDat &dat = *(args->dat);
    RegDescriptor reg;
    reg.threadNum = args->threadNum;
    reg.startIndex = args->startIndex;
    foreach_1D_SSE2_NT<regCount, TS,cs,TD,cd,pix_ref,pix_sse>( reg,dat,s,d,len );
}



//
// 1D SSE2 threader
//

HEAD1_1D_SSE2_Tr SYS_NOINLINE static void
foreach_1D_SSE2_tr(  FnDat &dat, TD * d, int len, unsigned int min_bytes_per_thread )
{
    unsigned int total_bytes = len * sizeof(TD);
    
    if (min_bytes_per_thread*2 >= total_bytes)
    {    
        RegDescriptor reg;
        dat.numThreads = 1;
        reg.startIndex = 0;
        reg.threadNum  = 0;
        if(pix_sse_setup != NO_SSE2_SETUP) pix_sse_setup(dat);
        foreach_1D_SSE2_NT<regCount, TD,cd,pix_ref,pix_sse>( reg,dat,d,len );
    }
    else
    {
    U32 numThreads = FW_SSE2::getThreadNum(total_bytes, min_bytes_per_thread);
    FnArgs1_1D<TD> *args = (FnArgs1_1D<TD> *) alloca( numThreads * sizeof(FnArgs1_1D<TD>) );
    dat.numThreads = numThreads;
    SetThreadData_1D(args,numThreads,dat,d,len);
    if(pix_sse_setup != NO_SSE2_SETUP) pix_sse_setup(dat);
    for(U32 i=0; i<numThreads-1; ++i)
    ThreadPool::Run(foreach_1D_SSE2_t<regCount,TD,cd,pix_ref,pix_sse>,&args[i]);
        
    foreach_1D_SSE2_t<regCount,TD,cd,pix_ref,pix_sse>(&args[numThreads-1]);
    
    ThreadPool::Wait();
    }

    PxCleanup cleanup = pix_sse_cleanup;
    if(cleanup != NO_SSE2_CLEANUP) pix_sse_cleanup(dat);

}

HEAD2_1D_SSE2_Tr SYS_NOINLINE static void
foreach_1D_SSE2_tr(  FnDat &dat, const TS *s, TD *d, int len, unsigned int min_bytes_per_thread )
{
    unsigned int total_bytes = len * sizeof(TD);
    min_bytes_per_thread;total_bytes;
    if (min_bytes_per_thread*2 >= total_bytes)
    {    
        RegDescriptor reg;
        dat.numThreads = 1;
        reg.startIndex = 0;
        reg.threadNum  = 0;
        if(pix_sse_setup != NO_SSE2_SETUP) pix_sse_setup(dat);
        foreach_1D_SSE2_NT<regCount, TS,cs,TD,cd,pix_ref,pix_sse>( reg,dat,s,d,len );;
    }
    else
    {

    U32 numThreads = FW_SSE2::getThreadNum(total_bytes, min_bytes_per_thread);
    FnArgs2_1D<TS,TD> *args = (FnArgs2_1D<TS,TD> *) alloca( numThreads * sizeof(FnArgs2_1D<TS,TD>) );
    dat.numThreads = numThreads;
    if(pix_sse_setup != NO_SSE2_SETUP) pix_sse_setup(dat);
    SetThreadData_1D(args,numThreads,dat,s,d,len);
        for(U32 i=0; i<numThreads-1; ++i)
        ThreadPool::Run(foreach_1D_SSE2_t<regCount,TS,cs,TD,cd,pix_ref,pix_sse>,&args[i]);
        
    foreach_1D_SSE2_t<regCount,TS,cs,TD,cd,pix_ref,pix_sse>(&args[numThreads-1]);
    
    ThreadPool::Wait();
    }

    PxCleanup cleanup = pix_sse_cleanup;
    if(cleanup != NO_SSE2_CLEANUP) pix_sse_cleanup(dat);
}



//
// 1D External
//

HEAD1_1D_EXT SYS_INLINE static FwStatus 
foreach_1D( TD *d, int len, FnDat& dat = g_dat, unsigned int min_bytes_per_thread = 65536 )
{
    if( FW_REF::PtrNotOK ( d   ) ) return fwStsNullPtrErr;
    if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;
        
    switch( Dispatch::Type(pix_sse != NO_SSE2) )
    {        
        case DT_SSE2:     foreach_1D_SSE2_tr<regCount,TD,cd,pix_ref,pix_sse,pix_sse_setup, pix_sse_cleanup>( dat,d,len,min_bytes_per_thread );break;
        default:          foreach_1D_REF <TD,cd,pix_ref>( dat,d,len );
    }
    return fwStsNoErr;
}


HEAD2_1D_EXT SYS_INLINE static FwStatus 
foreach_1D( const TS *s, TD *d, int len, FnDat& dat = g_dat, unsigned int min_bytes_per_thread = 65536 )
{
    if( FW_REF::PtrNotOK ( s   , d ) ) return fwStsNullPtrErr;
    if( FW_REF::SizeNotOK( len     ) ) return fwStsSizeErr;

    switch( Dispatch::Type(pix_sse != NO_SSE2) ){    
        case DT_SSE2:             foreach_1D_SSE2_tr<regCount,TS,cs,TD,cd,pix_ref,pix_sse,pix_sse_setup, pix_sse_cleanup>( dat,s,d,len,min_bytes_per_thread );break;
        default:                foreach_1D_REF <TS,cs,TD,cd,pix_ref>( dat,s,d,len );
    }
    return fwStsNoErr;
}



//
// 2D External
//

HEAD1_2D_EXT SYS_INLINE static FwStatus 
foreach( TD * d, int dStep, FwiSize &roi, FnDat& dat = g_dat, unsigned int min_bytes_per_thread = 65536  )
{
    if( FW_REF::PtrNotOK ( d     ) ) return fwStsNullPtrErr;    
    if( FW_REF::StepNotOK( dStep ) ) return fwStsStepErr;
    if( FW_REF::RoiNotOK ( roi   ) ) return fwStsSizeErr;

    switch( Dispatch::Type(pix_sse != NO_SSE2) ){
        case DT_SSE2: foreach_2D_SSE2<rs,regCount, TD,cd,pix_ref,pix_sse,pix_sse_setup, pix_sse_cleanup>(dat,d,dStep,roi,min_bytes_per_thread); break;
        default:      foreach_2D_REF < TD,cd,pix_ref >(dat,d,dStep,roi); break;    
    }
    return fwStsNoErr;
}

HEAD2_2D_EXT SYS_INLINE static FwStatus 
foreach( const TS *s, int sStep, TD *d, int dStep, FwiSize &roi, FnDat& dat = g_dat, unsigned int min_bytes_per_thread = 65536 )
{
    if( FW_REF::PtrNotOK ( s    , d     ) ) return fwStsNullPtrErr;
    if( FW_REF::StepNotOK( sStep, dStep ) ) return fwStsStepErr;
    if( FW_REF::RoiNotOK ( roi          ) ) return fwStsSizeErr;

    switch( Dispatch::Type(pix_sse != NO_SSE2) ){        
        case DT_SSE2: foreach_2D_SSE2<rs,regCount, TS,cs,TD,cd,pix_ref,pix_sse,pix_sse_setup, pix_sse_cleanup>(dat,s,sStep,d,dStep,roi); break;
        default:      foreach_2D_REF < TS,cs,TD,cd,pix_ref >(dat,s,sStep,d,dStep,roi); break;
    }
    return fwStsNoErr;
}


} // namespace OPT_LEVEL


#endif // __FOREACH_H__

