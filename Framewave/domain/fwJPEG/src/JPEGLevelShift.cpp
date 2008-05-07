/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

//************************************************************************* 
//This file include the level shift functions from JPEG Chapter
//	fwiSub128_JPEG_8u16s_C1R
//  fwiAdd128_JPEG_16s8u_C1R
//************************************************************************* 

#include "fwdev.h"
#include "fwJPEG.h"
#include "fe.h"
#include "FwSharedCode_SSE2.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 102 )

#ifndef __JPEGLEVELSHIFT
#define __JPEGLEVELSHIFT
#define STEPCHECK(X, Y) if (X<=0 || Y<=0) return fwStsStepErr
#define ROISIZECHECK(X) if (X.height <=0 || X.width <=0) return fwStsSizeErr
#endif


namespace OPT_LEVEL
{

template<typename TS,CH CSrc,typename TD, CH CDst>
struct Add128_JPEG : public fe2<TS,CSrc,TD,CDst>
{
	
	XMM128 mV128;
	Fw16s v128;
	Add128_JPEG(const Fw16s val)
	{
		v128 = val;
	}
    FE_SSE2_REF
    IV SSE2_Init()
	{
		mV128.i = _mm_set1_epi16(v128);
	}
    IV SSE2( RegFile & r ) const
    {
        __m128i res = _mm_add_epi16( r.src1[0].i, mV128.i);
		r.dst[0].i = _mm_packus_epi16(res,res);
    }
    IV REFR( const TS *s, TD *d ) const
    {
        *d = CBL_LIBRARY::Limits<U8>::Sat(*s + 128);
    }
};

}

using namespace OPT_LEVEL;
//-----------------------------------------------------------------------
//Convert data from unsigned char range to the signed 16 bit integer range
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSub128_JPEG_8u16s_C1R)(const Fw8u *pSrc, int srcStep, 
						Fw16s *pDst, int dstStep, FwiSize roiSize)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;
	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	int x, y;
	const Fw8u *ptSrc;
	Fw16s *ptDst;

	for (y=0;y<roiSize.height;y++) {
		ptSrc=pSrc+y*srcStep;
		ptDst=pDst+y*dstStep/2; //16 bits
		for (x=0;x<roiSize.width;x++) {
			*(ptDst++)=*(ptSrc++)-128;
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//Convert data from 16 bit signed integer to unsigned char range
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAdd128_JPEG_16s8u_C1R)(const Fw16s *pSrc, int srcStep,
						Fw8u *pDst, int dstStep, FwiSize roiSize)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;
	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	int x, y;
	const Fw16s *ptSrc;
	Fw8u *ptDst;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
		case DT_SSE3:
		case DT_SSE2:
			{
			if(roiSize.width%16 == 0)
			{
				__m128i mV128 = _mm_set1_epi16(128);
				for(y=0;y<roiSize.height;y++)
				{
						ptSrc=pSrc+y*srcStep/2; //16 bits
						ptDst=pDst+y*dstStep; 
						for (x=0;x<roiSize.width;x+=16) 
						{
							__m128i s1= _mm_loadu_si128( (__m128i*) ptSrc ); 
							__m128i s2= _mm_loadu_si128( (__m128i*) (ptSrc+8) ); 
							__m128i res1 = _mm_add_epi16( s1, mV128);
							__m128i res2 = _mm_add_epi16( s2, mV128);
							_mm_storeu_si128( (__m128i*) ptDst, _mm_packus_epi16(res1,res2)); 
							ptDst+=16;
							ptSrc+=16;
						}
				}
			}
			else if(0 == roiSize.width%8)
			{
				__m128i mV128 = _mm_set1_epi16(128);
				for(y=0;y<roiSize.height;y++)
				{
						ptSrc=pSrc+y*srcStep/2; //16 bits
						ptDst=pDst+y*dstStep; 
						for (x=0;x<roiSize.width;x+=8) 
						{
							__m128i s1= _mm_loadu_si128( (__m128i*) ptSrc ); 
							__m128i res1 = _mm_add_epi16( s1, mV128);
							XMM128 res;
							res.i = _mm_packus_epi16(res1,res1);
							_mm_storel_pd( (double*) ptDst, res.d); 
							ptDst+=8;
							ptSrc+=8;
						}
				}
			}
            else
            {
		        Add128_JPEG<Fw16s,C1,Fw8u,C1> data((Fw16s)128);
		        return OPT_LEVEL::fe< Add128_JPEG<Fw16s,C1,Fw8u,C1> >(data, pSrc, srcStep, pDst, dstStep, roiSize);
            }

			}
			break;
		default:
			for (y=0;y<roiSize.height;y++) {
				ptSrc=pSrc+y*srcStep/2; //16 bits
				ptDst=pDst+y*dstStep; 
				for (x=0;x<roiSize.width;x++) {
					*(ptDst++)=FW_REF::Limits<U8>::Sat(*(ptSrc++)+128);
				}
			}
	}

	return fwStsNoErr;
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2
