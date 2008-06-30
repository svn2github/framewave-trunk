/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __AUTOCORR_H__
#define __AUTOCORR_H__

#include "FwSharedCode.h"
#include "fwSignal.h"

namespace OPT_LEVEL
{
   namespace AutoCorr
   {
      // Regular computaion
	  FwStatus AutoCorr_fn_32f(const  Fw32f*  pSrc,  int  srcLen, Fw32f*  pDst,  int  dstLen)
      {
         Fw32f sum;
         
         for ( int i = 0; i < dstLen; i++ )
         {
            sum = 0;
            for ( int j = 0; j < srcLen && (j+i) < srcLen; j++ )
            {  
               sum += pSrc[j] * pSrc[j + i];
            }
            
			pDst[i] = FW_REF::Limits<F32>::Sat(sum);
               
         }
         return fwStsNoErr;
      }
      
	  // Biased Normalized computation
      FwStatus AutoCorr_NormA_fn_32f(const  Fw32f*  pSrc,  int  srcLen, Fw32f*  pDst,  int  dstLen)
      {
         Fw32f sum;
		 
         for ( int i = 0; i < dstLen; i++ )
         {
            sum = 0;
            for ( int j = 0; j < srcLen && (j+i) < srcLen; j++ )
            {  
               sum += pSrc[j] * pSrc[j + i];
            }
            
            pDst[i] = FW_REF::Limits<F32>::Sat(sum)/srcLen;
               
         }
         return fwStsNoErr;
      }
	  
	  FwStatus AutoCorr_NormB_fn_32f(const  Fw32f*  pSrc,  int  srcLen, Fw32f*  pDst,  int  dstLen) // TODo - Inline
      {
         Fw32f sum;
         
         for ( int i = 0; i < dstLen; i++ )
         {
            sum = 0;
            for ( int j = 0; j < srcLen && (j+i) < srcLen; j++ )
            {  
               sum += pSrc[j] * pSrc[j + i];
            }
            
			pDst[i] = FW_REF::Limits<F32>::Sat(sum)/(srcLen - i);
               
         }
         return fwStsNoErr;
      }
	  
      // SSE2 Implementation (32f) - Regular computation
      FwStatus AutoCorr_fn_32f_SSE2(const Fw32f* pSrc, int srcLen, Fw32f* pDst, int dstLen)
      {
         XMM128 sum;
         __m128 a, b, pr;
         float zero = 0.0;
   
         for ( int i = 0; i < dstLen; i++ )
         {
            sum.f = _mm_set_ss(zero);
      
            int j = 0;
            int count = srcLen - (srcLen % 4);
            for ( j = 0; j < count && (j+i) < srcLen; j += 4 )
            {  
               a = _mm_loadu_ps(&pSrc[j]);
               b = _mm_loadu_ps(&pSrc[j+i]);
               pr = _mm_mul_ps(a, b);
               
               sum.f = _mm_add_ps(sum.f, pr);         
            }
      
            float fSum = sum.f32[0] + sum.f32[1] + sum.f32[2] + sum.f32[3];
      
            for( ; j < srcLen && (j+i) < srcLen; j++)
	         {     
		         fSum += (pSrc[j] * pSrc[j + i]);
		      }
			
			pDst[i] = fSum;
         }
         
         return fwStsNoErr;             
      }
	  
	  
	  FwStatus AutoCorr_NormA_fn_32f_SSE2(const Fw32f* pSrc, int srcLen, Fw32f* pDst, int dstLen)
      {
         XMM128 sum;
        __m128 a, b, pr;
         float zero = 0.0;
   
         for ( int i = 0; i < dstLen; i++ )
         {
            sum.f = _mm_set_ss(zero);
      
            int j = 0;
            int count = srcLen - (srcLen % 4);
            for ( j = 0; j < count && (j+i) < srcLen; j += 4 )
            {  
               a = _mm_loadu_ps(&pSrc[j]);
               b = _mm_loadu_ps(&pSrc[j+i]);
               pr = _mm_mul_ps(a, b);
               
               sum.f = _mm_add_ps(sum.f, pr);         
            }
      
            float fSum = sum.f32[0] + sum.f32[1] + sum.f32[2] + sum.f32[3];
      
            for( ; j < srcLen && (j+i) < srcLen; j++)
	         {     
		         fSum += (pSrc[j] * pSrc[j + i]);
		      }
			
			pDst[i] = fSum/srcLen;
        
         }
         
         return fwStsNoErr;             
      }
	  
	  
	  FwStatus AutoCorr_NormB_fn_32f_SSE2(const Fw32f* pSrc, int srcLen, Fw32f* pDst, int dstLen)
      {
         XMM128 sum;
         __m128 a, b, pr;
         float zero = 0.0;
   
         for ( int i = 0; i < dstLen; i++ )
         {
            sum.f = _mm_set_ss(zero);
      
            int j = 0;
            int count = srcLen - (srcLen % 4);
            for ( j = 0; j < count && (j+i) < srcLen; j += 4 )
            {  
               a = _mm_loadu_ps(&pSrc[j]);
               b = _mm_loadu_ps(&pSrc[j+i]);
               pr = _mm_mul_ps(a, b);
               
               sum.f = _mm_add_ps(sum.f, pr);         
            }
      
            float fSum = sum.f32[0] + sum.f32[1] + sum.f32[2] + sum.f32[3];
      
            for( ; j < srcLen && (j+i) < srcLen; j++)
	         {     
		         fSum += (pSrc[j] * pSrc[j + i]);
		      }
		
			pDst[i] = fSum/(srcLen-i);
        
		}
         
         return fwStsNoErr;             
      }
	  
	  
      
      // Reference Implementation (32fc)
      FwStatus AutoCorr_fn_32fc(const  Fw32fc*  pSrc,  int  srcLen, Fw32fc*  pDst,  int  dstLen)
      {
         Fw32fc sum;
            
         for ( int i = 0; i < dstLen; i++ ) {
            sum.re = 0;
            sum.im = 0;
            
            for ( int j = 0; j < srcLen && (j+i) < srcLen; j++ )
            {  
               // Complex Conjugate of pSrc[j] * pSrc[j+i]
               sum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
               sum.im = sum.im - ( pSrc[j].re * pSrc[j+i].im + pSrc[j].im * pSrc[j+i].re);              
            }
            
			pDst[i].re = FW_REF::Limits<F32>::Sat(sum.re);
            pDst[i].im = FW_REF::Limits<F32>::Sat(sum.im);
            
         }
         
         return fwStsNoErr;
      }
      
	  
	  FwStatus AutoCorr_NormA_fn_32fc(const  Fw32fc*  pSrc,  int  srcLen, Fw32fc*  pDst,  int  dstLen)
      {
         Fw32fc sum;
            
         for ( int i = 0; i < dstLen; i++ ) {
            sum.re = 0;
            sum.im = 0;
            
            for ( int j = 0; j < srcLen && (j+i) < srcLen; j++ )
            {  
               // Complex Conjugate of pSrc[j] * pSrc[j+i]
               sum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
               sum.im = sum.im - ( pSrc[j].re * pSrc[j+i].im + pSrc[j].im * pSrc[j+i].re);              
            }
        
			   pDst[i].re = (FW_REF::Limits<F32>::Sat(sum.re))/srcLen;
            pDst[i].im = (FW_REF::Limits<F32>::Sat(sum.im))/srcLen;
        
         }
         
         return fwStsNoErr;
      }
	  
	  FwStatus AutoCorr_NormB_fn_32fc(const  Fw32fc*  pSrc,  int  srcLen, Fw32fc*  pDst,  int  dstLen)
      {
         Fw32fc sum;
        
         for ( int i = 0; i < dstLen; i++ ) {
            sum.re = 0;
            sum.im = 0;
            
            for ( int j = 0; j < srcLen && (j+i) < srcLen; j++ )
            {  
               // Complex Conjugate of pSrc[j] * pSrc[j+i]
               sum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
               sum.im = sum.im - ( pSrc[j].re * pSrc[j+i].im + pSrc[j].im * pSrc[j+i].re);              
            }
            
            pDst[i].re = (FW_REF::Limits<F32>::Sat(sum.re))/(srcLen - i);
            pDst[i].im = (FW_REF::Limits<F32>::Sat(sum.im))/(srcLen - i);
            
         }
         
         return fwStsNoErr;
      }
      
      // SSE2 Implementation
      FwStatus AutoCorr_fn_32fc_SSE2(const  Fw32fc*  pSrc,  int  srcLen, Fw32fc*  pDst,  int  dstLen)
      {
         XMM128 sumRe, sumIm;
         __m128 aRe, aIm, bRe, bIm, subRe, prRe, prIm, prReIm, prImRe, sumPrIm;
         Fw32f zero = 0.0;
         
         for ( int i = 0; i < dstLen; i++ )
         {
            sumRe.f = _mm_set_ss(zero);
            sumIm.f = _mm_set_ss(zero);
                  
            int j = 0;
            int count = srcLen - (srcLen % 4);
      
            for ( j = 0; j < count && (j+i) < srcLen; j += 4 )
            {  
               aRe = _mm_loadu_ps(&pSrc[j].re);
               bRe = _mm_loadu_ps(&pSrc[j+i].re);
               aIm = _mm_loadu_ps(&pSrc[j].im);
               bIm = _mm_loadu_ps(&pSrc[j+i].im);
         
               // Real-Part Calculation
               prRe  = _mm_mul_ps(aRe, bRe);
               prIm  = _mm_mul_ps(aIm, bIm);
               subRe = _mm_sub_ps(prRe, prIm);
               sumRe.f = _mm_add_ps(sumRe.f, subRe);
                        
               // Imag-Part Calculation
               prReIm = _mm_mul_ps(aRe, bIm);
               prImRe = _mm_mul_ps(bRe, aIm);
               sumPrIm = _mm_add_ps(prReIm, prImRe);
               sumIm.f = _mm_sub_ps(sumIm.f, sumPrIm);                
            }
      
            Fw32fc fSum;
            fSum.re = sumRe.f32[0] + sumRe.f32[1] + sumRe.f32[2] + sumRe.f32[3];
            fSum.im = sumIm.f32[0] + sumIm.f32[1] + sumIm.f32[2] + sumIm.f32[3];
      
            for( ; j < srcLen && (j+i) < srcLen; j++)
            {
	           fSum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
               fSum.im = fSum.im - ( (pSrc[j].re * pSrc[j+i].im) + (pSrc[j].im * pSrc[j+i].re) );
	         }
	        
			   pDst[i].re = fSum.re;
            pDst[i].im = fSum.im;
            
         }
         
         return fwStsNoErr;
      }
	  
	  FwStatus AutoCorr_NormA_fn_32fc_SSE2(const  Fw32fc*  pSrc,  int  srcLen, Fw32fc*  pDst,  int  dstLen)
      {
        XMM128 sumRe, sumIm;
         __m128 aRe, aIm, bRe, bIm, subRe, prRe, prIm, prReIm, prImRe, sumPrIm;
         Fw32f zero = 0.0;
         
         for ( int i = 0; i < dstLen; i++ )
         {
            sumRe.f = _mm_set_ss(zero);
            sumIm.f = _mm_set_ss(zero);
                  
            int j = 0;
            int count = srcLen - (srcLen % 4);
      
            for ( j = 0; j < count && (j+i) < srcLen; j += 4 )
            {  
               aRe = _mm_loadu_ps(&pSrc[j].re);
               bRe = _mm_loadu_ps(&pSrc[j+i].re);
               aIm = _mm_loadu_ps(&pSrc[j].im);
               bIm = _mm_loadu_ps(&pSrc[j+i].im);
         
               // Real-Part Calculation
               prRe  = _mm_mul_ps(aRe, bRe);
               prIm  = _mm_mul_ps(aIm, bIm);
               subRe = _mm_sub_ps(prRe, prIm);
               sumRe.f = _mm_add_ps(sumRe.f, subRe);
                        
               // Imag-Part Calculation
               prReIm = _mm_mul_ps(aRe, bIm);
               prImRe = _mm_mul_ps(bRe, aIm);
               sumPrIm = _mm_add_ps(prReIm, prImRe);
               sumIm.f = _mm_sub_ps(sumIm.f, sumPrIm);                
            }
      
            Fw32fc fSum;
            fSum.re = sumRe.f32[0] + sumRe.f32[1] + sumRe.f32[2] + sumRe.f32[3];
            fSum.im = sumIm.f32[0] + sumIm.f32[1] + sumIm.f32[2] + sumIm.f32[3];
      
            for( ; j < srcLen && (j+i) < srcLen; j++)
            {
	           fSum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
               fSum.im = fSum.im - ( (pSrc[j].re * pSrc[j+i].im) + (pSrc[j].im * pSrc[j+i].re) );
	         }
	        
            pDst[i].re = fSum.re/srcLen;  
            pDst[i].im = fSum.im/srcLen;
            
         }
         
         return fwStsNoErr;
      }
	  
	  FwStatus AutoCorr_NormB_fn_32fc_SSE2(const  Fw32fc*  pSrc,  int  srcLen, Fw32fc*  pDst,  int  dstLen)
      {
         XMM128 sumRe, sumIm;
         __m128 aRe, aIm, bRe, bIm, subRe, prRe, prIm, prReIm, prImRe, sumPrIm;
         Fw32f zero = 0.0;
         
         for ( int i = 0; i < dstLen; i++ )
         {
            sumRe.f = _mm_set_ss(zero);
            sumIm.f = _mm_set_ss(zero);
                  
            int j = 0;
            int count = srcLen - (srcLen % 4);
      
            for ( j = 0; j < count && (j+i) < srcLen; j += 4 )
            {  
               aRe = _mm_loadu_ps(&pSrc[j].re);
               bRe = _mm_loadu_ps(&pSrc[j+i].re);
               aIm = _mm_loadu_ps(&pSrc[j].im);
               bIm = _mm_loadu_ps(&pSrc[j+i].im);
         
               // Real-Part Calculation
               prRe  = _mm_mul_ps(aRe, bRe);
               prIm  = _mm_mul_ps(aIm, bIm);
               subRe = _mm_sub_ps(prRe, prIm);
               sumRe.f = _mm_add_ps(sumRe.f, subRe);
                        
               // Imag-Part Calculation
               prReIm = _mm_mul_ps(aRe, bIm);
               prImRe = _mm_mul_ps(bRe, aIm);
               sumPrIm = _mm_add_ps(prReIm, prImRe);
               sumIm.f = _mm_sub_ps(sumIm.f, sumPrIm);                
            }
      
            Fw32fc fSum;
            fSum.re = sumRe.f32[0] + sumRe.f32[1] + sumRe.f32[2] + sumRe.f32[3];
            fSum.im = sumIm.f32[0] + sumIm.f32[1] + sumIm.f32[2] + sumIm.f32[3];
      
            for( ; j < srcLen && (j+i) < srcLen; j++)
            {
	           fSum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
               fSum.im = fSum.im - ( (pSrc[j].re * pSrc[j+i].im) + (pSrc[j].im * pSrc[j+i].re) );
	         }
	         
	          pDst[i].re = fSum.re/(srcLen-i);
             pDst[i].im = fSum.im/(srcLen-i);
            
         }
         
         return fwStsNoErr;
      }
      
      // REFR 64f Implementation
      FwStatus AutoCorr_fn_64f(const  Fw64f*  pSrc,  int  srcLen, Fw64f*  pDst,  int  dstLen)
      {
         Fw64f sum;
          
         for ( int i = 0; i < dstLen; i++ )
         {
            sum = 0;
            for ( int j = 0; j < srcLen && (j+i) < srcLen; j++ )
            {
               sum += pSrc[j] * pSrc[j + i];
            }
            
          pDst[i] = sum;
		  
         }
         return fwStsNoErr;
      }
      
      FwStatus AutoCorr_NormA_fn_64f(const  Fw64f*  pSrc,  int  srcLen, Fw64f*  pDst,  int  dstLen)
      {
         Fw64f sum;
          
         for ( int i = 0; i < dstLen; i++ )
         {
            sum = 0;
            for ( int j = 0; j < srcLen && (j+i) < srcLen; j++ )
            {
               sum += pSrc[j] * pSrc[j + i];
            }
            
			pDst[i] = sum/srcLen;
            
         }
         return fwStsNoErr;
      }
	  
	  FwStatus AutoCorr_NormB_fn_64f(const  Fw64f*  pSrc,  int  srcLen, Fw64f*  pDst,  int  dstLen)
      {
         Fw64f sum;
          
         for ( int i = 0; i < dstLen; i++ )
         {
            sum = 0;
            for ( int j = 0; j < srcLen && (j+i) < srcLen; j++ )
            {
               sum += pSrc[j] * pSrc[j + i];
            }
            
			pDst[i] = sum/(srcLen - i);
            
         }
         return fwStsNoErr;
      }
	  
	  
	  // SSE2 64f Implementation
      FwStatus AutoCorr_fn_64f_SSE2(const  Fw64f*  pSrc,  int  srcLen, Fw64f*  pDst,  int  dstLen)
      {
		 XMM128 sum;
         __m128d a, b, pr;
         Fw64f zero = 0.0;
         
         for ( int i = 0; i < dstLen; i++ )
         {
            sum.d = _mm_set_sd(zero);
            
            int j = 0;
            int count = srcLen - (srcLen % 2);
            
            for ( j = 0; j < count && (j+i) < srcLen; j += 2 )
            {  
               a = _mm_loadu_pd(&pSrc[j]); b = _mm_loadu_pd(&pSrc[j+i]);
               pr = _mm_mul_pd(a, b);
               sum.d = _mm_add_pd(sum.d, pr);         
            }
            
            F64 fSum = sum.f64[0] + sum.f64[1];
            
            for( ; j < srcLen && (j+i) < srcLen; j++)
	         {
		            fSum += (pSrc[j] * pSrc[j + i]);
		      }
      				
            pDst[i] = fSum;
         }
         return fwStsNoErr;
      }
      
      FwStatus AutoCorr_NormA_fn_64f_SSE2(const  Fw64f*  pSrc,  int  srcLen, Fw64f*  pDst,  int  dstLen)
      {
		 XMM128 sum;
         __m128d a, b, pr;
         Fw64f zero = 0.0;
         
         for ( int i = 0; i < dstLen; i++ )
         {
            sum.d = _mm_set_sd(zero);
            
            int j = 0;
            int count = srcLen - (srcLen % 2);
            
            for ( j = 0; j < count && (j+i) < srcLen; j += 2 )
            {  
               a = _mm_loadu_pd(&pSrc[j]); b = _mm_loadu_pd(&pSrc[j+i]);
               pr = _mm_mul_pd(a, b);
               sum.d = _mm_add_pd(sum.d, pr);         
            }
            
            F64 fSum = sum.f64[0] + sum.f64[1];
            
            for( ; j < srcLen && (j+i) < srcLen; j++)
	         {
		            fSum += (pSrc[j] * pSrc[j + i]);
		      }
      				
            pDst[i] = fSum/srcLen;
            
         }
         return fwStsNoErr;
      }
      
	  
	  FwStatus AutoCorr_NormB_fn_64f_SSE2(const  Fw64f*  pSrc,  int  srcLen, Fw64f*  pDst,  int  dstLen)
      {
		 XMM128 sum;
         __m128d a, b, pr;
         Fw64f zero = 0.0;
         
         for ( int i = 0; i < dstLen; i++ )
         {
            sum.d = _mm_set_sd(zero);
            
            int j = 0;
            int count = srcLen - (srcLen % 2);
            
            for ( j = 0; j < count && (j+i) < srcLen; j += 2 )
            {  
               a = _mm_loadu_pd(&pSrc[j]); b = _mm_loadu_pd(&pSrc[j+i]);
               pr = _mm_mul_pd(a, b);
               sum.d = _mm_add_pd(sum.d, pr);         
            }
            
            F64 fSum = sum.f64[0] + sum.f64[1];
            
            for( ; j < srcLen && (j+i) < srcLen; j++)
	         {
		            fSum += (pSrc[j] * pSrc[j + i]);
		      }
      				
            pDst[i] = fSum/(srcLen - i);
            
         }
         return fwStsNoErr;
      }
      
	  
	  // REFR 64fc Implementation
      FwStatus AutoCorr_fn_64fc(const  Fw64fc*  pSrc,  int  srcLen, Fw64fc*  pDst,  int  dstLen)
      {
         Fw64fc sum;
         
         for ( int i = 0; i < dstLen; i++ ) {
            sum.re = 0.0;
            sum.im = 0.0;
            for ( int j = 0; j < srcLen && (j+i) < srcLen - 1; j++ )
            {
              // Complex Conjugate of pSrc[j] * pSrc[j+i]
              sum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
              sum.im = sum.im - ( pSrc[j].re * pSrc[j+i].im + pSrc[j].im * pSrc[j+i].re);              
            }
            
			pDst[i].re = sum.re;
            pDst[i].im = sum.im;
            
         }
         
         return fwStsNoErr;
      }
      
	  FwStatus AutoCorr_NormA_fn_64fc(const  Fw64fc*  pSrc,  int  srcLen, Fw64fc*  pDst,  int  dstLen)
      {
         Fw64fc sum;
         
         for ( int i = 0; i < dstLen; i++ ) {
            sum.re = 0.0;
            sum.im = 0.0;
            for ( int j = 0; j < srcLen && (j+i) < srcLen - 1; j++ )
            {
              // Complex Conjugate of pSrc[j] * pSrc[j+i]
              sum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
              sum.im = sum.im - ( pSrc[j].re * pSrc[j+i].im + pSrc[j].im * pSrc[j+i].re);              
            }
            
			pDst[i].re = sum.re/srcLen;
            pDst[i].im = sum.im/srcLen;
            
         }
         
         return fwStsNoErr;
      }
      
	  FwStatus AutoCorr_NormB_fn_64fc(const  Fw64fc*  pSrc,  int  srcLen, Fw64fc*  pDst,  int  dstLen)
      {
         Fw64fc sum;
         
         for ( int i = 0; i < dstLen; i++ ) {
            sum.re = 0.0;
            sum.im = 0.0;
            for ( int j = 0; j < srcLen && (j+i) < srcLen - 1; j++ )
            {
              // Complex Conjugate of pSrc[j] * pSrc[j+i]
              sum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
              sum.im = sum.im - ( pSrc[j].re * pSrc[j+i].im + pSrc[j].im * pSrc[j+i].re);              
            }
            
			pDst[i].re = sum.re/(srcLen - i);
            pDst[i].im = sum.im/(srcLen - i);
            
         }
         
         return fwStsNoErr;
      }
      
	  
      // SSE2 64fc Implementation
      FwStatus AutoCorr_fn_64fc_SSE2(const  Fw64fc*  pSrc,  int  srcLen, Fw64fc*  pDst,  int  dstLen)
      {
         XMM128 sumRe, sumIm;
         __m128d aRe, aIm, bRe, bIm, subRe, prRe, prIm, prReIm, prImRe, sumPrIm;
         Fw64f zero = 0.0;
         
         for ( int i = 0; i < dstLen; i++ )
         {
            sumRe.d = _mm_set_sd(zero);
            sumIm.d = _mm_set_sd(zero);
                  
            int j = 0;
            int count = srcLen - (srcLen % 2);
      
            for ( j = 0; j < count && (j+i) < srcLen; j += 2 )
            {  
               aRe = _mm_loadu_pd(&pSrc[j].re);
               bRe = _mm_loadu_pd(&pSrc[j+i].re);
               aIm = _mm_loadu_pd(&pSrc[j].im);
               bIm = _mm_loadu_pd(&pSrc[j+i].im);
         
               // Real-Part Calculation
               prRe  = _mm_mul_pd(aRe, bRe);
               prIm  = _mm_mul_pd(aIm, bIm);
               subRe = _mm_sub_pd(prRe, prIm);
               sumRe.d = _mm_add_pd(sumRe.d, subRe);
                        
               // Imag-Part Calculation
               prReIm = _mm_mul_pd(aRe, bIm);
               prImRe = _mm_mul_pd(bRe, aIm);
               sumPrIm = _mm_add_pd(prReIm, prImRe);
               sumIm.d = _mm_sub_pd(sumIm.d, sumPrIm);                
            }
      
            Fw64fc fSum;
            fSum.re = sumRe.f64[0] + sumRe.f64[1];
            fSum.im = sumIm.f64[0] + sumIm.f64[1];
      
            for( ; j < srcLen && (j+i) < srcLen; j++)
            {
	            fSum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
               fSum.im = fSum.im - ( (pSrc[j].re * pSrc[j+i].im) + (pSrc[j].im * pSrc[j+i].re) );
	         }
			 
	        pDst[i].re = fSum.re;
            pDst[i].im = fSum.im;
			
         }
         
         return fwStsNoErr;
      }

	FwStatus AutoCorr_NormA_fn_64fc_SSE2(const  Fw64fc*  pSrc,  int  srcLen, Fw64fc*  pDst,  int  dstLen)
      {
         XMM128 sumRe, sumIm;
         __m128d aRe, aIm, bRe, bIm, subRe, prRe, prIm, prReIm, prImRe, sumPrIm;
         Fw64f zero = 0.0;
         
         for ( int i = 0; i < dstLen; i++ )
         {
            sumRe.d = _mm_set_sd(zero);
            sumIm.d = _mm_set_sd(zero);
                  
            int j = 0;
            int count = srcLen - (srcLen % 2);
      
            for ( j = 0; j < count && (j+i) < srcLen; j += 2 )
            {  
               aRe = _mm_loadu_pd(&pSrc[j].re);
               bRe = _mm_loadu_pd(&pSrc[j+i].re);
               aIm = _mm_loadu_pd(&pSrc[j].im);
               bIm = _mm_loadu_pd(&pSrc[j+i].im);
         
               // Real-Part Calculation
               prRe  = _mm_mul_pd(aRe, bRe);
               prIm  = _mm_mul_pd(aIm, bIm);
               subRe = _mm_sub_pd(prRe, prIm);
               sumRe.d = _mm_add_pd(sumRe.d, subRe);
                        
               // Imag-Part Calculation
               prReIm = _mm_mul_pd(aRe, bIm);
               prImRe = _mm_mul_pd(bRe, aIm);
               sumPrIm = _mm_add_pd(prReIm, prImRe);
               sumIm.d = _mm_sub_pd(sumIm.d, sumPrIm);                
            }
      
            Fw64fc fSum;
            fSum.re = sumRe.f64[0] + sumRe.f64[1];
            fSum.im = sumIm.f64[0] + sumIm.f64[1];
      
            for( ; j < srcLen && (j+i) < srcLen; j++)
            {
	            fSum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
               fSum.im = fSum.im - ( (pSrc[j].re * pSrc[j+i].im) + (pSrc[j].im * pSrc[j+i].re) );
	         }
	        
			pDst[i].re = fSum.re/srcLen;  
            pDst[i].im = fSum.im/srcLen;
            
         }
         
         return fwStsNoErr;
      }          
      
	  FwStatus AutoCorr_NormB_fn_64fc_SSE2(const  Fw64fc*  pSrc,  int  srcLen, Fw64fc*  pDst,  int  dstLen)
      {
         XMM128 sumRe, sumIm;
         __m128d aRe, aIm, bRe, bIm, subRe, prRe, prIm, prReIm, prImRe, sumPrIm;
         Fw64f zero = 0.0;
         
         for ( int i = 0; i < dstLen; i++ )
         {
            sumRe.d = _mm_set_sd(zero);
            sumIm.d = _mm_set_sd(zero);
                  
            int j = 0;
            int count = srcLen - (srcLen % 2);
      
            for ( j = 0; j < count && (j+i) < srcLen; j += 2 )
            {  
               aRe = _mm_loadu_pd(&pSrc[j].re);
               bRe = _mm_loadu_pd(&pSrc[j+i].re);
               aIm = _mm_loadu_pd(&pSrc[j].im);
               bIm = _mm_loadu_pd(&pSrc[j+i].im);
         
               // Real-Part Calculation
               prRe  = _mm_mul_pd(aRe, bRe);
               prIm  = _mm_mul_pd(aIm, bIm);
               subRe = _mm_sub_pd(prRe, prIm);
               sumRe.d = _mm_add_pd(sumRe.d, subRe);
                        
               // Imag-Part Calculation
               prReIm = _mm_mul_pd(aRe, bIm);
               prImRe = _mm_mul_pd(bRe, aIm);
               sumPrIm = _mm_add_pd(prReIm, prImRe);
               sumIm.d = _mm_sub_pd(sumIm.d, sumPrIm);                
            }
      
            Fw64fc fSum;
            fSum.re = sumRe.f64[0] + sumRe.f64[1];
            fSum.im = sumIm.f64[0] + sumIm.f64[1];
      
            for( ; j < srcLen && (j+i) < srcLen; j++)
            {
	            fSum.re += (pSrc[j].re * pSrc[j+i].re) - (pSrc[j].im * pSrc[j+i].im);
               fSum.im = fSum.im - ( (pSrc[j].re * pSrc[j+i].im) + (pSrc[j].im * pSrc[j+i].re) );
	         }
	         
	        pDst[i].re = fSum.re/(srcLen-i);
            pDst[i].im = fSum.im/(srcLen-i);
            
         }
         
         return fwStsNoErr;
      }          
      
      
   }
}

#endif // __AUTOCORR_H__
