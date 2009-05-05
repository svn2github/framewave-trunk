/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __STATISUM_H__
#define __STATISUM_H__

#include "fex.h"

namespace OPT_LEVEL
{


struct StatISum_8u: public fe1St<Fw8u,C1>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw8u))  * 2; // Load two registers
    class SRC1: public RegDesc< Fw8u, C1, nPIX_SSE > {};

	mutable  Fw32s sum;
	mutable XMM128 mSum;
	FEX_SSE2_REF
	StatISum_8u(){ sum = 0;}
	IV REFR_Init() { sum = 0; }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		sum += feData.sum;
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		sum += feData.sum;
        sum += feData.mSum.s32[0] + feData.mSum.s32[1]+ feData.mSum.s32[2]+feData.mSum.s32[3];

    }

	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV REFR(const Fw8u *s1 ) const           // REFR Pixel function
	{
		sum = (sum + *s1);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 srcHi,imRes,src;
		src.i = r.src1[0].i;
        XMM128 src2;
        src2.i = r.src1[1].i;
		CBL_SSE2::Unpack8UTo16U(src.i,srcHi.i);
        CBL_SSE2::Unpack8UTo16U(src2.i,imRes.i);
		
        src.i = _mm_add_epi16(src.i,src2.i);
        imRes.i = _mm_add_epi16(srcHi.i,imRes.i);
        imRes.i = _mm_add_epi16(imRes.i,src.i);
        CBL_SSE2::Unpack16UTo32U(imRes.i,src2.i);
        imRes.i = _mm_add_epi32(imRes.i,src2.i);
        mSum.i = _mm_add_epi32(mSum.i,imRes.i);
	}
};

struct StatISum_16s: public fe1St<Fw16s,C1>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw16s))  * 2; // Load two registers
    class SRC1: public RegDesc< Fw16s, C1, nPIX_SSE > {};

	mutable  Fw64s sum;
	mutable XMM128 mSum;
	FEX_SSE2_REF
	StatISum_16s(){ sum = 0;}
	IV REFR_Init() { sum = 0; }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		sum += feData.sum;
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		sum += feData.sum;
        sum += feData.mSum.s64[0] + feData.mSum.s64[1];
	}

	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV REFR(const Fw16s *s1 ) const           // REFR Pixel function
	{
		sum = (sum + *s1);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 srcHi,imRes,src;
		src.i = r.src1[0].i;
        XMM128 src2;
        src2.i =r.src1[1].i;
		CBL_SSE2::Unpack16STo32S(src.i,srcHi.i);
        CBL_SSE2::Unpack16STo32S(src2.i,imRes.i);

		src.i = _mm_add_epi32(src.i,srcHi.i);
        imRes.i = _mm_add_epi32(src2.i,imRes.i);
        imRes.i = _mm_add_epi32(src.i,imRes.i);

        CBL_SSE2::Unpack32STo64S(imRes.i,srcHi.i);
        imRes.i = _mm_add_epi64(imRes.i,srcHi.i);
        mSum.i= _mm_add_epi64(mSum.i,imRes.i);
	}
};


struct StatISum_8uC3: public fe1St<Fw8u,C3>
{
   // class SRC1: public RegDesc< Fw8u, C3, nPIX_SSE > {};
    //const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw8u))  * 1; //
   // class SRC1: public RegDesc< Fw8u, C1, nPIX_SSE > {};

	mutable  Fw64f sum[3];
	mutable XMM128 mSum;

	FEX_SSE2_REF

	StatISum_8uC3()
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0;
    }
	IV REFR_Init() 
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0; 
    }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];

      
		sum[0] +=  feData.mSum.u32[0];
        sum[1] +=  feData.mSum.u32[1];
        sum[2] +=  feData.mSum.u32[2];

    }

	IV SSE2_Init()
	{
		sum[0]=sum[1]=sum[2] = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV REFR(const Fw8u *s1 ) const           // REFR Pixel function
	{
		sum[0] = (sum[0] + s1[0]);
		sum[1] = (sum[1] + s1[1]);
		sum[2] = (sum[2] + s1[2]);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
        XMM128 src1,src2,src3,src1h,src2h,src3h;
        src1.i = r.src1[0].i;
        src2.i = r.src1[1].i;
        src3.i = r.src1[2].i;



        CBL_SSE2::Unpack8UTo16U(src1.i,src1h.i);
        CBL_SSE2::Unpack8UTo16U(src2.i,src2h.i);
        CBL_SSE2::Unpack8UTo16U(src3.i,src3h.i);

        src1.i = _mm_add_epi16(src1.i,src2h.i);  //start  with  g r b g  r b g r 
        src3.i = _mm_add_epi16(src1h.i,src3.i);  // r b g r  b g r b
        src2.i = _mm_add_epi16(src2.i,src3h.i);  // b g r b  g r b g


        src1h.f = _mm_shuffle_ps(src3.f,src2.f,_MM_SHUFFLE(1, 0, 3, 2)); // g r b g r b g r

        src2.i =  _mm_srli_si128(src2.i,8);  // 0 0 0 0 b g r b


        src1.i = _mm_add_epi16(src1h.i,src1.i); //  g r b g  r b g r +  g r b g r b g r
        src2.i = _mm_add_epi16(src3.i,src2.i); //  r b g r  b g r b + 0 0 0 0 b g r b

        src2.i =  _mm_slli_si128( src2.i,8 );
        src2.i =  _mm_srli_si128( src2.i,4 );
        src1.i = _mm_add_epi16(src1.i,src2.i);

        src2.i =  _mm_srli_si128(src1.i,8);  // 0 0 0 0 g r b g
        src2.i =  _mm_slli_si128(src2.i,2);  // 0 0 0 g r b g 0

        src1.i =  _mm_slli_si128(src1.i,8);  //  r b g r 0 0 0 0
        src1.i =  _mm_srli_si128(src1.i,8);  //  0 0 0 0 r b g r

        src1.i = _mm_add_epi16(src1.i,src2.i);

        src2.i =  _mm_srli_si128(src1.i,6);  

        src1.i = _mm_add_epi16(src1.i,src2.i);

        CBL_SSE2::Unpack16UTo32U(src1.i,src1h.i);

        mSum.i = _mm_add_epi32(src1.i,mSum.i);

	}
};



struct StatISum_16sC3: public fe1St<Fw16s,C3>
{


	mutable  Fw64f sum[3];
	mutable XMM128 mSum[2];

	FEX_SSE2_REF

	StatISum_16sC3()
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0;
    }
	IV REFR_Init() 
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0; 
    }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];

      
		sum[0] +=  feData.mSum[1].s64[1];
        sum[1] +=  feData.mSum[0].s64[1];
        sum[2] +=  feData.mSum[1].s64[0];

    }

	IV SSE2_Init()
	{
		sum[0]=sum[1]=sum[2] = 0;
		mSum[0].i = _mm_setzero_si128();
		mSum[1].i = _mm_setzero_si128();
	}

	IV REFR(const Fw16s *s1 ) const           // REFR Pixel function
	{
		sum[0] = (sum[0] + s1[0]);
		sum[1] = (sum[1] + s1[1]);
		sum[2] = (sum[2] + s1[2]);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
        XMM128 src1,src2,src3,src1h,src2h,src3h;
        src1.i = r.src1[0].i;
        src2.i = r.src1[1].i;
        src3.i = r.src1[2].i;



        CBL_SSE2::Unpack16STo32S(src1.i,src1h.i);
        CBL_SSE2::Unpack16STo32S(src2.i,src2h.i);
        CBL_SSE2::Unpack16STo32S(src3.i,src3h.i);

        src1.i = _mm_add_epi32(src1.i,src2h.i);  //start  with   r b g r 
        src3.i = _mm_add_epi32(src1h.i,src3.i);  //              g r b g
        src2.i = _mm_add_epi32(src2.i,src3h.i);  //              b g r b  



        src2h.i = _mm_srli_si128(src2.i,8);  //              0 0 b g 

        src2.i = _mm_slli_si128(src2.i,8);  //              b r  00

        src3.i = _mm_add_epi32(src2h.i,src3.i);  //              0 0 b g   +  g r b g

        src2.i = _mm_add_epi32(src2.i,src1.i);  //              r b 00   +  r b g r 

        src1.i = _mm_slli_si128(src3.i,4);  //          r b g 0  
        src1.i =  _mm_add_epi32( src1.i, src2.i);
        src3.i = _mm_srli_si128(src3.i,8);  //          r b g 0  

        src2.i =  _mm_slli_si128( src1.i, 12); //
        src1.i =  _mm_add_epi32( src1.i, src3.i); //

        src1.i =  _mm_add_epi32( src1.i, src2.i); //  // r b g x

        CBL_SSE2::Unpack32STo64S(src1.i,src2.i);
        mSum[0].i = _mm_add_epi64(src1.i, mSum[0].i);// g x
        mSum[1].i = _mm_add_epi64(src2.i, mSum[1].i);// r b

	}
};



// AC4

struct StatISum_8u_AC4: public fe1St<Fw8u,AC4>
{
   // class SRC1: public RegDesc< Fw8u, C3, nPIX_SSE > {};
    //const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw8u))  * 1; //
   // class SRC1: public RegDesc< Fw8u, C1, nPIX_SSE > {};

	mutable  Fw64f sum[3];
	mutable XMM128 mSum;

	FEX_SSE2_REF

	StatISum_8u_AC4()
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0;
    }
	IV REFR_Init() 
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0; 
    }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];

      
		sum[0] +=  feData.mSum.u32[0];
        sum[1] +=  feData.mSum.u32[1];
        sum[2] +=  feData.mSum.u32[2];

    }

	IV SSE2_Init()
	{
		sum[0]=sum[1]=sum[2] = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV REFR(const Fw8u *s1 ) const           // REFR Pixel function
	{
		sum[0] = (sum[0] + s1[0]);
		sum[1] = (sum[1] + s1[1]);
		sum[2] = (sum[2] + s1[2]);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
        XMM128 src1,src2;
        src1.i = r.src1[0].i;

        CBL_SSE2::Unpack8UTo16U(src1.i,src2.i);


        src1.i = _mm_add_epi16(src1.i,src2.i);  // a b g r a b g r 
        src2.i =  _mm_srli_si128(src1.i,8);     //  0 0 0 0 a b g r
        src1.i = _mm_add_epi16(src2.i,src1.i);  // x x x x a b g r


        CBL_SSE2::Unpack16UTo32U(src1.i,src2.i);

        mSum.i = _mm_add_epi32(src1.i,mSum.i);

	}
};



struct StatISum_16s_AC4: public fe1St<Fw16s,AC4>
{


	mutable  Fw64f sum[3];
	mutable XMM128 mSum[2];

	FEX_SSE2_REF

	StatISum_16s_AC4()
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0;
    }
	IV REFR_Init() 
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0; 
    }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];

      
		sum[0] +=  feData.mSum[0].s64[0];
        sum[1] +=  feData.mSum[0].s64[1];
        sum[2] +=  feData.mSum[1].s64[0];

    }

	IV SSE2_Init()
	{
		sum[0]=sum[1]=sum[2] = 0;
		mSum[0].i = _mm_setzero_si128();
		mSum[1].i = _mm_setzero_si128();
	}

	IV REFR(const Fw16s *s1 ) const           // REFR Pixel function
	{
		sum[0] = (sum[0] + s1[0]);
		sum[1] = (sum[1] + s1[1]);
		sum[2] = (sum[2] + s1[2]);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
        XMM128 src1,src2;
        src1.i = r.src1[0].i;

        CBL_SSE2::Unpack16STo32S(src1.i,src2.i);


        src1.i = _mm_add_epi32(src1.i,src2.i);  // a b g r 
        CBL_SSE2::Unpack32STo64S(src1.i,src2.i);
        mSum[0].i = _mm_add_epi64(src1.i,mSum[0].i);
        mSum[1].i = _mm_add_epi64(src2.i,mSum[1].i);
	}
};



//

// C4
struct StatISum_8u_C4R: public fe1St<Fw8u,C4>
{
   // class SRC1: public RegDesc< Fw8u, C3, nPIX_SSE > {};
    //const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw8u))  * 1; //
   // class SRC1: public RegDesc< Fw8u, C1, nPIX_SSE > {};

	mutable  Fw64f sum[4];
	mutable XMM128 mSum;

	FEX_SSE2_REF

	StatISum_8u_C4R()
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0;
        sum[3]=0;
    }
	IV REFR_Init() 
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0; 
        sum[3]=0; 
    }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];
        sum[3] += feData.sum[3];
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];
        sum[3] += feData.sum[3];

      
		sum[0] +=  feData.mSum.u32[0];
        sum[1] +=  feData.mSum.u32[1];
        sum[2] +=  feData.mSum.u32[2];
        sum[3] +=  feData.mSum.u32[3];

    }

	IV SSE2_Init()
	{
		sum[0]=sum[1]=sum[2] = sum[3]=0;
		mSum.i = _mm_setzero_si128();
	}

	IV REFR(const Fw8u *s1 ) const           // REFR Pixel function
	{
		sum[0] = (sum[0] + s1[0]);
		sum[1] = (sum[1] + s1[1]);
		sum[2] = (sum[2] + s1[2]);
		sum[3] = (sum[3] + s1[3]);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
        XMM128 src1,src2;
        src1.i = r.src1[0].i;

        CBL_SSE2::Unpack8UTo16U(src1.i,src2.i);


        src1.i = _mm_add_epi16(src1.i,src2.i);  // a b g r a b g r 
        src2.i =  _mm_srli_si128(src1.i,8);     //  0 0 0 0 a b g r
        src1.i = _mm_add_epi16(src2.i,src1.i);  // x x x x a b g r


        CBL_SSE2::Unpack16UTo32U(src1.i,src2.i);

        mSum.i = _mm_add_epi32(src1.i,mSum.i);

	}
};



struct StatISum_16s_C4R: public fe1St<Fw16s,C4>
{


	mutable  Fw64f sum[4];
	mutable XMM128 mSum[2];

	FEX_SSE2_REF

	StatISum_16s_C4R()
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0;
       sum[3]=0;
    }
	IV REFR_Init() 
    { 
        sum[0] = 0;
        sum[1]=0;
        sum[2]=0; 
       sum[3]=0;
    }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];
        sum[3] += feData.sum[3];
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		sum[0] += feData.sum[0];
        sum[1] += feData.sum[1];
        sum[2] += feData.sum[2];
        sum[3] += feData.sum[3];

      
		sum[0] +=  feData.mSum[0].s64[0];
        sum[1] +=  feData.mSum[0].s64[1];
        sum[2] +=  feData.mSum[1].s64[0];
        sum[3] +=  feData.mSum[1].s64[1];

    }

	IV SSE2_Init()
	{
		sum[0]=sum[1]=sum[2] = sum[3]= 0;
		mSum[0].i = _mm_setzero_si128();
		mSum[1].i = _mm_setzero_si128();
	}

	IV REFR(const Fw16s *s1 ) const           // REFR Pixel function
	{
		sum[0] = (sum[0] + s1[0]);
		sum[1] = (sum[1] + s1[1]);
		sum[2] = (sum[2] + s1[2]);
		sum[3] = (sum[3] + s1[3]);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
        XMM128 src1,src2;
        src1.i = r.src1[0].i;

        CBL_SSE2::Unpack16STo32S(src1.i,src2.i);


        src1.i = _mm_add_epi32(src1.i,src2.i);  // a b g r 
        CBL_SSE2::Unpack32STo64S(src1.i,src2.i);
        mSum[0].i = _mm_add_epi64(src1.i,mSum[0].i);
        mSum[1].i = _mm_add_epi64(src2.i,mSum[1].i);
	}
};





//
}

#endif // __STATISUM_H__
