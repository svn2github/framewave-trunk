/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __COLOR_MODEL_CONVERSION_DEFS_H__
#define __COLOR_MODEL_CONVERSION_DEFS_H__

#include "colorModelConversion.h"
#include "fe.h"

namespace OPT_LEVEL
{
    namespace CMC
    {
        ///////////////////////////////////
        // Process Model (ie C1, C2, C3, P1, or P3)
        ///////////////////////////////////
        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc, ChCount ch>
        struct P_Model
        {
            static void Process( RegFile &r ) { STATIC_ASSERT(false); }
        };

        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc>
        struct P_Model <mp1, mp2, mp3, fn, prc, CH_C3>
        {
            static SYS_INLINE void Process( RegFile &r )
            {
                // Transpose 24-bit data from Channel format to Planar format.
                // Replace CBL with SSEPlus
                // CBL_SSE2::Convert_3C_to_3P_8bit( r.src1[0].i, r.src1[1].i, r.src1[2].i );
                ssp_convert_3c_3p_epi8( &r.src1[0].i, &r.src1[1].i, &r.src1[2].i );

                r.dst[0].i = Pixel<fn, X, prc, mp1>::Process( r.src1[0], r.src1[1], r.src1[2] );
                r.dst[1].i = Pixel<fn, Y, prc, mp2>::Process( r.src1[0], r.src1[1], r.src1[2] );
                r.dst[2].i = Pixel<fn, Z, prc, mp3>::Process( r.src1[0], r.src1[1], r.src1[2] );

                // Replace CBL with SSEPlus
                // CBL_SSE2::Convert_3P_to_3C_8bit( r.dst[0].i, r.dst[1].i, r.dst[2].i );
                ssp_convert_3p_3c_epi8( &r.dst[0].i, &r.dst[1].i, &r.dst[2].i );
            }
        };

        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc>
        struct P_Model <mp1, mp2, mp3, fn, prc, CH_AC4>
        {
            static SYS_INLINE void Process( RegFile &r )
            {
                // Transpose 32-bit data from Channel format to Planar format.   
                // CBL_SSE2::Convert_4C_to_4P_8bit( r.src1[0].i, r.src1[1].i, r.src1[2].i, r.src1[3].i );
                ssp_convert_4c_4p_epi8( &r.src1[0].i, &r.src1[1].i, &r.src1[2].i, &r.src1[3].i );

                r.dst[0].i = Pixel<fn, X, prc, mp1>::Process( r.src1[0], r.src1[1], r.src1[2] );
                r.dst[1].i = Pixel<fn, Y, prc, mp2>::Process( r.src1[0], r.src1[1], r.src1[2] );
                r.dst[2].i = Pixel<fn, Z, prc, mp3>::Process( r.src1[0], r.src1[1], r.src1[2] );

                // UnTranspose 32-bit data from Planar format to Channel format
                // CBL_SSE2::Convert_4P_to_4C_8bit( r.dst[0].i, r.dst[1].i, r.dst[2].i, r.dst[3].i );
                ssp_convert_4p_4c_epi8( &r.dst[0].i, &r.dst[1].i, &r.dst[2].i, &r.dst[3].i );
            }
        };

        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc>
        struct P_Model <mp1, mp2, mp3, fn, prc, CH_P3>
        {
            static SYS_INLINE void Process( RegFile &r )
            {
                r.dst[0].i  = Pixel<fn, X, prc, mp1>::Process( r.src1[0], r.src2[0], r.src3[0] );
                r.dst2[0].i = Pixel<fn, Y, prc, mp2>::Process( r.src1[0], r.src2[0], r.src3[0] );
                r.dst3[0].i = Pixel<fn, Z, prc, mp3>::Process( r.src1[0], r.src2[0], r.src3[0] );        
            }
        };

        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc>
        struct P_Model <mp1, mp2, mp3, fn, prc, CH_C3P3>
        {
            static SYS_INLINE void Process( RegFile &r )
            {
                // Transpose 24-bit data from Channel format to Planar format.
                ssp_convert_3c_3p_epi8( &r.src1[0].i, &r.src1[1].i, &r.src1[2].i);

                r.dst[0].i  = Pixel<fn, X, prc,mp1>::Process( r.src1[0], r.src1[1], r.src1[2] );
                r.dst2[0].i = Pixel<fn, Y, prc,mp2>::Process( r.src1[0], r.src1[1], r.src1[2] );
                r.dst3[0].i = Pixel<fn, Z, prc,mp3>::Process( r.src1[0], r.src1[1], r.src1[2] );
            }
        };

        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc>
        struct P_Model <mp1, mp2, mp3, fn, prc, CH_P3C3>
        {
            static SYS_INLINE void Process( RegFile &r )
            {
                r.dst[0].i = Pixel<fn, X, prc,mp1>::Process( r.src1[0], r.src2[0], r.src3[0] );
                r.dst[1].i = Pixel<fn, Y, prc,mp2>::Process( r.src1[0], r.src2[0], r.src3[0] );
                r.dst[2].i = Pixel<fn, Z, prc,mp3>::Process( r.src1[0], r.src2[0], r.src3[0] );

                // Replace CBL with SSEPlus
                // CBL_SSE2::Convert_3P_to_3C_8bit( r.dst[0].i, r.dst[1].i, r.dst[2].i );
                ssp_convert_3p_3c_epi8( &r.dst[0].i, &r.dst[1].i, &r.dst[2].i );
            }
        };
    };


    namespace NoChromaSampling
    {
        #define PIX_FN2_REF  void (*pix_ref  )( const TS*, CH,  TD*, CH )
        #define PIX_COUNT_FE2( T, c )                          \
                                   ((c == C1)  ? PIX_C1(T):    \
                                    (c == C2)  ? PIX_C2(T):    \
                                    (c == C3)  ? PIX_C3(T):    \
                                    (c == C4)  ? PIX_C4(T):    \
                                    (c == AC4) ? PIX_C1(T):    \
                                     0 )

        template< CMC::ChCount chCount, class TS, CH cs, class TD, CH cd, PIX_FN2_REF >
        struct ChannelToChannel_REF : public fe2<TS, cs, TD, cd>
        {
            ChannelToChannel_REF() {}
            FE_REF

            IV REFR(const TS *src, TD *dst) const 
            {
                pix_ref(src, cs, dst, cd);
            }
        };

        template< CMC::ChCount chCount, class TS, CH cs, class TD, CH cd, PIX_FN2_REF, 
                  CMC::MathPattern mp1, CMC::MathPattern mp2, CMC::MathPattern mp3, ConvFunc fn, CMC::ProcID prc >
        struct ChannelToChannel : public ChannelToChannel_REF<chCount, TS, cs, TD, cd, pix_ref>
        {

            const static A32U nPIX_SSE = M_MIN(  PIX_COUNT_FE2( TS, cs ), 
                                                PIX_COUNT_FE2( TD, cd  ) );
            
            class DST:  public RegDesc< TD, cd, nPIX_SSE > {};         
            class SRC1: public RegDesc< TS, cs, nPIX_SSE > {};

            ChannelToChannel() {}
            FE_SSE2_REF
		    IV SSE2_Init() {}
            IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
            {
                CMC::P_Model<mp1, mp2, mp3, fn, prc, chCount>::Process( r );
            }      
        };

        template< CMC::ChCount chCount, class TS, CH cs, class TD, CH cd, PIX_FN2_REF >
        struct PlanarToChannel_REF : public fe4< TS, cs, TS, cs, TS, cs, TD, cd >
        {
            PlanarToChannel_REF() {}
            FE_REF

            IV REFR(const TS *src1, const TS *src2, const TS *src3, TD *dst) const 
            {
                TS src[3] = { *src1, *src2, *src3 };
                pix_ref(src, C3, dst, cd);
            }
        };

        template< CMC::ChCount chCount, class TS, CH cs, class TD, CH cd, PIX_FN2_REF, 
                  CMC::MathPattern mp1, CMC::MathPattern mp2, CMC::MathPattern mp3, ConvFunc fn, CMC::ProcID prc >
        struct PlanarToChannel : public PlanarToChannel_REF<chCount, TS, cs, TD, cd, pix_ref>
        {
            PlanarToChannel() {}
            FE_SSE2_REF
		    IV SSE2_Init() {}
            IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
            {
                CMC::P_Model<mp1, mp2, mp3, fn, prc, chCount>::Process( r );
            }      
        };


        template< CMC::ChCount chCount, class TS, CH cs, class TD, CH cd, PIX_FN2_REF >
        struct Channel3ToPlanar3_REF : public fe1S3D< TS, cs, TD, cd, TD, cd, TD, cd >
        {
            Channel3ToPlanar3_REF() {}
            FE_REF

            IV REFR(const TS *src, TD *dst1, TD *dst2, TD *dst3 ) const 
            {
                TD dst[3];
                pix_ref(src, C3, dst, C3);
                *dst1 = dst[0];
                *dst2 = dst[1];
                *dst3 = dst[2];
            }
        };

        template< CMC::ChCount chCount, class TS, CH cs, class TD, CH cd, PIX_FN2_REF, 
                  CMC::MathPattern mp1, CMC::MathPattern mp2, CMC::MathPattern mp3, ConvFunc fn, CMC::ProcID prc >
        struct Channel3ToPlanar3 : public Channel3ToPlanar3_REF<chCount, TS, cs, TD, cd, pix_ref>
        {
            Channel3ToPlanar3() {}
            FE_SSE2_REF
		    IV SSE2_Init() {}
            IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
            {
                CMC::P_Model<mp1, mp2, mp3, fn, prc, chCount>::Process( r );
            }      
        };


        template< CMC::ChCount chCount, class TS, CH cs, class TD, CH cd, PIX_FN2_REF >
        struct Planar3ToPlanar3_REF : public fe3S3D< TS, cs, TS, cs, TS, cs, TD, cd, TD, cd, TD, cd >
        {
            Planar3ToPlanar3_REF() {}
            FE_REF

            IV REFR(const TS *src1, const TS *src2, const TS *src3, TD *dst1, TD *dst2, TD *dst3 ) const 
            {
                TS src[3] = { *src1, *src2, *src3 };
                TD dst[3];
                pix_ref(src, C3, dst, C3);
                *dst1 = dst[0];
                *dst2 = dst[1];
                *dst3 = dst[2];
            }
        };

        template< CMC::ChCount chCount, class TS, CH cs, class TD, CH cd, PIX_FN2_REF, 
                  CMC::MathPattern mp1, CMC::MathPattern mp2, CMC::MathPattern mp3, ConvFunc fn, CMC::ProcID prc >
        struct Planar3ToPlanar3 : public Planar3ToPlanar3_REF<chCount, TS, cs, TD, cd, pix_ref>
        {
            Planar3ToPlanar3() {}
            FE_SSE2_REF
		    IV SSE2_Init() {}
            IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
            {
                CMC::P_Model<mp1, mp2, mp3, fn, prc, chCount>::Process( r );
            }      
        };
    }; // NoChromaSampling
}; // OPT_LEVEL

#endif // __COLOR_MODEL_CONVERSION_DEFS_H__
