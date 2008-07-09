/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SET_H__
#define __SET_H__

#include "fe.h"
#include "Copy.h" 

namespace OPT_LEVEL
{
/////////////////////////////////////// REF FUNCTIONS ////////////////////////////////////////////

template<class TSD>
ISV Set1(TSD *d, const TSD &value)
{
	*d = value;
}

template<class TSD, CH CSD>
ISV Set(TSD *d, const TSD *value)
{
	for(int i=0;i<CSD;i++)
		d[i] = value[i];
}

template<class TSD>
ISV Set1M(const A8U *m, TSD *d, const TSD &value)
{
	if( *m )
		*d = value;
}

template<class TSD, CH CSD>
ISV SetM(const A8U *m, TSD *d, const TSD *value)
{
	if( *m )
         switch (CSD)
            {
            case C4 :
                d[3] = value[3];;
            case AC4:
            case C3	:
                d[2] = value[2];;
                d[1] = value[1];;
            case C1	:
                d[0] = value[0];;
            }
}

/////////////////////////////////////// SSE2 SETUP FUNCTIONS //////////////////////////////////////////

// C1R
ISV Set_C1R_SETUP_I(__m128i &value, const A8U &val)
{
	value = _mm_set1_epi8(val);		
}

ISV Set_C1R_SETUP_I(__m128i &value, const A16S &val)
{
	value = _mm_set1_epi16(val);
}
ISV Set_C1R_SETUP_I(__m128i &value, const A32S &val)
{
	value = _mm_set1_epi32(val);
}
ISV Set_C1R_SETUP_I(__m128 &value, const A32F &val)
{
	value = _mm_set1_ps(val);
}

// C3R
ISV Set_C3R_SETUP_I(XMM128 &value1, XMM128 &value2, XMM128 &value3, const A8U *value)
{
value1.i = _mm_set_epi8(value[0],value[2],value[1],value[0],value[2],value[1],value[0],value[2],value[1],value[0],value[2],value[1],value[0],value[2],value[1],value[0]);
value2.i = _mm_set_epi8(value[1],value[0],value[2],value[1],value[0],value[2],value[1],value[0],value[2],value[1],value[0],value[2],value[1],value[0],value[2],value[1]);
value3.i = _mm_set_epi8(value[2],value[1],value[0],value[2],value[1],value[0],value[2],value[1],value[0],value[2],value[1],value[0],value[2],value[1],value[0],value[2]);
}
ISV Set_C3R_SETUP_I(XMM128 &value1, XMM128 &value2, XMM128 &value3, const A16S *value)
{
value1.i = _mm_set_epi16(value[1],value[0],value[2],value[1],value[0],value[2],value[1],value[0]);
value2.i = _mm_set_epi16(value[0],value[2],value[1],value[0],value[2],value[1],value[0],value[2]);
value3.i = _mm_set_epi16(value[2],value[1],value[0],value[2],value[1],value[0],value[2],value[1]);
}
ISV Set_C3R_SETUP_I(XMM128 &value1, XMM128 &value2, XMM128 &value3, const A32S *value)
{
value1.i = _mm_set_epi32(value[0],value[2],value[1],value[0]);
value2.i = _mm_set_epi32(value[1],value[0],value[2],value[1]);
value3.i = _mm_set_epi32(value[2],value[1],value[0],value[2]);
}
ISV Set_C3R_SETUP_I(XMM128 &value1, XMM128 &value2, XMM128 &value3, const A32F *value)
{
value1.f = _mm_set_ps(value[0],value[2],value[1],value[0]);
value2.f = _mm_set_ps(value[1],value[0],value[2],value[1]);
value3.f = _mm_set_ps(value[2],value[1],value[0],value[2]);
}

// C4R
ISV Set_C4R_SETUP_I(__m128i &value, const A8U *val)
{
	value = _mm_set1_epi32((val[3] << 24) | (val[2] << 16) | (val[1] << 8) | val[0]);
}
ISV Set_C4R_SETUP_I(__m128i &value, const A16S *val)
{
	value = _mm_set_epi16(val[3],val[2],val[1],val[0],val[3],val[2],val[1],val[0]);
}
ISV Set_C4R_SETUP_I(__m128i &value, const A32S *val)
{
	value = _mm_set_epi32(val[3],val[2],val[1],val[0]);
}
ISV Set_C4R_SETUP_I(__m128 &value, const A32F *val)
{
	value = _mm_set_ps(val[3],val[2],val[1],val[0]);
}

// AC4R
ISV Set_AC4R_SETUP_I(__m128i &mask, XMM128 &value, const A8U *val)
{
	mask = _mm_setzero_si128();									// set to zeros
	mask = _mm_cmpeq_epi32(mask,mask);							// set to ones
	mask = _mm_slli_epi32(mask, 24);							// mask to get alpha

	value.i = _mm_set_epi8(0,val[2],val[1],val[0],0,val[2],val[1],val[0],0,val[2],val[1],val[0],0,val[2],val[1],val[0]);
}
ISV Set_AC4R_SETUP_I(__m128i &mask, XMM128 &value, const A16S *val)
{
	mask = _mm_setzero_si128();									// set to zeros
	mask = _mm_cmpeq_epi32(mask ,mask );						// set to ones
	mask = _mm_slli_epi64( mask, 48);							// mask to get alphas
	
	value.i = _mm_set_epi16(0,val[2],val[1],val[0],0,val[2],val[1],val[0]);
}
ISV Set_AC4R_SETUP_I(__m128i &mask, XMM128 &value, const A32S *val)
{
	mask = _mm_setzero_si128();									// set to zeros
	mask = _mm_cmpeq_epi32(mask,mask);							// set to ones
	mask = _mm_slli_si128( mask, 12);							// mask to get alpha

	value.i= _mm_set_epi32(0,val[2],val[1],val[0]);
}
ISV Set_AC4R_SETUP_I(__m128i &mask, XMM128 &value, const A32F *val)
{
	mask = _mm_setzero_si128();									// set to zeros
	mask = _mm_cmpeq_epi32(mask,mask);							// set to ones
	mask = _mm_slli_si128( mask, 12);							// mask to get alpha

	value.f= _mm_set_ps(0,val[2],val[1],val[0]);
}

// C1MR
ISV Set_C1MR_SETUP_I(__m128i &value, const A8U &v)
{
	value = _mm_set1_epi8(v);	
}
ISV Set_C1MR_SETUP_I(__m128i &value, const A16S &v)
{
	value = _mm_set1_epi16(v);	
}
ISV Set_C1MR_SETUP_I(__m128i &value, __m128i& zero, const A32S &v)
{
	value = _mm_set1_epi32(v);	
	zero  = _mm_setzero_si128();										// set to zeros
}
ISV Set_C1MR_SETUP_I(__m128 &value, __m128i &zero, const A32F &v)
{
	value = _mm_set1_ps(v);	
	zero  = _mm_setzero_si128();										// set to zeros
}

// C3MR
ISV Set_C3MR_SETUP_I(__m128i &value1, __m128i &value2, __m128i &value3, __m128i &zero, const A8U *val)// A8U
{
	value1 = _mm_set_epi8(val[0],val[2],val[1],val[0],val[2],val[1],val[0],val[2],val[1],val[0],val[2],val[1],val[0],val[2],val[1],val[0]);
	value2 = _mm_set_epi8(val[1],val[0],val[2],val[1],val[0],val[2],val[1],val[0],val[2],val[1],val[0],val[2],val[1],val[0],val[2],val[1]);
	value3 = _mm_set_epi8(val[2],val[1],val[0],val[2],val[1],val[0],val[2],val[1],val[0],val[2],val[1],val[0],val[2],val[1],val[0],val[2]);
	zero   = _mm_setzero_si128();										//set to zeros
}
ISV Set_C3MR_SETUP_I(__m128i &value1, __m128i &value2, __m128i &value3, __m128i &zero, const A16S *val)// A16S
{
	value1 = _mm_set_epi16(val[1],val[0],val[2],val[1],val[0],val[2],val[1],val[0]);
	value2 = _mm_set_epi16(val[0],val[2],val[1],val[0],val[2],val[1],val[0],val[2]);
	value3 = _mm_set_epi16(val[2],val[1],val[0],val[2],val[1],val[0],val[2],val[1]);
	zero   = _mm_setzero_si128();										// set to zeros									
}
ISV Set_C3MR_SETUP_I(__m128i &value, const A32S *val)
{
	value = _mm_set_epi32(val[0],val[2],val[1],val[0]);
}
ISV Set_C3MR_SETUP_I(__m128 &value, const A32F *val)
{
	value = _mm_set_ps(val[0],val[2],val[1],val[0]);
}

// C4MR
ISV Set_C4MR_SETUP_I(__m128i &value, __m128i &zero, const A8U *val)
{
	value = _mm_set_epi8(val[3],val[2],val[1],val[0],val[3],val[2],val[1],val[0],val[3],val[2],val[1],val[0],val[3],val[2],val[1],val[0]);
	zero  = _mm_setzero_si128();										// set to zeros		
}
ISV Set_C4MR_SETUP_I(__m128i &value1, __m128i &value2, __m128i &value3, const A16S *value)
{
	value1 = _mm_set_epi16(value[3],value[2],value[1],value[0],value[3],value[2],value[1],value[0]);
	value2 = _mm_set_epi16(value[3],value[2],value[1],value[0],       0,       0,       0,       0);
	value3 = _mm_set_epi16(       0,       0,       0,       0,value[3],value[2],value[1],value[0]);
}
ISV Set_C4MR_SETUP_I(__m128i &value, const A32S *val)
{
	value = _mm_set_epi32(val[3],val[2],val[1],val[0]);
}
ISV Set_C4MR_SETUP_I(__m128 &value, const A32F *val)
{
	value = _mm_set_ps(val[3],val[2],val[1],val[0]);
}

// AC4MR
ISV Set_AC4MR_SETUP_I(__m128i &value, __m128i &mask, __m128i &zero, const A8U *val)
{
	value = _mm_set_epi8(0,val[2],val[1],val[0],0,val[2],val[1],val[0],0,val[2],val[1],val[0],0,val[2],val[1],val[0]);
	zero  = _mm_setzero_si128();										// set to zeros	
	mask  = _mm_cmpeq_epi32(zero, zero);								// set to ones
	mask  = _mm_slli_epi32(mask, 24);	
}
ISV Set_AC4MR_SETUP_I(__m128i &value1, __m128i &value2, __m128i &value3, const A16S *val)
{
	value1 = _mm_set_epi16(0,val[2],val[1],val[0],0,val[2],val[1],val[0]);
	value2 = _mm_set_epi16(0,val[2],val[1],val[0],0,       0,       0,       0);
	value3 = _mm_set_epi16(0,       0,       0,       0,0,val[2],val[1],val[0]);
}
ISV Set_AC4MR_SETUP_I(__m128i &value, const A32S *val)
{
	value = _mm_set_epi32(0,val[2],val[1],val[0]);
}
ISV Set_AC4MR_SETUP_I(__m128 &value, const A32F *val)
{
	value = _mm_set_ps(0,val[2],val[1],val[0]);
}

/////////////////////////////////////// SSE2 FUNCTIONS //////////////////////////////////////////////////////

// C1R C4R
template <class TSD, CH CSD, IsAlign ia>
ISV Set_C1R_C4R_Custom_SSE2_I(TSD *d, U32 &pixCount, XMM128 &value)
{	
	__m128i *dst	= (__m128i*)d;
	__m128i *end	= (__m128i *) (d + pixCount * ChannelCount(CSD));

	for( ; (dst + 15) < end; dst += 16)
	{
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)dst);
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+1));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+2));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+3));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+4));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+5));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+6));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+7));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+8));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+9));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+10));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+11));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+12));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+13));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+14));
    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst+15));
	}

	for( ;dst < end; )		
	{
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(dst++));
	}
}

// C3R
template<class TSD, IsAlign ia>
ISV Set_C3R_Custom_SSE2_I(TSD *d, U32 &pixCount, XMM128 &value1, XMM128 &value2, XMM128 &value3)
{	
	__m128i *dst	= (__m128i*)d;
	__m128i *end	= (__m128i*)(d + pixCount * ChannelCount(C3));

	for( ; (dst + 14) < end; dst += 15)
	{
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value1, (void*)(dst+0));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value2, (void*)(dst+1));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value3, (void*)(dst+2));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value1, (void*)(dst+3));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value2, (void*)(dst+4));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value3, (void*)(dst+5));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value1, (void*)(dst+6));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value2, (void*)(dst+7));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value3, (void*)(dst+8));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value1, (void*)(dst+9));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value2, (void*)(dst+10));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value3, (void*)(dst+11));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value1, (void*)(dst+12));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value2, (void*)(dst+13));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value3, (void*)(dst+14));		
	}				  

	for( ;dst < end;)		
	{
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value1, (void*)(dst++));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value2, (void*)(dst++));
		LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value3, (void*)(dst++));
	}
    _mm_mfence();
}

//AC4R
ISV Set_AC4R_SSE2_I(RegFile &reg, const __m128i &mask, const __m128i &value)
{
	reg.dst[0].i = _mm_and_si128(mask, reg.dst[0].i);					// make dst to zero retaining only alphas
	reg.dst[0].i = _mm_or_si128(reg.dst[0].i, value);					// copy
}

//C1MR
ISV Set_C1MR_SSE2_I(RegFile &reg, const __m128i &value)
{
	__m128i mask;
	mask = _mm_cmpeq_epi8(reg.src1[0].i, _mm_setzero_si128());			// just opposite mask
	Copy_With_Mask(value, reg.dst[0].i, mask);
}

template<IsAlign ia>
ISV Set_16s_C1MR_Custom_SSE2_I(const A8U *s, A16S *d, U32 &pixCount, XMM128 &value)
{
	__m128i  *dst     = (__m128i*)d;
	__m128i  *end	  = (__m128i *) (d + pixCount * ChannelCount(C1));
	__m128i  mask_temp; 
    XMM128 dst_data;    
	int mask;

    for( ; dst < end; s+=8, dst++)
	{
		mask_temp = _mm_loadl_epi64((__m128i const*)s);
		mask_temp = _mm_cmpeq_epi8(mask_temp, _mm_setzero_si128());		// just opposite mask

		mask = _mm_movemask_epi8(mask_temp);
		
		switch(mask)
			{
			case 0x0000ff00:
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)dst);
				continue;

			default:
				mask_temp = _mm_unpacklo_epi8(mask_temp, mask_temp);
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)dst);
				Copy_With_Mask(value.i, dst_data.i, mask_temp);
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)dst);
			}
	}
    _mm_mfence();
}

template<class TSD, IsAlign ia>
ISV Set_32_C1MR_Custom_SSE2_I(const A8U *s, TSD *d, U32 &pixCount, XMM128 &value, const __m128i &zero)
{
	__m128i  *dst     = (__m128i*)d;
    __m128i  *end	  = (__m128i *) (d + pixCount * ChannelCount(C1));
    XMM128 mask_temp,dst_data;//, src_data;    
    int mask;

    for( ; dst < end; s+=4, dst++)
        {
        LoadStoreModules::LOAD<4, DT_SSE2, ia, STREAM_FLSE>(&mask_temp, (const void*)s);
        mask_temp.i = _mm_cmpeq_epi8(mask_temp.i, zero);	// just opposite mask

        mask = _mm_movemask_epi8(mask_temp.i);

        switch(mask)
            {
            case 0x0000fff0:
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)dst);
                continue;

            default:
                mask_temp.i = _mm_unpacklo_epi8(mask_temp.i, mask_temp.i);
                mask_temp.i = _mm_unpacklo_epi16(mask_temp.i, mask_temp.i);
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)dst);
                Copy_With_Mask(value.i, dst_data.i, mask_temp.i);
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)dst);
            }
        }
    _mm_mfence();
}

//C3MR
ISV Set_C3MR_SSE2_I(RegFile &reg, const __m128i &value1, const __m128i &value2, const __m128i &value3, const __m128i &zero, const A8U &)
{	
	__m128i mask_temp0,mask_temp1,mask_temp2;
	mask_temp1 = mask_temp2 = mask_temp0 = _mm_cmpeq_epi8(reg.src1[0].i,zero);// just opposite mask
	
	ssp_convert_3p_3c_epi8(&mask_temp0, &mask_temp1, &mask_temp2);

	Copy_With_Mask(value1, reg.dst[0].i, mask_temp0);
	Copy_With_Mask(value2, reg.dst[1].i, mask_temp1);
	Copy_With_Mask(value3, reg.dst[2].i, mask_temp2);
}

ISV Set_C3MR_SSE2_I(RegFile &reg, const __m128i &value1, const __m128i &value2, const __m128i &value3, const __m128i &zero, const A16S &)
{	
	__m128i mask,mask_temp,temp;
	
	mask = _mm_cmpeq_epi8(reg.src1[0].i,zero);							// just opposite mask
	mask = _mm_unpacklo_epi8(mask,mask);								// unpack to make it 16 bit
	
	mask_temp = _mm_unpacklo_epi16(mask, mask);
	mask_temp = _mm_unpacklo_epi16(mask_temp, mask);
	mask_temp = _mm_shufflehi_epi16(mask_temp, _MM_SHUFFLE(1,1,2,0));	
	Copy_With_Mask(value1, reg.dst[0].i, mask_temp);

	temp = _mm_srli_si128(mask, 6);
	temp = _mm_unpacklo_epi16(temp, temp);
	mask_temp = _mm_srli_si128(mask, 4);
	mask_temp = _mm_unpacklo_epi16(mask_temp, temp);
	mask_temp = _mm_shufflehi_epi16(mask_temp, _MM_SHUFFLE(2,3,1,0));
	Copy_With_Mask(value2, reg.dst[1].i, mask_temp);

	temp = _mm_srli_si128(mask, 10);
	mask_temp = _mm_unpacklo_epi16(temp, temp);
	mask_temp = _mm_unpacklo_epi16(mask_temp, temp);
	mask_temp = _mm_shufflehi_epi16(mask_temp, _MM_SHUFFLE(1,1,1,0));	
	mask_temp = _mm_shufflelo_epi16(mask_temp, _MM_SHUFFLE(3,3,1,0));	
	Copy_With_Mask(value3, reg.dst[2].i, mask_temp);
}

template<IsAlign ia>
ISV Set_16s_C3MR_SSE2_I(const A8U *m, A16S *d, U32 &pixCount, XMM128 &value1, XMM128 &value2, XMM128 &value3, XMM128 &zero )
    {
    A16S  *end	  = d + pixCount * ChannelCount(C3);
    XMM128 dst_data, mask_data;    
    __m128i mask;
    int mInt;

    for( ; d < end; m+=8, d+=24)
        {
        LoadStoreModules::LOAD<8, DT_SSE2, ia, STREAM_TRUE>(&mask_data, (const void*)m);
        mask = _mm_cmpeq_epi8(mask_data.i, zero.i);	// just opposite mask
        
        mInt = _mm_movemask_epi8(mask);

            if( (mInt&(7)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)d);
                
                if ((mInt&1) == 0)  dst_data.u32[0] = value1.u32[0] , dst_data.s16[2] = value1.s16[2];
                if ((mInt&2) == 0) dst_data.s16[3] = value1.s16[3], dst_data.u32[2] = value1.u32[2];
                if ((mInt&4) == 0) dst_data.u32[3] = value1.u32[3];

                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)d);
            }

            else
            {
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value1, (void*)d);
            }

            if( (mInt&(60)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)(d+8));
                if ((mInt&4) == 0)  dst_data.s16[0] = value2.s16[0];
                if ((mInt&8) == 0) dst_data.s16[1] = value2.s16[1], dst_data.u32[1] = value2.u32[1];
                if ((mInt&16) == 0) dst_data.u32[2] = value2.u32[2], dst_data.s16[6] = value2.s16[6];
                if ((mInt&32) == 0) dst_data.s16[7] = value2.s16[7];
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)(d+8));
            }

            else
            {
               LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value2, (void*)(d+8));
            }
            if( (mInt&(224)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)(d+16));
                if ((mInt&32) == 0) dst_data.u32[0] = value3.u32[0] ;
                if ((mInt&64) == 0) dst_data.u32[1] = value3.u32[1], dst_data.s16[4] = value3.s16[4] ;
                if ((mInt&128) == 0) dst_data.s16[5] = value3.s16[5], dst_data.u32[3] = value3.u32[3] ;
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)(d+16));
            }

            else
            {
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value3, (void*)(d+16));
            }          
        }

    _mm_mfence();
    }

template<IsAlign ia>
ISV Set_16s_C3MR_eq_SSE2_I(const A8U *m, A16S *d, U32 &pixCount, XMM128 &value, XMM128 &zero )
    {
    A16S  *end	  = d + pixCount * ChannelCount(C3);
    XMM128 dst_data, mask_data;    
    __m128i mask;
    int mInt;

    for( ; d < end; m+=8, d+=24)
        {
        LoadStoreModules::LOAD<8, DT_SSE2, ia, STREAM_TRUE>(&mask_data, (const void*)m);
        mask = _mm_cmpeq_epi8(mask_data.i, zero.i);	// just opposite mask
        
        mInt = _mm_movemask_epi8(mask);

            if( (mInt&(7)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)d);
                
                if ((mInt&1) == 0)  dst_data.u32[0] = value.u32[0] , dst_data.s16[2] = value.s16[2];
                if ((mInt&2) == 0) dst_data.s16[3] = value.s16[2], dst_data.u32[2] = value.u32[0];
                if ((mInt&4) == 0) dst_data.u32[3] = value.u32[0];

                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)d);
            }

            else
            {
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)d);
            }

            if( (mInt&(60)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)(d+8));
                if ((mInt&4) == 0)  dst_data.s16[0] = value.s16[2];
                if ((mInt&8) == 0) dst_data.s16[1] = value.s16[2], dst_data.u32[1] = value.u32[0];
                if ((mInt&16) == 0) dst_data.u32[2] = value.u32[0], dst_data.s16[6] = value.s16[2];
                if ((mInt&32) == 0) dst_data.s16[7] = value.s16[2];
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)(d+8));
            }

            else
            {
               LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(d+8));
            }
            if( (mInt&(224)))
            {
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)(d+16));
                if ((mInt&32) == 0) dst_data.u32[0] = value.u32[0] ;
                if ((mInt&64) == 0) dst_data.u32[1] = value.u32[0], dst_data.s16[4] = value.s16[2] ;
                if ((mInt&128) == 0) dst_data.s16[5] = value.s16[2], dst_data.u32[3] = value.u32[0] ;
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)(d+16));
            }

            else
            {
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)(d+16));
            }          
        }
    _mm_mfence();
    }

template<class TSD, IsAlign ia>
ISV Set_32_C3MR_SSE2_I(const A8U *s, TSD *d, U32 &pixCount, XMM128 &value)
{
	int *src1     = (int*)s;
	__m128i* dst  = (__m128i *)d;
	__m128i* end  = (__m128i *) (d + pixCount * ChannelCount(C3));
	XMM128 mask, temp1, temp2, mask_temp1, mask_temp2, mask_temp3;
	XMM128 dst_data1,dst_data2,dst_data3;
	int mask1;
	
	for( ; dst < end; src1++, dst += 3)
	{
	if(*src1)
		{
        LoadStoreModules::LOAD<4, DT_SSE2, ia, STREAM_FLSE>(&mask, (const void*)src1);
		mask.i = _mm_cmpeq_epi8(mask.i, _mm_setzero_si128());				// just opposite mask
		mask.i = _mm_unpacklo_epi8(mask.i, mask.i);							// unpack to make it 16 bit
		mask.i = _mm_unpacklo_epi16(mask.i, mask.i);						// agin unpack it to 16 bytes

		mask1 = _mm_movemask_epi8(mask.i);
		switch(mask1)
			{
			case 0:
                    LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&value, (void*)dst);
					temp1.i = _mm_shuffle_epi32(value.i, _MM_SHUFFLE(1,3,2,1));
					LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&temp1, (void*)(dst+1));
					temp2.i = _mm_shuffle_epi32(value.i, _MM_SHUFFLE(2,1,0,2));
					LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&temp2, (void*)(dst+2));
					break;

			default:
					mask_temp1.i   =  _mm_shuffle_epi32(mask.i, _MM_SHUFFLE(1,0,0,0));
					mask_temp2.i   =  _mm_shuffle_epi32(mask.i, _MM_SHUFFLE(2,2,1,1));
					mask_temp3.i   =  _mm_shuffle_epi32(mask.i, _MM_SHUFFLE(3,3,3,2));

                    LoadStoreModules::LOAD<16,DT_SSE2, ia, STREAM_FLSE>(&dst_data1, (const void*)dst);
					LoadStoreModules::LOAD<16,DT_SSE2, ia, STREAM_FLSE>(&dst_data2, (const void*)(dst+1));
					LoadStoreModules::LOAD<16,DT_SSE2, ia, STREAM_FLSE>(&dst_data3, (const void*)(dst+2));

					dst_data1.i    =  _mm_and_si128(dst_data1.i, mask_temp1.i);
					dst_data2.i    =  _mm_and_si128(dst_data2.i, mask_temp2.i);
					dst_data3.i    =  _mm_and_si128(dst_data3.i, mask_temp3.i);

					mask_temp1.i   =  _mm_andnot_si128(mask_temp1.i, value.i);
					mask_temp2.i   =  _mm_andnot_si128(mask_temp2.i, _mm_shuffle_epi32(value.i, _MM_SHUFFLE(1,3,2,1)));
					mask_temp3.i   =  _mm_andnot_si128(mask_temp3.i, _mm_shuffle_epi32(value.i, _MM_SHUFFLE(2,1,0,2)));

					dst_data1.i =  _mm_or_si128(dst_data1.i, mask_temp1.i);
					dst_data2.i =  _mm_or_si128(dst_data2.i, mask_temp2.i);
					dst_data3.i =  _mm_or_si128(dst_data3.i, mask_temp3.i);

					LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst_data1, (void*)dst);
					LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst_data2, (void*)(dst+1));
					LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_TRUE>(&dst_data3, (void*)(dst+2));
			}
		}
	}
    _mm_mfence();
}

// C4MR
ISV Set_C4MR_SSE2_I(RegFile &reg, const __m128i &value, const __m128i &zero, const A8U &)
{
	__m128i mask;
	mask = _mm_cmpeq_epi8(reg.src1[0].i,zero);							// just opposite mask
	mask = _mm_unpacklo_epi8(mask, mask);								// Distribute the mask to 8 bytes
	mask = _mm_unpacklo_epi16(mask, mask);								// Distribute the mask to 16 bytes
	Copy_With_Mask(value, reg.dst[0].i, mask);
}

template<IsAlign ia>
ISV Set_16s_C4MR_Custom_SSE2_I(const A8U *s, A16S *d, U32 &pixCount, XMM128 &value1, const __m128i &value2, const __m128i &value3)
{
	__m128i  *dst     = (__m128i*)d;
	__m128i  *end	  = (__m128i *) (d + pixCount * ChannelCount(C4));
	XMM128   dst_data;    
	int       index;

	for( ; dst < end; s+=2, dst++)
		{
			index = (((!(*(s+1))) << 1) | (!(*s)));
			switch(index)
				{
				case 0: 
                        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&value1, (void*)dst);
					    break;

				case 1: 
                        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)dst);
						dst_data.i = _mm_move_epi64(dst_data.i);
						dst_data.i = _mm_or_si128(dst_data.i, value2);
						LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)dst);
						break;

				case 2: 
                        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)dst);
						dst_data.i = _mm_srli_si128(dst_data.i, 8);
						dst_data.i = _mm_slli_si128(dst_data.i, 8);
						dst_data.i = _mm_or_si128(dst_data.i, value3);
						LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)dst);
				}
		}
}

template<class TSD, IsAlign ia>
ISV Set_32_C4MR_Custom_SSE2_I(const A8U *s, TSD *d, U32 &pixCount, XMM128 &value)
{
	__m128i  *dst     = (__m128i*)d;
	__m128i  *end	  = (__m128i *) (d + pixCount * ChannelCount(C4));

	for( ; dst < end; s++, dst++)
		{
			if(*s)
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&value, (void*) dst);
		}
}

// AC4MR
ISV Set_AC4MR_SSE2_I(RegFile &reg, const __m128i &value, const __m128i &AC4_mask, const __m128i &zero, const A8U &)
{
	__m128i mask;
	mask = _mm_cmpeq_epi8(reg.src1[0].i,zero);							// just opposite mask //The mask is only four bytes
	mask = _mm_unpacklo_epi8(mask, mask);								// Distribute the mask to 8 bytes
	mask = _mm_unpacklo_epi16(mask, mask);								// Distribute the mask to 16 bytes
	mask = _mm_or_si128(mask, AC4_mask);								// Make the alpha locations to one(So as not to copy to these locations)
	Copy_With_Mask(value, reg.dst[0].i, mask);
}

template<IsAlign ia>
ISV Set_16s_AC4MR_Custom_SSE2_I(const A8U *s, A16S *d, U32 &pixCount, const __m128i &value1, const __m128i &value2, const __m128i &value3)
{
	__m128i  *dst     = (__m128i*)d;
	__m128i  *end	  = (__m128i *) (d + pixCount * ChannelCount(AC4));
	XMM128   dst_data;
	int index;

	for( ; dst < end; s+=2, dst++)
		{
			index = (((!(*(s+1))) << 1) | (!(*s)));
			switch(index)
				{
				case 0: 
                        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)dst);
						dst_data.i = _mm_srli_epi64(dst_data.i, 48);
						dst_data.i = _mm_slli_epi64(dst_data.i, 48);
						dst_data.i = _mm_or_si128(dst_data.i, value1);
                        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)dst);
					    break;

				case 1: 
                        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)dst);
						dst_data.i = _mm_shufflehi_epi16(dst_data.i, _MM_SHUFFLE(2,1,0,3));
						dst_data.i = _mm_slli_si128(dst_data.i, 6);
						dst_data.i = _mm_srli_si128(dst_data.i, 6);
						dst_data.i = _mm_shufflehi_epi16(dst_data.i, _MM_SHUFFLE(0,3,2,1));
						dst_data.i = _mm_or_si128(dst_data.i, value2);
						LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)dst);
						break;

				case 2: 
                        LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)dst);
						dst_data.i = _mm_srli_si128(dst_data.i, 6);
						dst_data.i = _mm_slli_si128(dst_data.i, 6);
						dst_data.i = _mm_or_si128(dst_data.i, value3);
						LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)dst);
						break;
				}
		}
}

template<class TSD, IsAlign ia>
ISV Set_32_AC4MR_Custom_SSE2_I(const A8U *s, TSD *d, U32 &pixCount, const __m128i &value)
{
	__m128i  *dst     = (__m128i*)d;
	__m128i  *end	  = (__m128i *) (d + pixCount * ChannelCount(AC4));
	XMM128  dst_data;   

	for( ; dst < end; s++, dst++)
		{
			if(*s)
				{
                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (const void*)dst);
				dst_data.i = _mm_srli_si128(dst_data.i, 12);
				dst_data.i = _mm_slli_si128(dst_data.i, 12);
				dst_data.i = _mm_or_si128(dst_data.i, value);
                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&dst_data, (void*)dst);
				}
		}
}

//C3CR
template<class TSD, IsAlign>
ISV Set_C3CR_Custom_SSE2_I(TSD *d, U32 &pixCount, const TSD &val)
{	
	TSD *dst	=(TSD*) d;
	TSD *end	=(TSD*)(d + pixCount * ChannelCount(C3));

	for( ;(dst + 47) < end; dst += 48)
	{
		*(dst     ) = val;
		*(dst +  3) = val;
		*(dst +  6) = val;
		*(dst +  9) = val;

		*(dst + 12) = val;
		*(dst + 15) = val;
		*(dst + 18) = val;
		*(dst + 21) = val;

		*(dst + 24) = val;
		*(dst + 27) = val;
		*(dst + 30) = val;
		*(dst + 33) = val;

		*(dst + 36) = val;
		*(dst + 39) = val;
		*(dst + 42) = val;
		*(dst + 45) = val;
	}

	for( ;dst < end; dst += 3)
	{
		*dst = val;
	}
}	

//C4CR
template<class TSD, IsAlign>
ISV Set_C4CR_Custom_SSE2_I(TSD *d, U32 &pixCount, const TSD &val)
{	
	TSD *dst	=(TSD*) d;
	TSD *end	=(TSD*)(d + pixCount * ChannelCount(C4));

	for( ;(dst + 63) < end; dst += 64)
	{
		*(dst     ) = val;
		*(dst +  4) = val;
		*(dst +  8) = val;
		*(dst + 12) = val;

		*(dst + 16) = val;
		*(dst + 20) = val;
		*(dst + 24) = val;
		*(dst + 28) = val;

		*(dst + 32) = val;
		*(dst + 36) = val;
		*(dst + 40) = val;
		*(dst + 44) = val;

		*(dst + 48) = val;
		*(dst + 52) = val;
		*(dst + 56) = val;
		*(dst + 60) = val;
	}

	for( ;dst < end; dst += 4)
	{
		*dst = val;
	}
}	

} // OPT_LEVEL

#endif


