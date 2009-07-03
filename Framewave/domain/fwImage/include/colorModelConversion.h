/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __COLORMODELCONVERSION_H__
#define __COLORMODELCONVERSION_H__

#include "colorModelConvConst.h"
#include "Constants.h"
#include "fe.h"

#define HLSRGMac(a,h,m1,m2)  if (h < 60) { a = m1 + (m2 - m1)*h / 60; }                 \
                             else if (h < 180) { a = m2;                              } \
                             else if (h < 240) { a = m1 + (m2 - m1) * (240 - h) / 60; } \
                             else { a = m1; }

#define HLSBMac(a,h,m1,m2)   if (h < 0) h += 360;                                       \
                             if (h < 60) { a = m1 + (m2 - m1)*h / 60; }                 \
                             else if (h < 180) { a = m2;                              } \
                             else if (h < 240) { a = m1 + (m2 - m1) * (240 - h) / 60; } \
                             else { a = m1; }


extern const CMCDat CMCConstants[CF_MAXConv];

namespace OPT_LEVEL
{
    //////////////////////////////
    // Conversion Functions ( NO SAMPLING )
    //////////////////////////////

    //----------------------------
    // RGB/BGR -> YUV
    //----------------------------
    template< class TS, class TD >  
    SYS_INLINE void RGBToYUV( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;
        switch( cd )
        {
        case C4:
        case C3:    
        case AC4: 
          d[2] = (TD)FW_REF::Limits<TD>::Sat( (((A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YUV_VR )*(A32U)s[0]) -
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YUV_VG )*(A32U)s[1]) - 
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YUV_VB )*(A32U)s[2])) >> 16) +
                                                CONST_RGB2YUVaddV );
        case C2:  
          d[1] = (TD)FW_REF::Limits<TD>::Sat( (((A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YUV_UB )*(A32U)s[2]) - 
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YUV_UR )*(A32U)s[0]) - 
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YUV_UG )*(A32U)s[1])) >> 16) +
                                               CONST_RGB2YUVaddU );
        case C1:  
          d[0] = (TD)FW_REF::Limits<TD>::Sat( (((A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YUV_YR )*(A32U)s[0]) + 
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YUV_YG )*(A32U)s[1]) +
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YUV_YB )*(A32U)s[2])) >> 16) +
                                                CONST_RGB2YUVaddY );
        }
    }

    template< class TS, class TD >  
    SYS_INLINE void BGRToYUV( const TS *s, CH cs, TD *d, CH cd )
    {
        TS tempDat[3] = { s[2], s[1], s[0] };  // B, G, R
        RGBToYUV( tempDat, cs, d, cd );
    }

    //----------------------------
    // YUV -> RGB/BGR
    //----------------------------
    template< class TS, class TD >  
    SYS_INLINE void YUVToRGB( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;
        switch( cd )
        {
        case C4:
        case C3:
        case AC4: 
          d[2] = (TD)FW_REF::Limits<TD>::Sat( s[0] +  
                                               ((A32S)(FW_REF::Float2FixSh16_32U( CONST_YUV2RGB_BU )*((A32S)s[1] - CONST_YUV2RGBsubU))  >> 16 ) );
        case C2:  
          d[1] = (TD)FW_REF::Limits<TD>::Sat( s[0] - 
                                               (((A32S)(FW_REF::Float2FixSh16_32U( CONST_YUV2RGB_GU )*((A32S)s[1] - CONST_YUV2RGBsubU)) + 
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_YUV2RGB_GV )*((A32S)s[2] - CONST_YUV2RGBsubV))) >> 16 ) );
        case C1:  
          d[0] = (TD)FW_REF::Limits<TD>::Sat( s[0] +
                                               ((A32S)(FW_REF::Float2FixSh16_32U( CONST_YUV2RGB_RV )*((A32S)s[2] - CONST_YUV2RGBsubV))  >> 16 ) ); 
        }
    }

    template< class TS, class TD >  
    SYS_INLINE void YUVToBGR( const TS *s, CH cs, TD *d, CH cd )
    {
        TD rgb[3];
        YUVToRGB( s, cs, rgb, cd);

        d[0] = rgb[2]; // B
        d[1] = rgb[1]; // G
        d[2] = rgb[0]; // R
    }

    //----------------------------
    // RGB/BGR -> YCbCr
    //----------------------------
    template< class TS, class TD >  
    SYS_INLINE void RGBToYCbCr( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;

        switch( cd )
        {
        case C4:
        case C3:                                                        
        case AC4: 
          d[2] = (TD)FW_REF::Limits<TD>::Sat( (((A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YCbCr_CrR )*(A32U)s[0]) -
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YCbCr_CrG )*(A32U)s[1]) -
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YCbCr_CrB )*(A32U)s[2])) >> 16) + 
                                                 CONST_RGB2YCbCraddCr ); // R' - G' - B' + 128
        case C2:  
          d[1] = (TD)FW_REF::Limits<TD>::Sat( (((A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YCbCr_CbB )*(A32U)s[2]) -
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YCbCr_CbR )*(A32U)s[0]) -
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YCbCr_CbG )*(A32U)s[1])) >> 16) + 
                                                 CONST_RGB2YCbCraddCb ); // B' - R' - G' + 128
        case C1:  
          d[0] = (TD)FW_REF::Limits<TD>::Sat( (((A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YCbCr_YR  )*(A32U)s[0]) + 
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YCbCr_YG  )*(A32U)s[1]) + 
                                                 (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2YCbCr_YB  )*(A32U)s[2])) >> 16) + 
                                                 CONST_RGB2YCbCraddY );  // R' + G' + B' + 16
        }
    }
    template< class TS, class TD >  
    SYS_INLINE void RGBToYCbCrGamma( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;
        TD rgb[3];
        rgb[0] = FW_REF::GammaCorrect<TD,TD>(s[0]);
        rgb[1] = FW_REF::GammaCorrect<TD,TD>(s[1]);
        rgb[2] = FW_REF::GammaCorrect<TD,TD>(s[2]);
        RGBToYCbCr (rgb, cs, d, cd);
    }
    template< class TS, class TD >  
    SYS_INLINE void BGRToYCbCr( const TS *s, CH cs, TD *d, CH cd )
    {
        TS tempDat[3] = { s[2], s[1], s[0] }; // B, G, R
        RGBToYCbCr( tempDat, cs, d, cd );
    }

    //----------------------------
    // YCbCr -> RGB/BGR
    //----------------------------
    template< class TS, class TD >  
    SYS_INLINE void YCbCrToRGB( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) );

        switch( cd )
        {
        case C4:
        case C3:
        case AC4: 
          d[2] = (TD)FW_REF::Limits<TD>::Sat( ((A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_BY )*(A32S)(s[0] - CONST_YCbCr2RGBsubY )) + 
                                                (A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_BCb)*(A32S)(s[1] - CONST_YCbCr2RGBsubCb))) >> 16 );
        case C2:  
          d[1] = (TD)FW_REF::Limits<TD>::Sat( ((A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_GY )*(A32S)(s[0] - CONST_YCbCr2RGBsubY )) - 
                                                (A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_GCb)*(A32S)(s[1] - CONST_YCbCr2RGBsubCb)) - 
                                                (A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_GCr)*(A32S)(s[2] - CONST_YCbCr2RGBsubCr))) >> 16 );
        case C1:  
          d[0] = (TD)FW_REF::Limits<TD>::Sat( ((A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_RY )*(A32S)(s[0] - CONST_YCbCr2RGBsubY )) + 
                                                (A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_RCr)*(A32S)(s[2] - CONST_YCbCr2RGBsubCr))) >> 16 );
        }
    }

    template< class TS, class TD >  
    SYS_INLINE void YCbCrToRGBGamma( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;

        switch( cd )
        {
        case C4:
        case C3:
        case AC4: 
          d[2] = FW_REF::GammaCorrect<A8U,A8U>((TD)FW_REF::Limits<TD>::Sat( ((A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_BY  )*(A32S)(s[0] - CONST_YCbCr2RGBsubY )) + 
                                                                               (A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_BCb )*(A32S)(s[1] - CONST_YCbCr2RGBsubCb))) >> 16 ));
        case C2:  
          d[1] = FW_REF::GammaCorrect<A8U,A8U>((TD)FW_REF::Limits<TD>::Sat( ((A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_GY  )*(A32S)(s[0] - CONST_YCbCr2RGBsubY )) - 
                                                                               (A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_GCb )*(A32S)(s[1] - CONST_YCbCr2RGBsubCb)) - 
                                                                               (A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_GCr )*(A32S)(s[2] - CONST_YCbCr2RGBsubCr))) >> 16 ));
        case C1:  
          d[0] = FW_REF::GammaCorrect<A8U,A8U>((TD)FW_REF::Limits<TD>::Sat( ((A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_RY  )*(A32S)(s[0] - CONST_YCbCr2RGBsubY )) + 
                                                                               (A32S)(FW_REF::Float2FixSh16_32U( CONST_YCbCr2RGB_RCr )*(A32S)(s[2] - CONST_YCbCr2RGBsubCr))) >> 16 ));
        }
    }

    template< class TS, class TD >  
    SYS_INLINE void YCbCrToBGR( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;
        TD rgb[3];
        YCbCrToRGB (s, cs, rgb, cs);

        d[0] = rgb[2]; // B
        d[1] = rgb[1]; // G
        d[2] = rgb[0]; // R
    }

    template< class TS, class TD >  
    SYS_INLINE void YCbCrToRGB565( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;
        Fw8u tempDat[3];

        YCbCrToRGB( s, cs, tempDat, cs );
        *((A16U *)d) = FW_REF::Pack565<A8U,A16U>( tempDat[0], tempDat[1], tempDat[2] ); // R, G, B
    }
    SYS_INLINE void YCbCrToRGB565_8u16u( const Fw8u *s, CH cs, A16U *d, CH cd ) 
    {
        YCbCrToRGB565(s, cs, d, cd);
    }

    template< class TS, class TD >  
    SYS_INLINE void YCbCrToRGB555( const TS *s, CH cs, TD *d, CH cd )
    {
        Fw8u tempDat[3];
        cs;
        cd;

        YCbCrToRGB( s, cs, tempDat, cs );
        *((A16U *)d) = FW_REF::Pack555<A8U,A16U>( tempDat[0], tempDat[1], tempDat[2] ); // R, G, B
    }
    SYS_INLINE void YCbCrToRGB555_8u16u( const Fw8u *s, CH cs, A16U *d, CH cd ) 
    {
        YCbCrToRGB555(s, cs, d, cd);
    }

    template< class TS, class TD >  
    SYS_INLINE void YCbCrToRGB444( const TS *s, CH cs, TD *d, CH cd )
    {
        Fw8u tempDat[3];
        cs;
        cd;

        YCbCrToRGB( s, cs, tempDat, cs );
        *((A16U *)d) = FW_REF::Pack444<A8U,A16U>( tempDat[0], tempDat[1], tempDat[2] ); // R, G, B
    }
    SYS_INLINE void YCbCrToRGB444_8u16u( const Fw8u *s, CH cs, A16U *d, CH cd ) 
    {
        YCbCrToRGB444(s, cs, d, cd);
    }
    
    template< class TS, class TD >  
    SYS_INLINE void YCbCrToBGR565( const TS *s, CH cs, TD *d, CH cd )
    {
        Fw8u tempDat[3];
        cs;
        cd;

        YCbCrToRGB( s, cs, tempDat, cs );
        ((A16U *)d)[0] = FW_REF::Pack565<A8U,A16U>( tempDat[2], tempDat[1], tempDat[0] ); // B, G, R
    }

    SYS_INLINE void YCbCrToBGR565_8u16u( const Fw8u *s, CH cs, A16U *d, CH cd ) 
    {
        YCbCrToBGR565(s, cs, d, cd);
    }

    template< class TS, class TD >  
    SYS_INLINE void YCbCrToBGR555( const TS *s, CH cs, TD *d, CH cd )
    {
        Fw8u tempDat[3];
        cs;
        cd;

        YCbCrToRGB( s, cs, tempDat, cs );
        ((A16U *)d)[0] = FW_REF::Pack555<A8U,A16U>( tempDat[2], tempDat[1], tempDat[0] ); // B, G, R
    }

    SYS_INLINE void YCbCrToBGR555_8u16u( const Fw8u *s, CH cs, A16U *d, CH cd ) 
    {
        YCbCrToBGR555(s, cs, d, cd);
    }

    template< class TS, class TD >  
    SYS_INLINE void YCbCrToBGR444( const TS *s, CH cs, TD *d, CH cd )
    {
        Fw8u tempDat[3];
        cs;
        cd;

        YCbCrToRGB( s, cs, tempDat, cs );
        ((A16U *)d)[0] = FW_REF::Pack444<A8U,A16U>( tempDat[2], tempDat[1], tempDat[0] ); // B, G, R
    }

    SYS_INLINE void YCbCrToBGR444_8u16u( const Fw8u *s, CH cs, A16U *d, CH cd ) 
    {
        YCbCrToBGR444(s, cs, d, cd);
    }

    //----------------------------
    // RGB/BGR -> XYZ
    //----------------------------
    // RGB To XYZ Struct to process varying TS-TD datatypes differently
    template <class TS, class TD>
    struct RGB_XYZ
    {
        SYS_INLINE static void Process( const TS *, CH , TD *, CH  ) { ASSERT(false); }
    };

    template <>
    struct RGB_XYZ <Fw8u, Fw8u>
    {
        SYS_INLINE static void Process( const Fw8u *s, CH cs, Fw8u *d, CH cd )
        {
            cs;
        cd;
            switch( cd )
            {
            case C4:
            case C3:
            case AC4:
              d[2] = (A8U)FW_REF::Limits<Fw8u>::Sat( ((A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_ZR )*(A32U)s[0]) + 
                                                        (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_ZG )*(A32U)s[1]) + 
                                                        (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_ZB )*(A32U)s[2])) >> 16 );
            case C2:  
              d[1] = (A8U)FW_REF::Limits<Fw8u>::Sat( ((A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_YR )*(A32U)s[0]) + 
                                                        (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_YG )*(A32U)s[1]) + 
                                                        (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_YB )*(A32U)s[2])) >> 16 );
            case C1:  
              d[0] = (A8U)FW_REF::Limits<Fw8u>::Sat( ((A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_XR )*(A32U)s[0]) + 
                                                        (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_XG )*(A32U)s[1]) + 
                                                        (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_XB )*(A32U)s[2])) >> 16 );
            }
        }
    };

    template <>
    struct RGB_XYZ <Fw16s, Fw16s>
    {
        SYS_INLINE static void Process( const Fw16s *s, CH cs, Fw16s *d, CH cd )
        {
            cs;
        cd;
            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A16S)FW_REF::Limits<Fw16s>::Sat( (((A32S)(FW_REF::Float2Fix_32U( CONST_RGB2XYZ_ZR, 32768 )*(A32S)s[0]) + 
                                                           (A32S)(FW_REF::Float2Fix_32U( CONST_RGB2XYZ_ZG, 32768 )*(A32S)s[1]) + 
                                                           (A32S)(FW_REF::Float2Fix_32U( CONST_RGB2XYZ_ZB, 32768 )*(A32S)s[2])) >> 15) + CONST_RGB2XYZOffsetZ );
            case C2: 
              d[1] = (A16S)FW_REF::Limits<Fw16s>::Sat( (((A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_YR )*(A32S)s[0]) + 
                                                           (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_YG )*(A32S)s[1]) + 
                                                           (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_YB )*(A32S)s[2])) >> 16) + CONST_RGB2XYZOffsetY );
            case C1:  
              d[0] = (A16S)FW_REF::Limits<Fw16s>::Sat( (((A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_XR )*(A32S)s[0]) + 
                                                           (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_XG )*(A32S)s[1]) + 
                                                           (A32S)(FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_XB )*(A32S)s[2])) >> 16) + CONST_RGB2XYZOffsetX );
            }
        }
    };

    template <>
    struct RGB_XYZ <A16U, A16U>
    {
        SYS_INLINE static void Process( const A16U * s, CH cs, A16U * d, CH cd )
        {
            cs;
        cd;
            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A16U)FW_REF::Limits<A16U>::Sat( ((FW_REF::Float2Fix_32U( CONST_RGB2XYZ_ZR, 32768 )*(A32U)s[0]) + 
                                                        (FW_REF::Float2Fix_32U( CONST_RGB2XYZ_ZG, 32768 )*(A32U)s[1]) + 
                                                        (FW_REF::Float2Fix_32U( CONST_RGB2XYZ_ZB, 32768 )*(A32U)s[2])) >> 15 );
            case C2:  
              d[1] = (A16U)FW_REF::Limits<A16U>::Sat( ((FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_YR )*(A32U)s[0]) + 
                                                        (FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_YG )*(A32U)s[1]) + 
                                                        (FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_YB )*(A32U)s[2])) >> 16 );
            case C1:  
              d[0] = (A16U)FW_REF::Limits<A16U>::Sat( ((FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_XR )*(A32U)s[0]) + 
                                                        (FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_XG )*(A32U)s[1]) + 
                                                        (FW_REF::Float2FixSh16_32U( CONST_RGB2XYZ_XB )*(A32U)s[2])) >> 16 );
            }
        }
    };

    template <>
    struct RGB_XYZ <A32S, A32S>
    {
        SYS_INLINE static void Process( const A32S * s, CH cs, A32S * d, CH cd )
        {
            cs;
        cd;
            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A32S)FW_REF::Limits<A32S>::Sat( ((A32S)( CONST_RGB2XYZ_ZR *s[0]) + 
                                                        (A32S)( CONST_RGB2XYZ_ZG *s[1]) + 
                                                        (A32S)( CONST_RGB2XYZ_ZB *s[2])) + 
                                                       CONST_RGB2XYZOffsetX );
            case C2:  
              d[1] = (A32S)FW_REF::Limits<A32S>::Sat( ((A32S)( CONST_RGB2XYZ_YR *s[0]) + 
                                                        (A32S)( CONST_RGB2XYZ_YG *s[1]) + 
                                                        (A32S)( CONST_RGB2XYZ_YB *s[2])) + 
                                                       CONST_RGB2XYZOffsetY );
            case C1:  
              d[0] = (A32S)FW_REF::Limits<A32S>::Sat( ((A32S)( CONST_RGB2XYZ_XR *s[0]) + 
                                                        (A32S)( CONST_RGB2XYZ_XG *s[1]) + 
                                                        (A32S)( CONST_RGB2XYZ_XB *s[2])) + 
                                                       CONST_RGB2XYZOffsetZ );
            }
        }
    };

    template <>
    struct RGB_XYZ <A32U, A32U>
    {
        SYS_INLINE static void Process( const A32U * s, CH cs, A32U * d, CH cd )
        {
            cs;
        cd;
            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A32U)FW_REF::Limits<A32U>::Sat( (A32U)( CONST_RGB2XYZ_ZR *s[0]) + 
                                                       (A32U)( CONST_RGB2XYZ_ZG *s[1]) + 
                                                       (A32U)( CONST_RGB2XYZ_ZB *s[2]) );
            case C2:  
              d[1] = (A32U)FW_REF::Limits<A32U>::Sat( (A32U)( CONST_RGB2XYZ_YR *s[0]) + 
                                                       (A32U)( CONST_RGB2XYZ_YG *s[1]) + 
                                                       (A32U)( CONST_RGB2XYZ_YB *s[2]) );
            case C1:  
              d[0] = (A32U)FW_REF::Limits<A32U>::Sat( (A32U)( CONST_RGB2XYZ_XR *s[0]) + 
                                                       (A32U)( CONST_RGB2XYZ_XG *s[1]) + 
                                                       (A32U)( CONST_RGB2XYZ_XB *s[2]) );
            }
        }
    };

    template <>
    struct RGB_XYZ <A32F, A32F>
    {
        SYS_INLINE static void Process( const A32F * s, CH cs, A32F * d, CH cd )
        {
            cs;
        cd;

            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A32F)FW_REF::Limits<A32F>::Sat( CONST_RGB2XYZ_ZR*s[0] + 
                                                       CONST_RGB2XYZ_ZG*s[1] + 
                                                       CONST_RGB2XYZ_ZB*s[2] );
            case C2:  
              d[1] = (A32F)FW_REF::Limits<A32F>::Sat( CONST_RGB2XYZ_YR*s[0] + 
                                                       CONST_RGB2XYZ_YG*s[1] + 
                                                       CONST_RGB2XYZ_YB*s[2] );
            case C1:  
              d[0] = (A32F)FW_REF::Limits<A32F>::Sat( CONST_RGB2XYZ_XR*s[0] + 
                                                       CONST_RGB2XYZ_XG*s[1] + 
                                                       CONST_RGB2XYZ_XB*s[2] );
            }
        }
    };

    template <>
    struct RGB_XYZ <A64F, A64F>
    {
        SYS_INLINE static void Process( const A64F * s, CH cs, A64F * d, CH cd )
        {
            cs;
        cd;
            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A64F)( CONST_RGB2XYZ_ZR*s[0] + CONST_RGB2XYZ_ZG*s[1] + CONST_RGB2XYZ_ZB*s[2]);
            case C2:  
              d[1] = (A64F)( CONST_RGB2XYZ_YR*s[0] + CONST_RGB2XYZ_YG*s[1] + CONST_RGB2XYZ_YB*s[2]);
            case C1:  
              d[0] = (A64F)( CONST_RGB2XYZ_XR*s[0] + CONST_RGB2XYZ_XG*s[1] + CONST_RGB2XYZ_XB*s[2]);
            }
        }
    };

    template< class TS, class TD > 
    SYS_INLINE void RGBToXYZ( const TS *s, CH cs, TD *d, CH cd )
    {
        RGB_XYZ <TS, TD>:: Process (s, cs, d, cd);
    }

    SYS_INLINE void RGBToXYZ_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        RGB_XYZ<Fw8u,Fw8u>::Process(s, cs, d, cd);
    }

    SYS_INLINE void RGBToXYZ_16s16s( const Fw16s *s, CH cs, Fw16s *d, CH cd )
    {
        RGB_XYZ<Fw16s,Fw16s>::Process(s, cs, d, cd);
    }
    SYS_INLINE void RGBToXYZ_16u16u( const Fw16u *s, CH cs, Fw16u *d, CH cd )
    {
        RGB_XYZ<Fw16u,Fw16u>::Process(s, cs, d, cd);
    }
    SYS_INLINE void RGBToXYZ_32f32f( const Fw32f *s, CH cs, Fw32f *d, CH cd )
    {
        RGB_XYZ<Fw32f,Fw32f>::Process(s, cs, d, cd);
    }



    //----------------------------
    // XYZ -> RGB
    //----------------------------
    // XYZ To RGB Struct to process varying TS-TD datatypes differently
    template <class TS, class TD>
    struct XYZ_RGB
    {
        SYS_INLINE static void Process( const TS *, CH , TD *, CH  ) { ASSERT(false); }
    };

    template <>
    struct XYZ_RGB <Fw8u, Fw8u>
    {
        SYS_INLINE static void Process( const Fw8u *s, CH cs, Fw8u *d, CH cd )
        {
            cs;
        cd;
            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A8U)FW_REF::Limits<Fw8u>::Sat(( (A32S)(FW_REF::Float2FixSh13( CONST_XYZ2RGB_BX )*(A32S)(s[0])) - 
                                                        (A32S)(FW_REF::Float2FixSh13( CONST_XYZ2RGB_BY )*(A32S)(s[1])) + 
                                                        (A32S)(FW_REF::Float2FixSh13( CONST_XYZ2RGB_BZ )*(A32S)(s[2]))
                                                      ) >> 13);
            case C2:  
              d[1] = (A8U)FW_REF::Limits<Fw8u>::Sat((-(A32S)(FW_REF::Float2FixSh13( CONST_XYZ2RGB_GX )*(A32S)(s[0])) + 
                                                        (A32S)(FW_REF::Float2FixSh13( CONST_XYZ2RGB_GY )*(A32S)(s[1])) + 
                                                        (A32S)(FW_REF::Float2FixSh13( CONST_XYZ2RGB_GZ )*(A32S)(s[2]))
                                                      ) >> 13);
            case C1:  
              d[0] = (A8U)FW_REF::Limits<Fw8u>::Sat(( (A32S)(FW_REF::Float2FixSh13( CONST_XYZ2RGB_RX )*(A32S)(s[0])) - 
                                                        (A32S)(FW_REF::Float2FixSh13( CONST_XYZ2RGB_RY )*(A32S)(s[1])) - 
                                                        (A32S)(FW_REF::Float2FixSh13( CONST_XYZ2RGB_RZ )*(A32S)(s[2]))
                                                      ) >> 13);
            }
        }
    };

    template <>
    struct XYZ_RGB <Fw16s, Fw16s>
    {
        SYS_INLINE static void Process( const Fw16s *s, CH cs, Fw16s *d, CH cd )
        {
            cs;
        cd;
            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A16S)FW_REF::Limits<Fw16s>::Sat( (((A32S)(FW_REF::Float2Fix_32U( CONST_XYZ2RGB_BX, 32768 )*(A32S)s[0]) - 
                                                           (A32S)(FW_REF::Float2Fix_32U( CONST_XYZ2RGB_BY, 32768 )*(A32S)s[1]) + 
                                                           (A32S)(FW_REF::Float2Fix_32U( CONST_XYZ2RGB_BZ, 32768 )*(A32S)s[2])) >> 15) + CONST_XYZ2RGBOffsetB); // Constants to avoid actually converting 16S to [0..1]
            case C2:  
              d[1] = (A16S)FW_REF::Limits<Fw16s>::Sat( (((A32S)(FW_REF::Float2Fix_32U( CONST_XYZ2RGB_GY, 8192 )*(A32S)s[1]) -
                                                           (A32S)(FW_REF::Float2Fix_32U( CONST_XYZ2RGB_GX, 8192 )*(A32S)s[0]) + 
                                                           (A32S)(FW_REF::Float2Fix_32U( CONST_XYZ2RGB_GZ, 8192 )*(A32S)s[2])) >> 13) + CONST_XYZ2RGBOffsetG); // Constants to avoid actually converting 16S to [0..1]
            case C1:  
              d[0] = (A16S)FW_REF::Limits<Fw16s>::Sat( (((A32S)(FW_REF::Float2Fix_32U( CONST_XYZ2RGB_RX, 8192 )*(A32S)s[0]) - 
                                                           (A32S)(FW_REF::Float2Fix_32U( CONST_XYZ2RGB_RY, 8192 )*(A32S)s[1]) - 
                                                           (A32S)(FW_REF::Float2Fix_32U( CONST_XYZ2RGB_RZ, 8192 )*(A32S)s[2])) >> 13) + CONST_XYZ2RGBOffsetR); // Constants to avoid actually converting 16S to [0..1]
            }
        }
    };

    template <>
    struct XYZ_RGB <A16U, A16U>
    {
        SYS_INLINE static void Process( const A16U * s, CH cs, A16U * d, CH cd )
        {
            cs;
        cd;
            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A16U)FW_REF::Limits<A16U>::Sat( CONST_XYZ2RGB_BX*((A32S)s[0]) - 
                                                       CONST_XYZ2RGB_BY*((A32S)s[1]) + 
                                                       CONST_XYZ2RGB_BZ*((A32S)s[2]) );
            case C2:  
              d[1] = (A16U)FW_REF::Limits<A16U>::Sat(-CONST_XYZ2RGB_GX*((A32S)s[0]) + 
                                                       CONST_XYZ2RGB_GY*((A32S)s[1]) + 
                                                       CONST_XYZ2RGB_GZ*((A32S)s[2]) );
            case C1:  
              d[0] = (A16U)FW_REF::Limits<A16U>::Sat( CONST_XYZ2RGB_RX*((A32S)s[0]) - 
                                                       CONST_XYZ2RGB_RY*((A32S)s[1]) - 
                                                       CONST_XYZ2RGB_RZ*((A32S)s[2]) );
            }
        }
    };

    template <>
    struct XYZ_RGB <A32S, A32S>
    {
        SYS_INLINE static void Process( const A32S * s, CH cs, A32S * d, CH cd )
        {
            cs;
        cd;
            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A32S)FW_REF::Limits<A32S>::Sat( CONST_XYZ2RGB_BX*((A32S)s[0]) - 
                                                       CONST_XYZ2RGB_BY*((A32S)s[1]) + 
                                                       CONST_XYZ2RGB_BZ*((A32S)s[2]) );
            case C2:  
              d[1] = (A32S)FW_REF::Limits<A32S>::Sat(-CONST_XYZ2RGB_GX*((A32S)s[0]) + 
                                                       CONST_XYZ2RGB_GY*((A32S)s[1]) + 
                                                       CONST_XYZ2RGB_GZ*((A32S)s[2]) );
            case C1:  
              d[0] = (A32S)FW_REF::Limits<A32S>::Sat( CONST_XYZ2RGB_RX*((A32S)s[0]) - 
                                                       CONST_XYZ2RGB_RY*((A32S)s[1]) - 
                                                       CONST_XYZ2RGB_RZ*((A32S)s[2]) );
            }
        }
    };

    template <>
    struct XYZ_RGB <A32U, A32U>
    {
        SYS_INLINE static void Process( const A32U * s, CH cs, A32U * d, CH cd )
        {
            cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) );

            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A32U)FW_REF::Limits<A32U>::Sat( CONST_XYZ2RGB_BX*((A32S)s[0]) - 
                                                       CONST_XYZ2RGB_BY*((A32S)s[1]) + 
                                                       CONST_XYZ2RGB_BZ*((A32S)s[2]) );
            case C2:  
              d[1] = (A32U)FW_REF::Limits<A32U>::Sat(-CONST_XYZ2RGB_GX*((A32S)s[0]) + 
                                                       CONST_XYZ2RGB_GY*((A32S)s[1]) + 
                                                       CONST_XYZ2RGB_GZ*((A32S)s[2]) );
            case C1:  
              d[0] = (A32U)FW_REF::Limits<A32U>::Sat( CONST_XYZ2RGB_RX*((A32S)s[0]) - 
                                                       CONST_XYZ2RGB_RY*((A32S)s[1]) - 
                                                       CONST_XYZ2RGB_RZ*((A32S)s[2]) );
            }
        }
    };

    template <>
    struct XYZ_RGB <A32F, A32F>
    {
        SYS_INLINE static void Process( const A32F * s, CH cs, A32F * d, CH cd )
        {
            cs;
        cd;
            switch( cd )
            {
            case C4:
            case C3:
            case AC4: 
              d[2] = (A32F)FW_REF::CLIP<A32F, A64F>( 0,  CONST_XYZ2RGB_BX*s[0] - 
                                                          CONST_XYZ2RGB_BY*s[1] + 
                                                          CONST_XYZ2RGB_BZ*s[2], 1 );
            case C2:  
              d[1] = (A32F)FW_REF::CLIP<A32F, A64F>( 0, -CONST_XYZ2RGB_GX*s[0] + 
                                                          CONST_XYZ2RGB_GY*s[1] + 
                                                          CONST_XYZ2RGB_GZ*s[2], 1 );
            case C1:  
              d[0] = (A32F)FW_REF::CLIP<A32F, A64F>( 0,  CONST_XYZ2RGB_RX*s[0] - 
                                                          CONST_XYZ2RGB_RY*s[1] - 
                                                          CONST_XYZ2RGB_RZ*s[2], 1 );
            }
        }
    };

    template <>
    struct XYZ_RGB <A64F, A64F>
    {
        SYS_INLINE static void Process( const A64F * s, CH cs, A64F * d, CH cd )
        {
            cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) );

            switch( cd )
            {
            case C4:
            case C3:
            case AC4:    
              d[2] = (A64F)( CONST_XYZ2RGB_BX*s[0] - CONST_XYZ2RGB_BY*s[1] + CONST_XYZ2RGB_BZ*s[2] );
            case C2:	
              d[1] = (A64F)(-CONST_XYZ2RGB_GX*s[0] + CONST_XYZ2RGB_GY*s[1] + CONST_XYZ2RGB_GZ*s[2] );
            case C1:	
              d[0] = (A64F)( CONST_XYZ2RGB_RX*s[0] - CONST_XYZ2RGB_RY*s[1] - CONST_XYZ2RGB_RZ*s[2] );
            }
        }
    };

    template< class TS, class TD >  
    SYS_INLINE void XYZToRGB( const TS *s, CH cs, TD *d, CH cd )
    {
        XYZ_RGB <TS, TD>:: Process (s, cs, d, cd);
    }
    SYS_INLINE void XYZToRGB_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        XYZ_RGB<Fw8u,Fw8u>::Process(s, cs, d, cd);
    }
    SYS_INLINE void XYZToRGB_16s16s( const Fw16s *s, CH cs, Fw16s *d, CH cd )
    {
        XYZ_RGB<Fw16s,Fw16s>::Process(s, cs, d, cd);
    }
    SYS_INLINE void XYZToRGB_16u16u( const Fw16u *s, CH cs, Fw16u *d, CH cd )
    {
        XYZ_RGB<Fw16u,Fw16u>::Process(s, cs, d, cd);
    }
    SYS_INLINE void XYZToRGB_32f32f( const Fw32f *s, CH cs, Fw32f *d, CH cd )
    {
        XYZ_RGB<Fw32f,Fw32f>::Process(s, cs, d, cd);
    }

    //----------------------------
    // RGB -> LUV
    //----------------------------
    template< class TS, class TD > 
    SYS_INLINE void RGBToLUV( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) );

        double varLUV[3];
        double varXYZ[3];

        // Scale data to [0..1]
        varLUV[0] = FW_REF::Normalize<TS,TS>::Scale( (double)s[0] );
        varLUV[1] = FW_REF::Normalize<TS,TS>::Scale( (double)s[1] );
        varLUV[2] = FW_REF::Normalize<TS,TS>::Scale( (double)s[2] );

        // Convert RGB -> XYZ
        RGBToXYZ<double, double>(varLUV, cs, varXYZ, cd);

        if (varXYZ[1] > 0.008856)
            varLUV[0] = 116.*pow(varXYZ[1], 1./3.) - 16.;
        else
            varLUV[0] = 903.3*varXYZ[1];

        if( varLUV[0] )
        {
            double constant = varXYZ[0] + 15.*varXYZ[1] + 3.*varXYZ[2];
            switch( cd )
            {
            case C4:
            case C3:
            case AC4:  
                d[2] = FW_REF::Normalize<TD,TD>::LUVNormalizeV( 13.*varLUV[0]*((9*varXYZ[1] / constant) - CONST_LUV_Vn) ); // V
            case C2:  
                d[1] = FW_REF::Normalize<TD,TD>::LUVNormalizeU( 13.*varLUV[0]*((4*varXYZ[0] / constant) - CONST_LUV_Un) ); // U
            case C1:  
                d[0] = FW_REF::Normalize<TD,TD>::LUVNormalizeL( varLUV[0] );    // L
            }
        }
        else
        {
            switch( cd )
            {
            case C4:
            case C3:
            case AC4:  
                d[2] = FW_REF::Normalize<TD,TD>::LUVNormalizeV( 0 ); // V
            case C2:  
                d[1] = FW_REF::Normalize<TD,TD>::LUVNormalizeU( 0 ); // U
            case C1:  
                d[0] = FW_REF::Normalize<TD,TD>::LUVNormalizeL( 0 ); // L
            }
        }
    }

    SYS_INLINE void RGBToLUV_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        RGBToLUV<Fw8u,Fw8u>( s, cs, d, cd );
    }
    SYS_INLINE void RGBToLUV_16s16s( const Fw16s *s, CH cs, Fw16s *d, CH cd )
    {
        RGBToLUV<Fw16s,Fw16s>( s, cs, d, cd );
    }
    SYS_INLINE void RGBToLUV_16u16u( const Fw16u *s, CH cs, Fw16u *d, CH cd )
    {
        RGBToLUV<Fw16u,Fw16u>( s, cs, d, cd );
    }
    SYS_INLINE void RGBToLUV_32f32f( const Fw32f *s, CH cs, Fw32f *d, CH cd )
    {
        RGBToLUV<Fw32f,Fw32f>( s, cs, d, cd );
    }

    //----------------------------
    // LUV -> RGB
    //----------------------------
    template< class TS, class TD > 
    SYS_INLINE void LUVToRGB( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) );

        double varLUV[3];
        // Tranform LUV back to original Range
        varLUV[0] = FW_REF::Normalize<TS,TS>::LUVUnNormalizeL( s[0] ); // Scale L [   0..100]
        varLUV[1] = FW_REF::Normalize<TS,TS>::LUVUnNormalizeU( s[1] ); // Scale U [-134..220]
        varLUV[2] = FW_REF::Normalize<TS,TS>::LUVUnNormalizeV( s[2] ); // Scale V [-140..116]

        // Transform to XYZ
        double xyz[3];
        double u = CONST_LUV_Un, 
            v = CONST_LUV_Vn;

        if (varLUV[0])
        {
            u += varLUV[1]/(13*varLUV[0]);
            v += varLUV[2]/(13*varLUV[0]);
        }

        // Y
        xyz[1] = pow( (varLUV[0]+16)/116, 3.);
        // X
        xyz[0] = (9./4.)*xyz[1]*u/v;
        // Z
        xyz[2] = (9.*xyz[1] - 15*v*xyz[1] - v*xyz[0])/(3.*v);
        XYZToRGB<double, double> (xyz, cs, varLUV, cd);
        // varLUV - Computed RGB values between 0..1

        switch( cd )
        {
        case C4:
        case C3:
        case AC4: 
              d[2] = FW_REF::Normalize<TD,TD>::Unscale( varLUV[2] ); // B
        case C2:  
              d[1] = FW_REF::Normalize<TD,TD>::Unscale( varLUV[1] ); // G
        case C1:  
              d[0] = FW_REF::Normalize<TD,TD>::Unscale( varLUV[0] ); // R
        }
    }

    SYS_INLINE void LUVToRGB_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        LUVToRGB<Fw8u,Fw8u>(s, cs, d, cd);
    }
    SYS_INLINE void LUVToRGB_16s16s( const Fw16s *s, CH cs, Fw16s *d, CH cd )
    {
        LUVToRGB<Fw16s,Fw16s>(s, cs, d, cd);
    }
    SYS_INLINE void LUVToRGB_16u16u( const Fw16u *s, CH cs, Fw16u *d, CH cd )
    {
        LUVToRGB<Fw16u,Fw16u>(s, cs, d, cd);
    }
    SYS_INLINE void LUVToRGB_32f32f( const Fw32f *s, CH cs, Fw32f *d, CH cd )
    {
        LUVToRGB<Fw32f,Fw32f>(s, cs, d, cd);
    }

    //----------------------------
    // BGR -> LAB
    //----------------------------
    template< class TS, class TD >  
    SYS_INLINE void BGRToLab( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) );

        double ns[3]; // new Source
        ns[0] = FW_REF::Normalize<TS,TS>::Scale( s[2] );
        ns[1] = FW_REF::Normalize<TS,TS>::Scale( s[1] );
        ns[2] = FW_REF::Normalize<TS,TS>::Scale( s[0] );
        double varXYZ[3];
        double tempVarXYZ[3];
        RGBToXYZ(ns, cs, varXYZ, cd);
        tempVarXYZ[0] = varXYZ[0] / CONST_Lab_Xn;
        tempVarXYZ[1] = varXYZ[1];
        tempVarXYZ[2] = varXYZ[2] / CONST_Lab_Zn;

        // Calculate f(x/xn)
        if (tempVarXYZ[0] > CONST_BGR2Lab_ftConst)
        {
            tempVarXYZ[0] = pow(tempVarXYZ[0], 1./3.);
        }
        else
        {
            tempVarXYZ[0] = (7.787037*tempVarXYZ[0] + 16./116.);
        }

        // Calculate f(y/yn)
        if (tempVarXYZ[1] > CONST_BGR2Lab_ftConst)
        {
            tempVarXYZ[1] = pow(tempVarXYZ[1], 1./3.);
        }
        else
        {
            tempVarXYZ[1] = (7.787037*tempVarXYZ[1] + 16./116.);
        }

        // Calculate f(z/zn)
        if (tempVarXYZ[2] > CONST_BGR2Lab_ftConst)
        {
            tempVarXYZ[2] = pow(tempVarXYZ[2], 1./3.);
        }
        else
        {
            tempVarXYZ[2] = (7.787037*tempVarXYZ[2] + 16./116.);
        }

        switch (cd)
        {
        case C4:
        case C3:
        case AC4: 
              d[2] = FW_REF::Normalize<TD, TD>::LabNormalizeb( 200.*(tempVarXYZ[1] - tempVarXYZ[2]) );    // b
        case C2:  
              d[1] = FW_REF::Normalize<TD, TD>::LabNormalizea( 500.*(tempVarXYZ[0] - tempVarXYZ[1]) );    // a
        case C1:  
              d[0] = FW_REF::Normalize<TD, TD>::LabNormalizeL( 116.*tempVarXYZ[1] - 16 );                 // L
        }
    }


    SYS_INLINE void BGRToLab_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        BGRToLab<Fw8u,Fw8u>(s, cs, d, cd);
    }
    SYS_INLINE void BGRToLab_8u16u( const Fw8u *s, CH cs, Fw16u *d, CH cd )
    {
        BGRToLab<Fw8u,Fw16u>(s, cs, d, cd);
    }
    //----------------------------
    // LAB -> BGR
    //----------------------------
    template< class TS, class TD >  
    SYS_INLINE void LabToBGR( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) ); 

        double ns[3]; // new Source
        double varXYZ[3];
        double varRGB[3];
        ns[0] = FW_REF::Normalize<TS,TS>::LabUnNormalizeL( s[0] );    // L = [   0 ... 100]
        ns[1] = FW_REF::Normalize<TS,TS>::LabUnNormalizea( s[1] );    // a = [-128 ... 127]
        ns[2] = FW_REF::Normalize<TS,TS>::LabUnNormalizeb( s[2] );    // b = [-128 ... 127]

        const double P = (ns[0] + 16) / 116.0;                        // P = [(16/116) ... 1]

        varXYZ[0] = (CONST_Lab_Xn * pow(P + (double)ns[1]/500., 3));
        varXYZ[1] = pow (P, 3);
        varXYZ[2] = (CONST_Lab_Zn * pow(P - (double)ns[2]/200., 3));

        XYZToRGB (varXYZ, cd, varRGB, cd);
        switch (cd)
        {
        case C4:
        case C3:
        case AC4: 
              d[2] = FW_REF::Normalize<TD, TD>::Unscale( varRGB[0] );
        case C2:  
              d[1] = FW_REF::Normalize<TD, TD>::Unscale( varRGB[1] );
        case C1:  
              d[0] = FW_REF::Normalize<TD, TD>::Unscale( varRGB[2] );
        }
    }

    SYS_INLINE void LabToBGR_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        LabToBGR<Fw8u,Fw8u>(s, cs, d, cd);
    }
    SYS_INLINE void LabToBGR_16u8u( const Fw16u *s, CH cs, Fw8u *d, CH cd )
    {
        LabToBGR<Fw16u,Fw8u>(s, cs, d, cd);
    }

    //----------------------------
    // RGB -> YCC
    //----------------------------
    template< class TS, class TD > 
    SYS_INLINE void RGBToYCC( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) );

        double rgb[3];
        rgb[0] = FW_REF::Normalize<TS,TS>::Scale( (double)s[0] );
        rgb[1] = FW_REF::Normalize<TS,TS>::Scale( (double)s[1] );
        rgb[2] = FW_REF::Normalize<TS,TS>::Scale( (double)s[2] );

        switch( cd )
        {
        case C4:
        case C3:
        case AC4: 
              d[2] = FW_REF::Normalize<TS,TD>::Unscale( CONST_RGB2YCC_C2Const * 
                                                         ( CONST_RGB2YCC_C2R*(double)rgb[0] - 
                                                           CONST_RGB2YCC_C2G*(double)rgb[1] - 
                                                           CONST_RGB2YCC_C2B*(double)rgb[2] ) + 
                                                         CONST_YCC_C2Off );                         // C2
        case C2:  
              d[1] = FW_REF::Normalize<TS,TD>::Unscale( CONST_RGB2YCC_C1Const * 
                                                         ( -CONST_RGB2YCC_C1R*(double)rgb[0] - 
                                                            CONST_RGB2YCC_C1G*(double)rgb[1] + 
                                                            CONST_RGB2YCC_C1B*(double)rgb[2] ) + 
                                                         CONST_YCC_C1Off );                         // C1
        case C1:  
              d[0] = FW_REF::Normalize<TS,TD>::Unscale( CONST_RGB2YCC_YConst  * 
                                                         (  CONST_RGB2YCC_YR *(double)rgb[0] + 
                                                            CONST_RGB2YCC_YG *(double)rgb[1] + 
                                                            CONST_RGB2YCC_YB*(double)rgb[2] ) );    // Y
        }
    }


    SYS_INLINE void RGBToYCC_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        RGBToYCC<Fw8u, Fw8u>(s, cs, d, cd);
    }
    SYS_INLINE void RGBToYCC_16s16s( const Fw16s *s, CH cs, Fw16s *d, CH cd )
    {
        RGBToYCC<Fw16s, Fw16s>(s, cs, d, cd);
    }
    SYS_INLINE void RGBToYCC_16u16u( const Fw16u *s, CH cs, Fw16u *d, CH cd )
    {
        RGBToYCC<Fw16u, Fw16u>(s, cs, d, cd);
    }
    SYS_INLINE void RGBToYCC_32f32f( const Fw32f *s, CH cs, Fw32f *d, CH cd )
    {
        RGBToYCC<Fw32f, Fw32f>(s, cs, d, cd);
    }

    //----------------------------
    // YCC -> RGB
    //----------------------------
    template< class TS, class TD >  
    SYS_INLINE void YCCToRGB( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) );

        double rgb[3] = { FW_REF::Normalize<TS,TS>::Scale ((double)s[0]),
                          FW_REF::Normalize<TS,TS>::Scale ((double)s[1]),
                          FW_REF::Normalize<TS,TS>::Scale ((double)s[2]) };

        double Y   = CONST_YCC2RGB_Y  *  (double)rgb[0];
        double CC1 = CONST_YCC2RGB_C1 *( (double)rgb[1] - CONST_YCC_C1Off );
        double CC2 = CONST_YCC2RGB_C2 *( (double)rgb[2] - CONST_YCC_C2Off );

        switch( cd )
        {
        case C4:
        case C3:
        case AC4: 
              d[2] = FW_REF::Normalize<TS,TD>::Unscale( Y + CC1 );    // B
        case C2:  
              d[1] = FW_REF::Normalize<TS,TD>::Unscale( Y - 
                                                         CONST_YCC2RGB_GC1*CC1 - 
                                                         CONST_YCC2RGB_GC2*CC2 );  // G
        case C1:  
              d[0] = FW_REF::Normalize<TS,TD>::Unscale( Y + CC2 );    // R
        }
    }

    SYS_INLINE void YCCToRGB_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        YCCToRGB<Fw8u, Fw8u>(s, cs, d, cd);
    }
    SYS_INLINE void YCCToRGB_16s16s( const Fw16s *s, CH cs, Fw16s *d, CH cd )
    {
        YCCToRGB<Fw16s, Fw16s>(s, cs, d, cd);
    }
    SYS_INLINE void YCCToRGB_16u16u( const Fw16u *s, CH cs, Fw16u *d, CH cd )
    {
        YCCToRGB<Fw16u, Fw16u>(s, cs, d, cd);
    }
    SYS_INLINE void YCCToRGB_32f32f( const Fw32f *s, CH cs, Fw32f *d, CH cd )
    {
        YCCToRGB<Fw32f, Fw32f>(s, cs, d, cd);
        switch( cd )
        {
            case C4:
            case C3:
            case AC4: 
              d[2] = FW_REF::CLIP<Fw32f, Fw32f>( 0, d[2], 1 ); // B
            case C2:  
              d[1] = FW_REF::CLIP<Fw32f, Fw32f>( 0, d[1], 1 ); // G
            case C1:  
              d[0] = FW_REF::CLIP<Fw32f, Fw32f>( 0, d[0], 1 ); // R
        }
    }
    //----------------------------
    // RGB/BGR -> HLS
    //----------------------------
    template< class TS, class TD >  
    SYS_INLINE void RGBToHLS( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) );

        double rgb[3];
        rgb[0] = FW_REF::Normalize<TS,TS>::Scale ((double)s[0]);
        rgb[1] = FW_REF::Normalize<TS,TS>::Scale ((double)s[1]);
        rgb[2] = FW_REF::Normalize<TS,TS>::Scale ((double)s[2]);

        double m1 = MAX( MAX( rgb[0], rgb[1] ), rgb[2] );
        double m2 = MIN( MIN( rgb[0], rgb[1] ), rgb[2] );
        double H = 0, L = 0, S = 0;
        // Lightness:
        L = (m1 + m2) / 2.0;
        // Saturation:
        if (m1 == m2)
        {
            S = H = 0;
        }
        else
        {
            if (L <= 0.5)
            {
                S = (m1 - m2) / (m1 + m2);
            }
            else
            {
                S = (m1 - m2) / (2 - m1 - m2);
            }

            // Hue
            double delta = m1 - m2;
            double    Cr = (m1 - rgb[0]) / delta,
                Cg = (m1 - rgb[1]) / delta,
                Cb = (m1 - rgb[2]) / delta;

            if (m1 == rgb[0]) // R
            {
                H = Cb - Cg;
            }
            if (m1 == rgb[1]) // G
            {
                H = 2 + Cr - Cb;
            }
            if (m1 == rgb[2]) // B
            {
                H = 4 + Cg - Cr;
            }

            H = H*60.;
            if (H < 0)
            {
                H += 360.0;
            }
        }

        switch( cd )
        {
        case C4:
        case C3:                                                        
        case AC4: 
              d[2] = FW_REF::Normalize<TS,TD>::Unscale( S );         // S
        case C2:  
              d[1] = FW_REF::Normalize<TS,TD>::Unscale( L );         // L
        case C1:  
              d[0] = FW_REF::Normalize<TS,TD>::Scale( H, 360.0 );    // H
        }
    }
    SYS_INLINE void RGBToHLS_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        RGBToHLS<Fw8u, Fw8u>(s, cs, d, cd);
    }
    SYS_INLINE void RGBToHLS_16s16s( const Fw16s *s, CH cs, Fw16s *d, CH cd )
    {
        RGBToHLS<Fw16s, Fw16s>(s, cs, d, cd);
    }
    SYS_INLINE void RGBToHLS_16u16u( const Fw16u *s, CH cs, Fw16u *d, CH cd )
    {
        RGBToHLS<Fw16u, Fw16u>(s, cs, d, cd);
    }
    SYS_INLINE void RGBToHLS_32f32f( const Fw32f *s, CH cs, Fw32f *d, CH cd )
    {
        RGBToHLS<Fw32f, Fw32f>(s, cs, d, cd);
    }
    template< class TS, class TD >  
    SYS_INLINE void BGRToHLS( const TS *s, CH cs, TD *d, CH cd )
    {
        //assert (cs >= 3); cd; cs;
        TS tempDat[3] = {s[2], s[1], s[0]}; // R, G, B
        RGBToHLS( tempDat, cs, d, cd );
    }
    SYS_INLINE void BGRToHLS_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        BGRToHLS<Fw8u, Fw8u>(s, cs, d,cd);
    }

    //----------------------------
    // HLS -> RGB/BGR
    //----------------------------
    template< class TS, class TD >  
    SYS_INLINE void HLSToRGB( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) );

        double m1 = 0, m2 = 0, h = 0;
        double R = 0, G = 0, B = 0;
        double rgb[3];

        switch( cd )
        {
        case C4:
        case C3:                                                        
        case AC4:  
                rgb[0] = FW_REF::Normalize<TS,TS>::Scale( s[0] );
                rgb[0] *= 360;
                rgb[1] = FW_REF::Normalize<TS,TS>::Scale( s[1] );
                rgb[2] = FW_REF::Normalize<TS,TS>::Scale( s[2] );
                if (rgb[1] <= 0.5)
                {
                    m2 = rgb[1] * (1 + rgb[2]);
                }
                else
                {
                    m2 = rgb[1] + rgb[2] - rgb[1]*rgb[2];
                }

                m1 = 2 * rgb[1] - m2;
                if (rgb[2] == 0)
                {
                    R = G = B = rgb[1];
                }
                else
                {
                    h = rgb[0] + 120;

                    if (h > 360)
                    {
                        h -= 360;
                    }
                    HLSRGMac (R, h, m1, m2);
                    h = rgb[0];
                    HLSRGMac (G, h, m1, m2);
                    h = rgb[0] - 120;
                    HLSBMac (B, h, m1, m2);
                }
                d[2] = FW_REF::Normalize<TS,TD>::Unscale( B ); // B
        case C2:  
                d[1] = FW_REF::Normalize<TS,TD>::Unscale( G ); // G
        case C1:  
                d[0] = FW_REF::Normalize<TS,TD>::Unscale( R ); // R
        }
    }

    SYS_INLINE void HLSToRGB_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        HLSToRGB<Fw8u, Fw8u>(s, cs, d, cd);
    }
    SYS_INLINE void HLSToRGB_16s16s( const Fw16s *s, CH cs, Fw16s *d, CH cd )
    {
        HLSToRGB<Fw16s, Fw16s>(s, cs, d, cd);
    }
    SYS_INLINE void HLSToRGB_16u16u( const Fw16u *s, CH cs, Fw16u *d, CH cd )
    {
        HLSToRGB<Fw16u, Fw16u>(s, cs, d, cd);
    }
    SYS_INLINE void HLSToRGB_32f32f( const Fw32f *s, CH cs, Fw32f *d, CH cd )
    {
        HLSToRGB<Fw32f, Fw32f>(s, cs, d, cd);
    }

    template< class TS, class TD >  
    SYS_INLINE void HLSToBGR( const TS *s, CH cs, TD *d, CH cd )
    {
        //assert (cs >= 3); cd; cs;

        TD rgb[3];
        HLSToRGB (s, cs, rgb, cd);

        d[0] = rgb[2]; // B
        d[1] = rgb[1]; // G
        d[2] = rgb[0]; // R
    }

    SYS_INLINE void HLSToBGR_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        HLSToBGR<Fw8u, Fw8u>(s, cs, d, cd);
    }

    //----------------------------
    // RGB -> HSV
    //----------------------------
    template< class TS, class TD >  
    SYS_INLINE void RGBToHSV( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;

        double rgb[3];
        rgb[0] = FW_REF::Normalize<TS,TS>::Scale( (double)s[0] );
        rgb[1] = FW_REF::Normalize<TS,TS>::Scale( (double)s[1] );
        rgb[2] = FW_REF::Normalize<TS,TS>::Scale( (double)s[2] );
        double V = MAX( MAX( rgb[0], rgb[1] ), rgb[2]);
        double temp = MIN( MIN( rgb[0],rgb[1] ), rgb[2]);
        double H = 0, S = 0, delta, Cr, Cg, Cb;

        switch( cd )
        {
        case C4:
        case C3:                                                        
        case AC4: if (V == 0.0)
                  {
                      S = 0;
                  }
                  else
                  {
                      S = (V - temp) / V;
                  }

                  // Hue
                  delta = V - temp;
                  Cr = (V - rgb[0]) / delta;
                  Cg = (V - rgb[1]) / delta;
                  Cb = (V - rgb[2]) / delta;

                  if (V == rgb[0]) // R
                  {
                      H = Cb - Cg;
                  }
                  if (V == rgb[1]) // G
                  {
                      H = 2 + Cr - Cb;
                  }
                  if (V== rgb[2]) // B
                  {
                      H = 4 + Cg - Cr;
                  }

                  H = H*60;
                  if (H < 0)
                  {
                      H += 360.0;
                  }

                  d[2] = FW_REF::Normalize<TS,TD>::Unscale( V ); // V
        case C2:  d[1] = FW_REF::Normalize<TS,TD>::Unscale( S ); // S
        case C1:  d[0] = FW_REF::Normalize<TS,TD>::Scale( H, 360.0 ); // H
        }
    }


        
        template<typename TS,CH ChSrc,typename TD,CH ChDst>
        class iRGBToHSV_8u_C3R :  public fe2<TS,ChSrc,TD,ChDst>
        {
        public:
            FE_REF
            IV REFR( const Fw8u *s, Fw8u *d ) const
            {
                double rgb[3];
                rgb[0] = FW_REF::Normalize<TS,TS>::Scale( (double)s[0] );
                rgb[1] = FW_REF::Normalize<TS,TS>::Scale( (double)s[1] );
                rgb[2] = FW_REF::Normalize<TS,TS>::Scale( (double)s[2] );
                double V = MAX( MAX( rgb[0], rgb[1] ), rgb[2]);
                double temp = MIN( MIN( rgb[0],rgb[1] ), rgb[2]);
                double H = 0, S = 0, delta=0, Cr=0, Cg=0, Cb=0;

                if (V == 0.0){ S = 0; }
                else { S = (V - temp) / V; }

                // Hue
                delta = V - temp;
                Cr = (V - rgb[0]) / delta;
                Cg = (V - rgb[1]) / delta;
                Cb = (V - rgb[2]) / delta;

                if (V == rgb[0]){ H = Cb - Cg;} // R
                if (V == rgb[1]){H = 2 + Cr - Cb;} // G
                if (V== rgb[2]) {H = 4 + Cg - Cr;}// B
                H = H*60;
                if (H < 0){H += 360.0;}

                d[2] = FW_REF::Normalize<TS,TD>::Unscale( V ); // V
                d[1] = FW_REF::Normalize<TS,TD>::Unscale( S ); // S
                d[0] = FW_REF::Normalize<TS,TD>::Scale( H, 360.0 ); // H
            }

        };


    SYS_INLINE void RGBToHSV_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        RGBToHSV<Fw8u, Fw8u>(s, cs, d, cd);
    }
    SYS_INLINE void RGBToHSV_16u16u( const Fw16u *s, CH cs, Fw16u *d, CH cd )
    {
        RGBToHSV<Fw16u, Fw16u>(s, cs, d, cd);
    }
    //----------------------------
    // HSV -> RGB
    //----------------------------
    template< class TS, class TD >  
    SYS_INLINE void HSVToRGB( const TS *s, CH cs, TD *d, CH cd )
    {
        cs;
        cd;//ASSERT( ChannelCount(cs) == ChannelCount(cd) );

        double R = 0, G = 0, B = 0, H = 0;
        double rgb[3];

        switch( cd )
        {
        case C4:
        case C3:                                                        
        case AC4:  
                rgb[0] = FW_REF::Normalize<TS,TS>::Scale( s[0] );
                rgb[0] *= 360;
                rgb[1] = FW_REF::Normalize<TS,TS>::Scale( s[1] );
                rgb[2] = FW_REF::Normalize<TS,TS>::Scale( s[2] );
                if (rgb[1] == 0.0)
                {
                    R = G = B = rgb[2];
                }
                else
                {
                    //if (rgb[0] >= 360)
                    //H = 0;
                    //else
                    H = (double)rgb[0] / 60;

                    int I = (int)floor (H);
                    double F = H - I,
                        M = rgb[2] * (1.0 - rgb[1]),
                        N = rgb[2] * (1.0 - rgb[1] * F),
                        K = rgb[2] * (1.0 - rgb[1] * (1.0 - F));

                    switch (I)
                    {
                    case 0:
                        R = rgb[2]; 
                        G = K; 
                        B = M;
                        break;
                    case 1:
                        R = N; 
                        G = rgb[2]; 
                        B = M;
                        break;
                    case 2:
                        R = M;
                        G = rgb[2]; 
                        B = K;
                        break;
                    case 3:
                        R = M; 
                        G = N; 
                        B = rgb[2];
                        break;
                    case 4:
                        R = K; 
                        G = M; 
                        B = rgb[2];
                        break;
                    case 5:
                    default:
                        R = rgb[2];
                        G = M;
                        B = N;
                        break;
                    }
                }

                  d[2] = FW_REF::Normalize<TS,TD>::Unscale( B ); // B
        case C2:  d[1] = FW_REF::Normalize<TS,TD>::Unscale( G ); // G
        case C1:  d[0] = FW_REF::Normalize<TS,TD>::Unscale( R ); // R
        }
    }
    SYS_INLINE void HSVToRGB_8u8u( const Fw8u *s, CH cs, Fw8u *d, CH cd )
    {
        HSVToRGB<Fw8u, Fw8u>(s, cs, d, cd);
    }
    SYS_INLINE void HSVToRGB_16u16u( const Fw16u *s, CH cs, Fw16u *d, CH cd )
    {
        HSVToRGB<Fw16u, Fw16u>(s, cs, d, cd);
    }

    //
    //Color Model Conversion : Common-case Optimized/Reference Function Template (For RGB <-> YUV, RGB <-> YCbCr, etc.)
    //-- Common-Case - Transformation can be simply defined by newColor = Ax + By + Cz --
    //Color Model Conversion Processing follows this hierarchy:

    //- MultiThreading
    //-    Image Function (Vertical Steps)
    //-        Row Processing
    //-            Transposing (24bit or 32bit)
    //-                Pixel Processing (MathPattern Dependent)

    typedef void (*pixel_fn )(const Fw8u *, CH, Fw8u *, CH);

    namespace CMC // (Color Model Conversion) ColorModelConv
    {
        enum ChCount
        {
            CH_C2,          // 2-Channel buffer
            CH_C3,          // 3-Channel buffer
            CH_AC4,         // 4-Channel buffer (4th channel is the alpha channel)
            CH_P3,          // 3 Planar buffer
            CH_AP4,         // 4 Planar buffer (4th plane is the alpha channel)
            CH_C3P3,        // 3 Channel buffer to 3 Planar buffer
            CH_P3C3,        // 3 Planar buffer to 3 Channel buffer
            CH_C2_422_ABAC, // 2-Channel 422 Downsampled Format with the 1st and 3rd parts Constant [e.g. Y0 | U0 | Y1 | V0]
            CH_C2_422_BACA, // 2-Channel 422 Downsampled Format with the 2nd and 4th parts Constant [e.g. Cb0 | Y0 | Cr0 | Y1]
            CH_P3_422,      // 3-Channel Planar 422 Downsampled Format
            CH_P3_411,      // 3-Channel Planar 411 Downsampled Format
            CH_P3_420,      // 3-Channel Planar 420 Downsampled Format
            CH_FC_C2P3,     // 2-Channel format to 3-Planar format
            CH_FC_P3C2      // 3-Planar format to 2-Channel format
        };

        SYS_INLINE int CMC_ChannelCount( ChCount c)
        {
            switch(c) {
                case CH_C2:
                case CH_C2_422_ABAC:  
                case CH_C2_422_BACA:    return 2;
                case CH_C3:             return 3;
                case CH_AC4:            return 4;
                case CH_P3: 
                case CH_AP4:
                case CH_P3_422:
                case CH_P3_411:
                case CH_P3_420:         return 1;
                case CH_C3P3: 
                case CH_P3C3: 
                case CH_FC_C2P3:
                case CH_FC_P3C2:        return -1;
                default:                return -1;
            }
        }

        // MathPattern is a collection of order of operations for pixel conversion.
        enum MathPattern    
        {
            MP_PLU_PLU_PLU,    //  A + B + C
            MP_MIN_MIN_PLU,    // -A - B + C
            MP_PLU_MIN_MIN,    //  A - B - C
            MP_PLU_MIN_PLU,    //  A - B + C
            MP_MIN_PLU_PLU     // -A + B + C
        };
        // PixelID is an enum to differentiate between different pixels (ie. R, G, or B)
        enum PixelID
        {
            X = 0,
            Y = 1,
            Z = 2
        };
        // ProcID is an enum to differentiate between various fixed-precision calculations.
        enum ProcID
        {
            PROC_NORMAL = 0,
            PROC_SHIFT13
        };

        // Pixel Processing Functions - Dependent on MathPattern
        // - The Pixel Processing Details -
        //1.  Convert data from Channel to Planar (Transpose) -> e.g. pX (RRRR...) pY (GGGG...) pZ (BBBB...)
        //2.  Each plane is unpacked into 16-bit values
        //3.  Initial Range-adjusting Subtraction Constant is applied.
        //4.  Each plane is multiplied by 2^5 to perform fixed precision calculation.
        //5.  The high-16-bits are taken after multiplying the plane with its constants. 
        //    (Note: the constants are already adjusted to 2^15 or 2^13 offset, depending on the 
        //     respective conversion constant. The computed values are now offset by 2^2 or 2^4)
        //6.  For second and third [pX,Y,orZ], the multiplied planes are added to the original p* plane
        //    to get the calculated value.
        //7.  The additive constants are added to the calculated values.
        //8.  If (ProcID == PROC_SHIFT13)
        //        The plane is divided by by 2^2 to achieve the true 8-bit value.
        //    else
        //        The plane is divided by by 2^4 to achieve the true 8-bit value.
        //9.  The final calculation value is packed again into an 8-bit plane.
        //10. Convert data from Planar to Channel (UnTranspose) -> e.g. RGBRGBRGBRGB...

        // Compute Y' = kY * (Y - y)
        SYS_INLINE static void PixelCalc ( XMM128 &srcDstLo, XMM128 &dstHi, const A16U &subConst_val, const A16U &mulConst_val )
        {
            const __m128i subConst = _mm_set1_epi16( subConst_val);
            const __m128i mulConst = _mm_set1_epi16( mulConst_val);
            static const __m128i zero =  CONST_SETZERO_8I();//_mm_setzero_si128();

            // Unpack Planar Channel from 8-bit to 16-bit to allow 16-bit calculations
            dstHi.i    = _mm_unpackhi_epi8 ( srcDstLo.i, zero     );
            dstHi.i    = _mm_sub_epi16     ( dstHi.i,    subConst );   // Subtracting Constant
            dstHi.i    = _mm_slli_epi16    ( dstHi.i,    5        );   // Shift value to the left by 2^5
            dstHi.i    = _mm_mulhi_epi16   ( dstHi.i,    mulConst );   // Matrix Transformation

            srcDstLo.i = _mm_unpacklo_epi8 ( srcDstLo.i, zero     );
            srcDstLo.i = _mm_sub_epi16     ( srcDstLo.i, subConst );  // Subtracting Constant
            srcDstLo.i = _mm_slli_epi16    ( srcDstLo.i, 5        );  // Shift value to the left by 2^5
            srcDstLo.i = _mm_mulhi_epi16   ( srcDstLo.i, mulConst );  // Matrix Transformation
        }

        ///////////////////////////////////
        // Process 16 pixels
        ///////////////////////////////////
        template <ConvFunc fn, PixelID pid, ProcID prc, MathPattern mp>
        struct Pixel
        {
            static SYS_INLINE __m128i Process( XMM128 &pX, XMM128 &pY, XMM128 &pZ ) { STATIC_ASSERT(false); }
        };

        template <ConvFunc fn, PixelID pid, ProcID prc>
        struct Pixel <fn,pid,prc,MP_PLU_PLU_PLU>
        {
            static SYS_INLINE __m128i Process( XMM128 &pX, XMM128 &pY, XMM128 &pZ )
            {
                XMM128 tempLoX, tempLoY, tempLoZ, tempHiX, tempHiY, tempHiZ;

                tempLoX = pX;
                tempLoY = pY;
                tempLoZ = pZ;
                // pX, pY, pZ needed to be passed by reference to guarantee alignment

                PixelCalc( tempLoX, tempHiX, CMCConstants[fn].SubConst[0], CMCConstants[fn].MulConst[pid][0] );
                PixelCalc( tempLoY, tempHiY, CMCConstants[fn].SubConst[1], CMCConstants[fn].MulConst[pid][1] );
                PixelCalc( tempLoZ, tempHiZ, CMCConstants[fn].SubConst[2], CMCConstants[fn].MulConst[pid][2] );

                // Plus Plus Plus
                tempLoX.i = _mm_add_epi16( tempLoX.i, tempLoY.i ); // A += B
                tempLoX.i = _mm_add_epi16( tempLoX.i, tempLoZ.i ); // A += C        -> A = A + B + C
                tempHiX.i = _mm_add_epi16( tempHiX.i, tempHiY.i );
                tempHiX.i = _mm_add_epi16( tempHiX.i, tempHiZ.i );

                // Adding Constant
                const __m128i addConst = _mm_set1_epi16( CMCConstants[fn].AddConst[pid] );
                tempLoX.i = _mm_adds_epi16( tempLoX.i, addConst );
                tempHiX.i = _mm_adds_epi16( tempHiX.i, addConst );

                switch (prc) // Compile-Time Switch
                {
                case PROC_SHIFT13:
                    tempLoX.i = _mm_srai_epi16( tempLoX.i, 2 );
                    tempHiX.i = _mm_srai_epi16( tempHiX.i, 2 );
                    break;
                default:
                    tempLoX.i = _mm_srai_epi16( tempLoX.i, 4 );
                    tempHiX.i = _mm_srai_epi16( tempHiX.i, 4 );
                    break;
                }

                // Pack Back Together to 8-Bit Planar Channel
                tempLoX.i = _mm_packus_epi16( tempLoX.i, tempHiX.i );
                return tempLoX.i;
            }
        };

        template <ConvFunc fn, PixelID pid, ProcID prc>
        struct Pixel <fn,pid,prc,MP_MIN_MIN_PLU>
        {
            static SYS_INLINE __m128i Process( XMM128 &pX, XMM128 &pY, XMM128 &pZ )
            {
                XMM128 tempLoX, tempLoY, tempLoZ, tempHiX, tempHiY, tempHiZ;

                tempLoX = pX;
                tempLoY = pY;
                tempLoZ = pZ;
                // pX, pY, pZ needed to be passed by reference to guarantee alignment

                PixelCalc( tempLoX, tempHiX, CMCConstants[fn].SubConst[0], CMCConstants[fn].MulConst[pid][0] );
                PixelCalc( tempLoY, tempHiY, CMCConstants[fn].SubConst[1], CMCConstants[fn].MulConst[pid][1] );
                PixelCalc( tempLoZ, tempHiZ, CMCConstants[fn].SubConst[2], CMCConstants[fn].MulConst[pid][2] );

                // Minus Minus Plus
                tempLoX.i = _mm_add_epi16( tempLoX.i, tempLoY.i ); // Lo: X = A + B
                tempLoZ.i = _mm_sub_epi16( tempLoZ.i, tempLoX.i ); // Lo: Z = C - (A+B) = -A - B + C
                tempHiX.i = _mm_add_epi16( tempHiX.i, tempHiY.i ); // Hi: X = A + B
                tempHiZ.i = _mm_sub_epi16( tempHiZ.i, tempHiX.i ); // Hi: Z = C - (A+B) = -A - B + C

                // Adding Constant
                const __m128i addConst = _mm_set1_epi16( CMCConstants[fn].AddConst[pid] );
                tempLoZ.i = _mm_adds_epi16( tempLoZ.i, addConst );
                tempHiZ.i = _mm_adds_epi16( tempHiZ.i, addConst );


                switch (prc) // Compile-Time Switch
                {
                case PROC_SHIFT13:
                    tempLoZ.i = _mm_srai_epi16( tempLoZ.i, 2 );
                    tempHiZ.i = _mm_srai_epi16( tempHiZ.i, 2 );
                    break;
                default:
                    tempLoZ.i = _mm_srai_epi16( tempLoZ.i, 4 );
                    tempHiZ.i = _mm_srai_epi16( tempHiZ.i, 4 );
                    break;
                }

                // Pack Back Together to 8-Bit Planar Channel
                tempLoZ.i = _mm_packus_epi16( tempLoZ.i, tempHiZ.i );

                return tempLoZ.i;
            }
        };

        template <ConvFunc fn, PixelID pid, ProcID prc>
        struct Pixel <fn,pid,prc,MP_PLU_MIN_MIN>
        {
            static SYS_INLINE __m128i Process( XMM128 &pX, XMM128 &pY, XMM128 &pZ )
            {
                XMM128 tempLoX, tempLoY, tempLoZ, tempHiX, tempHiY, tempHiZ;

                tempLoX = pX;
                tempLoY = pY;
                tempLoZ = pZ;
                // pX, pY, pZ needed to be passed by reference to guarantee alignment

                PixelCalc( tempLoX, tempHiX, CMCConstants[fn].SubConst[0], CMCConstants[fn].MulConst[pid][0] );
                PixelCalc( tempLoY, tempHiY, CMCConstants[fn].SubConst[1], CMCConstants[fn].MulConst[pid][1] );
                PixelCalc( tempLoZ, tempHiZ, CMCConstants[fn].SubConst[2], CMCConstants[fn].MulConst[pid][2] );

                // Plus Minus Minus
                tempLoY.i = _mm_add_epi16( tempLoY.i, tempLoZ.i ); // Lo: Y = B + C
                tempLoX.i = _mm_sub_epi16( tempLoX.i, tempLoY.i ); // Lo: X = A - (B + C) = A - B - C
                tempHiY.i = _mm_add_epi16( tempHiY.i, tempHiZ.i ); // Hi: Y = B + C
                tempHiX.i = _mm_sub_epi16( tempHiX.i, tempHiY.i ); // Hi: X = A - (B + C) = A - B - C

                // Adding Constant
                const __m128i addConst = _mm_set1_epi16( CMCConstants[fn].AddConst[pid] );
                tempLoX.i = _mm_adds_epi16( tempLoX.i, addConst );
                tempHiX.i = _mm_adds_epi16( tempHiX.i, addConst );

                switch (prc) // Compile-Time Switch
                {
                case PROC_SHIFT13:
                    tempLoX.i = _mm_srai_epi16( tempLoX.i, 2 );
                    tempHiX.i = _mm_srai_epi16( tempHiX.i, 2 );
                    break;
                default:
                    tempLoX.i = _mm_srai_epi16( tempLoX.i, 4 );
                    tempHiX.i = _mm_srai_epi16( tempHiX.i, 4 );
                    break;
                }

                // Pack Back Together to 8-Bit Planar Channel
                tempLoX.i = _mm_packus_epi16( tempLoX.i, tempHiX.i );

                return tempLoX.i;
            }
        };

        template <ConvFunc fn, PixelID pid, ProcID prc>
        struct Pixel <fn,pid,prc,MP_PLU_MIN_PLU>
        {
            static SYS_INLINE __m128i Process( XMM128 &pX, XMM128 &pY, XMM128 &pZ )
            {
                XMM128 tempLoX, tempLoY, tempLoZ, tempHiX, tempHiY, tempHiZ;

                tempLoX = pX; 
                tempLoY = pY; 
                tempLoZ = pZ; 
                // pX, pY, pZ needed to be passed by reference to guarantee alignment

                PixelCalc( tempLoX, tempHiX, CMCConstants[fn].SubConst[0], CMCConstants[fn].MulConst[pid][0] );
                PixelCalc( tempLoY, tempHiY, CMCConstants[fn].SubConst[1], CMCConstants[fn].MulConst[pid][1] );
                PixelCalc( tempLoZ, tempHiZ, CMCConstants[fn].SubConst[2], CMCConstants[fn].MulConst[pid][2] );

                // Plus Minus Plus  *****
                tempLoX.i = _mm_sub_epi16( tempLoX.i, tempLoY.i ); // A -= B
                tempLoX.i = _mm_add_epi16( tempLoX.i, tempLoZ.i ); // A += C        -> A = A - B + C
                tempHiX.i = _mm_sub_epi16( tempHiX.i, tempHiY.i ); 
                tempHiX.i = _mm_add_epi16( tempHiX.i, tempHiZ.i );

                // Adding Constant
                const __m128i addConst = _mm_set1_epi16( CMCConstants[fn].AddConst[pid] );
                tempLoX.i = _mm_adds_epi16( tempLoX.i, addConst );
                tempHiX.i = _mm_adds_epi16( tempHiX.i, addConst );

                switch (prc) // Compile-Time Switch
                {
                case PROC_SHIFT13:
                    tempLoX.i = _mm_srai_epi16( tempLoX.i, 2 );
                    tempHiX.i = _mm_srai_epi16( tempHiX.i, 2 );
                    break;
                default:
                    tempLoX.i = _mm_srai_epi16( tempLoX.i, 4 );
                    tempHiX.i = _mm_srai_epi16( tempHiX.i, 4 );
                    break;
                }

                // Pack Back Together to 8-Bit Planar Channel
                tempLoX.i = _mm_packus_epi16( tempLoX.i, tempHiX.i );

                return tempLoX.i;
            }
        };

        template <ConvFunc fn, PixelID pid, ProcID prc>
        struct Pixel <fn,pid,prc,MP_MIN_PLU_PLU>
        {
            static SYS_INLINE __m128i Process( XMM128 &pX, XMM128 &pY, XMM128 &pZ )
            {
                XMM128 tempLoX, tempLoY, tempLoZ, tempHiX, tempHiY, tempHiZ;

                tempLoX = pX;
                tempLoY = pY;
                tempLoZ = pZ;
                // pX, pY, pZ needed to be passed by reference to guarantee alignment

                PixelCalc( tempLoX, tempHiX, CMCConstants[fn].SubConst[0], CMCConstants[fn].MulConst[pid][0] );
                PixelCalc( tempLoY, tempHiY, CMCConstants[fn].SubConst[1], CMCConstants[fn].MulConst[pid][1] );
                PixelCalc( tempLoZ, tempHiZ, CMCConstants[fn].SubConst[2], CMCConstants[fn].MulConst[pid][2] );

                // Minus Minus Plus
                tempLoZ.i = _mm_sub_epi16( tempLoZ.i, tempLoX.i ); // X = C - A
                tempLoZ.i = _mm_add_epi16( tempLoZ.i, tempLoY.i ); // X += B        -> X = C - A + B = -A + B + C
                tempHiZ.i = _mm_sub_epi16( tempHiZ.i, tempHiX.i );
                tempHiZ.i = _mm_add_epi16( tempHiZ.i, tempHiY.i );

                // Adding Constant
                const __m128i addConst = _mm_set1_epi16( CMCConstants[fn].AddConst[pid] );
                tempLoZ.i = _mm_adds_epi16( tempLoZ.i, addConst );
                tempHiZ.i = _mm_adds_epi16( tempHiZ.i, addConst );

                switch (prc) // Compile-Time Switch
                {
                case PROC_SHIFT13:
                    tempLoZ.i = _mm_srai_epi16( tempLoZ.i, 2 );
                    tempHiZ.i = _mm_srai_epi16( tempHiZ.i, 2 );
                    break;
                default:
                    tempLoZ.i = _mm_srai_epi16( tempLoZ.i, 4 );
                    tempHiZ.i = _mm_srai_epi16( tempHiZ.i, 4 );
                    break;
                }
                // Pack Back Together to 8-Bit Planar Channel
                tempLoZ.i = _mm_packus_epi16( tempLoZ.i, tempHiZ.i );

                return tempLoZ.i;
            }
        };

        ///////////////////////////////////
        // Pixel Row Processing Functions
        ///////////////////////////////////
        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc, ChCount ch>
        struct PRow
        {
            static void Process( Fw8u *&srcRow, Fw8u *&dstRow, Fw32u numIntervals ) 
            { 
                STATIC_ASSERT(false); 
            }
            static void ProcessPlanar( Fw8u *srcRow[3], Fw8u *dstRow[3], Fw32u numIntervals ) 
            { 
                STATIC_ASSERT(false); 
            }
        };

        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc>
        struct PRow <mp1, mp2, mp3, fn, prc, CH_C3>
        {
            static SYS_INLINE void Process( Fw8u *&srcRow, Fw8u *&dstRow, Fw32u numIntervals )
            {
                XMM128 xmm1, xmm2, xmm3;
                XMM128 xmmU, xmmV, xmmW;

                _mm_prefetch((const char *)srcRow, _MM_HINT_T0);
                for (Fw32u i = 0; i < numIntervals; i++)
                {
                    // Transpose 24-bit data into Column-major format of xxxx|xxxx|xxxx|xxxx, 
                    //                                                   yyyy|yyyy|yyyy|yyyy, 
                    //                                               and zzzz|zzzz|zzzz|zzzz
                    CBL_SSE2::LoadAlignedAndConvert_3C_to_3P_8bit(srcRow, xmm1.i, xmm2.i, xmm3.i);

                    xmmU.i = Pixel<fn, X, prc,mp1>::Process( xmm1, xmm2, xmm3 );
                    xmmV.i = Pixel<fn, Y, prc,mp2>::Process( xmm1, xmm2, xmm3 );
                    xmmW.i = Pixel<fn, Z, prc,mp3>::Process( xmm1, xmm2, xmm3 );

                    CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( xmmU.i, xmmV.i, xmmW.i, dstRow );

                    srcRow += 48;
                    _mm_prefetch( (const char *)(srcRow), _MM_HINT_T0 );
                    dstRow += 48;
                }
            }
        };

        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc>
        struct PRow <mp1, mp2, mp3, fn, prc, CH_AC4>
        {
            static SYS_INLINE void Process( Fw8u *&srcRow, Fw8u *&dstRow, Fw32u numIntervals )
            {
                XMM128 xmm1, xmm2, xmm3;
                XMM128 xmmU, xmmV, xmmW;

                _mm_prefetch((const char *)srcRow, _MM_HINT_T0);
                if ( IS_ALIGNED16_2(srcRow, dstRow) )
                {
                    for (Fw32u i = 0; i < numIntervals; i++)
                    {
                        // Transpose 32-bit data into Column-major format of xxxx|xxxx|xxxx|xxxx, 
                        //                                                   yyyy|yyyy|yyyy|yyyy, 
                        //                                               and zzzz|zzzz|zzzz|zzzz
                        CBL_SSE2::LoadAlignedAndConvert_4C_to_3P_8bit(srcRow, xmm1.i, xmm2.i, xmm3.i); 
                        // Alpha channel is zero'd.
                        xmmU.i = Pixel<fn, X, prc,mp1>::Process( xmm1, xmm2, xmm3 );
                        xmmV.i = Pixel<fn, Y, prc,mp2>::Process( xmm1, xmm2, xmm3 );
                        xmmW.i = Pixel<fn, Z, prc,mp3>::Process( xmm1, xmm2, xmm3 );
                        // UnTranspose 32-bit data from Column-major format of xxxx|xxxx|xxxx|xxxx, 
                        //                                                     yyyy|yyyy|yyyy|yyyy, 
                        //                                                 and zzzz|zzzz|zzzz|zzzz
                        //                                                 to  xyzx|yzxy|zxyz|xyzx|yzxy|zxyz

                        //CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit(xmmU.i, xmmV.i, xmmW.i, _mm_setzero_si128(), dstRow);
                        const U8 *pr = (const U8 *)&xmmU.i;
                        const U8 *pg = (const U8 *)&xmmV.i;
                        const U8 *pb = (const U8 *)&xmmW.i;
                        for( int n = 0; n < 16; ++n )
                        {
                            dstRow[4*n+0] = pr[n];
                            dstRow[4*n+1] = pg[n];
                            dstRow[4*n+2] = pb[n];
                        }

                        srcRow += 64;
                        _mm_prefetch( (const char *)(srcRow), _MM_HINT_T0 );
                        dstRow += 64;
                    }
                }
                else
                {
                    for (Fw32u i = 0; i < numIntervals; i++)
                    {
                        // Transpose 32-bit data into Column-major format of xxxx|xxxx|xxxx|xxxx, 
                        //                                                   yyyy|yyyy|yyyy|yyyy, 
                        //                                               and zzzz|zzzz|zzzz|zzzz
                        CBL_SSE2::LoadUnalignedAndConvert_4C_to_3P_8bit(srcRow, xmm1.i, xmm2.i, xmm3.i); 
                        // Alpha channel is zero'd.
                        xmmU.i = Pixel<fn, X, prc,mp1>::Process( xmm1, xmm2, xmm3 );
                        xmmV.i = Pixel<fn, Y, prc,mp2>::Process( xmm1, xmm2, xmm3 );
                        xmmW.i = Pixel<fn, Z, prc,mp3>::Process( xmm1, xmm2, xmm3 );
                        // UnTranspose 32-bit data from Column-major format of xxxx|xxxx|xxxx|xxxx, 
                        //                                                     yyyy|yyyy|yyyy|yyyy, 
                        //                                                 and zzzz|zzzz|zzzz|zzzz
                        //                                                 to  xyzx|yzxy|zxyz|xyzx|yzxy|zxyz

                        //CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit(xmmU.i, xmmV.i, xmmW.i, _mm_setzero_si128(), dstRow);
                        const U8 *pr = (const U8 *)&xmmU.i;
                        const U8 *pg = (const U8 *)&xmmV.i;
                        const U8 *pb = (const U8 *)&xmmW.i;
                        for( int n = 0; n < 16; ++n )
                        {
                            dstRow[4*n+0] = pr[n];
                            dstRow[4*n+1] = pg[n];
                            dstRow[4*n+2] = pb[n];
                        }

                        srcRow += 64;
                        _mm_prefetch( (const char *)(srcRow), _MM_HINT_T0 );
                        dstRow += 64;
                    }
                }
            }
        };

        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc>
        struct PRow <mp1, mp2, mp3, fn, prc, CH_P3>
        {
            static SYS_INLINE void ProcessPlanar( Fw8u *srcRow[3], Fw8u *dstRow[3], Fw32u numIntervals )
            {
                XMM128 xmm1, xmm2, xmm3;
                XMM128 xmmU, xmmV, xmmW;

                _mm_prefetch((const char *)srcRow[0], _MM_HINT_T0);
                if ( IS_ALIGNED16_3(srcRow[0], srcRow[1], srcRow[2]) && IS_ALIGNED16_3(dstRow[0], dstRow[1], dstRow[2]) )
                {
                    for (Fw32u i = 0; i < numIntervals; i++)
                    {
                        xmm1.i = _mm_load_si128((__m128i*)srcRow[0]);
                        xmm2.i = _mm_load_si128((__m128i*)srcRow[1]);
                        xmm3.i = _mm_load_si128((__m128i*)srcRow[2]);

                        xmmU.i = Pixel<fn, X, prc,mp1>::Process( xmm1, xmm2, xmm3 );
                        xmmV.i = Pixel<fn, Y, prc,mp2>::Process( xmm1, xmm2, xmm3 );
                        xmmW.i = Pixel<fn, Z, prc,mp3>::Process( xmm1, xmm2, xmm3 );

                        _mm_stream_si128((__m128i*)dstRow[0], xmmU.i);
                        _mm_stream_si128((__m128i*)dstRow[1], xmmV.i);
                        _mm_stream_si128((__m128i*)dstRow[2], xmmW.i);

                        srcRow[0] += 16;
                        _mm_prefetch((const char *)(srcRow[0]), _MM_HINT_T0);
                        srcRow[1] += 16;
                        srcRow[2] += 16;
                        dstRow[0] += 16;
                        dstRow[1] += 16;
                        dstRow[2] += 16;
                    }
                    _mm_mfence();
                }
                else
                {
                    for( Fw32u i = 0; i < numIntervals; i++ )
                    {
                        xmm1.i = _mm_loadu_si128( (__m128i*)srcRow[0] );
                        xmm2.i = _mm_loadu_si128( (__m128i*)srcRow[1] );
                        xmm3.i = _mm_loadu_si128( (__m128i*)srcRow[2] );

                        xmmU.i = Pixel<fn, X, prc,mp1>::Process( xmm1, xmm2, xmm3 );
                        xmmV.i = Pixel<fn, Y, prc,mp2>::Process( xmm1, xmm2, xmm3 );
                        xmmW.i = Pixel<fn, Z, prc,mp3>::Process( xmm1, xmm2, xmm3 );

                        _mm_storeu_si128((__m128i*)dstRow[0], xmmU.i);
                        _mm_storeu_si128((__m128i*)dstRow[1], xmmV.i);
                        _mm_storeu_si128((__m128i*)dstRow[2], xmmW.i);

                        srcRow[0] += 16;
                        _mm_prefetch( (const char *)(srcRow[0]), _MM_HINT_T0 );
                        srcRow[1] += 16;
                        srcRow[2] += 16;
                        dstRow[0] += 16;
                        dstRow[1] += 16;
                        dstRow[2] += 16;
                    }
                }
            }
        };

        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc>
        struct PRow <mp1, mp2, mp3, fn, prc, CH_C3P3>
        {
            static SYS_INLINE void ProcessPlanar( Fw8u *srcRow, Fw8u *dstRow[3], Fw32u numIntervals )
            {
                XMM128 xmm1, xmm2, xmm3;
                XMM128 xmmU, xmmV, xmmW;

                _mm_prefetch((const char *)srcRow, _MM_HINT_T0);
                if ( IS_ALIGNED16_4(srcRow, dstRow[0], dstRow[1], dstRow[2]) )
                {
                    for (Fw32u i = 0; i < numIntervals; i++)
                    {
                        // Transpose 24-bit data into Column-major format of xxxx|xxxx|xxxx|xxxx, 
                        //                                                   yyyy|yyyy|yyyy|yyyy, 
                        //                                               and zzzz|zzzz|zzzz|zzzz
                        CBL_SSE2::LoadAlignedAndConvert_3C_to_3P_8bit( srcRow, xmm1.i, xmm2.i, xmm3.i);
                        xmmU.i = Pixel<fn, X, prc,mp1>::Process( xmm1, xmm2, xmm3 );
                        xmmV.i = Pixel<fn, Y, prc,mp2>::Process( xmm1, xmm2, xmm3 );
                        xmmW.i = Pixel<fn, Z, prc,mp3>::Process( xmm1, xmm2, xmm3 );

                        _mm_stream_si128( (__m128i*)dstRow[0], xmmU.i );
                        _mm_stream_si128( (__m128i*)dstRow[1], xmmV.i );
                        _mm_stream_si128( (__m128i*)dstRow[2], xmmW.i );

                        srcRow += 48;
                        _mm_prefetch((const char *)(srcRow), _MM_HINT_T0);
                        dstRow[0] += 16;
                        dstRow[1] += 16;
                        dstRow[2] += 16;
                    }
                    _mm_mfence();
                }
                else
                {
                    for (Fw32u i = 0; i < numIntervals; i++)
                    {
                        // Transpose 24-bit data into Column-major format of xxxx|xxxx|xxxx|xxxx, 
                        //                                                   yyyy|yyyy|yyyy|yyyy, 
                        //                                               and zzzz|zzzz|zzzz|zzzz
                        CBL_SSE2::LoadUnalignedAndConvert_3C_to_3P_8bit( srcRow, xmm1.i, xmm2.i, xmm3.i);
                        xmmU.i = Pixel<fn, X, prc,mp1>::Process( xmm1, xmm2, xmm3 );
                        xmmV.i = Pixel<fn, Y, prc,mp2>::Process( xmm1, xmm2, xmm3 );
                        xmmW.i = Pixel<fn, Z, prc,mp3>::Process( xmm1, xmm2, xmm3 );

                        _mm_storeu_si128( (__m128i*)dstRow[0], xmmU.i );
                        _mm_storeu_si128( (__m128i*)dstRow[1], xmmV.i );
                        _mm_storeu_si128( (__m128i*)dstRow[2], xmmW.i );

                        srcRow += 48;
                        _mm_prefetch( (const char *)(srcRow), _MM_HINT_T0 );
                        dstRow[0] += 16;
                        dstRow[1] += 16;
                        dstRow[2] += 16;
                    }
                }
            }
        };

        template <MathPattern mp1, MathPattern mp2, MathPattern mp3, ConvFunc fn, ProcID prc>
        struct PRow <mp1, mp2, mp3, fn, prc, CH_P3C3>
        {
            static SYS_INLINE void ProcessPlanar( Fw8u *srcRow[3], Fw8u *dstRow, Fw32u numIntervals )
            {
                XMM128 xmm1, xmm2, xmm3;
                XMM128 xmmU, xmmV, xmmW;

                _mm_prefetch((const char *)srcRow[0], _MM_HINT_T0);

                if (IS_ALIGNED16_4(srcRow[0], srcRow[1], srcRow[2], dstRow))
                {
                    for (Fw32u i = 0; i < numIntervals; i++)
                    {
                        // Load Planar Data
                        xmm1.i = _mm_load_si128( (__m128i*)srcRow[0] );
                        xmm2.i = _mm_load_si128( (__m128i*)srcRow[1] );
                        xmm3.i = _mm_load_si128( (__m128i*)srcRow[2] );

                        xmmU.i = Pixel<fn, X, prc,mp1>::Process( xmm1, xmm2, xmm3 );
                        xmmV.i = Pixel<fn, Y, prc,mp2>::Process( xmm1, xmm2, xmm3 );
                        xmmW.i = Pixel<fn, Z, prc,mp3>::Process( xmm1, xmm2, xmm3 );

                        // Convert and Store planar 3 to channel 3
                        CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit( xmmU.i, xmmV.i, xmmW.i, dstRow );

                        _mm_prefetch( (const char *)(srcRow[0]+16), _MM_HINT_T0 );
                        srcRow[0] += 16;
                        srcRow[1] += 16;
                        srcRow[2] += 16;
                        dstRow += 48;
                    }
                }
                else
                {
                    for (Fw32u i = 0; i < numIntervals; i++)
                    {
                        // Load Planar Data
                        xmm1.i = _mm_loadu_si128( (__m128i*)srcRow[0] );
                        xmm2.i = _mm_loadu_si128( (__m128i*)srcRow[1] );
                        xmm3.i = _mm_loadu_si128( (__m128i*)srcRow[2] );

                        xmmU.i = Pixel<fn, X, prc,mp1>::Process( xmm1, xmm2, xmm3 );
                        xmmV.i = Pixel<fn, Y, prc,mp2>::Process( xmm1, xmm2, xmm3 );
                        xmmW.i = Pixel<fn, Z, prc,mp3>::Process( xmm1, xmm2, xmm3 );

                        // Convert and Store planar 3 to channel 3
                        CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( xmmU.i, xmmV.i, xmmW.i, dstRow );

                        _mm_prefetch( (const char *)(srcRow[0]+16), _MM_HINT_T0 );
                        srcRow[0] += 16;
                        srcRow[1] += 16;
                        srcRow[2] += 16;
                        dstRow += 48;
                    }
                }
            }
        };

        ///////////////////////////////////
        // Model Conversion Functions
        ///////////////////////////////////
        template <ChCount ch>
        struct ModelPlanar
        {
            template <pixel_fn pfn>
            static FwStatus ConvertREF( const Fw8u *const pSrc[3], int srcStep, 
                                         Fw8u *pDst[3],             int dstStep, FwiSize roiSize ) 
            { 
                STATIC_ASSERT(false); 
            }
        };

        // Pixel Processing
        template <>
        struct ModelPlanar <CH_P3>
        {
            template <pixel_fn pfn>
            static FwStatus ConvertREF( const Fw8u *const pSrc[3], int srcStep, 
                                         Fw8u *pDst[3],             int dstStep, FwiSize roiSize )
            {
                Fw8u tempSrc[3], tempDst[3];
                A32U srcOffset, dstOffset;
                //srcOffset = dstOffset = 0;

                for( A32S y=0; y<roiSize.height; ++y )
                {
                    srcOffset = y * srcStep;
                    dstOffset = y * dstStep;
                    for( A32S x=0; x<roiSize.width; ++x,
                                                   ++srcOffset,
                                                   ++dstOffset )
                    {
                        tempSrc[0] = pSrc[0][ srcOffset ];
                        tempSrc[1] = pSrc[1][ srcOffset ];
                        tempSrc[2] = pSrc[2][ srcOffset ];

                        pfn (tempSrc, C3, tempDst, C3); 

                        pDst[0][ dstOffset ] = tempDst[0]; 
                        pDst[1][ dstOffset ] = tempDst[1];
                        pDst[2][ dstOffset ] = tempDst[2];
                    }
                }
                return fwStsNoErr;
            }
        };

        template <ChCount ch>
        struct ModelCP
        {
            template <pixel_fn pfn>
            static FwStatus ConvertREF( const Fw8u *pSrc, int srcStep, Fw8u *pDst[3], int dstStep, FwiSize roiSize ) 
            { 
                STATIC_ASSERT(false); 
            }
        };

        // Pixel Processing
        template <>
        struct ModelCP <CH_C3P3>
        {
            template <pixel_fn pfn>
            static FwStatus ConvertREF( const Fw8u *pSrc, int srcStep, Fw8u *pDst[3], int dstStep, FwiSize roiSize )
            {
                const Fw8u *pSrcRow = pSrc;
                const Fw8u *pSrcRowEnd;
                Fw8u *pDstRow[3] = { pDst[0], pDst[1], pDst[2] };
                Fw8u pTempDst[3];

                for( A32S j=0; j<roiSize.height; ++j )
                {
                    pDstRow[0] = dstStep*j + pDst[0];
                    pDstRow[1] = dstStep*j + pDst[1];
                    pDstRow[2] = dstStep*j + pDst[2];
                    pSrcRow    = srcStep*j + pSrc;
                    pSrcRowEnd = pSrcRow + roiSize.width*3;
                    for( ; pSrcRow < pSrcRowEnd; pSrcRow+=3, ++pDstRow[0], ++pDstRow[1], ++pDstRow[2] )
                    {
                        pfn( pSrcRow, C3, pTempDst, C3 );
                        *pDstRow[0] = pTempDst[0];  // Y
                        *pDstRow[1] = pTempDst[1];  // U
                        *pDstRow[2] = pTempDst[2];  // V
                    }
                }
                return fwStsNoErr;
            }
        };

        template <>
        struct ModelCP <CH_FC_C2P3>
        {
            static FwStatus ConvertREF( const Fw8u *pSrc, int srcStep, Fw8u *pDst[3], int dstStep[3], FwiSize roiSize )
            {
                Fw8u *tempSrc = (Fw8u *)pSrc, *dst[3];
                A32S w_blocks = roiSize.width>>1;    // Find the number of even number of blocks
                A32S w_rem = roiSize.width&0x1;      // Check if the width is even or odd.

                for (A32S y=0; y<roiSize.height; ++y)
                {
                    tempSrc = (Fw8u *)pSrc+(y*srcStep);
                    dst[0] = pDst[0]+y*dstStep[0];
                    dst[1] = pDst[1]+y*dstStep[1];
                    dst[2] = pDst[2]+y*dstStep[2];

                    // Process 2pxl blocks at a time
                    for (A32S x=0; x<w_blocks; ++x,
                                              tempSrc+=4,
                                              dst[0]+=2,
                                              ++dst[1],
                                              ++dst[2] )
                    {
                        dst[0][0] = tempSrc[0];    //    Y1
                        dst[1][0] = tempSrc[1];    //    Cb1
                        dst[0][1] = tempSrc[2];    //    Y2
                        dst[2][0] = tempSrc[3];    //    Cr1
                    }
                    // Process any remaining pxl
                    if (w_rem)
                    {
                        dst[0][0] = tempSrc[0];    //    Y1
                        dst[1][0] = tempSrc[1];    //    Cb1
                    }
                }
                return fwStsNoErr;
            }
        };
        template <ChCount ch>
        struct ModelPC
        {
            template <pixel_fn pfn>
            static FwStatus ConvertREF( const Fw8u *const pSrc[3], int srcStep, 
                                         Fw8u *pDst,                int dstStep, FwiSize roiSize ) 
            { 
                STATIC_ASSERT(false); 
            }
            static FwStatus ConvertREF( const Fw8u *const pSrc[3], int srcStep[3], 
                                         Fw8u *pDst,                int dstStep, FwiSize roiSize ) 
            { 
                STATIC_ASSERT(false); 
            }
        };

        // Pixel Processing
        template <>
        struct ModelPC <CH_P3C3>
        {
            template <pixel_fn pfn>
            static FwStatus ConvertREF( const Fw8u *const pSrc[3], int srcStep, 
                                         Fw8u *pDst,                int dstStep, FwiSize roiSize )
            {
                const Fw8u *pSrcRow[3];
                const Fw8u *pSrcRow1End; 
                Fw8u *pDstRow;
                Fw8u pTempSrc[3];

                for( A32S j=0; j<roiSize.height; ++j )
                {
                    pSrcRow[0]  = srcStep*j + pSrc[0];
                    pSrcRow[1]  = srcStep*j + pSrc[1];
                    pSrcRow[2]  = srcStep*j + pSrc[2];
                    pSrcRow1End = pSrcRow[0] + roiSize.width;
                    pDstRow     = dstStep*j + pDst;
                    
                    for( ; pSrcRow[0] < pSrcRow1End; ++pSrcRow[0], ++pSrcRow[1], ++pSrcRow[2], pDstRow+=3 )
                    {
                        pTempSrc[0] = *(pSrcRow[0]);
                        pTempSrc[1] = *(pSrcRow[1]);
                        pTempSrc[2] = *(pSrcRow[2]);
                        pfn( pTempSrc, C3, pDstRow, C3 );
                    }
                }
                return fwStsNoErr;
            }
        };
        template<>
        struct ModelPC <CH_FC_P3C2> // Only used in Format Conversion
        {
            static FwStatus ConvertREF( const Fw8u *const pSrc[3], int srcStep[3], 
                                         Fw8u *pDst,                int dstStep, FwiSize roiSize )
            {
                Fw8u *src[3], *dst;
                //A32U dstOffset[3];
                A32S w_blocks = roiSize.width>>1;    // Find the number of even number of blocks
                A32S w_rem = roiSize.width&0x1;      // Check if the width is even or odd.

                for (A32S y=0; y<roiSize.height; ++y)
                {
                    dst = pDst+(y*dstStep);
                    src[0] = (Fw8u*)pSrc[0]+y*srcStep[0];
                    src[1] = (Fw8u*)pSrc[1]+y*srcStep[1];
                    src[2] = (Fw8u*)pSrc[2]+y*srcStep[2];

                    // Process 2pxl blocks
                    for (A32S x=0; x<w_blocks; ++x, 
                                              src[0]+=2, 
                                              ++src[1], 
                                              ++src[2], 
                                              dst+=4    )

                    {
                        dst[0] = *(src[0]);         //    Y1
                        dst[1] = *(src[1]);         //    Cb1
                        dst[2] = *(src[0]+1);       //    Y2
                        dst[3] = *(src[2]);         //    Cr1
                    }
                    // Process any remaining pxl
                    if (w_rem)
                    {
                        dst[0] = *(src[0]);         //    Y1
                        dst[1] = *(src[1]);         //    Cb1
                    }
                }
                return fwStsNoErr;
            }
        };
    }; // CMC 
}; // OPT_LEVEL

#endif // __COLORMODELCONVERSION_H__
