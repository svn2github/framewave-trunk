/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __CONVERSION_SIG_DEF__
#define __CONVERSION_SIG_DEF__ 


#include "fe.h"
#include "Conversion.h"

namespace OPT_LEVEL
{

struct Convert_8s16s: public fe2<Fw8s, C1,Fw16s,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_8s16s_sse2_S(r.src1[0].i,r.dst[0].i);
	}      
	IV REFR(const Fw8s *s,Fw16s *d) const                        // REFR Pixel function
	{         
		Convert(s,d);
	}  
};

struct Convert_8u16s: public fe2<Fw8u, C1,Fw16s,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_8u16s_sse2_S(r.src1[0].i,r.dst[0].i);
	}      
	IV REFR(const Fw8u *s,Fw16s *d) const                        // REFR Pixel function
	{         
		Convert(s,d);
	}  
};

struct Convert_8s32f: public fe2<Fw8s, C1,Fw32f,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_8s32f_sse2_S(r.src1[0].i,r.dst[0].f);
	}      
	IV REFR(const Fw8s *s,Fw32f *d) const                        // REFR Pixel function
	{         
		Convert(s,d);
	}  
};


struct Convert_8u32f: public fe2<Fw8u, C1,Fw32f,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_8u32f_sse2_S(r.src1[0].i,r.dst[0].f);
	}      
	IV REFR(const Fw8u *s,Fw32f *d) const                        // REFR Pixel function
	{         
		Convert(s,d);
	}  
};


struct Convert_16s32s: public fe2<Fw16s, C1,Fw32s,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_16s32s_sse2_S(r.src1[0].i,r.dst[0].i);
	}      
	IV REFR(const Fw16s *s,Fw32s *d) const                        // REFR Pixel function
	{         
		Convert(s,d);
	}  
};

struct Convert_16u32s: public fe2<Fw16u, C1,Fw32s,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_16u32s_sse2_S(r.src1[0].i,r.dst[0].i);
	}      
	IV REFR(const Fw16u *s,Fw32s *d) const                        // REFR Pixel function
	{         
		Convert(s,d);
	}  
};

struct Convert_16s32f: public fe2<Fw16s, C1,Fw32f,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_16s32f_sse2_S(r.src1[0].i,r.dst[0].f);
	}      
	IV REFR(const Fw16s *s,Fw32f *d) const                        // REFR Pixel function
	{         
		Convert(s,d);
	}  
};


struct Convert_16u32f: public fe2<Fw16u, C1,Fw32f,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_16u32f_sse2_S(r.src1[0].i,r.dst[0].f);
	}      
	IV REFR(const Fw16u *s,Fw32f *d) const                        // REFR Pixel function
	{         
		Convert16u32f(s,d);
	}  
};



struct Convert_32s16s: public fe2<Fw32s, C1,Fw16s,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32s16s_sse2_S(r.src1[0].i,r.dst[0].i);
	}      
	IV REFR(const Fw32s *s,Fw16s *d) const                        // REFR Pixel function
	{         
		Convert(s,d);
	}  
};

struct Convert_32u16s: public fe2<Fw32u, C1,Fw16s,C1>
{
	FE_SSE2_REF
	__m128i shiftone;
	__m128i shift31;
	IV SSE2_Init(){
		shiftone = _mm_set_epi32(0,0,0,1);
		shift31 = _mm_set_epi32(0,0,0,31);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32u16s_sse2_S(r.src1[0].i,r.dst[0].i,shiftone,shift31);
	}      
	IV REFR(const Fw32u *s,Fw16s *d) const                        // REFR Pixel function
	{   
		Convert1(s,d);
	}  
};

struct Convert_32s32f: public fe2<Fw32s, C1,Fw32f,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32s32f_sse2_S(r.src1[0].i,r.dst[0].f);
	}      
	IV REFR(const Fw32s *s,Fw32f *d) const                        // REFR Pixel function
	{         
		Convert(s,d);
	}
};

struct Convert_32u32f: public fe2<Fw32u, C1,Fw32f,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32u32f_sse2_S(r.src1[0].i,r.dst[0].f);
	}      
	IV REFR(const Fw32u *s,Fw32f *d) const                        // REFR Pixel function
	{         
		Convert32u32f(s,d);
	}  
};

struct Convert_32s64f: public fe2<Fw32s, C1,Fw64f,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32s64f_sse2_S(r.src1[0].i,r.dst[0].d);
	}      
	IV REFR(const Fw32s *s,Fw64f *d) const                        // REFR Pixel function
	{         
		Convert(s,d);
	}  
};

struct Convert_32u64f: public fe2<Fw32u, C1,Fw64f,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32u64f_sse2_S(r.src1[0].i,r.dst[0].d);
	}      
	IV REFR(const Fw32u *s,Fw64f *d) const                        // REFR Pixel function
	{         
		Convert(s,d);
	}  
};

struct Convert_32f64f: public fe2<Fw32f, C1,Fw64f,C1>
{
	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32f64f_sse2_S(r.src1[0].f,r.dst[0].d);
	}      
	IV REFR(const Fw32f *s,Fw64f *d) const                        // REFR Pixel function
	{         
		Convert32f64f(s,d);
	}  
};

template<class TS,class TD>
struct ConvertGen : public fe2<TS, C1,TD,C1>
{
	XMM128 imin;
	XMM128 imax;
	XMM128 iscale;
	int scale;
	ConvertGen(){}
	ConvertGen(int s)
	{
		scale = s;
	}
};

struct Convert_64f32f: public fe2<Fw64f, C1,Fw32f,C1>
{
	XMM128 imin;
	XMM128 imax;
	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_64f32f_setup_S(imin.d,imax.d);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_64f32f_sse2_S(r.src1[0].d,r.dst[0].f,imin.d,imax.d);
	}      
	IV REFR(const Fw64f *s,Fw32f *d) const                        // REFR Pixel function
	{         
		Convert64f32f(s,d);
	}  
};


struct Convert_16s32f_Sfs: public ConvertGen<Fw16s,Fw32f>
{
	Convert_16s32f_Sfs(int s) : ConvertGen<Fw16s,Fw32f>(s){}

	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_16s32f_Sfs_setup_S(iscale.f,scale);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_16s32f_Sfs_sse2_S(r.src1[0].i,r.dst[0].f,iscale.f);
	}      
	IV REFR(const Fw16s *s,Fw32f *d) const                        // REFR Pixel function
	{         
		ConvertScale16s32f(s,d,scale);
	}
};
	
struct Convert_16u32f_Sfs: public ConvertGen<Fw16u,Fw32f>
{
	Convert_16u32f_Sfs(int s) : ConvertGen<Fw16u,Fw32f>(s){}

	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_16s32f_Sfs_setup_S(iscale.f,scale); //same as 16s
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_16u32f_Sfs_sse2_S(r.src1[0].i,r.dst[0].f,iscale.f);
	}      
	IV REFR(const Fw16u *s,Fw32f *d) const                        // REFR Pixel function
	{         
		ConvertScale16u32f(s,d,scale);
	}
};


struct Convert_16s64f_Sfs: public ConvertGen<Fw16s,Fw64f>
{

	Convert_16s64f_Sfs(int s) : ConvertGen<Fw16s,Fw64f>(s){}
	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_16s64f_Sfs_setup_S(iscale.d,scale);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_16s64f_Sfs_sse2_S(r.src1[0].i,r.dst[0].d,iscale.d);
	}      
	IV REFR(const Fw16s *s,Fw64f *d) const                        // REFR Pixel function
	{         
		ConvertScale16s64f(s,d,scale);
	}
};


struct Convert_32s16s_Sfs: public ConvertGen<Fw32s,Fw16s>
{
	Convert_32s16s_Sfs(int s): ConvertGen<Fw32s,Fw16s>(s){}
		
	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_32s16s_Sfs_sse2_setup(iscale.f,scale,imin.f,imax.f);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32s16s_Sfs_sse2_S(r.src1[0].i,r.dst[0].i,iscale.f,imin.f,imax.f);
	}      
	IV REFR(const Fw32s *s,Fw16s *d) const                        // REFR Pixel function
	{         
		ConvertScale32s16s(s,d,scale);
	}
};


struct Convert_32s16s_Sfs_noscale: public fe2<Fw32s, C1,Fw16s,C1>
{

	FE_SSE2_REF
	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32s16s_Sfs_noscale_sse2_S(r.src1[0].i,r.dst[0].i);
	}      
	IV REFR(const Fw32s *s,Fw16s *d) const                        // REFR Pixel function
	{         
		ConvertScale32s16s_noscale(s,d);
	}
};



struct Convert_32s32f_Sfs: public ConvertGen<Fw32s,Fw32f>
{
	
	Convert_32s32f_Sfs(int s): ConvertGen<Fw32s,Fw32f>(s){}

	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_32s32f_Sfs_setup_S(iscale.f,scale);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32s32f_Sfs_sse2_S(r.src1[0].i,r.dst[0].f,iscale.f);
	}      
	IV REFR(const Fw32s *s,Fw32f *d) const                        // REFR Pixel function
	{         
		ConvertScale32s32f(s,d,scale);
	}
};


struct Convert_32s64f_Sfs: public ConvertGen<Fw32s,Fw64f>
{
	
	Convert_32s64f_Sfs(int s): ConvertGen<Fw32s,Fw64f>(s){}

	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_32s64f_Sfs_setup_S(iscale.d,scale);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32s64f_Sfs_sse2_S(r.src1[0].i,r.dst[0].d,iscale.d);
	}      
	IV REFR(const Fw32s *s,Fw64f *d) const                        // REFR Pixel function
	{         
		ConvertScale32s64f(s,d,scale);
	}
};


struct Convert_32f8s_Sfs: public ConvertGen<Fw32f,Fw8s>
{


	Convert_32f8s_Sfs(int s):ConvertGen<Fw32f,Fw8s>(s){}

	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_32f8s_Sfs_setup_S(iscale.f,scale,imin.f,imax.f);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32f8s_Sfs_sse2_S(r.src1[0].f,r.dst[0].i,iscale.f,imin.f,imax.f);
	}      
	IV REFR(const Fw32f *s,Fw8s *d) const                        // REFR Pixel function
	{         
		ConvertScale32f8s(s,d,scale);
	}
};

struct Convert_32f8s_Sfs_RndZero : public Convert_32f8s_Sfs
{
	Convert_32f8s_Sfs_RndZero(int s):Convert_32f8s_Sfs(s){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32f8s_Sfs_rndzero_sse2_S(r.src1[0].f,r.dst[0].i,iscale.f,imin.f,imax.f);
	}      
	IV REFR(const Fw32f *s,Fw8s *d) const                        // REFR Pixel function
	{         
		ConvertScaleRndZero32f8s(s,d,scale);
	}
};

struct Convert_32f8u_Sfs : public ConvertGen<Fw32f,Fw8u>
{
	Convert_32f8u_Sfs(int s):ConvertGen<Fw32f,Fw8u>(s){}
	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_32f8u_Sfs_setup_S(iscale.f,scale,imin.f,imax.f);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32f8u_Sfs_sse2_S(r.src1[0].f,r.dst[0].i,iscale.f,imin.f,imax.f);
	}      
	IV REFR(const Fw32f *s,Fw8u *d) const                        // REFR Pixel function
	{         
		ConvertScale32f8u(s,d,scale);
	}
};


struct Convert_32f8u_Sfs_RndZero : public Convert_32f8u_Sfs
{
		Convert_32f8u_Sfs_RndZero(int s):Convert_32f8u_Sfs(s){}

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			sConvert_32f8u_Sfs_rndzero_sse2_S(r.src1[0].f,r.dst[0].i,iscale.f,imin.f,imax.f);
		}      
		IV REFR(const Fw32f *s,Fw8u *d) const                        // REFR Pixel function
		{         
			ConvertScaleRndZero32f8u(s,d,scale);
		}
};


struct Convert_32f16s_Sfs : public ConvertGen<Fw32f,Fw16s>
{
	Convert_32f16s_Sfs(int s):ConvertGen<Fw32f,Fw16s>(s){}
	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_32f16s_Sfs_setup_S(iscale.f,scale,imin.f,imax.f);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32f16s_Sfs_sse2_S(r.src1[0].f,r.dst[0].i,iscale.f,imin.f,imax.f);
	}      
	IV REFR(const Fw32f *s,Fw16s *d) const                        // REFR Pixel function
	{         
		ConvertScale32f16s(s,d,scale);
	}
};


struct Convert_32f16s_Sfs_RndZero : public Convert_32f16s_Sfs
{
	Convert_32f16s_Sfs_RndZero(int s):Convert_32f16s_Sfs(s){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32f16s_Sfs_rndzero_sse2_S(r.src1[0].f,r.dst[0].i,iscale.f,imin.f,imax.f);
	}      
	IV REFR(const Fw32f *s,Fw16s *d) const                        // REFR Pixel function
	{         
		ConvertScaleRndZero32f16s(s,d,scale);
	}
};

struct Convert_32f16u_Sfs : public ConvertGen<Fw32f,Fw16u>
{
	Convert_32f16u_Sfs(int s):ConvertGen<Fw32f,Fw16u>(s){}
	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_32f16u_Sfs_setup_S(iscale.f,scale,imin.f,imax.f);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32f16u_Sfs_sse2_S(r.src1[0].f,r.dst[0].i,iscale.f,imin.f,imax.f);
	}      
	IV REFR(const Fw32f *s,Fw16u *d) const                        // REFR Pixel function
	{         
		ConvertScale32f16u(s,d,scale);
	}
};

struct Convert_32f16u_Sfs_RndZero : public Convert_32f16u_Sfs
{
	Convert_32f16u_Sfs_RndZero(int s):Convert_32f16u_Sfs(s){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32f16u_Sfs_rndzero_sse2_S(r.src1[0].f,r.dst[0].i,iscale.f,imin.f,imax.f);
	}      
	IV REFR(const Fw32f *s,Fw16u *d) const                        // REFR Pixel function
	{         
		ConvertScaleRndZero32f16u(s,d,scale);
	}
};


struct Convert_32f32s_Sfs : public ConvertGen<Fw32f,Fw32s>
{
	Convert_32f32s_Sfs(int s):ConvertGen<Fw32f,Fw32s>(s){}
	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_32f32s_Sfs_setup_S(iscale.d,scale,imin.d,imax.d);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32f32s_Sfs_sse2_S(r.src1[0].f,r.dst[0].i,iscale.d,imin.d,imax.d);
	}      
	IV REFR(const Fw32f *s,Fw32s *d) const                        // REFR Pixel function
	{         
		ConvertScale32f32s(s,d,scale);
	}
};

struct Convert_32f32s_Sfs_RndZero : public Convert_32f32s_Sfs
{
	Convert_32f32s_Sfs_RndZero(int s):Convert_32f32s_Sfs(s){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_32f32s_Sfs_rndzero_sse2_S(r.src1[0].f,r.dst[0].i,iscale.d,imin.d,imax.d);
	}      
	IV REFR(const Fw32f *s,Fw32s *d) const                        // REFR Pixel function
	{         
		ConvertScaleRndZero32f32s(s,d,scale);
	}
};


struct Convert_64s32s_Sfs : public ConvertGen<Fw64s,Fw32s>
{
	Convert_64s32s_Sfs(int s):ConvertGen<Fw64s,Fw32s>(s){}
	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_64s32s_Sfs_setup_S(iscale.d,scale,imin.d,imax.d);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_64s32s_Sfs_sse2_S(r.src1[0].i,r.dst[0].i,iscale.d,imin.d,imax.d);
	}      
	IV REFR(const Fw64s *s,Fw32s *d) const                        // REFR Pixel function
	{         
		ConvertScale64s32s(s,d,scale);
	}
};


struct Convert_64f32s_Sfs : public ConvertGen<Fw64f,Fw32s>
{
	Convert_64f32s_Sfs(int s):ConvertGen<Fw64f,Fw32s>(s){}
	FE_SSE2_REF
	IV SSE2_Init()
	{
		sConvert_64f32s_Sfs_setup_S(iscale.d,scale,imin.d,imax.d);
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_64f32s_Sfs_sse2_S(r.src1[0].d,r.dst[0].i,iscale.d,imin.d,imax.d);
	}      
	IV REFR(const Fw64f *s,Fw32s *d) const                        // REFR Pixel function
	{         
		ConvertScale64f32s(s,d,scale);
	}
};



struct Convert_64f32s_Sfs_RndZero : public Convert_64f32s_Sfs
{
	Convert_64f32s_Sfs_RndZero(int s):Convert_64f32s_Sfs(s){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		sConvert_64f32s_Sfs_rndzero_sse2_S(r.src1[0].d,r.dst[0].i,iscale.d,imin.d,imax.d);
	}      
	IV REFR(const Fw64f *s,Fw32s *d) const                        // REFR Pixel function
	{         
		ConvertScaleRndZero64f32s(s,d,scale);
	}
};

template<class TS,class TD>
struct Phase1 : public fe2<TS,C1,TD,C1>
{
	int scale;
	XMM128 iscale;
	
	Phase1(){}
	Phase1(int s)
	{
		scale = s;
	}
};

template<class TS1,class TS2, class TD>
struct Phase2 : public fe3<TS1,C1,TS2,C1,TD,C1>
{
	int scale;
	XMM128 iscale;
	
	Phase2(){}
	Phase2(int s)
	{
		scale = s;
	}
};

template<class TS, class TD>
struct Phase_gen : public Phase1<TS,TD>
{
	FE_REF
	IV REFR(TS *s,TD *d) const                        // REFR Pixel function
	{         
		Phase(s,d);
	}
};

template<class TS1, class TS2, class TD>
struct Phase_gen2 : public Phase2<TS1,TS2,TD>
{

	FE_REF
	IV REFR(TS1 *s1,TS2 *s2,TD *d) const                        // REFR Pixel function
	{         
		Phase(s1,s2,d);
	}
};


template<class TS, class TD>
struct Phase_gen_scale : public Phase1<TS,TD>
{
	
	Phase_gen_scale(int s): Phase1<TS,TD>(s){}
	FE_REF
	IV REFR(TS *s,TD *d) const                        // REFR Pixel function
	{         
		Phase(s,d,Phase1<TS,TD>::scale);
	}
};


template<class TS1, class TS2, class TD>
struct Phase_gen2_scale : public Phase2<TS1,TS2,TD>
{
	Phase_gen2_scale(int s): Phase2<TS1,TS2,TD>(s){}
	FE_REF
	IV REFR(TS1 *s1,TS2 *s2,TD *d) const                        // REFR Pixel function
	{         
		Phase(s1,s2,d,Phase2<TS1,TS2,TD>::scale);
	}
};


template<class TD>
struct Zero1 : public fe1<TD,C1>
{
	FE_SSE2_REF

	IV SSE2_Init(){}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		zero_si128_S(r.dst[0].i);
	}      
	IV REFR(TD *d) const                        // REFR Pixel function
	{         
		ZeroS(d);
	}
};

template<class TS, class TD>
struct Copy2 : public fe2 <TS,C1,TD,C1>
{
		FE_SSE2_REF
		IV SSE2_Init(){}

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			copy_128si_sse2_S(r.src1[0].i, r.dst[0].i);
		}      
		IV REFR(TS *s, TD *d) const                        // REFR Pixel function
		{         
			CopyS(s,d);
		}
};


template<class TD>
struct Set1 : public fe1 <TD,C1>
{
		TD value;
		XMM128 mValue;

		Set1(TD v)
		{
			value = v;
		}

		FE_SSE2_REF
		IV SSE2_Init()
		{
			set_setup_S(mValue,value);
		}

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			set_128si_sse2_S(mValue.i, r.dst[0].i);
		}      
		IV REFR(TD *d) const                        // REFR Pixel function
		{         
			SetS(d,value);
		}
};

} // namespace OPT_LEVEL

#endif


