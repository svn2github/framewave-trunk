/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __COPY_H__
#define __COPY_H__

#include "fe.h"

namespace OPT_LEVEL
{

/////////////////////////////////////// REF FUNCTIONS ///////////////////////////////////////////

template<class TSD, CH CSD>
ISV Copy(const TSD *s,TSD *d)
    {
    for(int i=0;i<CSD;i++)
        d[i] = s[i];
    }

template<class TSD>
ISV CopyNto1(const TSD *s, TSD *d)
    {
    *d = s[0];
    }

template<class TSD, CH CSD>
ISV CopyM( const A8U *m, const TSD *s, TSD * d)
    {
    if( *m )
        for(int i=0;i<CSD;i++)
            d[i] = s[i];		
    }

template<class TSD, CH CSD>
ISV Copy1toN(const TSD *s, TSD *d)
    {
    for(int i=0;i<CSD;i++)
        d[i] = s[0];
    }

ISV Copy_C3AC4(const A8U *s, A8U *d)
    {
    *d = *s;
    *((A16S*)(d+1)) = *((A16S*)(s+1));		
    }

ISV Copy_C3AC4(const A16S *s, A16S *d)
    {
    *d = *s;
    *((A32S*)(d+1)) = *((A32S*)(s+1));		
    }

ISV Copy_C3AC4(const A32S *s, A32S *d)
    {
    *d = *s;
    *((A64S*)(d+1)) = *((A64S*)(s+1));		
    }

ISV Copy_C3AC4(const A32F *s, A32F *d)
    {
    *d = *s;
    *(d+1) = *(s+1);
    *(d+2) = *(s+2);	
    }

template< class TSD>
ISV Copy1(const TSD *s, TSD *d)
    {
    *d = *s;		
    }

template<class TSD>
ISV Copy3to1( const TSD *s1, const TSD *s2, const TSD *s3, TSD * d)
    {
    d[0] = *s1;		
    d[1] = *s2;
    d[2] = *s3;
    }

template<class TSD>
ISV Copy4to1( const TSD *s1, const TSD *s2, const TSD *s3, const TSD *s4, TSD *d)
    {
    d[0] = *s1;		
    d[1] = *s2;
    d[2] = *s3;
    d[3] = *s4;
    }

template< class TSD>
ISV Copy1to3(const TSD * s, TSD * d1, TSD * d2, TSD * d3)
    {
    *d1 = s[0];		
    *d2 = s[1];
    *d3 = s[2];		
    }

template< class TSD>
ISV Copy1to4( const TSD * s, TSD * d1, TSD * d2, TSD * d3, TSD * d4)
    {
    *d1 = s[0];		
    *d2 = s[1];
    *d3 = s[2];		
    *d4 = s[3];
    }

/////////////////////////////////////// COMMON SSE2 FUNCTIONS FOR SET, COPY AND SWAP /////////////////////

ISV Copy_With_Mask(__m128i &src, __m128i &dst, __m128i &mask)
    {
    dst = _mm_and_si128(dst, mask);								// make the locations of bytes to be copied as zeros
    mask = _mm_andnot_si128(mask, src);							// get the bytes to be copied
    dst = _mm_or_si128(dst,mask);								// copy 
    }
ISV Copy_With_Mask(const __m128i &src, __m128i &dst, __m128i &mask)
    {
    dst = _mm_and_si128(dst, mask);								// make the locations of bytes to be copied as zeros
    mask = _mm_andnot_si128(mask, src);							// get the bytes to be copied
    dst = _mm_or_si128(dst,mask);								// copy 
    }
ISV Copy_With_Mask(__m128i &src, __m128i &dst, const __m128i &mask)
    {
    __m128i temp;
    temp = _mm_andnot_si128(mask, src);							// get the data to copy
    dst = _mm_and_si128(dst,mask);								// make dst to zero retaining only alphas
    dst = _mm_or_si128(dst, temp);								// copy
    }

/////////////////////////////////////// SSE2 SETUP FUNCTIONS ////////////////////////////////////////////////

// AC4R
ISV Copy_AC4R_SETUP_I(__m128i &mask, const A8U &)	// 8U
    {
    mask = _mm_setzero_si128();						// set to zeros
    mask = _mm_cmpeq_epi32(mask,mask);				// set to ones
    mask = _mm_slli_epi32(mask, 24);				// mask to get alpha    
    }
ISV Copy_AC4R_SETUP_I(__m128i &mask, const A16S &)	// 16S
    {
    mask = _mm_setzero_si128();						// set to zeros
    mask = _mm_cmpeq_epi32(mask ,mask );			// set to ones
    mask = _mm_slli_epi64( mask, 48);				// mask to get alphas
    }
ISV Copy_AC4R_SETUP_I(__m128i &mask, const A32S &)	// 32S
    {
    mask = _mm_setzero_si128();						// set to zeros
    mask = _mm_cmpeq_epi32(mask, mask);				// set to ones
    mask = _mm_slli_si128(mask, 12);				// mask to get alpha
    }
ISV Copy_AC4R_SETUP_I(__m128i &mask, const A32F &)	// 32F
    {
    mask = _mm_setzero_si128();						// set to zeros
    mask = _mm_cmpeq_epi32(mask, mask);				// set to ones
    mask = _mm_slli_si128(mask, 12);				// mask to get alpha
    }

// C1MR, C3MR, C4MR -- All Data Types
ISV Copy_CMR_SETUP_I(__m128i &mask)					// Used for all C1,C3,C4 MR functions
    {
    mask = _mm_setzero_si128();						// set to zeros
    }

// AC4MR
ISV Copy_AC4MR_SETUP_I(__m128i &AC4_mask, __m128i &zero, const A8U &)	// A8U
    {
    zero     = _mm_setzero_si128();					// set to zeros
    AC4_mask = _mm_cmpeq_epi32(zero, zero);			// set to ones	
    AC4_mask = _mm_slli_epi32(AC4_mask, 24);	
    }

// C1C3R
ISV Copy_C1C3R_SETUP_I(__m128i &mask1, __m128i &mask2, __m128i &mask3, const A8U &)
    {
    mask1 = _mm_set_epi32(0x00ffff00,0xffff00ff,0xff00ffff,0x00ffff00);
    mask2 = _mm_shuffle_epi32(mask1, _MM_SHUFFLE(1,3,2,1));
    mask3 = _mm_shuffle_epi32(mask1, _MM_SHUFFLE(2,1,3,2));
    }

ISV Copy_C1C3R_SETUP_I(__m128i &mask1, __m128i &mask2, __m128i &mask3, const A16S &)
    {
    mask1 = _mm_set_epi32(0xffff0000,0xffffffff,0x0000ffff,0xffff0000);
    mask2 = _mm_shuffle_epi32(mask1, _MM_SHUFFLE(1,3,2,1));
    mask3 = _mm_shuffle_epi32(mask1, _MM_SHUFFLE(2,1,3,2));
    }

//AC4C3R
ISV Copy_16s_AC4C3R_SETUP_I(__m128i &mask2, __m128i &notmask2)
    {
    //can use setones and shifts to increase speed
    mask2    = _mm_set_epi32(0x0000ffff,0xffffffff,0x00000000,0x00000000);		// mask to get r2g2b2
    notmask2 = _mm_set_epi32(0xffff0000,0x00000000,0x0000ffff,0xffffffff);		// mask to make r2g2b2 to zero
    }

/////////////////////////////////////// SSE2 FUNCTIONS ////////////////////////////////////////////////////

// C1R,C3R,C4R
template<class TSD, CH CSD, IsAlign ia>
ISV Copy_CR_Custom_SSE2_I(const TSD *s, TSD *d, U32 &pixCount)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;
    __m128i *end	= (__m128i *) (d + pixCount * ChannelCount(CSD));
	XMM128 val;	

    for( ; dst < end; )
        {
    		LoadStoreModules::LOAD<16,DT_SSE2,ia,STREAM_FLSE>(&val,(const void*)src++);
        	LoadStoreModules::STORE<16,DT_SSE2,ia,STREAM_TRUE>(&val,(void*)dst++);
        }
    _mm_mfence();
    }

template<class TSD,IsAlign ia>
ISV Copy_C3R_Custom_SSE2_I(const TSD *s, TSD *d, U32 &pixCount)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *dst	= (__m128i*)d;
    __m128i *end	= (__m128i *) (d + pixCount * ChannelCount(C3));
	XMM128 val;	

    for( ; dst < end; )
        {
    		LoadStoreModules::LOAD<16,DT_SSE2,ia,STREAM_FLSE>(&val,(const void*)src++);
        	LoadStoreModules::STORE<16,DT_SSE2,ia,STREAM_TRUE>(&val,(void*)dst++);

            LoadStoreModules::LOAD<16,DT_SSE2,ia,STREAM_FLSE>(&val,(const void*)src++);
        	LoadStoreModules::STORE<16,DT_SSE2,ia,STREAM_TRUE>(&val,(void*)dst++);

            LoadStoreModules::LOAD<16,DT_SSE2,ia,STREAM_FLSE>(&val,(const void*)src++);
        	LoadStoreModules::STORE<16,DT_SSE2,ia,STREAM_TRUE>(&val,(void*)dst++);
        }
    _mm_mfence();
    }

// AC4R	-- All Data Types
ISV Copy_AC4R_SSE2_I(RegFile &reg, const __m128i &data)
    {
    __m128i mask;
    mask = _mm_andnot_si128(data, reg.src1[0].i);					// get the data to copy
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,data);				// make dst to zero retaining only alphas
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i, mask);				// copy
    }

// C1MR
ISV Copy_C1MR_SSE2_I(RegFile &reg, const __m128i &data, const A8U &)// 8U
    {
    __m128i mask;
    mask = _mm_cmpeq_epi8(reg.src1[0].i, data);						// just opposite mask
    Copy_With_Mask(reg.src2[0].i, reg.dst[0].i, mask);
    }

template<IsAlign ia>
ISV Copy_16s_C1MR_Custom_SSE2_I(const A8U *m, const A16S *s, A16S *d, U32 &pixCount)
    {
    __m128i  *dst     = (__m128i*)d;
    __m128i  *end	  = (__m128i *) (d + pixCount * ChannelCount(C1));
    __m128i  mask_temp;
    XMM128 dst_data, src_data;    
    int mask;

    for( ; dst < end; m+=8, s+=8, dst++)
        {
        mask_temp = _mm_loadl_epi64((__m128i const*)m);
        mask_temp = _mm_cmpeq_epi8(mask_temp, _mm_setzero_si128());	// just opposite mask

        mask = _mm_movemask_epi8(mask_temp);

        switch(mask)
            {
            case 0x0000ff00:
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&src_data, (const void*)s);
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&src_data, (void*)dst);
                continue;

            default:
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&src_data, (const void*)s);
                mask_temp = _mm_unpacklo_epi8(mask_temp, mask_temp);
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)dst);
                Copy_With_Mask(src_data.i, dst_data.i, mask_temp);
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)dst);
            }
        }
    }

template<class TSD, IsAlign ia>
ISV Copy_32_C1MR_Custom_SSE2_I(const A8U *m, const TSD *s, TSD *d, U32 &pixCount)
    {
    __m128i  *dst     = (__m128i*)d;
    __m128i  *end	  = (__m128i *) (d + pixCount * ChannelCount(C1));
    XMM128 mask_temp,dst_data, src_data;    
    int mask;

    for( ; dst < end; m+=4, s+=4, dst++)
        {
        LoadStoreModules::LOAD<4, DT_SSE2, ia, STREAM_FLSE>(&mask_temp, (const void*)m);
        mask_temp.i = _mm_cmpeq_epi8(mask_temp.i, _mm_setzero_si128());	// just opposite mask

        mask = _mm_movemask_epi8(mask_temp.i);

        switch(mask)
            {
            case 0x0000fff0:
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)s);
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (void*)dst);
                continue;

            default:
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)s);
                mask_temp.i = _mm_unpacklo_epi8(mask_temp.i, mask_temp.i);
                mask_temp.i = _mm_unpacklo_epi16(mask_temp.i, mask_temp.i);
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)dst);
                Copy_With_Mask(src_data.i, dst_data.i, mask_temp.i);
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)dst);
            }
        }
    _mm_mfence();
    }

// C3MR
ISV Copy_C3MR_SSE2_I(RegFile &reg, const __m128i &data, const A8U &)			// 8U
    {
    __m128i mask_temp0,mask_temp1,mask_temp2;

    mask_temp1 = mask_temp2 = mask_temp0 = _mm_cmpeq_epi8(reg.src1[0].i,data);	// just opposite mask
    
    // Replace CBL with SSEPlus
    // CBL_SSE2::Convert_3P_to_3C_8bit(mask_temp0, mask_temp1, mask_temp2);
    ssp_convert_3p_3c_epi8(&mask_temp0, &mask_temp1, &mask_temp2);
    
    Copy_With_Mask( reg.src2[0].i, reg.dst[0].i, mask_temp0);
    Copy_With_Mask( reg.src2[1].i, reg.dst[1].i, mask_temp1);
    Copy_With_Mask( reg.src2[2].i, reg.dst[2].i, mask_temp2);
    }

template<IsAlign ia>
ISV Copy_C3MR_16s_Custom_SSE2_I(const A8U *m, const A16S *s, A16S *d, U32 &pixCount)
    {
    A16S  *end	  = d + pixCount * ChannelCount(C3);
    XMM128 dst_data, src_data, mask_data;    
    __m128i mask;
    int mInt;

    for( ; d < end; m+=8, s+=24, d+=24)
        {
        LoadStoreModules::LOAD<8, DT_SSE2, ia, STREAM_TRUE>(&mask_data, (const void*)m);
        mask = _mm_cmpeq_epi8(mask_data.i, _mm_setzero_si128());	// just opposite mask
        
        mInt = _mm_movemask_epi8(mask);

            if( (mInt&(7)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)s);
               
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)d);
                
                if ((mInt&1) == 0)  dst_data.u32[0] = src_data.u32[0] , dst_data.s16[2] = src_data.s16[2];
                if ((mInt&2) == 0) dst_data.s16[3] = src_data.s16[3], dst_data.u32[2] = src_data.u32[2];
                if ((mInt&4) == 0) dst_data.u32[3] = src_data.u32[3];

                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)d);
            }

            else
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)s);
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (void*)d);
            }

            if( (mInt&(60)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)(s+8));
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)(d+8));
                if ((mInt&4) == 0)  dst_data.s16[0] = src_data.s16[0];
                if ((mInt&8) == 0) dst_data.s16[1] = src_data.s16[1], dst_data.u32[1] = src_data.u32[1];
                if ((mInt&16) == 0) dst_data.u32[2] = src_data.u32[2], dst_data.s16[6] = src_data.s16[6];
                if ((mInt&32) == 0) dst_data.s16[7] = src_data.s16[7];
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)(d+8));
            }

            else
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)(s+8));
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (void*)(d+8));
            }
            if( (mInt&(224)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)(s+16));
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)(d+16));
                if ((mInt&32) == 0) dst_data.u32[0] = src_data.u32[0] ;
                if ((mInt&64) == 0) dst_data.u32[1] = src_data.u32[1], dst_data.s16[4] = src_data.s16[4] ;
                if ((mInt&128) == 0) dst_data.s16[5] = src_data.s16[5], dst_data.u32[3] = src_data.u32[3] ;
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)(d+16));
            }

            else
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)(s+16));
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (void*)(d+16));
            }          
        }
    _mm_mfence();
    }


template<class TSD, IsAlign ia>
ISV Copy_C3MR_Custom_SSE2_I(const A8U *m, const TSD *s, TSD *d, U32 &pixCount)
    {
    TSD  *end	  = d + pixCount * ChannelCount(C3);
    XMM128 dst_data, src_data, mask_data;    
    __m128i mask;
    int mInt;

    for( ; d < end; m+=4, s+=12, d+=12)
        {
        LoadStoreModules::LOAD<4, DT_SSE2, ia, STREAM_TRUE>(&mask_data, (const void*)m);
        mask = _mm_cmpeq_epi8(mask_data.i, _mm_setzero_si128());	// just opposite mask
        
        mInt = _mm_movemask_epi8(mask);

            if( (mInt&(3)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)s);
               
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)d);
                
                if ((mInt&1) == 0)  dst_data.u64[0] = src_data.u64[0] , dst_data.u32[2] = src_data.u32[2];
                if ((mInt&2) == 0)  dst_data.u32[3] = src_data.u32[3];

                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)d);
            }

            else
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)s);
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (void*)d);
            }

            if( (mInt&(6)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)(s+4));
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)(d+4));
                if ((mInt&2) == 0)  dst_data.u64[0] = src_data.u64[0];
                if ((mInt&4) == 0) dst_data.u64[1] = src_data.u64[1];
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)(d+4));
            }

            else
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)(s+4));
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (void*)(d+4));
            }
            if( (mInt&(12)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)(s+8));
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)(d+8));
                if ((mInt&4) == 0) dst_data.u32[0] = src_data.u32[0];
                if ((mInt&8) == 0) dst_data.u32[1] = src_data.u32[1], dst_data.u64[1] = src_data.u64[1];
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)(d+8));
            }

            else
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (const void*)(s+8));
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&src_data, (void*)(d+8));
            } 
        }
    _mm_mfence();
    }




ISV Copy_C3MR_SSE2_I(RegFile &reg, const __m128i &data, const A32S &)// 32S
    {
    __m128i mask, mask_temp;

    mask = _mm_cmpeq_epi8(reg.src1[0].i,data);						 // just opposite mask
    mask = _mm_unpacklo_epi8(mask,mask);							 // the mask is only 4 bytes so.. unpack it to 8 bytes
    mask = _mm_unpacklo_epi16(mask,mask);							 // again unpack it to 16 bytes

    mask_temp = _mm_unpacklo_epi32(mask, mask);
    mask_temp = _mm_shuffle_epi32(mask_temp, _MM_SHUFFLE(3,1,1,0));
    Copy_With_Mask(reg.src2[0].i, reg.dst[0].i, mask_temp);

    mask_temp = _mm_srli_si128(mask, 4);
    mask_temp = _mm_unpacklo_epi32(mask_temp, mask_temp);
    Copy_With_Mask(reg.src2[1].i, reg.dst[1].i, mask_temp);

    mask_temp = _mm_srli_si128(mask, 8);
    mask_temp = _mm_unpacklo_epi32(mask_temp, mask_temp);
    mask_temp = _mm_shuffle_epi32(mask_temp, _MM_SHUFFLE(3,2,2,0));
    Copy_With_Mask(reg.src2[2].i, reg.dst[2].i, mask_temp);
    }

ISV Copy_C3MR_SSE2_I(RegFile &reg, const __m128i &data, const A32F &)// 32F
    {
    __m128i mask, mask_temp;

    mask = _mm_cmpeq_epi8(reg.src1[0].i,data);						 // just opposite mask
    mask = _mm_unpacklo_epi8(mask,mask);							 // the mask is only 4 bytes so.. unpack it to 8 bytes
    mask = _mm_unpacklo_epi16(mask,mask);							 // again unpack it to 16 bytes

    mask_temp = _mm_unpacklo_epi32(mask, mask);
    mask_temp = _mm_shuffle_epi32(mask_temp, _MM_SHUFFLE(3,1,1,0));
    Copy_With_Mask(reg.src2[0].i, reg.dst[0].i, mask_temp);

    mask_temp = _mm_srli_si128(mask, 4);
    mask_temp = _mm_unpacklo_epi32(mask_temp, mask_temp);
    Copy_With_Mask(reg.src2[1].i, reg.dst[1].i, mask_temp);

    mask_temp = _mm_srli_si128(mask, 8);
    mask_temp = _mm_unpacklo_epi32(mask_temp, mask_temp);
    mask_temp = _mm_shuffle_epi32(mask_temp, _MM_SHUFFLE(3,2,2,0));
    Copy_With_Mask(reg.src2[2].i, reg.dst[2].i, mask_temp);
    }

// C4MR
ISV Copy_C4MR_SSE2_I(RegFile &reg, const __m128i &data, const A8U &) // 8U
    {
    __m128i mask;
    mask = _mm_cmpeq_epi8(reg.src1[0].i,data);						 // just opposite mask
    mask = _mm_unpacklo_epi8(mask, mask);							 // Distribute the mask to 8 bytes
    mask = _mm_unpacklo_epi16(mask, mask);							 // Distribute the mask to 16 bytes

    Copy_With_Mask(reg.src2[0].i, reg.dst[0].i, mask);
    }

template<IsAlign ia>
ISV Copy_16s_C4MR_Custom_SSE2_I(const A8U *m, const A16S *s, A16S *d, U32 &pixCount)
    {
    A16S*  end	  = (d + pixCount * ChannelCount(C4));
    XMM128 src_data;    
    int       index;

    for( ; d < end; m+=2, s+=8, d+=8)
        {
        index = (((!(*(m+1))) << 1) | (!(*m)));
        switch(index)
            {
            case 0:	
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&src_data, (const void*)s);
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&src_data, (void*) d);
                break;

            case 1: *((A64S*)(d+4)) = *((A64S*)(s+4));
                break;

            case 2: *((A64S*)d) = *((A64S*)s);
            }
        }
    }

template<class TSD, IsAlign ia>
ISV Copy_32_C4MR_Custom_SSE2_I(const A8U *m, const TSD *s, TSD *d, U32 &pixCount)
    {
    __m128i  *src     = (__m128i*)s;
    __m128i  *dst     = (__m128i*)d;
    __m128i  *end	  = (__m128i *) (d + pixCount * ChannelCount(C4));
    XMM128 src_data;    

    for( ; dst < end; m++, src++, dst++)
        {
        if(*m)
            {
            LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&src_data, (const void*)src);
            LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&src_data, (void*)dst);
            }
        }
    }

// AC4MR
ISV Copy_AC4MR_SSE2_I(RegFile &reg, const __m128i &AC4_mask, const __m128i &zero, const A8U &)
    {
    __m128i mask;	
    mask = _mm_cmpeq_epi8(reg.src1[0].i,zero);						 // just opposite mask
    mask = _mm_unpacklo_epi8(mask, mask);							 // Distribute the mask to 8 bytes
    mask = _mm_unpacklo_epi16(mask, mask);							 // Distribute the mask to 16 bytes
    mask = _mm_or_si128(mask, AC4_mask);							 // Make the alpha locations to one(So as not to copy to these locations)
    Copy_With_Mask(reg.src2[0].i, reg.dst[0].i, mask);
    }

template<IsAlign ia>
ISV Copy_16s_AC4MR_Custom_SSE2_I(const A8U *m, const A16S *s, A16S *d, U32 &pixCount)
    {
    A16S*  end	  = (d + pixCount * ChannelCount(AC4));
    XMM128 src_data, dst_data;    
    int       index;

    for( ; d < end; m+=2, s+=8, d+=8)
        {
        index = (((!(*(m+1))) << 1) | (!(*m)));
        switch(index)
            {
            case 0:	
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&src_data, (const void*)s);
                src_data.i = _mm_slli_epi64(src_data.i, 16);
                src_data.i = _mm_srli_epi64(src_data.i, 16);
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)d);
                dst_data.i = _mm_srli_epi64(dst_data.i, 48);
                dst_data.i = _mm_slli_epi64(dst_data.i, 48);
                dst_data.i = _mm_or_si128(src_data.i, dst_data.i);
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)d);
                break;

            case 1: *((A32S*)(d+4)) = *((A32S*)(s+4));
                *(d+6) = *(s+6);
                break;

            case 2: *((A32S*)d) = *((A32S*)s);
                *(d+2) = *(s+2);
            }
        }
    }

template<class TSD, IsAlign>
ISV Copy_32_AC4MR_Custom_SSE2_I(const A8U *m, const TSD *s, TSD *d, U32 &pixCount)
    {
    TSD*  end	  = (d + pixCount * ChannelCount(AC4));

    for( ; d < end; m++, s+=4, d+=4)
        {
        if(*m)
            {
            *((A64S*)d) = *((A64S*)s);
            *(d+2) = *(s+2);
            }
        }
    }

//C3CR
template<class TSD, IsAlign>
ISV Copy_C3CR_Custom_SSE2_I(const TSD *s, TSD *d, U32 &pixCount)
    {
    const TSD *src	= s;
    TSD *dst	= d;
    TSD *end	= d + pixCount * ChannelCount(C3);

    for( ;(dst + 47) < end; dst += 48, src+= 48)
        {
        *(dst     ) = *(src     );
        *(dst +  3) = *(src +  3);
        *(dst +  6) = *(src +  6);
        *(dst +  9) = *(src +  9);

        *(dst + 12) = *(src + 12);
        *(dst + 15) = *(src + 15);
        *(dst + 18) = *(src + 18);
        *(dst + 21) = *(src + 21);

        *(dst + 24) = *(src + 24);
        *(dst + 27) = *(src + 27);
        *(dst + 30) = *(src + 30);
        *(dst + 33) = *(src + 33);

        *(dst + 36) = *(src + 36);
        *(dst + 39) = *(src + 39);
        *(dst + 42) = *(src + 42);
        *(dst + 45) = *(src + 45);
        }

    for( ;dst < end; dst += 3, src+=3)
        {
        *dst = *src;
        }		
    }

//C4CR
template<class TSD, IsAlign>
ISV Copy_C4CR_Custom_SSE2_I(const TSD *s, TSD *d, U32 &pixCount)
    {
    const TSD *src	= s;
    TSD *dst	= d;
    TSD *end	= d + pixCount * ChannelCount(C4);

    for( ;(dst + 63) < end; dst += 64, src+= 64)
        {
        *(dst     ) = *(src     );
        *(dst +  4) = *(src +  4);
        *(dst +  8) = *(src +  8);
        *(dst + 12) = *(src + 12);

        *(dst + 16) = *(src + 16);
        *(dst + 20) = *(src + 20);
        *(dst + 24) = *(src + 24);
        *(dst + 28) = *(src + 28);

        *(dst + 32) = *(src + 32);
        *(dst + 36) = *(src + 36);
        *(dst + 40) = *(src + 40);
        *(dst + 44) = *(src + 44);

        *(dst + 48) = *(src + 48);
        *(dst + 52) = *(src + 52);
        *(dst + 56) = *(src + 56);
        *(dst + 60) = *(src + 60);
        }

    for( ;dst < end; dst += 4, src+= 4)
        {
        *dst = *src;
        }
    }

// C3C1R
ISV Copy_C3C1R_SSE2_I(RegFile &reg, const A8U &)
    {  	
    __m128i temp1, temp2;

    temp1 = _mm_srli_si128(reg.src1[1].i, 8);
    temp1 = _mm_unpacklo_epi8(reg.src1[0].i, temp1);					
    temp2 = _mm_srli_si128(reg.src1[2].i, 8);
    temp2 = _mm_unpacklo_epi8(reg.src1[1].i, temp2);					
    reg.dst[0].i = _mm_srli_si128(reg.src1[0].i, 8);
    reg.dst[0].i = _mm_unpacklo_epi8(reg.dst[0].i, reg.src1[2].i );

    reg.src1[0].i = _mm_srli_si128(temp1, 8);
    reg.src1[0].i = _mm_unpacklo_epi8(reg.src1[0].i, temp2);					
    reg.src1[1].i = _mm_srli_si128(temp2, 8);
    reg.src1[1].i = _mm_unpacklo_epi8(reg.dst[0].i, reg.src1[1].i);					
    reg.src1[2].i = _mm_srli_si128(reg.dst[0].i, 8);
    reg.src1[2].i = _mm_unpacklo_epi8(temp1, reg.src1[2].i );

    reg.src1[0].i = _mm_srli_si128(reg.src1[0].i, 8);
    reg.src1[0].i = _mm_unpacklo_epi8(reg.src1[2].i, reg.src1[0].i);
    reg.src1[1].i = _mm_srli_si128(reg.src1[1].i, 4);
    reg.src1[2].i = _mm_srli_si128(reg.src1[2].i, 12);
    reg.src1[2].i = _mm_unpacklo_epi8(reg.src1[2].i, reg.src1[1].i);

    reg.dst[0].i  = _mm_unpacklo_epi8(reg.src1[0].i, reg.src1[2].i);	
    }

ISV Copy_C3C1R_SSE2_I(RegFile &reg, const A16S &)
    {
    __m128i temp1, temp2;

    temp1 = _mm_srli_si128(reg.src1[1].i, 8);
    temp1 = _mm_unpacklo_epi16(reg.src1[0].i, temp1);					// r5,r1,x,x,x,xr4,r0
    temp2 = _mm_srli_si128(reg.src1[2].i, 8);
    temp2 = _mm_unpacklo_epi16(reg.src1[1].i, temp2);					// x,x,x,x,r7,r3,x,x
    reg.dst[0].i = _mm_srli_si128(reg.src1[0].i, 8);
    reg.dst[0].i = _mm_unpacklo_epi16(reg.dst[0].i, reg.src1[2].i );	// x,x,r6,r2,x,x,x,x
    reg.src1[0].i = _mm_srli_si128(temp1, 12);
    temp1         = _mm_unpacklo_epi16(temp1, reg.src1[0].i);			// x,x,x,x,r5,r4,r1,r0
    reg.dst[0].i = _mm_srli_si128(reg.dst[0].i, 8);
    temp2         = _mm_srli_si128(temp2, 4);
    reg.dst[0].i  = _mm_unpacklo_epi16(reg.dst[0].i, temp2);
    reg.dst[0].i  = _mm_unpacklo_epi32(temp1, reg.dst[0].i);
    }  

ISV Copy_C3C1R_SSE2_I(RegFile &reg, const A32S &)
    {
    reg.src1[0].i = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(2,1,3,0));
    reg.src1[1].i = _mm_srli_si128(reg.src1[1].i, 8);
    reg.src1[2].i = _mm_srli_si128(reg.src1[2].i, 4);
    reg.src1[1].i = _mm_unpacklo_epi32(reg.src1[1].i, reg.src1[2].i );
    reg.dst[0].i  = _mm_unpacklo_epi64(reg.src1[0].i, reg.src1[1].i);
    }

ISV Copy_C3C1R_SSE2_I(RegFile &reg, const A32F &)
    {
    reg.src1[0].i = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(2,1,3,0));
    reg.src1[1].i = _mm_srli_si128(reg.src1[1].i, 8);
    reg.src1[2].i = _mm_srli_si128(reg.src1[2].i, 4);
    reg.src1[1].i = _mm_unpacklo_epi32(reg.src1[1].i, reg.src1[2].i );
    reg.dst[0].i  = _mm_unpacklo_epi64(reg.src1[0].i, reg.src1[1].i);
    }

// C4C1R
ISV Convert_C4_to_C1_8bit(__m128i &rgba1, __m128i &rgba2, __m128i &rgba3, __m128i &rgba4)
    {
    __m128i temp;

    temp  = _mm_unpacklo_epi8(rgba1, rgba2);
    rgba2 = _mm_unpackhi_epi8(rgba1, rgba2);
    rgba1 = _mm_unpacklo_epi8(temp,  rgba2);
    temp  = _mm_unpackhi_epi8(temp,  rgba2);

    rgba1 = _mm_unpacklo_epi8(rgba1, temp);

    temp  = _mm_unpacklo_epi8(rgba3, rgba4);
    rgba4 = _mm_unpackhi_epi8(rgba3, rgba4);
    rgba3 = _mm_unpacklo_epi8(temp, rgba4);
    temp  = _mm_unpackhi_epi8(temp,  rgba4);

    rgba3 = _mm_unpacklo_epi8(rgba3, temp);

    rgba1 = _mm_unpacklo_epi64(rgba1, rgba3);				
    }

template<IsAlign ia>
ISV Copy_8u_C4C1R_Custom_SSE2_I(const A8U *s, A8U *d, U32 &pixCount)
    {
    Fw8u *end	= d + pixCount * ChannelCount(C1);
    XMM128 rgba1, rgba2, rgba3, rgba4;
    

    for( ;(d+15) < end; d+=16, s+=64)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba1, (const void*)s);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba2, (const void*)(s+16));
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba3, (const void*)(s+32));
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba4, (const void*)(s+48));

        Convert_C4_to_C1_8bit(rgba1.i, rgba2.i, rgba3.i, rgba4.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&rgba1, (void*)d);
        }

    for(; d<end; d++, s+=4)
        {
        *d = *s;
        }
    }

ISV Convert_C4_to_C1_16bit(__m128i &rgba1, __m128i &rgba2, __m128i &rgba3, __m128i &rgba4)
    {
    __m128i temp;

    temp  = _mm_unpacklo_epi16(rgba1, rgba2);
    rgba1 = _mm_unpackhi_epi16(rgba1, rgba2);
    rgba1 = _mm_unpacklo_epi16(temp,  rgba1);		

    temp  = _mm_unpacklo_epi16(rgba3, rgba4);
    rgba3 = _mm_unpackhi_epi16(rgba3, rgba4);
    rgba3 = _mm_unpacklo_epi16(temp, rgba3);

    rgba1 = _mm_unpacklo_epi64(rgba1, rgba3);
    }

template<IsAlign ia>
ISV Copy_16s_C4C1R_Custom_SSE2_I(const A16S *s, A16S *d, U32 &pixCount)
    {
    Fw16s *end	= d + pixCount * ChannelCount(C1);
    XMM128 rgba1, rgba2, rgba3, rgba4;    

    for( ;(d+7) < end; d+=8, s+=32)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba1, (const void*)s);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba2, (const void*)(s+8));
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba3, (const void*)(s+16));
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba4, (const void*)(s+24));

        Convert_C4_to_C1_16bit(rgba1.i, rgba2.i, rgba3.i, rgba4.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&rgba1, (void*)d);
        }

    for(; d<end; d++, s+=4)
        {
        *d = *s;
        }
    }

// C1C3R
ISV Copy_C1C3R_SSE2_I(RegFile &reg, const __m128i &mask1, const __m128i &mask2, const __m128i &mask3, const A8U &)
    {
    __m128i temp;

    // First  register
    temp = _mm_slli_si128(reg.src1[0].i,1);								// shift source by 1 byte to use shuffles
    temp = _mm_unpacklo_epi8(temp,temp);
    temp = _mm_shufflelo_epi16(temp,_MM_SHUFFLE(3,2,2,1));				// get r2,r1 to correct places
    temp = _mm_shufflehi_epi16(temp,_MM_SHUFFLE(2,1,1,0));				// get r6 to correct place
    Copy_With_Mask(temp, reg.dst[0].i, mask1 );

    // Second  register
    temp = _mm_srli_si128(reg.src1[0].i,4);								// shift source by 4 bytes to get 7, 8, 9, 10,11 
    temp = _mm_unpacklo_epi8(temp,temp);
    temp = _mm_shufflelo_epi16(temp,_MM_SHUFFLE(3,3,2,0));				// get r7,r8 to correct places
    temp = _mm_shufflehi_epi16(temp,_MM_SHUFFLE(2,2,1,0));				// get r11 to correct place
    Copy_With_Mask(temp, reg.dst[1].i, mask2 );

    // Third  register
    temp = _mm_srli_si128(reg.src1[0].i,10);							// shift source by 10 bytes to get 12,13,14,15,16
    temp = _mm_unpacklo_epi8(temp,temp);
    temp = _mm_shufflelo_epi16(temp,_MM_SHUFFLE(3,2,2,1));				// get r12,r13 to correct places
    temp = _mm_shufflehi_epi16(temp,_MM_SHUFFLE(1,1,0,0));				// get r15,r16 to correct place
    Copy_With_Mask(temp, reg.dst[2].i, mask3 );
    }

ISV Copy_C1C3R_SSE2_I(RegFile &reg,  const __m128i &mask1, const __m128i &mask2, const __m128i &mask3, const A16S &)
    {
    __m128i temp;

    //copy data to 1st dst register
    temp = _mm_slli_si128(reg.src1[0].i,4);								// shift source by 4 bytes to get r3 to correct place
    temp = _mm_shufflelo_epi16(temp,_MM_SHUFFLE(3,3,2,2));				// get r2,r1 to correct places
    temp = _mm_shufflehi_epi16(temp,_MM_SHUFFLE(0,0,0,0));				// get r3 to correct place
    Copy_With_Mask(temp, reg.dst[0].i, mask1 );

    //copy data to 2nd dst register
    temp = _mm_shufflelo_epi16(reg.src1[0].i,_MM_SHUFFLE(3,3,3,3));		// get r4 to correct place
    temp = _mm_shufflehi_epi16(temp,_MM_SHUFFLE(1,1,0,0));				// get r6 to correct place
    Copy_With_Mask(temp, reg.dst[1].i, mask2 );

    //copy data to 3rd dst register
    temp = _mm_srli_si128(reg.src1[0].i,6);								// shift source by 6 bytes to get r7,r8
    temp = _mm_shufflelo_epi16(temp,_MM_SHUFFLE(3,3,3,3));				// get r7 to correct place
    temp = _mm_shufflehi_epi16(temp,_MM_SHUFFLE(0,0,0,0));				// get r8 to correct place
    Copy_With_Mask(temp, reg.dst[2].i, mask3 );
    }

template<class TSD, IsAlign>
ISV Copy_32_C1C3R_Custom_SSE2_I(const TSD *s, TSD *d, U32 &pixCount)
    {
    TSD *end	= d + pixCount * ChannelCount(C3);
    for( ;d < end; d+= 3, s+= 1)
        {
        *d = *s;
        }
    }

// C1C4R
template<class TSD, IsAlign>
ISV Copy_C1C4R_Custom_SSE2_I(const TSD *s, TSD *d, U32 &pixCount)
    {
    const TSD *src	= s;
    TSD *dst	= d;
    TSD *end	= d + pixCount * ChannelCount(C4);

    for( ;(dst + 63) < end; dst += 64, src+= 16)
        {
        *(dst     ) = *(src     );
        *(dst +  4) = *(src +  1);
        *(dst +  8) = *(src +  2);
        *(dst + 12) = *(src +  3);

        *(dst + 16) = *(src +  4);
        *(dst + 20) = *(src +  5);
        *(dst + 24) = *(src +  6);
        *(dst + 28) = *(src +  7);

        *(dst + 32) = *(src +  8);
        *(dst + 36) = *(src +  9);
        *(dst + 40) = *(src + 10);
        *(dst + 44) = *(src + 11);

        *(dst + 48) = *(src + 12);
        *(dst + 52) = *(src + 13);
        *(dst + 56) = *(src + 14);
        *(dst + 60) = *(src + 15);
        }

    for( ;dst < end; dst += 4, src+= 1)
        {
        *dst = *src;
        }
    }

//AC4C3R
ISV Copy_8u_AC4C3R_Custom_SSE2_I(const A8U *s, A8U *d, U32 &pixCount)
    {
    A8U *end	=  d + pixCount * ChannelCount(C3);
    for(; d<end; d+=3, s+=4)
        {
        *((A16S*)d) = *((A16S*)s);
        d[2]  = s[2];	
        }
    }

ISV Copy_16s_AC4C3R_SSE2_I(RegFile &reg, const __m128i &mask2, const __m128i &notmask2)
    {
    __m128i temp;
    temp = _mm_and_si128(mask2,reg.src1[0].i);							// get r2,g2,b2
    temp = _mm_srli_si128(temp,2);										// get to correct place
    reg.dst[0].i = reg.src1[0].i;										// copy r1,g1,b1 to correct places
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i, notmask2);				// make the dst locations fo r2,g2,b2,alpha1 to zero
    reg.dst[0].i = _mm_or_si128(reg.dst[0].i,temp);						// copy r2,g2,b2
    }

ISV Copy_32S_AC4C3R_SSE2_I(RegFile &reg)
    {
    reg.dst[0].i = reg.src1[0].i;
    }

ISV Copy_32F_AC4C3R_SSE2_I(RegFile &reg)
    {
    reg.dst[0].f = reg.src1[0].f;
    }

// P3C3R
template<IsAlign ia>
ISV Copy_P3C3R_Custom_SSE2_I(const A8U *s1, const A8U *s2, const A8U *s3, A8U *d, U32 &pixCount)
    {
    A8U *end	=  d + pixCount * ChannelCount(C3);
    XMM128 r,g,b;    

    for( ; d < end; d+=48, s1+=16,s2+=16,s3+=16)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&r, (const void*)s1);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&g, (const void*)s2);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&b, (const void*)s3);

        // Replace CBL with SSEPlus
        // CBL_SSE2::Convert_3P_to_3C_8bit(r.i,g.i,b.i);
        ssp_convert_3p_3c_epi8(&r.i, &g.i, &b.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&r, (void*)d);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&g, (void*)(d+16));
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&b, (void*)(d+32));
        }	
    _mm_mfence();
    }

template<IsAlign ia>
ISV Copy_P3C3R_Custom_SSE2_I(const A16S *s1, const A16S *s2, const A16S *s3, A16S *d, U32 &pixCount)
    {
    A16S *end	=  d + pixCount * ChannelCount(C3);
    XMM128 r,g,b;   

    for( ; d < end; d+=24, s1+=8,s2+=8,s3+=8)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&r, (const void*)s1);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&g, (const void*)s2);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&b, (const void*)s3);

        // CBL_SSE2::Convert_3P_to_3C_16bit(r.i,g.i,b.i);
        ssp_convert_3p_3c_epi16(&r.i, &g.i, &b.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&r, (void*)d);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&g, (void*)(d+8));
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&b, (void*)(d+16));
        }	
    _mm_mfence();
    }

template<class TSD, IsAlign ia>
ISV Copy_P3C3R_Custom_SSE2_I(const TSD *s1, const TSD *s2, const TSD *s3, TSD *d, U32 &pixCount)
    {
    TSD *end	=  d + pixCount * ChannelCount(C3);
    XMM128 r,g,b;

    for( ; d < end; d+=12, s1+=4,s2+=4,s3+=4)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&r, (const void*)s1);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&g, (const void*)s2);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&b, (const void*)s3);

        // CBL_SSE2::Convert_3P_to_3C_32bit(r.i,g.i,b.i);
        ssp_convert_3p_3c_epi32(&r.i, &g.i, &b.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&r, (void*)d);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&g, (void*)(d+4));
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&b, (void*)(d+8));
        }
    _mm_mfence();
    }

// P4C4R
template<IsAlign ia>
ISV Copy_P4C4R_Custom_SSE2_I(const A8U *s1, const A8U *s2, const A8U *s3, const A8U *s4, A8U *d, U32 &pixCount)
    {
    A8U *end	=  d + pixCount * ChannelCount(C4);
    XMM128 r,g,b,a;    

    for( ; (d+48) < end; d+=64, s1+=16,s2+=16,s3+=16,s4+=16)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&r, (const void*)s1);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&g, (const void*)s2);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&b, (const void*)s3);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&a, (const void*)s4);

        // CBL_SSE2::Convert_4P_to_4C_8bit(r.i,g.i,b.i,a.i);
        ssp_convert_4p_4c_epi8(&r.i, &g.i, &b.i, &a.i);        

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&r, (void*)d);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&g, (void*)(d+16));
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&b, (void*)(d+32));
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&a, (void*)(d+48));
        }
    _mm_mfence();

    for( ; d < end; d+=16, s1+=4,s2+=4,s3+=4,s4+=4)
        {
        d[0] = *s1;		
        d[1] = *s2;
        d[2] = *s3;
        d[3] = *s4;
        d[4] = *(s1+1);
        d[5] = *(s2+1);
        d[6] = *(s3+1);
        d[7] = *(s4+1);		
        d[8] = *(s1+2);
        d[9] = *(s2+2);
        d[10] = *(s3+2);
        d[11] = *(s4+2);		
        d[12] = *(s1+3);
        d[13] = *(s2+3);
        d[14] = *(s3+3);
        d[15] = *(s4+3);			
        }
    }

template<IsAlign ia>
ISV Copy_P4C4R_Custom_SSE2_I(const A16S *s1, const A16S *s2, const A16S *s3, const A16S *s4, A16S *d, U32 &pixCount)
    {
    A16S *end	= d + pixCount * ChannelCount(C4);
    XMM128 r,g,b,a;   

    for( ; (d+24) < end; d+=32, s1+=8,s2+=8,s3+=8,s4+=8)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&r, (const void*)s1);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&g, (const void*)s2);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&b, (const void*)s3);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&a, (const void*)s4);

        // CBL_SSE2::Convert_4P_to_4C_16bit(r.i,g.i,b.i,a.i);
        ssp_convert_4p_4c_epi16(&r.i, &g.i, &b.i, &a.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&r, (void*)d);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&g, (void*)(d+8));
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&b, (void*)(d+16));
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&a, (void*)(d+24));
        }
    _mm_mfence();

    for( ; d < end; d+=8, s1+=2,s2+=2,s3+=2,s4+=2)
        {
        d[0] = s1[0];		
        d[1] = s2[0];
        d[2] = s3[0];
        d[3] = s4[0];
        d[4] = s1[1];
        d[5] = s2[1];
        d[6] = s3[1];
        d[7] = s4[1];
        }
    }

template <class TSD, IsAlign ia>
ISV Copy_P4C4R_Custom_SSE2_I(const TSD *s1, const TSD *s2, const TSD *s3, const TSD *s4, TSD *d, U32 &pixCount)
    {
    TSD *end	= d + pixCount * ChannelCount(C4);
    XMM128 r,g,b,a;   

    for( ; (d+12) < end; d+=16, s1+=4,s2+=4,s3+=4,s4+=4)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&r, (const void*)s1);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&g, (const void*)s2);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&b, (const void*)s3);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&a, (const void*)s4);

        // CBL_SSE2::Convert_4P_to_4C_32bit(r.i,g.i,b.i,a.i);
        ssp_convert_4p_4c_epi32(&r.i, &g.i, &b.i, &a.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&r, (void*)d);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&g, (void*)(d+4));
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&b, (void*)(d+8));
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&a, (void*)(d+12));
        }
    _mm_mfence();

    for( ; d < end; d+=4, s1++,s2++,s3++,s4++)
        {
        d[0] = *s1;		
        d[1] = *s2;
        d[2] = *s3;
        d[3] = *s4;
        }
    }

// C3P3R
template<IsAlign ia>
ISV Copy_C3P3R_Custom_SSE2_I(const A8U *s, A8U *d1, A8U *d2, A8U *d3, U32 &pixCount)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(C3));
    XMM128 rgb1,rgb2,rgb3;    

    for( ; src < end; src+= 3, d1+=16,d2+=16,d3+=16)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgb1, (const void*)src);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgb2, (const void*)(src+1));
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgb3, (const void*)(src+2));

        ssp_convert_3c_3p_epi8(&rgb1.i,&rgb2.i,&rgb3.i);


        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgb1, (void*)d1);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgb2, (void*)d2);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgb3, (void*)d3);
        }
    _mm_mfence();
    }

template<IsAlign ia>
ISV Copy_C3P3R_Custom_SSE2_I(const A16S *s, A16S *d1, A16S *d2, A16S *d3, U32 &pixCount)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(C3));
    XMM128 rgb1,rgb2,rgb3;   

    for( ; src < end; src+= 3, d1+=8,d2+=8,d3+=8)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgb1, (const void*)src);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgb2, (const void*)(src+1));
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgb3, (const void*)(src+2));

        // Replace CBL with SSEPlus
        // CBL_SSE2::Convert_3C_to_3P_16bit(rgb1.i,rgb2.i,rgb3.i);
        ssp_convert_3c_3p_epi16(&rgb1.i, &rgb2.i, &rgb3.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgb1, (void*)d1);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgb2, (void*)d2);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgb3, (void*)d3);
        }
    _mm_mfence();
    }

template <class TSD, IsAlign ia>
ISV Copy_C3P3R_Custom_SSE2_I(const TSD *s, TSD *d1, TSD *d2, TSD *d3, U32 &pixCount)
    {
    __m128i *src	= (__m128i*)s;
    __m128i *end	= (__m128i *) (s + pixCount * ChannelCount(C3));
    XMM128 rgb1,rgb2,rgb3;   

    for( ; src < end; src+= 3, d1+=4,d2+=4,d3+=4)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgb1, (const void*)src);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgb2, (const void*)(src+1));
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgb3, (const void*)(src+2));

        // Replace CBL with SSEPlus
        // CBL_SSE2::Convert_3C_to_3P_32bit(rgb1.i,rgb2.i,rgb3.i);
        ssp_convert_3c_3p_epi32(&rgb1.i, &rgb2.i, &rgb3.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgb1, (void*)d1);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgb2, (void*)d2);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgb3, (void*)d3);
        }
    _mm_mfence();
    }

// C4P4R
template<IsAlign ia>
ISV Copy_C4P4R_Custom_SSE2_I(const A8U *s, A8U *d1, A8U *d2, A8U *d3, A8U *d4, U32 &pixCount)
    {
    const A8U *end	= s + pixCount * ChannelCount(C4);
    XMM128 rgba1,rgba2,rgba3, rgba4;   

    for( ; (s+48) < end; s+=64, d1+=16,d2+=16,d3+=16,d4+=16)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba1, (const void*)s);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba2, (const void*)(s+16));
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba3, (const void*)(s+32));
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba4, (const void*)(s+48));

        // CBL_SSE2::Convert_4C_to_4P_8bit(rgba1.i,rgba2.i,rgba3.i,rgba4.i);
        ssp_convert_4c_4p_epi8(&rgba1.i, &rgba2.i, &rgba3.i, &rgba4.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba1, (void*)d1);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba2, (void*)d2);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba3, (void*)d3);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba4, (void*)d4);
        }
    _mm_mfence();

    for( ; s < end; s+=16, d1+=4,d2+=4,d3+=4,d4+=4)
        {
        d1[0] = s[0];		
        d2[0] = s[1];
        d3[0] = s[2];
        d4[0] = s[3];
        d1[1] = s[4];
        d2[1] = s[5];
        d3[1] = s[6];
        d4[1] = s[7];	
        d1[2] = s[8];		
        d2[2] = s[9];
        d3[2] = s[10];
        d4[2] = s[11];
        d1[3] = s[12];
        d2[3] = s[13];
        d3[3] = s[14];
        d4[3] = s[15];
        }
    }

template<IsAlign ia>
ISV Copy_C4P4R_Custom_SSE2_I(const A16S *s, A16S *d1, A16S *d2, A16S *d3, A16S *d4, U32 &pixCount)
    {
    const A16S *end	= s + pixCount * ChannelCount(C4);
    XMM128 rgba1,rgba2,rgba3, rgba4;   

    for( ; (s+24) < end; s+=32, d1+=8,d2+=8,d3+=8,d4+=8)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba1, (const void*)s);
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba2, (const void*)(s+8));
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba3, (const void*)(s+16));
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba4, (const void*)(s+24));

        // CBL_SSE2::Convert_4C_to_4P_16bit(rgba1.i,rgba2.i,rgba3.i,rgba4.i);
        ssp_convert_4c_4p_epi16(&rgba1.i, &rgba2.i, &rgba3.i, &rgba4.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba1, (void*)d1);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba2, (void*)d2);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba3, (void*)d3);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba4, (void*)d4);
        }
    _mm_mfence();

    for( ; s < end; s+=8, d1+=2,d2+=2,d3+=2,d4+=2)
        {
        d1[0] = s[0];		
        d2[0] = s[1];
        d3[0] = s[2];
        d4[0] = s[3];
        d1[1] = s[4];
        d2[1] = s[5];
        d3[1] = s[6];
        d4[1] = s[7];
        }
    }

template< class TSD, IsAlign ia>
ISV Copy_C4P4R_Custom_SSE2_I(const TSD *s, TSD *d1, TSD *d2, TSD *d3, TSD *d4, U32 &pixCount)
    {
    const TSD *end	= s + pixCount * ChannelCount(C4);
    XMM128 rgba1,rgba2,rgba3, rgba4;    

    for( ; (s+12) < end; s+=16, d1+=4,d2+=4,d3+=4,d4+=4)
        {
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba1, (const void*)s);     
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba2, (const void*)(s+4)); 
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba3, (const void*)(s+8)); 
        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&rgba4, (const void*)(s+12));

        // CBL_SSE2::Convert_4C_to_4P_32bit(rgba1.i, rgba2.i, rgba3.i, rgba4.i);
        ssp_convert_4c_4p_epi32(&rgba1.i, &rgba2.i, &rgba3.i, &rgba4.i);

        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba1, (void*)d1);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba2, (void*)d2);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba3, (void*)d3);
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&rgba4, (void*)d4);
        }
    _mm_mfence();
    for( ; s < end; s+=4, d1++,d2++,d3++,d4++)
        {
        d1[0] = s[0];		
        d2[0] = s[1];
        d3[0] = s[2];
        d4[0] = s[3];
        }
    }

}; // OPT_LEVEL

#endif


