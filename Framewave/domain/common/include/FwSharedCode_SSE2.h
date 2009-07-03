/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FW_SHARED_CODE_SSE2_H__
#define __FW_SHARED_CODE_SSE2_H__

#include "fwdev.h"
#include "FwSharedCode.h"

namespace OPT_LEVEL
{

//=======================================
//            Type Wrappers
//=======================================
struct M128T
{
    XMM128 data;
    XMM128 & operator()(){ return data; }

    M128T(){}
    M128T( U64 a, U64 b )
    {
        data.u64[0] = a;
        data.u64[1] = b;
    }

    M128T( U16 a, U16 b, U16 c, U16 d, U16 e, U16 f, U16 g, U16 h )
    {
        data.u16[0] = a;
        data.u16[1] = b;
        data.u16[2] = c;
        data.u16[3] = d;
        data.u16[4] = e;
        data.u16[5] = f;
        data.u16[6] = g;
        data.u16[7] = h;
    }
};


struct FW_SSE2 : public FW_REF
{
    template< class T >
    SYS_INLINE static void Load1( XMM128 &  , const T     &      ){ ASSERT(false); }

    SYS_INLINE static void Load1( XMM128 & v , int &value)
    {
        CBL_SSE2::Load_1x4_32s( v.i, value );
    }
    
    SYS_INLINE static void Load1( XMM128 & v, const Fw8s &value   )
    { 
        CBL_SSE2::Load_1x16_8s( v.i, value );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw8u &value   )
    { 
        CBL_SSE2::Load_1x16_8u( v.i, value );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw16s &value  )
    { 
        CBL_SSE2::Load_1x8_16s( v.i, value );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw16u &value  )
    { 
        CBL_SSE2::Load_1x8_16u( v.i, value );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw16sc &value )
    { 
        CBL_SSE2::Load_2x4_16s( v.i, value.re, value.im );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw32s &value  )
    { 
        CBL_SSE2::Load_1x4_32s( v.i, value );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw32u &value  )
    { 
        CBL_SSE2::Load_1x4_32u( v.i, value );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw32sc &value )
    { 
        CBL_SSE2::Load_2x2_32s( v.i, value.re, value.im );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw32f &value  )
    { 
        CBL_SSE2::Load_1x4_32f( v.f, value );
    }    

    SYS_INLINE static void Load1( XMM128 & v, const Fw32fc &value )
    { 
        CBL_SSE2::Load_2x2_32f( v.f, value.re, value.im );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw64f &value  )
    { 
        CBL_SSE2::Load_1x2_64f( v.d, value );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw64fc &value )
    { 
        CBL_SSE2::Load_2x1_64f( v.d, value.re, value.im );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw64sc &value )
    { 
        CBL_SSE2::Load_2x1_64s( v.i, value.re, value.im );
    }

    SYS_INLINE static void Load1( XMM128 & v, const Fw64s &value  )
    { 
        CBL_SSE2::Load_1x2_64s( v.i, value );
    }

    SYS_INLINE static void Load2( XMM128 & v, const Fw8u value[],CH noChannels, int firstChannel )
    { 
            switch(noChannels)
            {
                  
                case AC4:   v.u8[0]  = value[0]; 
                            v.u8[1]  = value[1];
                            v.u8[2]  = value[2]; 
                            v.u8[3]  = 0;
                            v.i = _mm_shuffle_epi32( v.i, _MM_SHUFFLE( 0,0,0,0 ) );
                            break;

                case C4:    v.u8[0]  = value[0]; 
                            v.u8[1]  = value[1];
                            v.u8[2]  = value[2]; 
                            v.u8[3]  = value[3];
                            v.i = _mm_shuffle_epi32( v.i, _MM_SHUFFLE( 0,0,0,0 ) );
                            break;

                case C3:    v.u8[0]  = value[firstChannel%3]; 
                            v.u8[1]  = value[(firstChannel+1)%3];
                            v.u8[2]  = value[(firstChannel+2)%3]; 
                            v.u8[3]  = value[firstChannel%3];
                            v.u8[4]  = value[(firstChannel+1)%3]; 
                            v.u8[5]  = value[(firstChannel+2)%3];
                            v.u8[6]  = value[firstChannel%3]; 
                            v.u8[7]  = value[(firstChannel+1)%3];
                            v.u8[8]  = value[(firstChannel+2)%3];
                            v.u8[9]  = value[firstChannel%3]; 
                            v.u8[10] = value[(firstChannel+1)%3]; 
                            v.u8[11] = value[(firstChannel+2)%3];
                            v.u8[12] = value[firstChannel%3];
                            v.u8[13] = value[(firstChannel+1)%3]; 
                            v.u8[14] = value[(firstChannel+2)%3];
                            v.u8[15] = value[firstChannel%3];
                            break;
            }
      }   

    SYS_INLINE static void Load2( XMM128 & v, const Fw8s value[],CH noChannels, int firstChannel )
    { 
            switch(noChannels)
            {
                case AC4:   v.s8[0]  = value[0]; 
                            v.s8[1]  = value[1];
                            v.s8[2]  = value[2]; 
                            v.s8[3]  = 0;
                            v.i = _mm_shuffle_epi32( v.i, _MM_SHUFFLE( 0,0,0,0 ) );
                            break;
            
                case C4:    v.s8[0]  = value[0]; 
                            v.s8[1]  = value[1];
                            v.s8[2]  = value[2]; 
                            v.s8[3]  = value[3];
                            v.i = _mm_shuffle_epi32( v.i, _MM_SHUFFLE( 0,0,0,0 ) );
                            break;

                case C3:    v.s8[0]  = value[firstChannel%3]; 
                            v.s8[1]  = value[(firstChannel+1)%3];
                            v.s8[2]  = value[(firstChannel+2)%3]; 
                            v.s8[3]  = value[firstChannel%3];
                            v.s8[4]  = value[(firstChannel+1)%3]; 
                            v.s8[5]  = value[(firstChannel+2)%3];
                            v.s8[6]  = value[firstChannel%3]; 
                            v.s8[7]  = value[(firstChannel+1)%3];
                            v.s8[8]  = value[(firstChannel+2)%3];
                            v.s8[9]  = value[firstChannel%3]; 
                            v.s8[10] = value[(firstChannel+1)%3]; 
                            v.s8[11] = value[(firstChannel+2)%3];
                            v.s8[12] = value[firstChannel%3];
                            v.s8[13] = value[(firstChannel+1)%3]; 
                            v.s8[14] = value[(firstChannel+2)%3];
                            v.s8[15] = value[firstChannel%3];
                            break;
            }
      } 

    SYS_INLINE static void Load2( XMM128 & v, const Fw16u value[],CH noChannels, int firstChannel )
    { 
            switch(noChannels)
            {
                case AC4:   v.u16[0] = value[0];
                            v.u16[1] = value[1];
                            v.u16[2] = value[2];
                            v.u16[3] = 0;
                            v.u16[4] = value[0];
                            v.u16[5] = value[1];
                            v.u16[6] = value[2];
                            v.u16[7] = 0;
                            break;

                  case C4:  v.u16[0] = value[0]; 
                            v.u16[1] = value[1];
                            v.u16[2] = value[2];
                            v.u16[3] = value[3];
                            v.u16[4] = value[0]; 
                            v.u16[5] = value[1];
                            v.u16[6] = value[2];
                            v.u16[7] = value[3];
                            break;

                  case C3:  v.u16[0] = value[firstChannel%3]; 
                            v.u16[1] = value[(firstChannel+1)%3];
                            v.u16[2] = value[(firstChannel+2)%3]; 
                            v.u16[3] = value[firstChannel%3];
                            v.u16[4] = value[(firstChannel+1)%3]; 
                            v.u16[5] = value[(firstChannel+2)%3];
                            v.u16[6] = value[firstChannel%3]; 
                            v.u16[7] = value[(firstChannel+1)%3];
                            break;
            }
      } 
 
    SYS_INLINE static void Load2( XMM128 & v, const Fw16s value[],CH noChannels, int firstChannel )
    { 
            switch(noChannels)
            {
                 case AC4:  v.s16[0] = value[0]; 
                            v.s16[1] = value[1];
                            v.s16[2] = value[2];
                            v.s16[3] = 0;
                            v.s16[4] = value[0];
                            v.s16[5] = value[1];
                            v.s16[6] = value[2];
                            v.s16[7] = 0;   
                            break;

                 case C4:   v.s16[0] = value[0]; 
                            v.s16[1] = value[1];
                            v.s16[2] = value[2];
                            v.s16[3] = value[3];
                            v.s16[4] = value[0];
                            v.s16[5] = value[1];
                            v.s16[6] = value[2];
                            v.s16[7] = value[3];
                            break;

                 case C3:   v.s16[0] = value[firstChannel%3]; 
                            v.s16[1] = value[(firstChannel+1)%3];
                            v.s16[2] = value[(firstChannel+2)%3]; 
                            v.s16[3] = value[firstChannel%3];
                            v.s16[4] = value[(firstChannel+1)%3]; 
                            v.s16[5] = value[(firstChannel+2)%3];
                            v.s16[6] = value[firstChannel%3]; 
                            v.s16[7] = value[(firstChannel+1)%3];
                            break;
            }
      } 
    
    SYS_INLINE static void Load2( XMM128 & v, const Fw32s value[4],CH noChannels, int firstChannel )
    { 
            switch(noChannels)
            {
                case AC4:   v.s32[0] = value[0];
                            v.s32[1] = value[1];
                            v.s32[2] = value[2];
                            v.s32[3] = 0;
                            break;

                case C4:    v.s32[0] = value[0]; 
                            v.s32[1] = value[1];
                            v.s32[2] = value[2];
                            v.s32[3] = value[3];
                            break;

                case C3:    v.s32[0] = value[firstChannel%3]; 
                            v.s32[1] = value[(firstChannel+1)%3];
                            v.s32[2] = value[(firstChannel+2)%3]; 
                            v.s32[3] = value[firstChannel%3];
                            break;
            }
    } 

    SYS_INLINE static void Load2( XMM128 & v, const Fw16sc value[],CH noChannels, int firstChannel )
    { 
            switch(noChannels)
            {
                  case AC4: v.s16[0] = value[0].re; 
                            v.s16[1] = value[0].im; 
                            v.s16[2] = value[1].re;
                            v.s16[3] = value[1].im;
                            v.s16[4] = value[2].re;
                            v.s16[5] = value[2].im;
                            v.s16[6] = 0;
                            v.s16[7] = 0;
                            break;


                  case C4:  v.s16[0] = value[0].re;  
                            v.s16[1] = value[0].im; 
                            v.s16[2] = value[1].re;
                            v.s16[3] = value[1].im;
                            v.s16[4] = value[2].re;
                            v.s16[5] = value[2].im;
                            v.s16[6] = value[3].re;
                            v.s16[7] = value[3].im;
                            break;

                  case C3:  v.s16[0] = value[firstChannel%3].re; 
                            v.s16[1] = value[firstChannel%3].im;
                            v.s16[2] = value[(firstChannel+1)%3].re; 
                            v.s16[3] = value[(firstChannel+1)%3].im;
                            v.s16[4] = value[(firstChannel+2)%3].re; 
                            v.s16[5] = value[(firstChannel+2)%3].im;
                            v.s16[6] = value[firstChannel%3].re; 
                            v.s16[7] = value[firstChannel%3].im;
                            break;
            }
    } 

    SYS_INLINE static void Load2( XMM128 & v, const Fw32sc value[],CH noChannels, int firstChannel )
    { 
            switch(noChannels)
            {
                 case AC4:  v.s32[0] = value[0].re;  //TODO, 2nd reg will contain AC4
                            v.s32[1] = value[1].im;
                            v.s32[2] = value[2].re;  
                            v.s32[3] = value[3].im;
                            break;

                 case C4:   v.s32[0] = value[0].re;  
                            v.s32[1] = value[1].im;
                            v.s32[2] = value[2].re;  
                            v.s32[3] = value[3].im;
                            break;

                 case C3:   v.s32[0] = value[firstChannel%3].re;
                            v.s32[1] = value[firstChannel%3].im;
                            v.s32[2] = value[(firstChannel+1)%3].re; 
                            v.s32[3] = value[(firstChannel+1)%3].im;
                            break;
            }
    } 
    
    SYS_INLINE static void Load2( XMM128 & v, const Fw32f value[],CH noChannels, int firstChannel )
    { 
            switch(noChannels)
            {                
                  case AC4: v.f32[0] = value[0]; 
                            v.f32[1] = value[1];
                            v.f32[2] = value[2];
                            v.f32[3] = 0;
                            break;

                  case C4:  v.f32[0] = value[0]; 
                            v.f32[1] = value[1];
                            v.f32[2] = value[2];
                            v.f32[3] = value[3];
                            break;

                  case C3:  v.f32[0] = value[firstChannel%3]; 
                            v.f32[1] = value[(firstChannel+1)%3];
                            v.f32[2] = value[(firstChannel+2)%3]; 
                            v.f32[3] = value[firstChannel%3];
                            break;
            }
    } 
    
    SYS_INLINE static void Load2( XMM128 & v, const Fw32fc value[],CH noChannels, int firstChannel )
    { 
            switch(noChannels)
            {
                                    
                  case AC4: v.f32[0] = value[0].re;  //TODO, 2nd reg will contain AC4
                            v.f32[1] = value[1].im;
                            v.f32[2] = value[2].re;
                            v.f32[3] = value[3].im;
                            break;

                  case C4:  v.f32[0] = value[0].re;
                            v.f32[1] = value[1].im;
                            v.f32[2] = value[2].re;
                            v.f32[3] = value[3].im;
                            break;

                  case C3:  v.f32[0] = value[firstChannel%3].re; 
                            v.f32[1] = value[firstChannel%3].im;
                            v.f32[2] = value[(firstChannel+1)%3].re; 
                            v.f32[3] = value[(firstChannel+1)%3].im;
                            break;
            }
            
            
     }
    //Returns the optimal number of threads to use for a given size buffer
    SYS_INLINE static U32 getThreadNum(U32 total_bytes, U32 min_bytes_per_thread)
    {
        U32 numThreads = ThreadPool::ThreadCount(); 
        U32 tempNum = total_bytes/(min_bytes_per_thread);
        tempNum = tempNum - 1;
        numThreads = MIN(numThreads, tempNum);
        return numThreads;
    }

    SYS_INLINE static void Pack16Uto8( XMM128 &dstSrcLo, XMM128 &srcHi )
    {
        dstSrcLo.i = _mm_packus_epi16( dstSrcLo.i, srcHi.i );
    }
    
    //packs and saturates two 4 X 32 bit signed integers into 8 X 16 bit signed integer(srcDstLo)
    SYS_INLINE static void pack32STo16S( __m128i &srcDstLo, __m128i &dstHi )
    {
        srcDstLo = _mm_packs_epi32  (srcDstLo, dstHi);
    }

    //packs and saturates two 8 X 16 bit signed integers into 16 X 8 bit unsigned integer(srcDstLo)
    SYS_INLINE static void pack16STo8U( __m128i &srcDstLo, __m128i &dstHi )
    {
        srcDstLo = _mm_packus_epi16 (srcDstLo, dstHi);
    }

    //packs and saturates two 8 X 16 bit signed integers into 16 X 8 bit signed integer(srcDstLo)
    SYS_INLINE static void pack16STo8S( __m128i &srcDstLo, __m128i &dstHi )
    {
        srcDstLo = _mm_packs_epi16  (srcDstLo, dstHi);
    }

};


} // namespace OPT_LEVEL

#endif // __FW_SHARED_CODE_SSE2_H__
