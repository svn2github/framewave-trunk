/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "foreachx.h"

namespace OPT_LEVEL
{

namespace
{
    template< class TS >
    SYS_INLINE STATIC void Sum ( FnDat &data, TS *s1, CH cs, int index )
    {
        Fw64f *pSum  = (Fw64f*)data.data[0];
        ASSERT( ChannelCount(cs) == ChannelCount(cs) ); cs;
        switch( cs )
        {
        case C4:    pSum[index] = s1[3]+ pSum[index];
        case C3:
        case AC4:   pSum[index] = s1[2]+ pSum[index];
                    pSum[index] = s1[1]+ pSum[index];
        case C1:    pSum[index] = s1[0]+ pSum[index];
        }
    }

    SYS_INLINE STATIC void Sum_8u_unit ( const XMM128& src, XMM128 &accum)
    {
        XMM128 temp1, temp2;
        temp1 = src;
        CBL_SSE2::Unpack8UTo16U(temp1, temp2);
        temp1.i = _mm_add_epi16(temp1.i, temp2.i);
        accum.i = _mm_add_epi16(temp1.i, accum.i);
    }

    SYS_INLINE STATIC void Sum_8u_SSE2 ( FnDat &data , RegDescriptor &reg )
    {   XMM128 temp1,temp2;
        temp1.i = _mm_setzero_si128();
        switch( reg.dst.RegCount() )
        {
        case 4: Sum_8u_unit( reg.dst[3], temp1 );
        case 3: Sum_8u_unit( reg.dst[2], temp1 );
        case 2: Sum_8u_unit( reg.dst[1], temp1 );
        case 1: Sum_8u_unit( reg.dst[0], temp1 );
        }
        
        CBL_SSE2::Unpack16UTo32U(temp1.i, temp2.i);
        temp1.i = _mm_add_epi32 (temp1.i,temp2.i);
        temp2.i = _mm_srli_si128(temp1.i, 8);
        temp1.i = _mm_add_epi32 (temp1.i, temp2.i);
        temp1.d = _mm_cvtepi32_pd(temp1.i);
        data.xmmData[reg.threadNum].d = _mm_add_pd(temp1.d, data.xmmData[reg.threadNum].d);
    }

    SYS_INLINE STATIC void Sum_16s_unit ( XMM128& src, XMM128 &accum)
    {
        XMM128 temp1, temp2;    
        temp1 = src;
        CBL_SSE2::Unpack16STo32S(temp1.i, temp2.i);
        temp1.i = _mm_add_epi32(temp1.i, temp2.i);
        accum.i = _mm_add_epi32(accum.i, temp1.i);
    }
    SYS_INLINE STATIC void Sum_16s_SSE2 ( FnDat &data , RegDescriptor &reg )
    {
        XMM128 accum, temp;
        accum.i = _mm_setzero_si128();
        switch( reg.dst.RegCount() )
        {
        case 4: Sum_16s_unit( reg.dst[3], accum );
        case 3: Sum_16s_unit( reg.dst[2], accum );
        case 2: Sum_16s_unit( reg.dst[1], accum );
        case 1: Sum_16s_unit( reg.dst[0], accum );
        }
        temp.i  = _mm_srli_si128(accum.i, 8);
        accum.i = _mm_add_epi32 (accum.i, temp.i);
        accum.d = _mm_cvtepi32_pd(accum.i);
        data.xmmData[reg.threadNum].d = _mm_add_pd(accum.d, data.xmmData[reg.threadNum].d);
    }

    SYS_INLINE STATIC void Sum_32f_unit ( XMM128& src, FnDat& data, int &threadNum)
    {
        XMM128 temp;
        //temp1 = src;
        temp.d = _mm_cvtps_pd(src.f);
        data.xmmData[threadNum].d = _mm_add_pd(temp.d, data.xmmData[threadNum].d);
        temp.i = _mm_srli_si128(src.i, 8);
        temp.d = _mm_cvtps_pd(temp.f);
        data.xmmData[threadNum].d = _mm_add_pd(temp.d, data.xmmData[threadNum].d);
    }
    SYS_INLINE STATIC void Sum_32f_SSE2 ( FnDat &data , RegDescriptor &reg )
    {
        switch( reg.dst.RegCount() )
        {
        case 4: Sum_32f_unit( reg.dst[3], data, reg.threadNum );
        case 3: Sum_32f_unit( reg.dst[2], data, reg.threadNum  );
        case 2: Sum_32f_unit( reg.dst[1], data, reg.threadNum  );
        case 1: Sum_32f_unit( reg.dst[0], data, reg.threadNum  );
        }
    }


    SYS_INLINE STATIC void Sum_func_C1_setup ( FnDat &dat)
    {
            for(int i=0;i< dat.numThreads; i++)
            {
                    dat.xmmData[i].f64[0] = 0;
                    dat.xmmData[i].f64[1] = 0;
            }
    }
    SYS_INLINE    STATIC void Sum_func_C1_cleanup(FnDat &dat)
    {
        A64F *sum = (A64F*)dat.data[0];
            for(int i=0;i< dat.numThreads; i++)
            {
                    sum[0] = sum[0]+dat.xmmData[i].f64[0];
                    sum[0] = sum[0]+dat.xmmData[i].f64[1];
            }
            for(int i=1;i< dat.numThreads; i++)
            {
                    sum[0] = sum[0]+sum[i];
            }
    }

//End Sum

};


} // namespace OPT_LEVEL

