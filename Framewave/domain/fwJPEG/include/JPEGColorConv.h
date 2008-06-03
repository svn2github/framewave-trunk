/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __JPEGCOLORCONV_H__
#define __JPEGCOLORCONV_H__

#include "fe.h"

namespace OPT_LEVEL
{
SYS_INLINE STATIC void  Mul_16s(__m128i const &s1,__m128i const &s2,__m128i &d1,__m128i &d2)
{
    __m128i temp1,temp2;
    temp1 = _mm_mullo_epi16(s1,s2);
    temp2 = _mm_mulhi_epi16(s1,s2);
    d1    = _mm_unpacklo_epi16(temp1,temp2); 
    d2    = _mm_unpackhi_epi16(temp1,temp2);         
}

SYS_INLINE STATIC void MulCbCr_RGB(__m128i &s1lo,__m128i &s2lo,__m128i &s3lo,const XMM128 mCoef[3])
{
    __m128i s1hi,s2hi,s3hi,s1lo1,s2lo1,s3lo1,s1lo2,s2lo2,s3lo2,s1hi1,s2hi1,s3hi1,s1hi2,s2hi2,s3hi2;
    const __m128i val128 = CONST_SET1_32I(128);            
    CBL_SSE2::Unpack8UTo16U(s1lo,s1hi);
    CBL_SSE2::Unpack8UTo16U(s2lo,s2hi);
    CBL_SSE2::Unpack8UTo16U(s3lo,s3hi);

    Mul_16s(s1lo,mCoef[0].i,s1lo1,s1lo2);
    Mul_16s(s2lo,mCoef[1].i,s2lo1,s2lo2);
    Mul_16s(s3lo,mCoef[2].i,s3lo1,s3lo2);

    Mul_16s(s1hi,mCoef[0].i,s1hi1,s1hi2);
    Mul_16s(s2hi,mCoef[1].i,s2hi1,s2hi2);
    Mul_16s(s3hi,mCoef[2].i,s3hi1,s3hi2);
    
    s1lo1 = _mm_add_epi32(s1lo1,s2lo1);
    s1lo1 = _mm_add_epi32(s1lo1,s3lo1);
    s1lo1 = _mm_add_epi32(s1lo1,val128);

    s1lo2 = _mm_add_epi32(s1lo2,s2lo2);
    s1lo2 = _mm_add_epi32(s1lo2,s3lo2);
    s1lo2 = _mm_add_epi32(s1lo2,val128);

    s1hi1 = _mm_add_epi32(s1hi1,s2hi1);
    s1hi1 = _mm_add_epi32(s1hi1,s3hi1);
    s1hi1 = _mm_add_epi32(s1hi1,val128);

    s1hi2 = _mm_add_epi32(s1hi2,s2hi2);
    s1hi2 = _mm_add_epi32(s1hi2,s3hi2);
    s1hi2 = _mm_add_epi32(s1hi2,val128);

    s1lo1 = _mm_srai_epi32(s1lo1,8);
    s1lo2 = _mm_srai_epi32(s1lo2,8);
    s1hi1 = _mm_srai_epi32(s1hi1,8);
    s1hi2 = _mm_srai_epi32(s1hi2,8);

    s1lo1 = _mm_add_epi32(s1lo1,val128);
    s1lo2 = _mm_add_epi32(s1lo2,val128);
    s1hi1 = _mm_add_epi32(s1hi1,val128);
    s1hi2 = _mm_add_epi32(s1hi2,val128);

    s1lo1 = _mm_packs_epi32(s1lo1,s1lo2);
    s1hi1 = _mm_packs_epi32(s1hi1,s1hi2);
    s1lo = _mm_packus_epi16(s1lo1,s1hi1);

}

SYS_INLINE STATIC void MulY_RGB(__m128i &s1lo,__m128i &s2lo,__m128i &s3lo,const XMM128 mCoef[3])
{
    __m128i s1hi,s2hi,s3hi;
    const __m128i val128 = CONST_SET1_16I(128);     
    CBL_SSE2::Unpack8UTo16U(s1lo,s1hi);
    CBL_SSE2::Unpack8UTo16U(s2lo,s2hi);
    CBL_SSE2::Unpack8UTo16U(s3lo,s3hi);

    s1lo = _mm_mullo_epi16(s1lo,mCoef[0].i);
    s2lo = _mm_mullo_epi16(s2lo,mCoef[1].i);
    s3lo = _mm_mullo_epi16(s3lo,mCoef[2].i);

    s1hi = _mm_mullo_epi16(s1hi,mCoef[0].i);
    s2hi = _mm_mullo_epi16(s2hi,mCoef[1].i);
    s3hi = _mm_mullo_epi16(s3hi,mCoef[2].i);

    s1lo = _mm_add_epi16(s1lo,s2lo);
    s1lo = _mm_add_epi16(s1lo,s3lo);
    s1lo = _mm_add_epi16(s1lo,val128);

    s1hi = _mm_add_epi16(s1hi,s2hi);
    s1hi = _mm_add_epi16(s1hi,s3hi);
    s1hi = _mm_add_epi16(s1hi,val128);

    s1lo = _mm_srli_epi16(s1lo,8);
    s1hi = _mm_srli_epi16(s1hi,8);

    s1lo = _mm_packus_epi16(s1lo,s1hi);
}

class RGBToYCBCR_C3P3 : public fe1S3D<Fw8u,C3,Fw8u,C1,Fw8u,C1,Fw8u,C1>
{
public:
    FE_SSE2_REF
    S16 coey[3],coecb[3],coecr[3];
    XMM128 mCoeffy[3],mCoeffcb[3],mCoeffcr[3];
    RGBToYCBCR_C3P3()
    {
       //Y = 77*R + 150*G + 29*B
       coey[0] = 77, coey[1] = 150, coey[2] = 29;
       //Cb = -43 * R - 85 * G + 128 * B;
       coecb[0] = -43, coecb[1] = -85, coecb[2] = 128;
       //Cr = 128 * R - 107 * G - 21 * B;
       coecr[0] = 128, coecr[1] = -107, coecr[2] = -21;


    }
    IV SSE2_Init()
    {
        mCoeffy[0].i = _mm_set1_epi16(coey[0]);
        mCoeffy[1].i = _mm_set1_epi16(coey[1]);
        mCoeffy[2].i = _mm_set1_epi16(coey[2]);

        mCoeffcb[0].i = _mm_set1_epi16(coecb[0]);
        mCoeffcb[1].i = _mm_set1_epi16(coecb[1]);
        mCoeffcb[2].i = _mm_set1_epi16(coecb[2]);

        mCoeffcr[0].i = _mm_set1_epi16(coecr[0]);
        mCoeffcr[1].i = _mm_set1_epi16(coecr[1]);
        mCoeffcr[2].i = _mm_set1_epi16(coecr[2]);
    }

    IV SSE2( RegFile & r ) const									// SSE2 Pixel function
    {
        XMM128  regG,regB;
        r.dst[0].i = r.src1[0].i;
        regG.i = r.src1[1].i;
        regB.i = r.src1[2].i;

        CBL_SSE2::Convert_3C_to_3P_8bit(r.dst[0].i,regG.i,regB.i);
        r.dst2[0].i = r.dst[0].i; 
        r.dst3[0].i = r.dst[0].i; 

        __m128i s2=regG.i, s3=regB.i, s5=regG.i, s6=regB.i;

        MulY_RGB(r.dst[0].i,regG.i,regB.i,mCoeffy);
        MulCbCr_RGB(r.dst2[0].i,s2,s3,mCoeffcb);                
        MulCbCr_RGB(r.dst3[0].i,s5,s6,mCoeffcr);
    }  

    IV REFR( const Fw8u *s, Fw8u *d1,Fw8u *d2,Fw8u *d3) const
    {
        d1[0] = FW_REF::Limits<Fw8u>::Sat((s[0] * coey[0] + s[1] * coey[1] + s[2] * coey[2]+ 128)>>8);
        d2[0] = FW_REF::Limits<Fw8u>::Sat(((s[0] * coecb[0] + s[1] * coecb[1] + s[2] * coecb[2]+ 128)>>8) +128);
        d3[0] = FW_REF::Limits<Fw8u>::Sat(((s[0] * coecr[0] + s[1] * coecr[1] + s[2] * coecr[2]+ 128)>>8) +128);
    }

   IV SSE_32(const Fw8u *src,int srcstep,Fw8u *pDstY,Fw8u *pDstCb,Fw8u *pDstCr, int dststep)
    {
        RegFile r;
        SSE2_Init();

        for(int h=0;h<32;h++)
        {
            r.src1[0].i= _mm_loadu_si128((__m128i*)src);
            r.src1[1].i= _mm_loadu_si128((__m128i*)(src+16));
            r.src1[2].i= _mm_loadu_si128((__m128i*)(src+32));
            SSE2(r);
            _mm_storeu_si128((__m128i*)pDstY,r.dst[0].i);
            _mm_storeu_si128((__m128i*)pDstCb,r.dst2[0].i);
            _mm_storeu_si128((__m128i*)pDstCr,r.dst3[0].i);

            r.src1[0].i= _mm_loadu_si128((__m128i*)(src+48));
            r.src1[1].i= _mm_loadu_si128((__m128i*)(src+64));
            r.src1[2].i= _mm_loadu_si128((__m128i*)(src+80));
            SSE2(r);
            _mm_storeu_si128((__m128i*)(pDstY+16),r.dst[0].i);
            _mm_storeu_si128((__m128i*)(pDstCb+16),r.dst2[0].i);
            _mm_storeu_si128((__m128i*)(pDstCr+16),r.dst3[0].i);
            
            src+=srcstep;
            pDstY +=dststep;
            pDstCb +=dststep;
            pDstCr +=dststep;
        }

    }

   IV REF_CODE(const Fw8u *pSrcRGB,int srcStep,Fw8u *pDstYCbCr[3], int dstStep,const FwiSize &roiSize)
   {
	            //DEV code use shift 8 bit data for coeffcients
	            //0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	            //We use 77, 150, 29 as the modified coeff, and then shift the result
	            //-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
	            //0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
	            //The final answer is equal to nearest neighbor rounding
	            //SEE2 should use 16 bit data shift
	            unsigned short result, RVal, GVal, BVal;
	            int x, y;
	            int srcPos, dstPos;

	            for (y=0;y<roiSize.height; y++) {
		            srcPos = y*srcStep;
		            dstPos = y*dstStep;
		            for (x=0;x<roiSize.width;x++) {
			            RVal=pSrcRGB[srcPos++]; 
			            GVal=pSrcRGB[srcPos++];
			            BVal=pSrcRGB[srcPos++];
            			
			            //add 0.5 for nearest neighbor rounding
			            result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
			            pDstYCbCr [0][dstPos] = (Fw8u)(result>>8);
			            result = -43 * RVal - 85 * GVal	+ 128 * BVal + 128;
			            pDstYCbCr [1][dstPos] = (Fw8u)((result>>8)+128);
			            result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			            pDstYCbCr [2][dstPos++] = (Fw8u)((result>>8)+128);
		            }
	            }
   }

};


class RGBToY_JPEG_8u_C3C1 : public fe2<Fw8u,C3,Fw8u,C1>
{
public:
    FE_SSE2_REF
    S16 coey[3];
    XMM128 mCoeffy[3];
    RGBToY_JPEG_8u_C3C1()
    {
       //Y = 77*R + 150*G + 29*B
       coey[0] = 77, coey[1] = 150, coey[2] = 29;
    }
    IV SSE2_Init()
    {
        mCoeffy[0].i = _mm_set1_epi16(coey[0]);
        mCoeffy[1].i = _mm_set1_epi16(coey[1]);
        mCoeffy[2].i = _mm_set1_epi16(coey[2]);
    }

    IV SSE2( RegFile & r ) const									// SSE2 Pixel function
    {
        XMM128  regG,regB;
        r.dst[0].i = r.src1[0].i;
		regG.i = r.src1[1].i;
		regB.i = r.src1[2].i;
        CBL_SSE2::Convert_3C_to_3P_8bit(r.dst[0].i,regG.i,regB.i);
        MulY_RGB(r.dst[0].i,regG.i,regB.i,mCoeffy);

    }  

    IV REFR( const Fw8u *s, Fw8u *d1) const
    {
        d1[0] = FW_REF::Limits<Fw8u>::Sat((s[0] * coey[0] + s[1] * coey[1] + s[2] * coey[2]+ 128)>>8);
    }

   IV SSE_32(const Fw8u *src,int srcstep,Fw8u *pDstY,int dststep)
    {
        RegFile r;
        SSE2_Init();

		for(int h=0;h<32;h++)
        {
            r.src1[0].i= _mm_loadu_si128((__m128i*)src);
            r.src1[1].i= _mm_loadu_si128((__m128i*)(src+16));
            r.src1[2].i= _mm_loadu_si128((__m128i*)(src+32));
            SSE2(r);
            _mm_storeu_si128((__m128i*)pDstY,r.dst[0].i);

            r.src1[0].i= _mm_loadu_si128((__m128i*)(src+48));
            r.src1[1].i= _mm_loadu_si128((__m128i*)(src+64));
            r.src1[2].i= _mm_loadu_si128((__m128i*)(src+80));
            SSE2(r);
            _mm_storeu_si128((__m128i*)(pDstY+16),r.dst[0].i);
            
            src+=srcstep;
            pDstY +=dststep;
        }

    }

   IV REF_CODE(const Fw8u *pSrcRGB,int srcStep,Fw8u *pDstY, int dstStep, FwiSize roiSize)
   {
				//DEV code use shift 8 bit data for coeffcients
				//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
				//We use 77, 150, 29 as the modified coeff, and then shift the result
				//The final answer is equal to nearest neighbor rounding
				//SEE2 should use 16 bit data shift
				unsigned short result;
				int x, y;
				int srcPos, dstPos;

				for (y=0;y<roiSize.height; y++) {
					srcPos = y*srcStep;
					dstPos = y*dstStep;
					for (x=0;x<roiSize.width;x++) {
						//add 0.5 for nearest neighbor rounding
						result = 77 * pSrcRGB[srcPos] + 150 * pSrcRGB[srcPos+1]
							+ 29 * pSrcRGB[srcPos+2] + 128;
						srcPos += 3;
						pDstY [dstPos++] = (Fw8u)(result>>8);
					}
				}
   }

};

}

#endif // __JPEGCOLORCONV_H__
