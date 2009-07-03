/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


//    Contains core functions and generic utilities.
//    Includes system.h, which contains configuration
//    specific codes.


#ifndef __FWDEV_H__
#define __FWDEV_H__

#pragma warning(disable:4505)

#include "buildnum.h"
#include "fwBase.h"
#include "system.h"
#include "shortcuts.h"

//=======================================
//    Optimization level prefix macro
//=======================================

#define xPREFIX_OPT(PREFIX, FNAME) PREFIX##FNAME
#define PREFIX_OPT(PREFIX, FNAME) xPREFIX_OPT(PREFIX, FNAME)

// Reference marker
#define OREFR
// SSE2 marker
#define OSSE2
// Barcelona marker
#define OF10H

//=======================================
//    Static Assert
//=======================================
#define STATIC_ASSERT( arg ) static_assert<arg>()


//=======================================
//   Constants
//   Note: Global constants are not
//         compiled multipass
//=======================================
namespace Const
{
    extern const U32    FW_ALIGN;
    extern const double FW_PI;
    extern const double FW_2PI;

    // Debug
    extern const U32 ImageDisplayOnPass;
    extern const U32 ImageDisplayOnFail;
           
    extern const double BayerDither44[4][4];
           
    extern const U32 NAN_32Fi;
    extern const U32 INF_32Fi;
    extern const U32 INF_NEG_32Fi;
    extern const U32 IND_32Fi;
           
    extern const U32 NAN_64Fi[2];
    extern const U32 INF_64Fi[2];
    extern const U32 INF_NEG_64Fi[2];
    extern const U32 IND_64Fi[2];
           
           
    extern const double NAN_64F;
    extern const double INF_64F;
    extern const double INF_NEG_64F;
    extern const double IND_64F;
           
    extern const float NAN_32F;
    extern const float INF_32F;
    extern const float INF_NEG_32F;
    extern const float IND_32F;
};


namespace OPT_LEVEL
{

    template<bool b>
    struct static_assert {};
    template<>
    struct static_assert<false>
    {
        private:
            static_assert();
    };
} // namespace OPT_LEVEL

#define COMPILE_TIME_ASSERT(expr) typedef char __COMPILE_TIME_ASSERT__[(expr)?1:-1];

#ifdef  NO_ASSERT
#include <fstream>
#define ASSERT(value) { if(!(value)){ std::ofstream f( "assert.txt", std::ios_base::app ); f<<__FILE__<<"("<<__LINE__<<"): "<<#value<<std::endl; }}
#else
#include <assert.h>
#define ASSERT(value) assert(value)
#endif

//=======================================
//   Check Type Sizes
//=======================================

COMPILE_TIME_ASSERT ( 1 == sizeof ( Fw8s  ) );
COMPILE_TIME_ASSERT ( 2 == sizeof ( Fw16s ) );
COMPILE_TIME_ASSERT ( 4 == sizeof ( Fw32s ) );
COMPILE_TIME_ASSERT ( 8 == sizeof ( Fw64s ) );

COMPILE_TIME_ASSERT ( 1 == sizeof ( Fw8u  ) );
COMPILE_TIME_ASSERT ( 2 == sizeof ( Fw16u ) );
COMPILE_TIME_ASSERT ( 4 == sizeof ( Fw32u ) );
COMPILE_TIME_ASSERT ( 8 == sizeof ( Fw64u ) );

COMPILE_TIME_ASSERT ( 4 == sizeof ( Fw32f ) );
COMPILE_TIME_ASSERT ( 8 == sizeof ( Fw64f ) );

//=======================================
//  Preprocessor Switches
//=======================================
#pragma warning( disable: 4786 )    // truncated identifier name

namespace OPT_LEVEL
{

//=======================================
//    Enums
//=======================================

    enum CH
    {
        C1  = 1,
        C2  = 2,
        C3  = 3,
        C4  = 4,
        AC4 = 5
    };

    enum FWType   // NOTE: If this enum is changed, it must also be changed in the imageFunc.php file in the ImageViewer
    {
        FW_NONE,
        FW_8U,  FW_8S,
        FW_16U, FW_16S,  FW_16SC,
        FW_32U, FW_32S,  FW_32SC,
        FW_32F, FW_32FC,
        FW_64U,FW_64S, FW_64F,  FW_64FC, FW_64SC
    };

//! return the size (in bytes) corresponding to the specified FWType (enumerated) value.
    SYS_WEAKINLINE static U32 Size ( FWType type )
    {
        switch ( type )
        {
            case FW_8U:   return sizeof ( Fw8u   );
            case FW_8S:   return sizeof ( Fw8s   );
            case FW_16U:  return sizeof ( Fw16u  );
            case FW_16S:  return sizeof ( Fw16s  );
            case FW_16SC: return sizeof ( Fw16sc );
            case FW_32U:  return sizeof ( Fw32u  );
            case FW_32S:  return sizeof ( Fw32s  );
            case FW_32SC: return sizeof ( Fw32sc );
            case FW_32F:  return sizeof ( Fw32f  );
            case FW_32FC: return sizeof ( Fw32fc );
            case FW_64S:  return sizeof ( Fw64s  );
            case FW_64U:  return sizeof ( Fw64u  );
            case FW_64F:  return sizeof ( Fw64f  );
            case FW_64FC: return sizeof ( Fw64fc );
            case FW_64SC: return sizeof ( Fw64sc );
            default: ASSERT ( false ); return 0;
        }
    }

//! the GetType struct contains only one, static function Index() that returns an enumerated value that represents specified type.

    template<class T> struct GetType          { static FWType Index() { ASSERT ( false ); return 0; } };
    template<       > struct GetType<Fw8s  > { static FWType Index() { return FW_8S;   } };
    template<       > struct GetType<Fw8u  > { static FWType Index() { return FW_8U;   } };
    template<       > struct GetType<Fw16u > { static FWType Index() { return FW_16U;  } };
    template<       > struct GetType<Fw16s > { static FWType Index() { return FW_16S;  } };
    template<       > struct GetType<Fw16sc> { static FWType Index() { return FW_16SC; } };
    template<       > struct GetType<Fw32u > { static FWType Index() { return FW_32U;  } };
    template<       > struct GetType<Fw32s > { static FWType Index() { return FW_32S;  } };
    template<       > struct GetType<Fw32sc> { static FWType Index() { return FW_32SC; } };
    template<       > struct GetType<Fw32f > { static FWType Index() { return FW_32F;  } };
    template<       > struct GetType<Fw32fc> { static FWType Index() { return FW_32FC; } };
    template<       > struct GetType<Fw64s > { static FWType Index() { return FW_64S;  } };
    template<       > struct GetType<Fw64u > { static FWType Index() { return FW_64U;  } };
    template<       > struct GetType<Fw64f > { static FWType Index() { return FW_64F;  } };
    template<       > struct GetType<Fw64fc> { static FWType Index() { return FW_64FC; } };
    template<       > struct GetType<Fw64sc> { static FWType Index() { return FW_64SC; } };

//=======================================
//    Macros
//=======================================
//#define ChannelCount(c) ((c == 5) ? 3 : (MIN(c,4))) //changed to accomodate AC4(Ravi)
#define ChannelCount(c) MIN(c,(CH)4)
//#define MERGE( a, b ) ((a<<16) | b )

   // Test if the specified value(s) (typically a pointer) is 16 bytes aligned.
   // the different variants of this macro can perform the test on muliple values at a time.
    
#define IS_ALIGNED16_1( a         )     ( 0 == (0xF & (((U64)a)        )))
#define IS_ALIGNED16_2( a, b      )     ( 0 == (0xF & (((U64)a)|((U64)b)      )))
#define IS_ALIGNED16_3( a, b, c   )     ( 0 == (0xF & (((U64)a)|((U64)b)|((U64)c)   )))
#define IS_ALIGNED16_4( a, b, c, d)     ( 0 == (0xF & (((U64)a)|((U64)b)|((U64)c)|((U64)d))))
#define IS_ALIGNED16_5( a, b, c, d, e)  ( 0 == (0xF & (((U64)a)|((U64)b)|((U64)c)|((U64)d)|((U64)e))))

   // Define a shuffle mask used as the immediate value for a 4 fields shuffle instruction.
   // a - the index value for all four fields
   
#define SHUF4(    a      ) _MM_SHUFFLE( a, a, a, a)

   // returns the count of elements in a C/C++ array
   // NOTE - the parameters must be of an array type, not of a pointer type!
   
#define COUNTOF(a) (sizeof(a) / sizeof( a[0] ))


} // namespace OPT_LEVEL


#endif // __FWDEV_H__
