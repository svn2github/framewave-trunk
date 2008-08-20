/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __JPEGCOLORCONV_H__
#define __JPEGCOLORCONV_H__

#include "fe.h"

namespace OPT_LEVEL
{
SYS_INLINE static void  Mul_16s(__m128i const &s1,__m128i const &s2,__m128i &d1,__m128i &d2)
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

SYS_INLINE STATIC  void MulY_RGB(__m128i &s1lo,__m128i &s2lo,__m128i &s3lo,const XMM128 mCoef[3])
{
    __m128i s1hi,s2hi,s3hi;
    static const __m128i val128 = CONST_SET1_16I(128);     
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

        mCoeffcr[0].i = mCoeffcb[2].i;
        mCoeffcr[1].i = _mm_set1_epi16(coecr[1]);
        mCoeffcr[2].i = _mm_set1_epi16(coecr[2]);
    }

    IV SSE2( RegFile & r ) const									// SSE2 Pixel function
    {
        XMM128  regG,regB;
        r.dst[0].i = r.src1[0].i;
        regG.i = r.src1[1].i;
        regB.i = r.src1[2].i;

        ssp_convert_3c_3p_epi8(&r.dst[0].i,&regG.i,&regB.i);

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

   IV REF_CODE(const Fw8u *pSrcRGB,int srcStep,Fw8u * const pDstYCbCr[3], int dstStep,const FwiSize &roiSize)
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




class CMYKToYCCK_JPEG_8u_C4P4R : public fe1S4D<Fw8u,C4,Fw8u,C1,Fw8u,C1,Fw8u,C1,Fw8u,C1>
{
public:
    FE_SSE2_REF
    S16 coey[3],coecb[3],coecr[3];
    XMM128 mCoeffy[3],mCoeffcb[3],mCoeffcr[3];
	XMM128 mMask;
    CMYKToYCCK_JPEG_8u_C4P4R()
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

        mCoeffcr[0].i = mCoeffcb[2].i;
        mCoeffcr[1].i = _mm_set1_epi16(coecr[1]);
        mCoeffcr[2].i = _mm_set1_epi16(coecr[2]);
		mMask.i = _mm_set1_epi32(0xFFFFFFFF);
    }

    IV SSE2( RegFile & r ) const									// SSE2 Pixel function
    {
        XMM128  regG,regB,alpha;
        r.dst[0].i = r.src1[0].i;
        regG.i = r.src1[1].i;
        regB.i = r.src1[2].i;
		alpha.i = r.src1[3].i;


        ssp_convert_4c_4p_epi8(&r.dst[0].i,&regG.i,&regB.i,&alpha.i);
		r.dst[0].i = _mm_andnot_si128(r.dst[0].i,mMask.i);
		regG.i = _mm_andnot_si128(regG.i,mMask.i);
		regB.i = _mm_andnot_si128(regB.i,mMask.i);

        r.dst2[0].i = r.dst[0].i; 
        r.dst3[0].i = r.dst[0].i; 
		r.dst4[0].i = alpha.i;


        __m128i s2=regG.i, s3=regB.i, s5=regG.i, s6=regB.i;

        MulY_RGB(r.dst[0].i,regG.i,regB.i,mCoeffy);
        MulCbCr_RGB(r.dst2[0].i,s2,s3,mCoeffcb);                
        MulCbCr_RGB(r.dst3[0].i,s5,s6,mCoeffcr);
    }  

    IV REFR( const Fw8u *s, Fw8u *d1,Fw8u *d2,Fw8u *d3,Fw8u *d4) const
    {
		Fw8u RVal = ~(s[0]);//R=255-C
		Fw8u GVal = ~(s[1]);//G=255-M
		Fw8u BVal = ~(s[2]);//B=255-Y


        d1[0] = FW_REF::Limits<Fw8u>::Sat((RVal * coey[0] + GVal * coey[1] + BVal * coey[2]+ 128)>>8);
        d2[0] = FW_REF::Limits<Fw8u>::Sat(((RVal * coecb[0] + GVal * coecb[1] + BVal * coecb[2]+ 128)>>8) +128);
        d3[0] = FW_REF::Limits<Fw8u>::Sat(((RVal * coecr[0] + GVal * coecr[1] + BVal * coecr[2]+ 128)>>8) +128);
		d4[0] = s[3];
    }

   IV SSE_32(const Fw8u *src,int srcstep,Fw8u *pDstY,Fw8u *pDstCb,Fw8u *pDstCr,Fw8u *alpha, int dststep)
    {
        RegFile r;
        SSE2_Init();

        for(int h=0;h<32;h++)
        {
            r.src1[0].i= _mm_loadu_si128((__m128i*)src);
            r.src1[1].i= _mm_loadu_si128((__m128i*)(src+16));
            r.src1[2].i= _mm_loadu_si128((__m128i*)(src+32));
			r.src1[3].i= _mm_loadu_si128((__m128i*)(src+48));
            SSE2(r);
            _mm_storeu_si128((__m128i*)pDstY,r.dst[0].i);
            _mm_storeu_si128((__m128i*)pDstCb,r.dst2[0].i);
            _mm_storeu_si128((__m128i*)pDstCr,r.dst3[0].i);
            _mm_storeu_si128((__m128i*)alpha,r.dst4[0].i);

            r.src1[0].i= _mm_loadu_si128((__m128i*)(src+64));
            r.src1[1].i= _mm_loadu_si128((__m128i*)(src+80));
            r.src1[2].i= _mm_loadu_si128((__m128i*)(src+96));
            r.src1[3].i= _mm_loadu_si128((__m128i*)(src+112));
            SSE2(r);
            _mm_storeu_si128((__m128i*)(pDstY+16),r.dst[0].i);
            _mm_storeu_si128((__m128i*)(pDstCb+16),r.dst2[0].i);
            _mm_storeu_si128((__m128i*)(pDstCr+16),r.dst3[0].i);
            _mm_storeu_si128((__m128i*)(alpha+16),r.dst4[0].i);
            
            src+=srcstep;
            pDstY +=dststep;
            pDstCb +=dststep;
            pDstCr +=dststep;
            alpha +=dststep;
        }

    }

   IV REF_CODE(const Fw8u * const pSrcCMYK,int srcStep,Fw8u * const pDstYCCK[3], int dstStep,const FwiSize &roiSize)
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
						RVal = ~(pSrcCMYK[srcPos++]);//R=255-C
						GVal = ~(pSrcCMYK[srcPos++]);//G=255-M
						BVal = ~(pSrcCMYK[srcPos++]);//B=255-Y
            			
					//add 0.5 for nearest neighbor rounding
					result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
					pDstYCCK [0][dstPos] = (Fw8u)(result>>8);
					result = -43 * RVal - 85 * GVal + 128 * BVal + 128;
					pDstYCCK [1][dstPos] = (Fw8u)((result>>8)+128);
					result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
					pDstYCCK [2][dstPos] = (Fw8u)((result>>8)+128);
					pDstYCCK [3][dstPos++] = pSrcCMYK[srcPos++];
		            }
	            }
   }

   //IV SSE_CODE(const Fw8u * const pSrcCMYK,int srcStep,Fw8u * const pDstYCCK[3], int dstStep,const FwiSize &roiSize)
   IV SSE_CODE(const Fw8u *src,int srcStep,Fw8u *pDstY,Fw8u *pDstCb,Fw8u *pDstCr,Fw8u *alpha, int dstStep,const FwiSize &roiSize)
   {
	            //DEV code use shift 8 bit data for coeffcients
	            //0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	            //We use 77, 150, 29 as the modified coeff, and then shift the result
	            //-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
	            //0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
	            //The final answer is equal to nearest neighbor rounding
	            //SEE2 should use 16 bit data shift
                RegFile r;
                SSE2_Init();

	            unsigned short result, RVal, GVal, BVal;
	            int x, y;
	            //int srcPos, dstPos;

	            for (y=0;y<roiSize.height; y++) {
		            //srcPos = y*srcStep;
		           // dstPos = y*dstStep;
                    const Fw8u* pSrc = src + y*srcStep;
                    Fw8u* tmpDstY = pDstY + y*dstStep;;
                    Fw8u* tmpDstCb = pDstCb + y*dstStep;;
                    Fw8u* tmpDstCr =    pDstCr + y*dstStep;
                    Fw8u* tmpAlpha =    alpha + y*dstStep;
		            for (x=0;x<roiSize.width-16;x=x+16) {
					//	RVal = ~(pSrcCMYK[srcPos++]);//R=255-C
					//	GVal = ~(pSrcCMYK[srcPos++]);//G=255-M
					//	BVal = ~(pSrcCMYK[srcPos++]);//B=255-Y
     //       			
					////add 0.5 for nearest neighbor rounding
					//result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
					//pDstYCCK [0][dstPos] = (Fw8u)(result>>8);
					//result = -43 * RVal - 85 * GVal + 128 * BVal + 128;
					//pDstYCCK [1][dstPos] = (Fw8u)((result>>8)+128);
					//result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
					//pDstYCCK [2][dstPos] = (Fw8u)((result>>8)+128);
					//pDstYCCK [3][dstPos++] = pSrcCMYK[srcPos++];
                        r.src1[0].i= _mm_loadu_si128((__m128i*)pSrc);
                        r.src1[1].i= _mm_loadu_si128((__m128i*)(pSrc+16));
                        r.src1[2].i= _mm_loadu_si128((__m128i*)(pSrc+32));
			            r.src1[3].i= _mm_loadu_si128((__m128i*)(pSrc+48));
                        SSE2(r);
                        _mm_storeu_si128((__m128i*)tmpDstY,r.dst[0].i);
                        _mm_storeu_si128((__m128i*)tmpDstCb,r.dst2[0].i);
                        _mm_storeu_si128((__m128i*)tmpDstCr,r.dst3[0].i);
                        _mm_storeu_si128((__m128i*)tmpAlpha,r.dst4[0].i);

                        pSrc+=64;
                        tmpDstY +=16;
                        tmpDstCb +=16;
                        tmpDstCr +=16;
                        tmpAlpha +=16;
		            }

                    for (;x<roiSize.width;x++) {
						RVal = ~(*pSrc++);//R=255-C
						GVal = ~(*pSrc++);//G=255-M
						BVal = ~(*pSrc++);//B=255-Y
            			
					//add 0.5 for nearest neighbor rounding
					    result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
					    *tmpDstY++ = (Fw8u)(result>>8);
					    result = -43 * RVal - 85 * GVal + 128 * BVal + 128;
					    *tmpDstCb++ = (Fw8u)((result>>8)+128);
					    result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
					    *tmpDstCr++ = (Fw8u)((result>>8)+128);
					    *tmpAlpha++ = *pSrc++;
		            }
	            }
   }

};


/////////
class RGBToY_JPEG_8u_C3C1 : public fe2<Fw8u,C3,Fw8u,C1>
{
public:
    FE_SSE2_REF
    S16 coey[3];
    XMM128 coeffR,coeffG,coeffB;
    RGBToY_JPEG_8u_C3C1()
    {
       //Y = 77*R + 150*G + 29*B
       coey[0] = 77, coey[1] = 150, coey[2] = 29;
    }
    IV SSE2_Init()
    {
        coeffR.i  = _mm_set1_epi16(coey[0]);
        coeffG.i  = _mm_set1_epi16(coey[1]);
        coeffB.i = _mm_set1_epi16(coey[2]);
    }

    IV SSE2( RegFile & r ) const									// SSE2 Pixel function
    {
        XMM128  regG,regB;
        r.dst[0].i = r.src1[0].i;
		regG.i = r.src1[1].i;
		regB.i = r.src1[2].i;

        ssp_convert_3c_3p_epi8(&r.dst[0].i,&regG.i,&regB.i);

		__m128i s1hi,s2hi,s3hi;  

		CBL_SSE2::Unpack8UTo16U(r.dst[0].i,s1hi);
		CBL_SSE2::Unpack8UTo16U(regG.i,s2hi);
		CBL_SSE2::Unpack8UTo16U(regB.i,s3hi);

		r.dst[0].i = _mm_mullo_epi16(r.dst[0].i,coeffR.i);
		regG.i = _mm_mullo_epi16(regG.i,coeffG.i);
		regB.i = _mm_mullo_epi16(regB.i,coeffB.i);

		s1hi = _mm_mullo_epi16(s1hi,coeffR.i);
		s2hi = _mm_mullo_epi16(s2hi,coeffG.i);
		s3hi = _mm_mullo_epi16(s3hi,coeffB.i);

		r.dst[0].i = _mm_add_epi16(r.dst[0].i,regG.i);
		r.dst[0].i = _mm_add_epi16(r.dst[0].i,regB.i);

		__m128i val128 = _mm_srli_epi16(coeffR.i,7);
		val128 = _mm_slli_epi16(val128,7);

		r.dst[0].i = _mm_add_epi16(r.dst[0].i,val128);

		s1hi = _mm_add_epi16(s1hi,s2hi);
		s1hi = _mm_add_epi16(s1hi,s3hi);
		s1hi = _mm_add_epi16(s1hi,val128);

		r.dst[0].i = _mm_srli_epi16(r.dst[0].i,8);
		s1hi = _mm_srli_epi16(s1hi,8);

		r.dst[0].i = _mm_packus_epi16(r.dst[0].i,s1hi);


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




/////////////////

class RGBToY_JPEG_8u_P3C1R : public fe4<Fw8u,C1,Fw8u,C1,Fw8u,C1,Fw8u,C1>
{
public:
    FE_SSE2_REF
    S16 coey[3];
    XMM128 coeffR,coeffG,coeffB;
    RGBToY_JPEG_8u_P3C1R()
    {
       //Y = 77*R + 150*G + 29*B
       coey[0] = 77, coey[1] = 150, coey[2] = 29;
    }
    IV SSE2_Init()
    {
        coeffR.i  = _mm_set1_epi16(coey[0]);
        coeffG.i  = _mm_set1_epi16(coey[1]);
        coeffB.i = _mm_set1_epi16(coey[2]);
    }

    IV SSE2( RegFile & r ) const									// SSE2 Pixel function
    {
        XMM128  regG,regB;
        r.dst[0].i = r.src1[0].i;
		regG.i = r.src2[0].i;
		regB.i = r.src3[0].i;

 
		__m128i s1hi,s2hi,s3hi;  

		CBL_SSE2::Unpack8UTo16U(r.dst[0].i,s1hi);
		CBL_SSE2::Unpack8UTo16U(regG.i,s2hi);
		CBL_SSE2::Unpack8UTo16U(regB.i,s3hi);

		r.dst[0].i = _mm_mullo_epi16(r.dst[0].i,coeffR.i);
		regG.i = _mm_mullo_epi16(regG.i,coeffG.i);
		regB.i = _mm_mullo_epi16(regB.i,coeffB.i);

		s1hi = _mm_mullo_epi16(s1hi,coeffR.i);
		s2hi = _mm_mullo_epi16(s2hi,coeffG.i);
		s3hi = _mm_mullo_epi16(s3hi,coeffB.i);

		r.dst[0].i = _mm_add_epi16(r.dst[0].i,regG.i);
		r.dst[0].i = _mm_add_epi16(r.dst[0].i,regB.i);

		__m128i val128 = _mm_srli_epi16(coeffR.i,7);
		val128 = _mm_slli_epi16(val128,7);

		r.dst[0].i = _mm_add_epi16(r.dst[0].i,val128);

		s1hi = _mm_add_epi16(s1hi,s2hi);
		s1hi = _mm_add_epi16(s1hi,s3hi);
		s1hi = _mm_add_epi16(s1hi,val128);

		r.dst[0].i = _mm_srli_epi16(r.dst[0].i,8);
		s1hi = _mm_srli_epi16(s1hi,8);

		r.dst[0].i = _mm_packus_epi16(r.dst[0].i,s1hi);


    }  

    IV REFR( const Fw8u *r,const Fw8u *g,const Fw8u *b, Fw8u *d1) const
    {
        d1[0] = FW_REF::Limits<Fw8u>::Sat((r[0] * coey[0] + g[0] * coey[1] + b[0] * coey[2]+ 128)>>8);
    }

   IV SSE_32(const Fw8u *src1,const Fw8u *src2,const Fw8u *src3,int srcstep,Fw8u *pDstY,int dststep)
    {
        RegFile r;
        SSE2_Init();

		for(int h=0;h<32;h++)
        {
            r.src1[0].i= _mm_loadu_si128((__m128i*)src1);
            r.src2[0].i= _mm_loadu_si128((__m128i*)(src2));
            r.src3[0].i= _mm_loadu_si128((__m128i*)(src3));
            SSE2(r);
            _mm_storeu_si128((__m128i*)pDstY,r.dst[0].i);

            r.src1[0].i= _mm_loadu_si128((__m128i*)(src1+16));
            r.src2[0].i= _mm_loadu_si128((__m128i*)(src2+16));
            r.src3[0].i= _mm_loadu_si128((__m128i*)(src3+16));
            SSE2(r);
            _mm_storeu_si128((__m128i*)(pDstY+16),r.dst[0].i);
            
            src1+=srcstep;
            src2+=srcstep;
            src3+=srcstep;
            pDstY +=dststep;
        }

    }

   IV REF_CODE(const Fw8u *pSrc1,const Fw8u *pSrc2,const Fw8u *pSrc3,int srcStep,Fw8u *pDstY, int dstStep, FwiSize roiSize)
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
						result = 77 * pSrc1[srcPos] + 150 * pSrc2[srcPos]
							+ 29 * pSrc3[srcPos] + 128;
						srcPos ++;
						pDstY [dstPos++] = (Fw8u)(result>>8);
					}
				}
   }

};




/////////////////



class BGRToY_JPEG_8u_C3C1 : public fe2<Fw8u,C3,Fw8u,C1>
{
public:
    FE_SSE2_REF
    S16 coey[3];
    XMM128 coeffR,coeffG,coeffB;
    BGRToY_JPEG_8u_C3C1()
    {
       //Y = 77*R + 150*G + 29*B
       coey[0] = 77, coey[1] = 150, coey[2] = 29;
    }
    IV SSE2_Init()
    {
        coeffR.i  = _mm_set1_epi16(coey[0]);
        coeffG.i  = _mm_set1_epi16(coey[1]);
        coeffB.i = _mm_set1_epi16(coey[2]);
    }

    IV SSE2( RegFile & r ) const									// SSE2 Pixel function
    {
        XMM128  regG,regB;
        r.dst[0].i = r.src1[0].i;
		regG.i = r.src1[1].i;
		regB.i = r.src1[2].i;

        ssp_convert_3c_3p_epi8(&r.dst[0].i,&regG.i,&regB.i);

		__m128i s1hi,s2hi,s3hi;  

		CBL_SSE2::Unpack8UTo16U(r.dst[0].i,s1hi);
		CBL_SSE2::Unpack8UTo16U(regG.i,s2hi);
		CBL_SSE2::Unpack8UTo16U(regB.i,s3hi);

		r.dst[0].i = _mm_mullo_epi16(r.dst[0].i,coeffB.i);
		regG.i = _mm_mullo_epi16(regG.i,coeffG.i);
		regB.i = _mm_mullo_epi16(regB.i,coeffR.i);

		s1hi = _mm_mullo_epi16(s1hi,coeffB.i);
		s2hi = _mm_mullo_epi16(s2hi,coeffG.i);
		s3hi = _mm_mullo_epi16(s3hi,coeffR.i);

		r.dst[0].i = _mm_add_epi16(r.dst[0].i,regG.i);
		r.dst[0].i = _mm_add_epi16(r.dst[0].i,regB.i);

		__m128i val128 = _mm_srli_epi16(coeffR.i,7);
		val128 = _mm_slli_epi16(val128,7);

		r.dst[0].i = _mm_add_epi16(r.dst[0].i,val128);

		s1hi = _mm_add_epi16(s1hi,s2hi);
		s1hi = _mm_add_epi16(s1hi,s3hi);
		s1hi = _mm_add_epi16(s1hi,val128);

		r.dst[0].i = _mm_srli_epi16(r.dst[0].i,8);
		s1hi = _mm_srli_epi16(s1hi,8);

		r.dst[0].i = _mm_packus_epi16(r.dst[0].i,s1hi);


    }  

    IV REFR( const Fw8u *s, Fw8u *d1) const
    {
        d1[0] = FW_REF::Limits<Fw8u>::Sat((s[0] * coey[2] + s[1] * coey[1] + s[2] * coey[0]+ 128)>>8);
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
						result = 29 * pSrcRGB[srcPos] + 150 * pSrcRGB[srcPos+1]
							+ 77 * pSrcRGB[srcPos+2] + 128;
						srcPos += 3;
						pDstY [dstPos++] = (Fw8u)(result>>8);
					}
				}
   }

};



SYS_INLINE static __m128i Compute_R(const __m128i &Y,const __m128i &Cr)//,const __m128i &mCoeff)
{
    
		// R = Y + 1.402*Cr - 179.456
        __m128i d1;//,d2;
        const __m128i val180 = CONST_SET1_16I(180);
        const __m128i mCoeff = CONST_SET1_16I(90);
        //Mul_16s(Cr,mCoeff,d1,d2);
		d1 = _mm_mullo_epi16(Cr,mCoeff);

        d1 = _mm_srli_epi16(d1,6);
        //d2 = _mm_srli_epi32(d2,8);
        
       // __m128i rCr = _mm_packs_epi32(d1,d2);        
        d1 = _mm_sub_epi16(d1,val180);
        return _mm_add_epi16(d1,Y);

}

SYS_INLINE static __m128i Compute_G(__m128i &Y,__m128i &Cb,__m128i &Cr)//,const __m128i &mCoeffCb,const __m128i &mCoeffCr)
{
           // G = Y - (0.34414*Cb + 0.71414*Cr) + 135.45984
        __m128i d1,d2;
        const __m128i val135 = CONST_SET1_16I(135);
        const __m128i mCoeffCb = CONST_SET1_16I(22);
        const __m128i mCoeffCr = CONST_SET1_16I(46);
		d1 = _mm_mullo_epi16(Cb,mCoeffCb);
		d2 = _mm_mullo_epi16(Cr,mCoeffCr);
        
        d1 = _mm_add_epi16(d1,d2);

        d1 = _mm_srli_epi16(d1,6);
        
        d1 = _mm_sub_epi16(Y,d1);
        return  _mm_add_epi16(d1,val135);

}

SYS_INLINE static __m128i Compute_B(__m128i &Y,__m128i &Cb)//,const __m128i &mCoeffCb)
{
                 // B = Y + 1.772*Cb - 226.816
        __m128i d1;
        const __m128i val227 = CONST_SET1_16I(227);
        const __m128i mCoeffCb = CONST_SET1_16I(113);
		d1 = _mm_mullo_epi16(Cb,mCoeffCb);
        
        d1 = _mm_srli_epi16(d1,6);

        d1 = _mm_sub_epi16(d1,val227);
        return  _mm_add_epi16(Y,d1);

}



static SYS_INLINE void YCbCrToRGBConv(__m128i &y,__m128i &cb,__m128i &cr)
{
                    __m128i r, g, b;
                    static const __m128i val180 = CONST_SET1_32I((S16)(180*64.0));
					static const __m128i val135 = CONST_SET1_32I((S16)(135*64.0));
					static const __m128i val226 = CONST_SET1_32I((S16)(226*64.0));

                    static const __m128i val90 = CONST_SET1_16I		( (S16)90		);		// R = ( 1.402*2^6 )
                    static const __m128i val46 = CONST_SET1_16I		( (S16)(-46)	);		// G = ( -.71414*2^6 )
                    static const __m128i val22 = CONST_SET1_16I		( (S16)(-22)	);		// cr= ( -0.34414*(2^6) )
                    static const __m128i val113 = CONST_SET1_16I( (S16)(113)	);		// B = ( 1.772*(2^6) )

					//R = Y + 1.402*Cr - 179,456
					//G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
					//B = Y + 1.772*Cb - 226.816
					//90, 22, 46,113;
                    // RED
                    __m128i r1,r2;
                    Mul_16s( val90, cr,r1,r2);		// R = ( 1.402*Cr )*(2^6)
                    r1 = _mm_sub_epi32		( r1, val180	);		// R = ( 1.402*Cr - 180)*(2^6)
                    r2 = _mm_sub_epi32		( r2, val180	);		// R = ( 128.5 + 1.402*Cr )*(2^6)

                    r1 = _mm_srai_epi32		( r1, 6			);		// R = ((128.5 + 1.402*Cr )*(2^6)) / (2^6)
                    r2 = _mm_srai_epi32		( r2, 6			);		// R = ((128.5 + 1.402*Cr )*(2^6)) / (2^6)
                    r  = _mm_packs_epi32    (r1,r2);
                    r  = _mm_adds_epi16		( r, y			);		// R = Y + 128.5 + 1.402*Cr

                    // GREEN
                    __m128i g1,g2,g3,g4;
                    Mul_16s( val46, cr,g1,g2);		// G = ( -.71414*Cr )*(2^6)
                    g1 = _mm_add_epi32		( g1, val135	);		// G = ( -.71414*Cr + 128.5 )*(2^6)
                    g2 = _mm_add_epi32		( g2, val135	);		// G = ( -.71414*Cr + 128.5 )*(2^6)

                    Mul_16s( val22, cb,g3,g4);		// cr= ( -0.34414*Cb )*(2^6)


                    g1 = _mm_add_epi32		( g1, g3			);		// G = ( -.71414*Cr + 128.5 + (-0.34414*Cb) )*(2^6)
                    g2 = _mm_add_epi32		( g2, g4			);		// G = ( -.71414*Cr + 128.5 + (-0.34414*Cb) )*(2^6)
                    g1 = _mm_srai_epi32		( g1, 6			);		// G = ((-.71414*Cr + 128.5 + (-0.34414*Cb) )*(2^6)) / (2^6)
                    g2 = _mm_srai_epi32		( g2, 6			);		// G = ((-.71414*Cr + 128.5 + (-0.34414*Cb) )*(2^6)) / (2^6)
        
                    __m128i y1=y,y2;

                    CBL_SSE2::Unpack16STo32S(y1,y2);
                    g1 = _mm_add_epi32		( g1, y1			);		// G = Y - 0.34414*Cb - 0.71414*Cr + 128.5 
                    g2 = _mm_add_epi32		( g2, y2			);		// G = Y - 0.34414*Cb - 0.71414*Cr + 128.5 

                    g = _mm_packs_epi32(g1,g2);

                    // BLUE
                    __m128i b1,b2;
                    Mul_16s		( val113, cb,b1,b2			);		// B = ( 1.772*Cb )*(2^6)
                    b1 = _mm_sub_epi32		( b1, val226	);		// B = ( 1.772*Cb + 128.5 )*(2^6)
                    b2 = _mm_sub_epi32		( b2, val226	);		// B = ( 1.772*Cb + 128.5 )*(2^6)

                    b1 = _mm_srai_epi16		( b1, 6			);		// B = ((1.772*Cb + 128.5 )*(2^6)) / (2^6)
                    b2 = _mm_srai_epi16		( b2, 6			);		// B = ((1.772*Cb + 128.5 )*(2^6)) / (2^6)
                    b = _mm_packs_epi32(b1,b2);
                    b = _mm_adds_epi16		( b, y			);		// B = Y + 1.772*Cb + 128.5

                    // CBL_SSE2::Convert_3P_to_3C_16bit( r, g, b);
//                   ssp_convert_3p_3c_epi16( &r, &g, &b);
                    // r = {g2,r2,b1,g1,r1,b0,g0,r0}
                    // g = {r5,b4,g4,r4,b3,g3,r3,b2}
                    // b = {b7,g7,r7,b6,g6,r6,b5,g5}
					y = r;
					cb= g;
					cr = b;

}



class YCbCrToRGB_JPEG_8u_P3C3R : public fe4<Fw8u,C1,Fw8u,C1,Fw8u,C1,Fw8u,C3>
{
public:
    FE_SSE2_REF
    S16 coe[4];
 //   XMM128 mCoeff[4];
    YCbCrToRGB_JPEG_8u_P3C3R()
    {
       // R = Y + 1.402*Cr - 179.456
       // G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
       // B = Y + 1.772*Cb - 226.816
		//90  Coeffs * 2^6
		//22
		//46
		//113

       coe[0] = 90, coe[1] = 22, coe[2] = 46,coe[3]=113;
    }

    IV SSE2( RegFile & r ) const									// SSE2 Pixel function
    {
 		XMM128 Y,Cb,Cr,Y1,Cb1,Cr1;

        Y.i = r.src1[0].i;
        Cb.i = r.src2[0].i;
        Cr.i = r.src3[0].i;

	    CBL_SSE2::Unpack8UTo16U(Y.i,Y1.i);
        CBL_SSE2::Unpack8UTo16U(Cb.i,Cb1.i);
        CBL_SSE2::Unpack8UTo16U(Cr.i,Cr1.i);


		YCbCrToRGBConv(Y.i,Cb.i,Cr.i);
		YCbCrToRGBConv(Y1.i,Cb1.i,Cr1.i);


		r.dst[0].i = _mm_packus_epi16(Y.i,Y1.i);  
		r.dst[1].i = _mm_packus_epi16(Cb.i,Cb1.i);  
		r.dst[2].i = _mm_packus_epi16(Cr.i,Cr1.i);  
		ssp_convert_3p_3c_epi8(&r.dst[0].i,&r.dst[1].i,&r.dst[2].i);

    }  

   IV SSE_32(const Fw8u *Y,const Fw8u *Cb,const Fw8u *Cr,int srcstep,Fw8u *pDst,int dststep)
    {
        RegFile r;
		for(int h=0;h<32;h++)
        {
            r.src1[0].i= _mm_loadu_si128((__m128i*)Y);
            r.src2[0].i= _mm_loadu_si128((__m128i*)Cb);
            r.src3[0].i= _mm_loadu_si128((__m128i*)Cr);
            SSE2(r);
            _mm_storeu_si128((__m128i*)pDst,r.dst[0].i);
            _mm_storeu_si128((__m128i*)(pDst + 16),r.dst[1].i);
            _mm_storeu_si128((__m128i*)(pDst + 32),r.dst[2].i);

            r.src1[0].i= _mm_loadu_si128((__m128i*)(Y+16));
            r.src2[0].i= _mm_loadu_si128((__m128i*)(Cb+16));
            r.src3[0].i= _mm_loadu_si128((__m128i*)(Cr+16));
            SSE2(r);
            _mm_storeu_si128((__m128i*)(pDst + 48 ),r.dst[0].i);
            _mm_storeu_si128((__m128i*)(pDst + 64),r.dst[1].i);
            _mm_storeu_si128((__m128i*)(pDst + 80),r.dst[2].i);
            
            Y+=srcstep;
            Cb+=srcstep;
            Cr+=srcstep;
            pDst +=dststep;
        }

    }

   IV SSE_16(const Fw8u *Y,const Fw8u *Cb,const Fw8u *Cr,int srcstep,Fw8u *pDst,int dststep)
    {
        RegFile r;
		for(int h=0;h<16;h++)
        {
            r.src1[0].i= _mm_loadu_si128((__m128i*)Y);
            r.src2[0].i= _mm_loadu_si128((__m128i*)Cb);
            r.src3[0].i= _mm_loadu_si128((__m128i*)Cr);
            SSE2(r);
            _mm_storeu_si128((__m128i*)pDst,r.dst[0].i);
            _mm_storeu_si128((__m128i*)(pDst + 16),r.dst[1].i);
            _mm_storeu_si128((__m128i*)(pDst + 32),r.dst[2].i);

            Y+=srcstep;
            Cb+=srcstep;
            Cr+=srcstep;
            pDst +=dststep;
        }

    }


    IV REFR( const Fw8u *Y,const Fw8u *Cb,const Fw8u *Cr, Fw8u *d) const
    {

        d[0] = CBL_LIBRARY::Limits<Fw8u>::Sat(Y[0] + ((int)((int)coe[0] * Cr[0])>>6) - (int)180);
        d[1] = CBL_LIBRARY::Limits<Fw8u>::Sat(Y[0] - ((int)(((int)coe[1]* Cb[0]) + ((int)coe[2]* Cr[0]))>>6) + (int)135);
        d[2] = CBL_LIBRARY::Limits<Fw8u>::Sat(Y[0] + ((int)((int)coe[3] * Cb[0])>>6) - (int)227);
    }
 
    IV REF_CODE(const Fw8u *pSrcYCbCr[3], int srcStep,Fw8u *pDstRGB, int dstStep, FwiSize roiSize)
    {
       // R = Y + 1.402*Cr - 179.456
       // G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
       // B = Y + 1.772*Cb - 226.816

        //Reference code only.
        //SSE2 code need to shift 16 bit 
        int x, y;
        int srcPos, dstPos;

        for (y=0;y<roiSize.height; y++) 
        {
            srcPos = y*srcStep;
            dstPos = y*dstStep;
            for (x=0;x<roiSize.width;x++) 
                {
                    //add 0.5 for nearest neighbor rounding
                    pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] + 
                    1.402*pSrcYCbCr[2][srcPos] - 178.956);
                    pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] - 
                    0.34414*pSrcYCbCr[1][srcPos] - 0.71414*pSrcYCbCr[2][srcPos]+ 135.95984);
                    pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] + 
                    1.772*pSrcYCbCr[1][srcPos++] - 226.316);
                }
        }
    }

    IV REF_CODE_OPT( const Fw8u * const pSrcYCbCr[3], int srcStep,Fw8u *pDstRGB, int dstStep, FwiSize roiSize)
    {
       // R = Y + 1.402*Cr - 179.456
       // G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
       // B = Y + 1.772*Cb - 226.816

       // 1.402	* 256	358.912	    359
       // 0.34414 * 256	88.09984	88
       // 0.71414 * 256	182.81984	183
       // 1.772	*   256	453.632	    454


        int x, y;
        int srcPos, dstPos;

        for (y=0;y<roiSize.height; y++) 
        {
            srcPos = y*srcStep;
            dstPos = y*dstStep;
            for (x=0;x<roiSize.width;x++) 
                {
                    //add 0.5 for nearest neighbor rounding
                    pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] + 
                    ((int)((int)coe[0]*pSrcYCbCr[2][srcPos])>>6) - (int)180);
                    pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] - 
                    ((int)((int)coe[1]*pSrcYCbCr[1][srcPos] + (int)coe[2]*pSrcYCbCr[2][srcPos])>>6) + 135);
                    pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] + 
                    ((int)((int)coe[3]*pSrcYCbCr[1][srcPos++])>>6) - 227);
                }
        }

    }


};


/////////

class YCCKToCMYK_JPEG_8u_P4C4R : public fe4S1D<Fw8u,C1,Fw8u,C1,Fw8u,C1,Fw8u,C1,Fw8u,C4>
{
public:
    FE_SSE2_REF
    S16 coe[4];
	XMM128 mMask;
 //   XMM128 mCoeff[4];
    YCCKToCMYK_JPEG_8u_P4C4R()
    {
       // R = Y + 1.402*Cr - 179.456
       // G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
       // B = Y + 1.772*Cb - 226.816
		//90  Coeffs * 2^6
		//22
		//46
		//113

       coe[0] = 90, coe[1] = 22, coe[2] = 46,coe[3]=113;
    }

    IV SSE2_Init()
    {
		mMask.i = _mm_set1_epi32(0xFFFFFFFF);
    }

    IV SSE2( RegFile & r ) const									// SSE2 Pixel function
    {
        XMM128 Y,Cb,Cr,Y1,Cb1,Cr1;

        Y.i = r.src1[0].i;
        Cb.i = r.src2[0].i;
        Cr.i = r.src3[0].i;

        CBL_SSE2::Unpack8UTo16U(Y.i,Y1.i);
        CBL_SSE2::Unpack8UTo16U(Cb.i,Cb1.i);
        CBL_SSE2::Unpack8UTo16U(Cr.i,Cr1.i);


       // R = Y + 1.402*Cr - 179.456
        __m128i rV = Compute_R(Y.i,Cr.i);
        __m128i rV1 = Compute_R(Y1.i,Cr1.i);
        r.dst[0].i = _mm_packus_epi16(rV,rV1);  

       // G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
        __m128i gV = Compute_G(Y.i,Cb.i,Cr.i);
        __m128i gV1 = Compute_G(Y1.i,Cb1.i,Cr1.i);
        r.dst[1].i = _mm_packus_epi16(gV,gV1);  
       
        // B = Y + 1.772*Cb - 226.816
        __m128i bV = Compute_B(Y.i,Cb.i);
        __m128i bV1= Compute_B(Y1.i,Cb1.i);
        r.dst[2].i = _mm_packus_epi16(bV,bV1);  
		
		r.dst[0].i = _mm_andnot_si128(r.dst[0].i,mMask.i);
		r.dst[1].i = _mm_andnot_si128(r.dst[1].i,mMask.i);
		r.dst[2].i = _mm_andnot_si128(r.dst[2].i,mMask.i);
		r.dst[3].i = r.src4[0].i;

        ssp_convert_4p_4c_epi8(&r.dst[0].i,&r.dst[1].i,&r.dst[2].i,&r.dst[3].i);


    }  

   IV SSE_32(const Fw8u *Y,const Fw8u *Cb,const Fw8u *Cr,const Fw8u *Al,int srcstep,Fw8u *pDst,int dststep)
    {
        RegFile r;
		SSE2_Init();
		for(int h=0;h<32;h++)
        {
            r.src1[0].i= _mm_loadu_si128((__m128i*)Y);
            r.src2[0].i= _mm_loadu_si128((__m128i*)Cb);
            r.src3[0].i= _mm_loadu_si128((__m128i*)Cr);
            r.src4[0].i= _mm_loadu_si128((__m128i*)Al);
            SSE2(r);
            _mm_storeu_si128((__m128i*)pDst,r.dst[0].i);
            _mm_storeu_si128((__m128i*)(pDst + 16),r.dst[1].i);
            _mm_storeu_si128((__m128i*)(pDst + 32),r.dst[2].i);
            _mm_storeu_si128((__m128i*)(pDst + 48),r.dst[3].i);

            r.src1[0].i= _mm_loadu_si128((__m128i*)(Y+16));
            r.src2[0].i= _mm_loadu_si128((__m128i*)(Cb+16));
            r.src3[0].i= _mm_loadu_si128((__m128i*)(Cr+16));
            r.src4[0].i= _mm_loadu_si128((__m128i*) (Al+16));
            SSE2(r);
            _mm_storeu_si128((__m128i*)(pDst + 64 ),r.dst[0].i);
            _mm_storeu_si128((__m128i*)(pDst + 80),r.dst[1].i);
            _mm_storeu_si128((__m128i*)(pDst + 96),r.dst[2].i);
            _mm_storeu_si128((__m128i*)(pDst + 112),r.dst[3].i);
            
            Y+=srcstep;
            Cb+=srcstep;
            Cr+=srcstep;
			Al+=srcstep;
            pDst +=dststep;
        }

    }

   IV SSE_16(const Fw8u *Y,const Fw8u *Cb,const Fw8u *Cr,const Fw8u *Al,int srcstep,Fw8u *pDst,int dststep)
    {
        RegFile r;
		SSE2_Init();
		for(int h=0;h<16;h++)
        {
            r.src1[0].i= _mm_loadu_si128((__m128i*)Y);
            r.src2[0].i= _mm_loadu_si128((__m128i*)Cb);
            r.src3[0].i= _mm_loadu_si128((__m128i*)Cr);
            r.src4[0].i= _mm_loadu_si128((__m128i*)Al);
            SSE2(r);
            _mm_storeu_si128((__m128i*)pDst,r.dst[0].i);
            _mm_storeu_si128((__m128i*)(pDst + 16),r.dst[1].i);
            _mm_storeu_si128((__m128i*)(pDst + 32),r.dst[2].i);
            _mm_storeu_si128((__m128i*)(pDst + 48),r.dst[3].i);

            Y+=srcstep;
            Cb+=srcstep;
            Cr+=srcstep;
			Al+=srcstep;
            pDst +=dststep;
        }

    }

    IV REFR( const Fw8u *Y,const Fw8u *Cb,const Fw8u *Cr, const Fw8u *A,Fw8u *d) const
    {

        d[0] = ~(CBL_LIBRARY::Limits<Fw8u>::Sat(Y[0] + ((int)((int)coe[0] * Cr[0])>>6) - (int)180));
        d[1] = ~(CBL_LIBRARY::Limits<Fw8u>::Sat(Y[0] - ((int)(((int)coe[1]* Cb[0]) + ((int)coe[2]* Cr[0]))>>6) + (int)135));
        d[2] = ~(CBL_LIBRARY::Limits<Fw8u>::Sat(Y[0] + ((int)((int)coe[3] * Cb[0])>>6) - (int)227));
		d[3] = *A;
    }
 
    IV REF_CODE(const Fw8u *pSrcYCCK[4], int srcStep,Fw8u *pDstCMYK, int dstStep, FwiSize roiSize)
    {
			//Reference code only.
			//SSE2 code need to shift 16 bit 
			int x, y;
			unsigned char RVal, GVal, BVal;
			int srcPos, dstPos;

			for (y=0;y<roiSize.height; y++) {
				srcPos = y*srcStep;
				dstPos = y*dstStep;
				for (x=0;x<roiSize.width;x++) {
					//add 0.5 for nearest neighbor rounding
					RVal = FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] + 
						1.402*pSrcYCCK[2][srcPos] - 178.956);
					GVal = FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] - 
						0.34414*pSrcYCCK[1][srcPos] - 0.71414*pSrcYCCK[2][srcPos]+ 135.95984);
					BVal = FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] + 
						1.772*pSrcYCCK[1][srcPos] - 226.316);
					pDstCMYK[dstPos++] = ~RVal; //C=255-R
					pDstCMYK[dstPos++] = ~GVal; //M=255-G
					pDstCMYK[dstPos++] = ~BVal; //Y=255-B
					pDstCMYK[dstPos++] = pSrcYCCK[3][srcPos++];
				}
			}

    }

    IV REF_CODE_OPT(const Fw8u * const pSrcYCCK[4], int srcStep,Fw8u *pDstCMYK, int dstStep, FwiSize roiSize)
    {
       // R = Y + 1.402*Cr - 179.456
       // G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
       // B = Y + 1.772*Cb - 226.816

       // 1.402	* 256	358.912	    359
       // 0.34414 * 256	88.09984	88
       // 0.71414 * 256	182.81984	183
       // 1.772	*   256	453.632	    454


        int x, y;
        int srcPos, dstPos;

        for (y=0;y<roiSize.height; y++) 
        {
            srcPos = y*srcStep;
            dstPos = y*dstStep;
            for (x=0;x<roiSize.width;x++) 
                {
                    //add 0.5 for nearest neighbor rounding
                    pDstCMYK[dstPos++] = ~(FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] + 
                    ((int)((int)coe[0]*pSrcYCCK[2][srcPos])>>6) - (int)180));
                    pDstCMYK[dstPos++] = ~(FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] - 
                    ((int)((int)coe[1]*pSrcYCCK[1][srcPos] + (int)coe[2]*pSrcYCCK[2][srcPos])>>6) + 135));
                    pDstCMYK[dstPos++] = ~(FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] + 
                    ((int)((int)coe[3]*pSrcYCCK[1][srcPos])>>6) - 227));
					pDstCMYK[dstPos++] = pSrcYCCK[3][srcPos++];
                }
        }

    }


};

/////////
}

#endif // __JPEGCOLORCONV_H__
