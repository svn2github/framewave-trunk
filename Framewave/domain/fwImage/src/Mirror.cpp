/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "FwSharedCode_SSE2.h"
#include "fwImage.h"

#pragma warning ( disable : 4328 )

#if BUILD_NUM_AT_LEAST( 100 )

//Load/stores : Neither fe.h nor carbide load store gives the flexibility of loads and stores to simply do this.

namespace OPT_LEVEL
{
	template <bool isAligned>
	SYS_INLINE STATIC __m128i MyLoad_128i(__m128i *ptr)
	{
		if(isAligned)
			return _mm_load_si128(ptr);
		else
			return _mm_loadu_si128(ptr);
	}

	template <bool isAligned>
	SYS_INLINE STATIC void MyStore_128i(__m128i *ptr, __m128i &reg)
	{
		if(isAligned)
			_mm_store_si128(ptr, reg);
		else
			_mm_storeu_si128(ptr, reg);
		return ;
	}

	template <bool isAligned>
	SYS_INLINE STATIC void MyStream_128i(__m128i *ptr, __m128i &reg)
	{
		if(isAligned)
			_mm_stream_si128(ptr, reg);
		else
			_mm_storeu_si128(ptr, reg);
		return ;
	}

	//Reference
	template <class TS>
	SYS_INLINE static void MySwap(TS &a, TS &b)
	{
		TS pexchange;

		pexchange = a;
		a = b ;
		b = pexchange;
	}

	template<class DATATYPE>
	static void Reverse_memSwap (DATATYPE* pDst, DATATYPE *pSrc, int pixelsize, 
		int channel, int channel1) 
	{	
		int i, k;

		for (i=0;i<(pixelsize/2); i++) {
			for (k=0;k<channel1;k++) {
				MySwap(pSrc[i*channel+k], pDst[(pixelsize-i-1)*channel+k]);
			}	
		}
		return;
	}

	//include the middle point
	template<class DATATYPE>
	static void Reverse_memSwap_m (DATATYPE* pDst, DATATYPE *pSrc, int pixelsize,
		int channel, int channel1) 
	{	
		int i;

		for (i=0;i<=((pixelsize-1)/2); i++) {
			for (int k=0;k<channel1;k++) {
				MySwap(pSrc[i*channel+k], pDst[(pixelsize-i-1)*channel+k]);
			}		
		}
		return;
	}

	// called only in ref. TODO : could be made much better.
	SYS_INLINE static void fw_memcpy(void *dst, const void *src, int numBytes)
	{
		int l = sizeof(Fw64s);
		Fw64s *pnDst = (Fw64s *)dst;
		Fw64s *pnSrc = (Fw64s *)src;

		int rem = numBytes % l;
		numBytes /= l ;
		while(numBytes--)
			*pnDst++ = *pnSrc++ ;

		char *pcSrc = (char *)pnSrc;
		char *pcDst = (char *)pnDst;

		while(rem--)
			*pcDst++ = *pcSrc++ ;
		return;
	}

	template<class TS>
	static void reverse_memcpy (TS* pDst, const TS *pSrc, int pixelsize,
		int channel, int AC4_channel) 
	{	
		for (int i=0;i<pixelsize; i++) {
			fw_memcpy(&pDst[(pixelsize-i-1)*channel], &pSrc[i*channel], sizeof(TS)*AC4_channel);
		}
		return;
	}

	template< class TS, CH chSrc>	
	static FwStatus MyMirror_I(TS *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip )
	{
		if (pSrpcDst==0) return fwStsNullPtrErr;
		if (roiSize.height <=0 || roiSize.width <=0) return fwStsSizeErr;
		if (srpcDstStep <=0) return fwStsStepErr;

		int channel = ChannelCount(chSrc);
		// Will not change 4th channel element in AC4
		int channel1 = (chSrc == AC4) ? 3 : channel;

		//srpcDstStep is in bytes, to be changed with data type size
		srpcDstStep = srpcDstStep / (sizeof(TS));

		int x, y;
		switch(flip) {
		case(fwAxsHorizontal):
			if (chSrc !=AC4) {
				for (y=0; y<(roiSize.height/2); y++) {
					for (x=0; x< roiSize.width*channel; x++) {
						MySwap(pSrpcDst[y*srpcDstStep+x], pSrpcDst[(roiSize.height-y-1)*srpcDstStep+x]);						
					}
				}
			}else {
				for (y=0; y<(roiSize.height/2); y++) {
					for (x=0; x< roiSize.width; x++) {
						for (int k=0;k<3;k++) {
							MySwap(pSrpcDst[(y*srpcDstStep+x*channel+k)], pSrpcDst[(roiSize.height-y-1)*srpcDstStep+x*channel+k]);
						}					
					}
				}
			}
			return fwStsNoErr;

		case(fwAxsVertical):

			for (y=0; y<roiSize.height; y++) {
				Reverse_memSwap<TS>(&pSrpcDst[y*srpcDstStep],
					&pSrpcDst[y*srpcDstStep], roiSize.width, channel, channel1);
			}
			return fwStsNoErr;

		case(fwAxsBoth): 
			for (y=0; y<roiSize.height/2; y++) {
				Reverse_memSwap_m<TS>(&pSrpcDst[(roiSize.height-y-1)*srpcDstStep],
					&pSrpcDst[y*srpcDstStep], roiSize.width, channel, channel1);
			}
			for (; y<roiSize.height; y++) {
				Reverse_memSwap<TS>(&pSrpcDst[(roiSize.height-y-1)*srpcDstStep],
					&pSrpcDst[y*srpcDstStep], roiSize.width, channel, channel1);
			}
			return fwStsNoErr;

		default:
			return fwStsMirrorFlipErr;	
		}
	}


	template< class TS, CH chSrc>	
	static FwStatus My_FW_Mirror(const TS* pSrc, int srcStep, TS* pDst, int dstStep, 
		FwiSize roiSize, FwiAxis flip) 
	{
		if (pSrc==0 || pDst==0) return fwStsNullPtrErr;

		if (roiSize.height <=0 || roiSize.width <=0) return fwStsSizeErr;

		if (srcStep <=0 || dstStep <=0) return fwStsStepErr;

		int y;
		int channel = ChannelCount(chSrc);
		// Will not change 4th channel element in AC4
		int channel1 = (chSrc == AC4) ? 3 : channel;

		//srcStep and DstStep are in bytes, to be changed with data type size
		dstStep = dstStep / (sizeof(TS));
		srcStep = srcStep / (sizeof(TS));

		switch(flip) {
		case fwAxsHorizontal :
			if (chSrc != AC4) {
				for (y=0; y<roiSize.height; y++) {
					fw_memcpy(&pDst[(roiSize.height-y-1)*dstStep],
						&pSrc[y*srcStep], roiSize.width*channel*sizeof(TS));
				}
			} else {
				for (y=0; y<roiSize.height; y++) {
					for (int x=0; x<roiSize.width; x++) {
						fw_memcpy(&pDst[(roiSize.height-y-1)*dstStep+x*4],
							&pSrc[y*srcStep+x*4], 3*sizeof(TS));
					}
				}				
			}
			return fwStsNoErr;

		case fwAxsVertical :
			for (y=0; y<roiSize.height; y++) {
				reverse_memcpy<TS>(&pDst[y*dstStep],
					&pSrc[y*srcStep], roiSize.width, channel, channel1);
			}
			return fwStsNoErr;

		case fwAxsBoth : 
			for (y=0; y<roiSize.height; y++) {
				reverse_memcpy<TS>(&pDst[(roiSize.height-y-1)*dstStep],
					&pSrc[y*srcStep], roiSize.width, channel, channel1);
			}
			return fwStsNoErr;

		default:
			return fwStsMirrorFlipErr;
		} 	
	}


	// SSE2

	//Swaps two rows pointing to src1, src2 in an image with width "width".
	// for C1, C3, C4 channels 

	template< class TS, bool isAligned>
	SYS_INLINE STATIC void RowSwap_SSE2(TS *src1, TS *src2, int rowLen)
	{
		__m128i ts1, ts2 ;
		int num_TS = sizeof(__m128i)/sizeof(TS);
		TS *prowpend = src1 + rowLen;

		for ( ; src1 <= (prowpend-num_TS) ; src1+=num_TS, src2+=num_TS ){
			ts1 = MyLoad_128i<isAligned>((__m128i*)src1); 
			ts2 = MyLoad_128i<isAligned>((__m128i*)src2);
			MyStore_128i<isAligned>((__m128i*)src1, ts2); 
			MyStore_128i<isAligned>((__m128i*)src2, ts1); 					 
		}

		for( ; src1 < prowpend ; src1++, src2++){
			MySwap<TS>(*src1, *src2);	
		}
		return;
	}


	template< class TS, bool isAligned>
	SYS_INLINE STATIC void RowSwap_AC4_SSE2(TS *src1, TS *src2, int rowLen, const __m128i &mask)
	{
		__m128i ts1, ts2 ;
		int num_TS = sizeof(__m128i)/sizeof(TS);
		TS *prowpend = src1 + rowLen;

		for ( ; src1 <= (prowpend-num_TS) ; src1+=num_TS, src2+=num_TS ){
			ts1 = MyLoad_128i<isAligned>((__m128i*)src1); 
			ts2 = MyLoad_128i<isAligned>((__m128i*)src2);

			__m128i alpha1 = _mm_and_si128(ts1, mask);
			__m128i alpha2 = _mm_and_si128(ts2, mask);

			ts1 = _mm_or_si128(_mm_xor_si128(ts1, alpha1), alpha2);	
			ts2 = _mm_or_si128(_mm_xor_si128(ts2, alpha2), alpha1);	

			MyStore_128i<isAligned>((__m128i*)src1, ts2); 
			MyStore_128i<isAligned>((__m128i*)src2, ts1); 					 
		}

		for( ; src1 <= (prowpend-4); src1+=4, src2+=4){
			MySwap<TS>(src1[0], src2[0]);	
			MySwap<TS>(src1[1], src2[1]);
			MySwap<TS>(src1[2], src2[2]);
		}
		return;
	}

	SYS_INLINE STATIC void fw_C1_reverse(__m128i &r, Fw8u /*toTellType*/)
	{
		CBL_SSE2::Reverse8(r);
	}

	SYS_INLINE STATIC void fw_C1_reverse(__m128i &r, Fw16u /*toTellType*/)
	{
		CBL_SSE2::Reverse16(r);
	}

	SYS_INLINE STATIC void fw_C1_reverse(__m128i &r, Fw32s /*toTellType*/)
	{
		CBL_SSE2::Reverse32(r);
	}

	SYS_INLINE STATIC void fw_C4_reverse(__m128i &r, Fw8u /*toTellType*/)
	{
		r = _mm_shuffle_epi32(r, _MM_SHUFFLE(0, 1, 2, 3));
	}

	SYS_INLINE STATIC void fw_C4_reverse(__m128i &r, Fw16u /*toTellType*/)
	{
		r = _mm_shuffle_epi32(r, _MM_SHUFFLE(1, 0, 3, 2));
	}

	SYS_INLINE STATIC void fw_C4_reverse(__m128i &/*r*/, Fw32s /*toTellType*/)
	{
		return;
	}

	//  r1 r2 r3 <--> r3 r2 r1
	//	input:
	//	r1 : f1 e3 e2 e1 d3 d2 d1 c3 pC2 pC1 b3 b2 b1 a3 a2 a1
	//	r2 : k2 k1 j3 j2 j1 i3 i2 i1 h3 h2 h1 g3 g2 g1 f3 f2
	//	r3 : p3 p2 p1 o3 o2 o1 n3 n2 n1 m3 m2 m1 l3 l2 l1 k3
	//	
	//	output:
	//	r3 : k1 l3 l2 l1 m3 m2 m1 n3 n2 n1 o3 o2 o1 p3 p2 p1
	//	r2 : f2 f1 g3 g2 g1 h3 h2 h1 i3 i2 i1 j3 j2 j1 k3 k2
	//	r1 : a3 a2 a1 b3 b2 b1 c3 pC2 pC1 d3 d2 d1 e3 e2 e1 f3	 

	SYS_INLINE void fw_C3_reverse(__m128i &r1, __m128i &r2, __m128i &r3, Fw8u /*toTellType*/)
	{
		__m128i dst_r1 = _mm_slli_si128(r1, 13);				//  a .... 0000 
		__m128i tmp1 = _mm_srli_si128(_mm_slli_si128( _mm_srli_si128(r1, 3), 13), 3);
		dst_r1 = _mm_or_si128(dst_r1, tmp1);					//  a b ... 00 
		tmp1 =  _mm_srli_si128(_mm_slli_si128( _mm_srli_si128(r1, 6), 13), 6);
		dst_r1 = _mm_or_si128(dst_r1, tmp1);					//  a b c ..0000
		tmp1 =  _mm_srli_si128(_mm_slli_si128( _mm_srli_si128(r1, 9), 13), 9);
		dst_r1 = _mm_or_si128(dst_r1, tmp1);					//  a b c d ... 000
		tmp1 =  _mm_srli_si128(_mm_slli_si128( _mm_srli_si128(r1, 12), 13), 12);
		dst_r1 = _mm_or_si128(dst_r1, tmp1);					//  a b c d e 0
		tmp1 = _mm_srli_si128(_mm_slli_si128(r2, 14), 15);
		dst_r1 = _mm_or_si128(dst_r1, tmp1);					// a b c d e f3 - complete


		__m128i dst_r2 = _mm_slli_si128(r2, 15);			// f2 ....000 
		__m128i tmp2 = _mm_slli_si128(_mm_srli_si128(r1, 15), 14);	
		r1 = dst_r1 ;

		dst_r2 = _mm_or_si128(dst_r2, tmp2);				// f2 f1
		tmp2 = _mm_srli_si128(_mm_slli_si128(_mm_srli_si128(r2, 2), 13), 2);
		dst_r2 = _mm_or_si128(dst_r2, tmp2);				// f2 f1 g
		tmp2 = _mm_srli_si128(_mm_slli_si128(_mm_srli_si128(r2, 5), 13), 5);
		dst_r2 = _mm_or_si128(dst_r2, tmp2);				// f2 f1 g h
		tmp2 = _mm_srli_si128(_mm_slli_si128(_mm_srli_si128(r2, 8), 13), 8);
		dst_r2 = _mm_or_si128(dst_r2, tmp2);				// f2 f1 g h i
		tmp2 = _mm_srli_si128(_mm_slli_si128(_mm_srli_si128(r2, 11), 13), 11);
		dst_r2 = _mm_or_si128(dst_r2, tmp2);				// f2 f1 g h i j
		tmp2 = _mm_srli_si128(_mm_slli_si128(r3, 15), 14);
		dst_r2 = _mm_or_si128(dst_r2, tmp2);				// f2 f1 g h i j k3
		tmp2 = _mm_srli_si128(r2, 15);
		dst_r2 = _mm_or_si128(dst_r2, tmp2);				// f2 f1 g h i j k3 k2 - complete

		__m128i dst_r3 = _mm_slli_si128(_mm_srli_si128(r2, 14), 15);	// k1
		r2 =  dst_r2 ;

		__m128i tmp3 = _mm_srli_si128(_mm_slli_si128(_mm_srli_si128(r3, 1), 13), 1);
		dst_r3 = _mm_or_si128(dst_r3, tmp3);							//  k1 l
		tmp3   = _mm_srli_si128(_mm_slli_si128(_mm_srli_si128(r3, 4), 13), 4);
		dst_r3 = _mm_or_si128(dst_r3, tmp3);							// k1 l m
		tmp3 = _mm_srli_si128(_mm_slli_si128(_mm_srli_si128(r3, 7), 13), 7);	
		dst_r3 = _mm_or_si128(dst_r3, tmp3);							// k1 l m n
		tmp3 = _mm_srli_si128(_mm_slli_si128(_mm_srli_si128(r3, 10), 13), 10);
		dst_r3 = _mm_or_si128(dst_r3, tmp3);							// k1 l m n o
		tmp3 = _mm_srli_si128(r3, 13);
		r3 = _mm_or_si128(dst_r3, tmp3);							// k1 l m n o p

	}

	//  r3 r2 r1  <-->  r1 r2 r3
	//	input:
	//	r1 : pC2 pC1 b3 b2 b1 a3 a2 a1
	//	r2 : f1 e3 e2 e1 d3 d2 d1 c3 
	//	r3 : h3 h2 h1 g3 g2 g1 f3 f2
	//	
	//	output:
	//	r3 : f2 f1 g3 g2 g1 h3 h2 h1
	//	r2 : pC1 d3 d2 d1 e3 e2 e1 f3  
	//	r1 : a3 a2 a1 b3 b2 b1 c3 pC2 	 

	SYS_INLINE void fw_C3_reverse(__m128i &r1, __m128i &r2, __m128i &r3, Fw16u /*toTellType*/)
	{
		__m128i dst_r1 = _mm_slli_si128(r1, 10);	//a 000
		__m128i tmp1 = _mm_slli_si128(_mm_srli_si128(_mm_slli_si128(r1, 4), 10), 4); // b
		dst_r1	= _mm_or_si128(dst_r1, tmp1);		// a b 
		tmp1	= _mm_srli_si128(_mm_slli_si128(r2, 14), 12); //c3
		dst_r1	= _mm_or_si128(dst_r1, tmp1);		// a b c3
		tmp1	= _mm_srli_si128(r1, 14);	//pC2
		dst_r1	= _mm_or_si128(dst_r1, tmp1);		// a b c3 pC2

		__m128i dst_r2 = _mm_slli_si128(_mm_srli_si128(r1, 12), 14);	//pC1
		r1 = dst_r1 ;
		__m128i tmp2 = _mm_slli_si128(_mm_srli_si128(_mm_slli_si128(r2, 8), 10), 8); // d
		dst_r2	= _mm_or_si128(dst_r2, tmp2);	//pC1 d
		tmp2	= _mm_slli_si128(_mm_srli_si128(_mm_slli_si128(r2, 2), 10), 2); // e
		dst_r2	= _mm_or_si128(dst_r2, tmp2);	//pC1 d e
		tmp2	= _mm_srli_si128(_mm_slli_si128(r3, 12), 14); // f3
		dst_r2	= _mm_or_si128(dst_r2, tmp2);	//pC1 d e f3

		__m128i dst_r3 = _mm_slli_si128(_mm_srli_si128(r2, 14), 12);	//f1
		r2 = dst_r2 ;
		__m128i tmp3 = _mm_slli_si128(r3, 14);	//f2
		dst_r3	= _mm_or_si128(dst_r3, tmp3);	//f2 f1
		tmp3	= _mm_slli_si128(_mm_srli_si128(_mm_slli_si128(r3, 6), 10), 6); // g
		dst_r3	= _mm_or_si128(dst_r3, tmp3);	//f2 f1 g
		tmp3	= _mm_srli_si128(r3, 10);	// h
		r3		= _mm_or_si128(dst_r3, tmp3);	//f2 f1 g h
	}

	//  r3 r2 r1  <-->  r1 r2 r3
	//	input:
	//	r1 : b1 a3 a2 a1
	//	r2 : pC2 pC1 b3 b2  
	//	r3 : d3 d2 d1 c3
	//	
	//	output:
	//	r3 : pC1 d3 d2 d1
	//	r2 : b2 b1 c3 pC2   
	//	r1 : a3 a2 a1 b3  	 
	//

	SYS_INLINE void fw_C3_reverse(__m128i &r1, __m128i &r2, __m128i &r3, Fw32s /*toTellType*/)
	{
		__m128i dst_r1 = _mm_slli_si128(r1, 4);		// a
		__m128i tmp = _mm_srli_si128(_mm_slli_si128(r2, 8), 12);	//b3
		dst_r1 = _mm_or_si128(dst_r1, tmp);

		__m128i dst_r2 = _mm_slli_si128(r2, 12);	//b2
		tmp		= _mm_slli_si128(_mm_srli_si128(r1, 12), 8);	//b1
		r1 = dst_r1 ;
		dst_r2	= _mm_or_si128(dst_r2, tmp);	//b2 b1
		tmp		= _mm_srli_si128(_mm_slli_si128(r3, 12), 8);	//c3
		dst_r2	= _mm_or_si128(dst_r2, tmp);	//b2 b1 c3
		tmp		= _mm_srli_si128(r2, 12);	//pC2
		dst_r2	= _mm_or_si128(dst_r2, tmp);	//b2 b1 c3 pC2

		__m128i dst_r3 = _mm_slli_si128(_mm_srli_si128(r2, 8), 12); //pC1
		r2 = dst_r2;
		tmp		= _mm_srli_si128(r3, 4);	//d3 d2 d1
		r3	= _mm_or_si128(dst_r3, tmp);	//pC1 d3 d2
	}

	template <class TS, bool isAligned>
	SYS_INLINE STATIC void ColSwap_C1(TS *left, TS *right, int width)
	{
		TS *pend = left+width ;
		int jmp = sizeof(__m128i)/sizeof(TS);
		__m128i l, r;
		TS toTellType =0 ;

		for(  ; left <= pend-jmp ; left += jmp ){	// jmp no. of TS's are there in one __m128i
			right -= jmp;

			r = MyLoad_128i<isAligned>((__m128i*)right);
			fw_C1_reverse(r, toTellType);

			l = MyLoad_128i<isAligned>((__m128i*)left );

			fw_C1_reverse(l, toTellType);

			MyStore_128i<isAligned>((__m128i*)left,  r); 
			MyStore_128i<isAligned>((__m128i*)right, l); 		
		}

		for( ; left < pend ; left++ ){
			right-- ;
			MySwap(*left, *right);		
		}
	}

	template <class TS, bool isAligned>
	SYS_INLINE STATIC void ColSwap_C3(TS *left, TS *right, int width)
	{
		int jmp = 3*sizeof(__m128i)/sizeof(TS);
		__m128i l1, l2, l3, r1, r2, r3 ;
		TS toTellType = 0;
		TS *pend = left+width ;

		for(  ; left <= pend-jmp ; left+= jmp ){	
			right -= jmp;

			r1 = MyLoad_128i<isAligned>((__m128i*)right );
			r2 = MyLoad_128i<isAligned>(((__m128i*)right)+1 );
			r3 = MyLoad_128i<isAligned>(((__m128i*)right)+2 );

			fw_C3_reverse(r1, r2, r3, toTellType);

			l1 = MyLoad_128i<isAligned>((__m128i*)left );
			l2 = MyLoad_128i<isAligned>(((__m128i*)left)+1 );
			l3 = MyLoad_128i<isAligned>(((__m128i*)left)+2 );

			fw_C3_reverse(l1, l2, l3, toTellType);

			MyStore_128i<isAligned>((__m128i*)left, r3 ); 
			MyStore_128i<isAligned>(((__m128i*)left)+1, r2 );
			MyStore_128i<isAligned>(((__m128i*)left)+2, r1 );	

			MyStore_128i<isAligned>((__m128i*)right, l3 ); 
			MyStore_128i<isAligned>(((__m128i*)right)+1, l2 );
			MyStore_128i<isAligned>(((__m128i*)right)+2, l1 );	 
		}

		for( ; left < pend ; left+=3 ){
			right-=3 ;
			MySwap<TS>(left[0], right[0]);
			MySwap<TS>(left[1], right[1]);
			MySwap<TS>(left[2], right[2]);
		}
	}

	template <class TS, bool isAligned>
	SYS_INLINE STATIC void ColSwap_C4(TS *left, TS *right, int width)
	{
		TS *pend = left+width ;
		int jmp = sizeof(__m128i)/sizeof(TS);
		__m128i l, r;
		TS toTellType = 0;	

		for(  ; left <= pend-jmp ; left += jmp ){	// jmp no. of TS's are there in one __m128i
			right -= jmp;

			r = MyLoad_128i<isAligned>((__m128i*)right);
			fw_C4_reverse(r, toTellType);

			l = MyLoad_128i<isAligned>((__m128i*)left );
			fw_C4_reverse(l, toTellType);

			MyStore_128i<isAligned>((__m128i*)left,  r); 
			MyStore_128i<isAligned>((__m128i*)right, l); 		
		}

		for( ; left <= pend-4 ; left+=4 ){
			right -= 4 ;
			MySwap(left[0], right[0]);		
			MySwap(left[1], right[1]);
			MySwap(left[2], right[2]);
			MySwap(left[3], right[3]);
		}
	}

	template <class TS, bool isAligned>
	SYS_INLINE STATIC void ColSwap_AC4(TS *left, TS *right, const __m128i &mask, int width)
	{
		int jmp = sizeof(__m128i)/sizeof(TS);
		__m128i l, r, alphaR, alphaL;
		TS toTellType=0 ;
		TS *pend = left +width ;

		for(  ; left <= pend-jmp ; left += jmp ){	// jmp no. of TS's are there in one __m128i
			right -= jmp;

			r = MyLoad_128i<isAligned>((__m128i*)right );
			alphaR = _mm_and_si128(r, mask);
			r = _mm_xor_si128(r, alphaR);	//take off the alpha channel and then reverse.
			fw_C4_reverse(r, toTellType);

			l = MyLoad_128i<isAligned>((__m128i*)left);
			alphaL = _mm_and_si128(l, mask);
			l = _mm_xor_si128(l, alphaL);
			fw_C4_reverse(l, toTellType);

			r = _mm_or_si128(r, alphaL);	
			l = _mm_or_si128(l, alphaR);	

			MyStore_128i<isAligned>((__m128i*)left,  r); 
			MyStore_128i<isAligned>((__m128i*)right, l); 		
		}

		for( ; left <= pend-4 ; left+=4 ){
			right -= 4 ;
			MySwap(left[0], right[0]);
			MySwap(left[1], right[1]);
			MySwap(left[2], right[2]);
		}
	}

	template <class TS>
	SYS_INLINE static void Get_AC4_mask(__m128i &mask)
	{
		switch (sizeof(TS)){
		case 1 : //8u
			mask = _mm_set_epi32(0xff000000, 0xff000000, 0xff000000, 0xff000000); 
			break;
		case 2 : //16u
			mask =  _mm_set_epi32(0xffff0000, 0, 0xffff0000, 0);
			break;
		case 4 : //32s
			mask = _mm_set_epi32(0xffffffff, 0, 0, 0);
			break;
		default :
			assert(false);
		}
	}


	template<class TS, CH chSrc>  
	static FwStatus Mirror_I_Horizontal(TS *pSrpcDst, int srpcDstStep, FwiSize roiSize )
	{
		int y;
		int totalWidth = roiSize.width * (ChannelCount(chSrc));
		TS *topRow, *bottomRow ;
		bool isAligned ;

		if(chSrc != AC4){
			for (y=0; y<(roiSize.height/2); y++) {		
				topRow = &pSrpcDst[y*srpcDstStep] ;
				bottomRow = &pSrpcDst[(roiSize.height-y-1)*srpcDstStep] ;
				isAligned = IS_ALIGNED16_2(topRow, bottomRow);

				if(isAligned)
					RowSwap_SSE2<TS, true>(topRow, bottomRow,  totalWidth);
				else
					RowSwap_SSE2<TS, false>(topRow, bottomRow,  totalWidth);
			}
		}
		else{	
			__m128i  mask_AC4;
			Get_AC4_mask<TS>(mask_AC4); 
			for (y=0; y<(roiSize.height/2); y++) {
				topRow = &pSrpcDst[y*srpcDstStep] ;
				bottomRow = &pSrpcDst[(roiSize.height-y-1)*srpcDstStep] ;
				isAligned = IS_ALIGNED16_2(topRow, bottomRow);

				if(isAligned)
					RowSwap_AC4_SSE2<TS, true>(topRow, bottomRow, totalWidth, mask_AC4);
				else
					RowSwap_AC4_SSE2<TS, false>(topRow, bottomRow, totalWidth, mask_AC4);
			}
		}	
		return fwStsNoErr;
	}

	template <class TS, CH chSrc>
	static FwStatus Mirror_I_Vertical(TS *pSrpcDst, int srpcDstStep, FwiSize roiSize)	
	{
		int y;
		int totalWidth =  roiSize.width*ChannelCount(chSrc);
		int halfWidth = totalWidth/2 ;
		TS *rowPtrLeft, *rowPtrRight ;
		bool isAligned ;

		switch(chSrc){
			case(C1):  
				for (y=0; y<roiSize.height; y++){
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = rowPtrLeft +totalWidth ; 

					isAligned = IS_ALIGNED16_2(rowPtrLeft, rowPtrRight);
					if(isAligned)
						ColSwap_C1<TS, true>(rowPtrLeft, rowPtrRight, halfWidth);				
					else 
						ColSwap_C1<TS, false>(rowPtrLeft, rowPtrRight, halfWidth);		
				}
				break;

			case(C3):
				for (y=0; y<roiSize.height; y++){ 
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = rowPtrLeft +totalWidth ; 

					isAligned = IS_ALIGNED16_2(rowPtrLeft, rowPtrRight);
					if(isAligned)
						ColSwap_C3<TS, true>(rowPtrLeft, rowPtrRight, halfWidth);				
					else 
						ColSwap_C3<TS, false>(rowPtrLeft, rowPtrRight, halfWidth);				

				}
				break;

			case(C4) : 
				for (y=0; y<roiSize.height; y++){
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = rowPtrLeft +totalWidth ; 

					isAligned = IS_ALIGNED16_2(rowPtrLeft, rowPtrRight);
					if(isAligned)
						ColSwap_C4<TS, true>(rowPtrLeft, rowPtrRight, halfWidth);				
					else 
						ColSwap_C4<TS, false>(rowPtrLeft, rowPtrRight, halfWidth);	
				}
				break;	

			case(AC4) : 
				__m128i mask ;
				Get_AC4_mask<TS>(mask);				
				for (y=0; y<roiSize.height; y++){
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = rowPtrLeft +totalWidth ; 

					isAligned = IS_ALIGNED16_2(rowPtrLeft, rowPtrRight);
					if(isAligned)
						ColSwap_AC4<TS, true>(rowPtrLeft, rowPtrRight, mask, halfWidth);				
					else 
						ColSwap_AC4<TS, false>(rowPtrLeft, rowPtrRight, mask, halfWidth);								
				}
				break;

			default:
				assert(false);
		}	
		return fwStsNoErr;
	};

	template <class TS, CH chSrc>
	static FwStatus Mirror_I_Both(TS *pSrpcDst, int srpcDstStep, FwiSize roiSize)	
	{
		int y;
		int totalWidth =  roiSize.width*ChannelCount(chSrc);
		TS *rowPtrLeft, *rowPtrRight ;
		bool isAligned ;

		switch(chSrc){
			case(C1):  
				for (y=0; y<roiSize.height/2; y++){
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = &pSrpcDst[(roiSize.height-y-1)*srpcDstStep] +totalWidth ; 

					isAligned = IS_ALIGNED16_2(rowPtrLeft, rowPtrRight);
					if(isAligned)
						ColSwap_C1<TS, true>(rowPtrLeft, rowPtrRight, totalWidth);				
					else 
						ColSwap_C1<TS, false>(rowPtrLeft, rowPtrRight, totalWidth);		
				}
				if(roiSize.height%2){	//for the middle row
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = &pSrpcDst[y*srpcDstStep] +totalWidth ; 
					ColSwap_C1<TS, false>(rowPtrLeft, rowPtrRight, totalWidth/2);
				}

				break;

			case(C3):
				for (y=0; y<roiSize.height/2; y++){ 
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = &pSrpcDst[(roiSize.height-y-1)*srpcDstStep] +totalWidth ; 

					isAligned = IS_ALIGNED16_2(rowPtrLeft, rowPtrRight);
					if(isAligned)
						ColSwap_C3<TS, true>(rowPtrLeft, rowPtrRight, totalWidth);				
					else 
						ColSwap_C3<TS, false>(rowPtrLeft, rowPtrRight, totalWidth);				

				}
				if(roiSize.height%2){	//for the middle row
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = &pSrpcDst[y*srpcDstStep] +totalWidth ; 
					ColSwap_C3<TS, false>(rowPtrLeft, rowPtrRight, totalWidth/2);
				}
				break;

			case(C4) : 
				for (y=0; y<roiSize.height/2; y++){
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = &pSrpcDst[(roiSize.height-y-1)*srpcDstStep] +totalWidth ; 

					isAligned = IS_ALIGNED16_2(rowPtrLeft, rowPtrRight);
					if(isAligned)
						ColSwap_C4<TS, true>(rowPtrLeft, rowPtrRight, totalWidth);				
					else 
						ColSwap_C4<TS, false>(rowPtrLeft, rowPtrRight, totalWidth);	
				}
				if(roiSize.height%2){	//for the middle row
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = &pSrpcDst[y*srpcDstStep] +totalWidth ; 
					ColSwap_C4<TS, false>(rowPtrLeft, rowPtrRight, totalWidth/2);
				}
				break;	

			case(AC4) : 
				__m128i mask ;
				Get_AC4_mask<TS>(mask);				
				for (y=0; y<roiSize.height/2; y++){
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = &pSrpcDst[(roiSize.height-y-1)*srpcDstStep] +totalWidth ;  

					isAligned = IS_ALIGNED16_2(rowPtrLeft, rowPtrRight);
					if(isAligned)
						ColSwap_AC4<TS, true>(rowPtrLeft, rowPtrRight, mask, totalWidth);				
					else 
						ColSwap_AC4<TS, false>(rowPtrLeft, rowPtrRight, mask, totalWidth);								
				}
				if(roiSize.height%2){	//for the middle row
					rowPtrLeft = &pSrpcDst[y*srpcDstStep] ;
					rowPtrRight = &pSrpcDst[y*srpcDstStep] +totalWidth ; 
					ColSwap_AC4<TS, false>(rowPtrLeft, rowPtrRight, mask, totalWidth/2);
				}
				break;

			default:
				assert(false);
		}	
		return fwStsNoErr;
	};

	template<class TS, CH chSrc> 
	static FwStatus MyMirror_I_sse2(TS *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip )
	{
		if (pSrpcDst==0) return fwStsNullPtrErr;
		if (roiSize.height <=0 || roiSize.width <=0) return fwStsSizeErr;
		if (srpcDstStep <=0) return fwStsStepErr;

		//srpcDstStep is in bytes, to be changed with data type size
		srpcDstStep = srpcDstStep / (sizeof(TS));

		switch(flip) {
		case(fwAxsHorizontal):
			return Mirror_I_Horizontal<TS, chSrc>(pSrpcDst, srpcDstStep, roiSize );
		case(fwAxsVertical):
			return Mirror_I_Vertical<TS, chSrc>(pSrpcDst, srpcDstStep, roiSize);
		case(fwAxsBoth): 
			return Mirror_I_Both<TS, chSrc>(pSrpcDst, srpcDstStep, roiSize);			 
		default:
			return fwStsMirrorFlipErr;	
		}
	}

	//Not-in-place operation

	template <bool isAligned>
	SYS_INLINE static void Memcpy_SSE2(void *dst, const void *src, int numBytes)
	{
		char *pcSrc = (char*)src ;
		char *pcDst = (char*)dst ;
		char *pcend = pcSrc + numBytes;

		__m128i tSrc;

		for( ; pcSrc+16 <= pcend ; pcSrc += 16, pcDst +=16 ){
			tSrc = MyLoad_128i<isAligned>((__m128i *)pcSrc);
			MyStream_128i<isAligned>((__m128i*)pcDst, tSrc);
		}

		while(pcSrc < pcend)
		{
			*pcDst++ = *pcSrc++ ;
		}
		_mm_mfence();
	}

	template <class TS, bool isAligned>
	SYS_INLINE static void Memcpy_AC4_SSE2(TS *dst, const TS *src, int numBytes, const __m128i &mask)
	{
		char *pcSrc = (char*)src ;
		char *pcDst = (char*)dst ;
		char *pcend = pcSrc + numBytes;

		__m128i tSrc, alphaDst;

		for( ; pcSrc+16 <= pcend ; pcSrc += 16, pcDst +=16 ){
			tSrc = MyLoad_128i<isAligned>((__m128i *)pcSrc);
			tSrc = _mm_xor_si128(tSrc, _mm_and_si128(tSrc, mask));	//remove alpha of src
			alphaDst = _mm_and_si128(MyLoad_128i<isAligned>((__m128i *)pcDst), mask);
			tSrc = _mm_or_si128(tSrc, alphaDst);
			MyStore_128i<isAligned>((__m128i*)pcDst, tSrc);
		}

		dst = (TS *)pcDst ;
		src = (TS *)pcSrc ;

		for( ; src <= ((TS*)(pcend)-4) ; src += 4, dst+=4 )
		{
			dst[0] = src[0] ;
			dst[1] = src[1] ;
			dst[2] = src[2] ;
		}
	}

	template <class TS, bool isAligned>
	SYS_INLINE STATIC void ReverseRow_C1(TS* dstRowpend, const TS* srcRow, int width)
	{
		const TS *pend = srcRow + width;
		int jmp = sizeof(__m128i)/sizeof(TS);
		__m128i r;
		TS toTellType = 0 ;

		for(  ; srcRow <= pend-jmp ; srcRow += jmp )
		{
			dstRowpend -= jmp;			
			r = MyLoad_128i<isAligned>((__m128i*)srcRow);
			fw_C1_reverse(r, toTellType);
			MyStream_128i<isAligned>((__m128i*)dstRowpend, r); 			
		}

		while(srcRow < pend)
		{
			*--dstRowpend = *srcRow++ ;
		}	
		_mm_mfence();
	}

	template <class TS, bool isAligned>
	SYS_INLINE STATIC void ReverseRow_C3(TS *dstRowpend, const TS *srcRow, int width)
	{
		const TS *pend = srcRow + width;
		int jmp = 3*sizeof(__m128i)/sizeof(TS);
		__m128i r1, r2, r3 ;
		TS toTellType = 0;

		for(  ; srcRow <= pend-jmp ; srcRow+= jmp ){	
			dstRowpend -= jmp;

			r1 = MyLoad_128i<isAligned>((__m128i*)srcRow );
			r2 = MyLoad_128i<isAligned>(((__m128i*)srcRow)+1 );
			r3 = MyLoad_128i<isAligned>(((__m128i*)srcRow)+2 );

			fw_C3_reverse(r1, r2, r3, toTellType);

			MyStream_128i<isAligned>((__m128i*)dstRowpend, r3 ); 
			MyStream_128i<isAligned>(((__m128i*)dstRowpend)+1, r2);
			MyStream_128i<isAligned>(((__m128i*)dstRowpend)+2, r1);	
		}

		for( ; srcRow < pend ; srcRow+=3 ){
			dstRowpend -= 3 ;
			dstRowpend[0] = srcRow[0];
			dstRowpend[1] = srcRow[1];
			dstRowpend[2] = srcRow[2];
		}
		_mm_mfence();
	}

	template <class TS, bool isAligned>
	SYS_INLINE STATIC void ReverseRow_C4(TS* dstRowpend, const TS* srcRow, int width)
	{
		const TS *pend = srcRow + width;
		int jmp = sizeof(__m128i)/sizeof(TS);
		__m128i r;
		TS toTellType = 0;

		for(  ; srcRow <= pend-jmp ; srcRow += jmp )	{	// jmp no. of TS's are there in one __m128i
			dstRowpend -= jmp;

			r = MyLoad_128i<isAligned>((__m128i*)srcRow);
			fw_C4_reverse(r, toTellType);	 
			MyStream_128i<isAligned>((__m128i*)dstRowpend,  r ); 		
		}

		for( ; srcRow+4 <= pend ; srcRow+=4 ){
			dstRowpend -= 4 ;
			dstRowpend[0] = srcRow[0] ;
			dstRowpend[1] = srcRow[1] ;
			dstRowpend[2] = srcRow[2] ;
			dstRowpend[3] = srcRow[3] ;		
		}
		_mm_mfence();
	}

	template <class TS, bool isAligned>
	SYS_INLINE STATIC void ReverseRow_AC4(TS* dstRowpend, const TS* srcRow, int width, const __m128i &mask)
	{
		const TS *pend = srcRow + width;
		int jmp = sizeof(__m128i)/sizeof(TS);
		__m128i r, alphaDst;
		TS toTellType = 0 ;

		for(  ; srcRow <= pend-jmp ; srcRow += jmp )	{			// jmp no. of TS's are there in one __m128i
			dstRowpend -= jmp;

			r = MyLoad_128i<isAligned>((__m128i*)srcRow );
			r = _mm_xor_si128(r, _mm_and_si128(r, mask));	//take off the alpha channel and then reverse.
			fw_C4_reverse(r, toTellType);

			alphaDst = _mm_and_si128(MyLoad_128i<isAligned>((__m128i*)dstRowpend), mask);
			r = _mm_or_si128(r, alphaDst);					// keep the alpha channel of the dstRowpend

			MyStore_128i<isAligned>((__m128i*)dstRowpend,  r ); 
		}

		for( ; srcRow+4 <= pend ; srcRow += 4 ){
			dstRowpend -= 4 ;
			dstRowpend[0] = srcRow[0];
			dstRowpend[1] = srcRow[1];
			dstRowpend[2] = srcRow[2];
		}
	}

	template <class TS, CH chSrc>
	static FwStatus MirrorBoth(const TS *pSrc, TS *pDst, int srcStep, int dstStep, FwiSize roiSize)
	{
		int y;
		int totalWidth = roiSize.width * ChannelCount(chSrc);
		bool isAligned ;
		const TS *ptSrcRow;
		TS *ptDstRow;

		switch(chSrc){
		case(C1):
			for (y=0; y<roiSize.height; y++){
				ptSrcRow = &pSrc[y*srcStep]; 
				ptDstRow = &pDst[(roiSize.height-y-1)*dstStep] + totalWidth;

				isAligned = IS_ALIGNED16_2(ptSrcRow, ptDstRow);

				if(isAligned)
					ReverseRow_C1<TS, true>(ptDstRow, ptSrcRow, totalWidth);
				else
					ReverseRow_C1<TS, false>(ptDstRow, ptSrcRow, totalWidth);
			}
			break;
		case(C3):				
			for (y=0; y<roiSize.height; y++){
				ptSrcRow = &pSrc[y*srcStep] ;
				ptDstRow = &pDst[(roiSize.height-y-1)*dstStep] + totalWidth;
				isAligned = IS_ALIGNED16_2(ptSrcRow, ptDstRow);

				if(isAligned)
					ReverseRow_C3<TS, true>(ptDstRow, ptSrcRow, totalWidth);	
				else
					ReverseRow_C3<TS, false>(ptDstRow, ptSrcRow, totalWidth);	
			}
			break;
		case(C4) :
			for (y=0; y<roiSize.height; y++){
				ptSrcRow = &pSrc[y*srcStep];
				ptDstRow = &pDst[(roiSize.height-y-1)*dstStep] + totalWidth;

				isAligned = IS_ALIGNED16_2(ptSrcRow, ptDstRow);

				if(isAligned)
					ReverseRow_C4<TS, true>(ptDstRow, ptSrcRow, totalWidth);
				else
					ReverseRow_C4<TS, false>(ptDstRow, ptSrcRow, totalWidth);
			}
			break;			
		case(AC4) : // AC4
			__m128i mask;
			Get_AC4_mask<TS>(mask);

			for (y=0; y<roiSize.height; y++){
				ptSrcRow = &pSrc[y*srcStep];
				ptDstRow = &pDst[(roiSize.height-y-1)*dstStep] + totalWidth;

				isAligned = IS_ALIGNED16_2(ptSrcRow, ptDstRow);
				if(isAligned)
					ReverseRow_AC4<TS, true>(ptDstRow, ptSrcRow, totalWidth, mask);
				else
					ReverseRow_AC4<TS, false>(ptDstRow, ptSrcRow, totalWidth, mask);
			}
			break;

		default:
			assert(false);
		}	
		return fwStsNoErr;
	};

	template <class TS, CH chSrc>
	static FwStatus MirrorVertical(const TS *pSrc, TS *pDst, int srcStep, int dstStep, FwiSize roiSize)
	{
		int y;
		int totalWidth = roiSize.width * ChannelCount(chSrc);
		bool isAligned ;
		const TS *ptSrcRow;
		TS *ptDstRowpend;

		switch(chSrc){
		case(C1):
			for (y=0; y<roiSize.height; y++){
				ptSrcRow = &pSrc[y*srcStep]; 
				ptDstRowpend = &pDst[y*dstStep] + totalWidth;

				isAligned = IS_ALIGNED16_2(ptSrcRow, ptDstRowpend);

				if(isAligned)
					ReverseRow_C1<TS, true>(ptDstRowpend, ptSrcRow, totalWidth);
				else
					ReverseRow_C1<TS, false>(ptDstRowpend, ptSrcRow, totalWidth);
			}
			break;
		case(C3):				
			for (y=0; y<roiSize.height; y++){
				ptSrcRow = &pSrc[y*srcStep] ;
				ptDstRowpend = &pDst[y*dstStep] + totalWidth;
				isAligned = IS_ALIGNED16_2(ptSrcRow, ptDstRowpend);

				if(isAligned)
					ReverseRow_C3<TS, true>(ptDstRowpend, ptSrcRow, totalWidth);	
				else
					ReverseRow_C3<TS, false>(ptDstRowpend, ptSrcRow, totalWidth);	
			}
			break;
		case(C4) :
			for (y=0; y<roiSize.height; y++){
				ptSrcRow = &pSrc[y*srcStep];
				ptDstRowpend = &pDst[y*dstStep]+ totalWidth;

				isAligned = IS_ALIGNED16_2(ptSrcRow, ptDstRowpend);

				if(isAligned)
					ReverseRow_C4<TS, true>(ptDstRowpend, ptSrcRow, totalWidth);
				else
					ReverseRow_C4<TS, false>(ptDstRowpend, ptSrcRow, totalWidth);
			}
			break;			
		case(AC4) : // AC4
			__m128i mask;
			Get_AC4_mask<TS>(mask);

			for (y=0; y<roiSize.height; y++){
				ptSrcRow = &pSrc[y*srcStep];
				ptDstRowpend = &pDst[y*dstStep]+ totalWidth;

				isAligned = IS_ALIGNED16_2(ptSrcRow, ptDstRowpend);
				if(isAligned)
					ReverseRow_AC4<TS, true>(ptDstRowpend, ptSrcRow, totalWidth, mask);
				else
					ReverseRow_AC4<TS, false>(ptDstRowpend, ptSrcRow, totalWidth, mask);
			}
			break;

		default:
			assert(false);
		}	
		return fwStsNoErr;
	};


	template <class TS, CH chSrc>
	static FwStatus MirrorHorizontal(const TS *pSrc, int srcStep, TS *pDst, int dstStep, FwiSize roiSize )
	{
		int numBytes = roiSize.width * ChannelCount(chSrc) * sizeof(TS);
		int y ;
		const TS *ptSrcRow; 
		TS *ptDstRow ;
		bool isAligned ;

		switch((chSrc <= 4))
		{
		case(true):
			for (y=0; y<roiSize.height; y++) {
				ptSrcRow = &pSrc[y*srcStep] ;
				ptDstRow = &pDst[(roiSize.height-y-1)*dstStep];
				isAligned = IS_ALIGNED16_2(ptSrcRow, ptDstRow);

				if(isAligned)
					Memcpy_SSE2<true>(ptDstRow, ptSrcRow, numBytes);
				else
					Memcpy_SSE2<false>(ptDstRow, ptSrcRow, numBytes);
			}
			break;
		default: //AC4
			__m128i mask;
			Get_AC4_mask<TS>(mask);	
			for (y=0; y<roiSize.height; y++){
				ptSrcRow = &pSrc[y*srcStep] ;
				ptDstRow = &pDst[(roiSize.height-y-1)*dstStep];
				isAligned = IS_ALIGNED16_2(ptSrcRow, ptDstRow);

				if(isAligned)
					Memcpy_AC4_SSE2<TS, true>(ptDstRow, ptSrcRow, numBytes, mask);	
				else
					Memcpy_AC4_SSE2<TS, false>(ptDstRow, ptSrcRow, numBytes, mask);	
			}
			break;
		}
		return fwStsNoErr;
	}

	template< class TS, CH chSrc>	
	static FwStatus My_FW_Mirror_SSE2(const TS* pSrc, int srcStep, TS* pDst, int dstStep, 
		FwiSize roiSize, FwiAxis flip) 
	{
		if (pSrc==0 || pDst==0) return fwStsNullPtrErr;
		if (roiSize.height <=0 || roiSize.width <=0) return fwStsSizeErr;
		if (srcStep <=0 || dstStep <=0) return fwStsStepErr;


		//dstStep and srcStep are byte size
		//we need to change it with data array size
		dstStep = dstStep / (sizeof(TS));
		srcStep = srcStep / (sizeof(TS));

		switch(flip) {
		case fwAxsHorizontal :
			return MirrorHorizontal<TS, chSrc>(pSrc, srcStep, pDst, dstStep, roiSize );

		case fwAxsVertical :
			return MirrorVertical<TS, chSrc>(pSrc, pDst, srcStep, dstStep, roiSize);

		case fwAxsBoth : 
			return MirrorBoth<TS, chSrc>(pSrc, pDst, srcStep, dstStep, roiSize);

		default:
			return fwStsMirrorFlipErr;
		} 	
	}
} //namespace OPT_LEVEL

using namespace OPT_LEVEL;
// FW calls
// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C1R)(const Fw8u *pSrc, int srcStep, 
													Fw8u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw8u, C1>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw8u, C1> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C3R)(const Fw8u *pSrc, int srcStep, 
													Fw8u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw8u, C3>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw8u, C3> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C4R)(const Fw8u *pSrc, int srcStep, 
													Fw8u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw8u, C4>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw8u, C4> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_AC4R)(const Fw8u *pSrc, int srcStep, 
													 Fw8u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw8u, AC4>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw8u, AC4> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

// unsigned short data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C1R)(
	const Fw16u *pSrc, int srcStep, 
	Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw16u, C1>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw16u, C1> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

// unsigned short data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C3R)(
	const Fw16u *pSrc, int srcStep, 
	Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw16u, C3>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw16u, C3> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

// unsigned short data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C4R)(
	const Fw16u *pSrc, int srcStep, 
	Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw16u, C4>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw16u, C4> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

// unsigned short data type with 4(3+alpha) channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_AC4R)(
	const Fw16u *pSrc, int srcStep, 
	Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw16u, AC4>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw16u, AC4> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

//integer data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C1R)(
	const Fw32s* pSrc, int srcStep, 
	Fw32s* pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw32s, C1>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw32s, C1> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

//integer data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C3R)(
	const Fw32s* pSrc, int srcStep, 
	Fw32s* pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw32s, C3>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw32s, C3> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

//integer data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C4R)(
	const Fw32s* pSrc, int srcStep, 
	Fw32s* pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw32s, C4>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw32s, C4> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

//integer data with 4(3+alpha) channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_AC4R)(
	const Fw32s* pSrc, int srcStep, 
	Fw32s* pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return My_FW_Mirror_SSE2<Fw32s, AC4>(pSrc, srcStep, pDst, dstStep, roiSize, flip);	
	default:
		return My_FW_Mirror<Fw32s, AC4> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
	}
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C1R)(const Fw32f *pSrc, int srcStep, 
													 Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	return My_FW_Mirror<Fw32f, C1> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C3R)(const Fw32f *pSrc, int srcStep, 
													 Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	return My_FW_Mirror<Fw32f, C3> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C4R)(const Fw32f *pSrc, int srcStep, 
													 Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	return My_FW_Mirror<Fw32f, C4> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_AC4R)(const Fw32f *pSrc, int srcStep, 
													  Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip) 
{
	return My_FW_Mirror<Fw32f, AC4> (pSrc, srcStep, pDst, dstStep, roiSize, flip);
}

// In-place calls
// 8 data with 1 channel, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C1IR)(Fw8u *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip)
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw8u, C1>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw8u, C1> (pSrpcDst, srpcDstStep, roiSize, flip);
	}
}

// 8 data with 3 channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C3IR)(Fw8u *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw8u, C3>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw8u, C3> (pSrpcDst, srpcDstStep, roiSize, flip);
	}
}

// 8 data with 4 channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C4IR)(Fw8u *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw8u, C4>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw8u, C4> (pSrpcDst, srpcDstStep, roiSize, flip);
	}
}

// 8 data with 4(3+alpha) channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_AC4IR)(Fw8u *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw8u, AC4>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw8u, AC4> (pSrpcDst, srpcDstStep, roiSize, flip);
	}
}

// unsigned short data with 1 channel, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C1IR)(Fw16u *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw16u, C1>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw16u, C1> (pSrpcDst, srpcDstStep, roiSize, flip);
	}	
}

// unsigned short data with 3 channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C3IR)(Fw16u *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw16u, C3>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw16u, C3> (pSrpcDst, srpcDstStep, roiSize, flip);
	}		
}

// unsigned short data with 4 channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C4IR)(Fw16u *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{	
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw16u, C4>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw16u, C4> (pSrpcDst, srpcDstStep, roiSize, flip);
	}
}

// unsigned short data with 4(3+alpha) channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_AC4IR)(Fw16u *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw16u, AC4>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw16u, AC4> (pSrpcDst, srpcDstStep, roiSize, flip);
	}
}

// integer data with 1 channel, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C1IR)(Fw32s* pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw32s, C1>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw32s, C1> (pSrpcDst, srpcDstStep, roiSize, flip);
	}
}

// integer data with 3 channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C3IR)(Fw32s* pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw32s, C3>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw32s, C3> (pSrpcDst, srpcDstStep, roiSize, flip);
	}
}

// integer data with 4 channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C4IR)(Fw32s* pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw32s, C4>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw32s, C4> (pSrpcDst, srpcDstStep, roiSize, flip);
	}
}

// integer data with 4(=3+alpha) channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_AC4IR)(Fw32s* pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		return MyMirror_I_sse2<Fw32s, AC4>(pSrpcDst, srpcDstStep, roiSize, flip);	
	default:
		return MyMirror_I<Fw32s, AC4> (pSrpcDst, srpcDstStep, roiSize, flip);
	}
}

// float data with 1 channel, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C1IR)(Fw32f *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	return MyMirror_I<Fw32f, C1> (pSrpcDst, srpcDstStep, roiSize, flip);
}

// float data with 3 channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C3IR)(Fw32f *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	return MyMirror_I<Fw32f, C3> (pSrpcDst, srpcDstStep, roiSize, flip);
}

// float data with 4 channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C4IR)(Fw32f *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	return MyMirror_I<Fw32f, C4> (pSrpcDst, srpcDstStep, roiSize, flip);
}

// float data with 4(=3+alpha) channels, in-place
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_AC4IR)(Fw32f *pSrpcDst, int srpcDstStep, FwiSize roiSize, FwiAxis flip) 
{
	return MyMirror_I<Fw32f, AC4> (pSrpcDst, srpcDstStep, roiSize, flip);
}

#endif //BUILD_NUM_AT_LEAST


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
