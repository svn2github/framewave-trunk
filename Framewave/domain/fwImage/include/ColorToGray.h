/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __COLORTOGRAY_H__
#define __COLORTOGRAY_H__

#include "fe.h"

namespace OPT_LEVEL
{


namespace ColorToGray
{
    namespace C3C1
    {
        template<typename TD>
        struct Impl{ };

	    template<>
	    struct Impl< U8 >
	    {


            ISV Setup(XMM128 mCoeff[3],F32 coeff[3])
            {
                U16 coe[3];
                coe[0] = (U16) CBL_LIBRARY::FloatToInt( coeff[0] * 256 );
                coe[1] = (U16) CBL_LIBRARY::FloatToInt( coeff[1] * 256 );
                coe[2] = (U16) CBL_LIBRARY::FloatToInt( coeff[2] * 256 );
                mCoeff[0].i = _mm_set_epi16(coe[0],coe[0],coe[0],coe[0],coe[0],coe[0],coe[0],coe[0]);
                mCoeff[1].i = _mm_set_epi16(coe[1],coe[1],coe[1],coe[1],coe[1],coe[1],coe[1],coe[1]);
                mCoeff[2].i = _mm_set_epi16(coe[2],coe[2],coe[2],coe[2],coe[2],coe[2],coe[2],coe[2]);
            }


            IS __m128i MulRGB_Coef(__m128i &s1lo,__m128i &s2lo,__m128i &s3lo,const XMM128 mCoef[3])
            {
                __m128i s1hi,s2hi,s3hi;

                CBL_SSE2::Unpack8UTo16U(s1lo,s1hi);
                CBL_SSE2::Unpack8UTo16U(s2lo,s2hi);
                CBL_SSE2::Unpack8UTo16U(s3lo,s3hi);

                s1lo = _mm_mullo_epi16(s1lo,mCoef[0].i);
                s1hi = _mm_mullo_epi16(s1hi,mCoef[0].i);
                s2lo = _mm_mullo_epi16(s2lo,mCoef[1].i);
                s2hi = _mm_mullo_epi16(s2hi,mCoef[1].i);
                s3lo = _mm_mullo_epi16(s3lo,mCoef[2].i);
                s3hi = _mm_mullo_epi16(s3hi,mCoef[2].i);

                s1lo = _mm_srli_epi16(s1lo,8);
                s2lo = _mm_srli_epi16(s2lo,8);
                s3lo = _mm_srli_epi16(s3lo,8);

                s1lo = _mm_add_epi16(s1lo,s2lo);
                s1lo = _mm_add_epi16(s1lo,s3lo);
                
                s1hi = _mm_srli_epi16(s1hi,8);
                s2hi = _mm_srli_epi16(s2hi,8);
                s3hi = _mm_srli_epi16(s3hi,8);
                s1hi = _mm_add_epi16(s1hi,s2hi);
                s1hi = _mm_add_epi16(s1hi,s3hi);

               return  _mm_packus_epi16(s1lo,s1hi);
            }

            ISV SSE2(const XMM128 src[3],XMM128 &dst,const XMM128 mCoef[3])
            {
                __m128i s1lo  = src[0].i,s2lo = src[1].i, s3lo = src[2].i;

                ssp_convert_3c_3p_epi8(&s1lo,&s2lo,&s3lo);

                dst.i = MulRGB_Coef(s1lo,s2lo,s3lo,mCoef);
            }
	    };
	    template<>
	    struct Impl< U16 >
	    {

            IS __m128i Pack32UTo16U(__m128i &srclo,__m128i &srchi)
            {
                __m128i lo = _mm_unpacklo_epi16(srclo,srchi);
                __m128i hi = _mm_unpackhi_epi16(srclo,srchi);

                __m128i lo1 = _mm_unpacklo_epi32(lo,hi);
                __m128i hi1 = _mm_unpackhi_epi32(lo,hi);
                
                lo1 = _mm_unpacklo_epi16(lo1,hi1);
                return _mm_shuffle_epi32(lo1,_MM_SHUFFLE(3, 1, 2, 0));
            }

            ISV Setup(XMM128 mCoeff[3],F32 coeff[3])
            {
                mCoeff[0].f = _mm_set_ps(coeff[0],coeff[0],coeff[0],coeff[0]);
                mCoeff[1].f = _mm_set_ps(coeff[1],coeff[1],coeff[1],coeff[1]);
                mCoeff[2].f = _mm_set_ps(coeff[2],coeff[2],coeff[2],coeff[2]);
            }


            IS __m128i MulRGB_Coef(XMM128 &s1lo,XMM128 &s2lo,XMM128 &s3lo,const XMM128 mCoef[3])
            {
                XMM128 s1hi,s2hi,s3hi;

                CBL_SSE2::Unpack16UTo32U(s1lo.i,s1hi.i);
                CBL_SSE2::Unpack16UTo32U(s2lo.i,s2hi.i);
                CBL_SSE2::Unpack16UTo32U(s3lo.i,s3hi.i);


                s1lo.f = _mm_cvtepi32_ps(s1lo.i);
                s1hi.f = _mm_cvtepi32_ps(s1hi.i);
                s2lo.f = _mm_cvtepi32_ps(s2lo.i);
                s2hi.f = _mm_cvtepi32_ps(s2hi.i);
                s3lo.f = _mm_cvtepi32_ps(s3lo.i);
                s3hi.f = _mm_cvtepi32_ps(s3hi.i);

                s1lo.f = _mm_mul_ps(s1lo.f,mCoef[0].f);
                s1hi.f = _mm_mul_ps(s1hi.f,mCoef[0].f);
                s2lo.f = _mm_mul_ps(s2lo.f,mCoef[1].f);
                s2hi.f = _mm_mul_ps(s2hi.f,mCoef[1].f);
                s3lo.f = _mm_mul_ps(s3lo.f,mCoef[2].f);
                s3hi.f = _mm_mul_ps(s3hi.f,mCoef[2].f);

                s1lo.f = _mm_add_ps(s1lo.f,s2lo.f);
                s1lo.f = _mm_add_ps(s1lo.f,s3lo.f);

                s1hi.f = _mm_add_ps(s1hi.f,s2hi.f);
                s1hi.f = _mm_add_ps(s1hi.f,s3hi.f);

                s1lo.i = _mm_cvtps_epi32(s1lo.f);
                s1hi.i = _mm_cvtps_epi32(s1hi.f);

                return  Pack32UTo16U(s1lo.i,s1hi.i);            
            }

            ISV SSE2(const XMM128 src[3],XMM128 &dst,const XMM128 mCoef[3])
            {
                XMM128 s1lo,s2lo,s3lo;

                s1lo.i = src[0].i, s2lo.i = src[1].i, s3lo.i = src[2].i;

                // Replace CBL with SSEPlus
                // CBL_SSE2::Convert_3C_to_3P_16bit( s1lo.i,s2lo.i,s3lo.i);
                ssp_convert_3c_3p_epi16(&s1lo.i, &s2lo.i, &s3lo.i);
                dst.i = MulRGB_Coef(s1lo,s2lo,s3lo,mCoef);

            }
        };

	    template<>
	    struct Impl< S16 >
	    {
            ISV Setup(XMM128 mCoeff[3],F32 coeff[3])
            {
                Impl<U16>::Setup( mCoeff,coeff );
            }
            IS __m128i MulRGB_Coef(XMM128 &s1lo,XMM128 &s2lo,XMM128 &s3lo,const XMM128 mCoef[3])
            {
                XMM128 s1hi,s2hi,s3hi;

                CBL_SSE2::Unpack16STo32S(s1lo.i,s1hi.i);
                CBL_SSE2::Unpack16STo32S(s2lo.i,s2hi.i);
                CBL_SSE2::Unpack16STo32S(s3lo.i,s3hi.i);


                s1lo.f = _mm_cvtepi32_ps(s1lo.i);
                s1hi.f = _mm_cvtepi32_ps(s1hi.i);
                s2lo.f = _mm_cvtepi32_ps(s2lo.i);
                s2hi.f = _mm_cvtepi32_ps(s2hi.i);
                s3lo.f = _mm_cvtepi32_ps(s3lo.i);
                s3hi.f = _mm_cvtepi32_ps(s3hi.i);

                s1lo.f = _mm_mul_ps(s1lo.f,mCoef[0].f);
                s1hi.f = _mm_mul_ps(s1hi.f,mCoef[0].f);
                s2lo.f = _mm_mul_ps(s2lo.f,mCoef[1].f);
                s2hi.f = _mm_mul_ps(s2hi.f,mCoef[1].f);
                s3lo.f = _mm_mul_ps(s3lo.f,mCoef[2].f);
                s3hi.f = _mm_mul_ps(s3hi.f,mCoef[2].f);

                s1lo.f = _mm_add_ps(s1lo.f,s2lo.f);
                s1lo.f = _mm_add_ps(s1lo.f,s3lo.f);

                s1hi.f = _mm_add_ps(s1hi.f,s2hi.f);
                s1hi.f = _mm_add_ps(s1hi.f,s3hi.f);

                s1lo.i = _mm_cvtps_epi32(s1lo.f);
                s1hi.i = _mm_cvtps_epi32(s1hi.f);

                return  _mm_packs_epi32(s1lo.i,s1hi.i);
            }

            ISV SSE2(const XMM128 src[3],XMM128 &dst,const XMM128 mCoef[3])
            {
                XMM128 s1lo,s2lo,s3lo;

                s1lo.i = src[0].i, s2lo.i = src[1].i, s3lo.i = src[2].i;

                // Replace CBL with SSEPlus
                // CBL_SSE2::Convert_3C_to_3P_16bit( s1lo.i,s2lo.i,s3lo.i);
                ssp_convert_3c_3p_epi16(&s1lo.i, &s2lo.i, &s3lo.i);
                dst.i = MulRGB_Coef(s1lo,s2lo,s3lo,mCoef);

            }
        };
	    template<>
	    struct Impl< F32 >
	    {
            ISV Setup(XMM128 mCoeff[3],F32 coeff[3])
            {
                Impl<U16>::Setup( mCoeff,coeff );
            }

            ISV SSE2(const XMM128 src[3],XMM128 &dst,const XMM128 mCoef[3])
            {
                XMM128 s1,s2,s3;

                s1.f = src[0].f, s2.f = src[1].f, s3.f = src[2].f;

                // Replace CBL with SSEPlus
                // CBL_SSE2::Convert_3C_to_3P_32bit( s1.i,s2.i,s3.i);
                ssp_convert_3c_3p_epi32(&s1.i, &s2.i, &s3.i);

                s1.f = _mm_mul_ps(s1.f,mCoef[0].f);
                s2.f = _mm_mul_ps(s2.f,mCoef[1].f);
                s3.f = _mm_mul_ps(s3.f,mCoef[2].f);
 
                s1.f = _mm_add_ps(s1.f,s2.f);
                dst.f = _mm_add_ps(s1.f,s3.f);
            }
        };
    }
    namespace AC4C1
    {
        template<typename TD>
        struct Impl{ };

	    template<>
	    struct Impl< U8 >
	    {
            ISV Setup(XMM128 mCoeff[3],F32 coeff[3])
            {
                C3C1::Impl<U8>::Setup(mCoeff,coeff);
            }

            ISV SSE2(const XMM128 src[3],XMM128 &dst,const XMM128 mCoef[3])
            {
                __m128i s1lo  = src[0].i,s2lo = src[1].i, s3lo = src[2].i,s4lo = src[3].i;

                // CBL_SSE2::Convert_4C_to_4P_8bit( s1lo,s2lo,s3lo,s4lo);
                ssp_convert_4c_4p_epi8( &s1lo, &s2lo, &s3lo, &s4lo);
                
                dst.i = C3C1::Impl<U8>::MulRGB_Coef(s1lo,s2lo,s3lo,mCoef);

            }
	    };
	    template<>
	    struct Impl< U16 >
	    {
            ISV Setup(XMM128 mCoeff[3],F32 coeff[3])
            {
                C3C1::Impl<U16>::Setup(mCoeff,coeff);
            }

            ISV SSE2(const XMM128 src[3],XMM128 &dst,const XMM128 mCoef[3])
            {
                XMM128 s1lo,s2lo,s3lo,s4lo;

                s1lo.i = src[0].i, s2lo.i = src[1].i, s3lo.i = src[2].i,s4lo.i=src[3].i ;
                // CBL_SSE2::Convert_4C_to_4P_16bit( s1lo.i,s2lo.i,s3lo.i,s4lo.i);
                ssp_convert_4c_4p_epi16( &s1lo.i, &s2lo.i, &s3lo.i, &s4lo.i);
                
                dst.i = C3C1::Impl<U16>::MulRGB_Coef(s1lo,s2lo,s3lo,mCoef);
            }
        };

	    template<>
	    struct Impl< S16 >
	    {
            ISV Setup(XMM128 mCoeff[3],F32 coeff[3])
            {
                 C3C1::Impl<S16>::Setup(mCoeff,coeff);
            }

            ISV SSE2(const XMM128 src[3],XMM128 &dst,const XMM128 mCoef[3])
            {
                XMM128 s1lo,s2lo,s3lo,s4lo;

                s1lo.i = src[0].i, s2lo.i = src[1].i, s3lo.i = src[2].i,s4lo.i=src[3].i ;
                // CBL_SSE2::Convert_4C_to_4P_16bit( s1lo.i,s2lo.i,s3lo.i,s4lo.i);
                ssp_convert_4c_4p_epi16( &s1lo.i, &s2lo.i, &s3lo.i, &s4lo.i);
                
                dst.i = C3C1::Impl<S16>::MulRGB_Coef(s1lo,s2lo,s3lo,mCoef);

            }
        };
	    template<>
	    struct Impl< F32 >
	    {
            ISV Setup(XMM128 mCoeff[3],F32 coeff[3])
            {
                Impl<U16>::Setup( mCoeff,coeff );
            }

            ISV SSE2(const XMM128 src[3],XMM128 &dst,const XMM128 mCoef[3])
            {
                XMM128 s1,s2,s3,s4;

                s1.f = src[0].f, s2.f = src[1].f, s3.f = src[2].f,s4.f=src[3].f;

                // CBL_SSE2::Convert_4C_to_4P_32bit( s1.i,s2.i,s3.i,s4.i);
                ssp_convert_4c_4p_epi32( &s1.i, &s2.i, &s3.i, &s4.i);

                s1.f = _mm_mul_ps(s1.f,mCoef[0].f);
                s2.f = _mm_mul_ps(s2.f,mCoef[1].f);
                s3.f = _mm_mul_ps(s3.f,mCoef[2].f);

                s1.f = _mm_add_ps(s1.f,s2.f);
                dst.f = _mm_add_ps(s1.f,s3.f);
            }
        };
    }
}

template<typename TSrc,CH Csrc,typename TDst,CH Cdst>
class ColorToGray_C3C1 : public fe2<TSrc,Csrc,TDst,Cdst>
{
public:
    FE_SSE2_REF
    F32 coe[3];
    XMM128 mCoeff[3];
    ColorToGray_C3C1(const Fw32f coeffs[3])
    {
       coe[0] = coeffs[0];
       coe[1] = coeffs[1];
       coe[2] = coeffs[2];
    }
    IV SSE2_Init()
    {
        ColorToGray::C3C1::Impl<TSrc>::Setup(mCoeff,coe);
    }

    IV SSE2( RegFile & r ) const									// SSE2 Pixel function
    {
	    ColorToGray::C3C1::Impl<TSrc>::SSE2(r.src1,r.dst[0],mCoeff);
    }  
    IV REFR( const TSrc *s, TDst *d ) const
    {
        d[0] = FW_REF::Limits<TSrc>::Sat(s[0] * coe[0] + s[1] * coe[1] + s[2] * coe[2]);
    }

};

template<typename TSrc,CH Csrc,typename TDst,CH Cdst>
class ColorToGray_AC4C1 : public fe2<TSrc,Csrc,TDst,Cdst>
{
public:

    FE_CUSTOM_SSE2_REF
    F32 coe[3];
    XMM128 mCoeff[3];
    ColorToGray_AC4C1(const Fw32f coeffs[3])
    {
       coe[0] = coeffs[0];
       coe[1] = coeffs[1];
       coe[2] = coeffs[2];
    }
    IV SSE2_Init()
    {
        ColorToGray::AC4C1::Impl<TSrc>::Setup(mCoeff,coe);
    }

    IV REFR( const TSrc *s, TDst *d ) const
    {
        d[0] = FW_REF::Limits<TSrc>::Sat(s[0] * coe[0] + s[1] * coe[1] + s[2] * coe[2]);
    }

    template<IsAlign ia>
	IV Custom1D_sse2(const TSrc *s, TDst *d, U32 pixCount) const 
	{ 

        __m128i *src	= (__m128i*)s;
        __m128i *dst	= (__m128i*)d;
        int rem = (pixCount * ChannelCount(Csrc)) % (64/sizeof(TSrc));

        __m128i *end	= (__m128i *)   (s + (pixCount * ChannelCount(Csrc)) - rem );

        XMM128 val[4];	
        XMM128 dest;

        for( ; src < end; )
        {
            LoadStoreModules::LOAD<16,DT_SSE2,ia,STREAM_FLSE>(&val[0],(const void*)src++);
            LoadStoreModules::LOAD<16,DT_SSE2,ia,STREAM_FLSE>(&val[1],(const void*)src++);
            LoadStoreModules::LOAD<16,DT_SSE2,ia,STREAM_FLSE>(&val[2],(const void*)src++);
            LoadStoreModules::LOAD<16,DT_SSE2,ia,STREAM_FLSE>(&val[3],(const void*)src++);

            ColorToGray::AC4C1::Impl<TSrc>::SSE2(val,dest,mCoeff);

            LoadStoreModules::STORE<16,DT_SSE2,ia,STREAM_TRUE>(&dest,(void*)dst++);
        }
        _mm_mfence();

        TSrc *e = (TSrc*)end + rem;
        TSrc *srend = (TSrc*) src;
        TDst *sedest = (TDst *)dst;

        for(;srend<e; sedest  += CH_COUNT(Cdst),
                       srend += CH_COUNT(Csrc) )
               REFR( srend, sedest );        
	}
};

}

#endif // __COLORTOGRAY_H__
