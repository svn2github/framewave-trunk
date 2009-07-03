/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Statistics1.h"
#include "fwSignal.h"

namespace OPT_LEVEL
{

struct MaxEvery_16s : public fe2 <Fw16s,C1,Fw16s,C1>
{
		FE_SSE2_REF
	/*	IV SSE2_Init()
		{
		}
   */
		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			MaxEvery_sse2_16s(r.src1[0].i, r.dst[0].i);
		}      
		IV REFR(const Fw16s *s,Fw16s *d) const                        // REFR Pixel function
		{         
			MaxEveryC1(s,d);
		}
};

struct MaxEvery_32s : public fe2 <Fw32s,C1,Fw32s,C1>
{
		FE_SSE2_REF
	/*	IV SSE2_Init()
		{
		}
   */
		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			MaxEvery_sse2_32s(r.src1[0].i, r.dst[0].i);
		}      
		IV REFR(const Fw32s *s,Fw32s *d) const                        // REFR Pixel function
		{         
			MaxEveryC1(s,d);
		}
};

struct MaxEvery_32f : public fe2 <Fw32f,C1,Fw32f,C1>
{
		FE_SSE2_REF
	/*	IV SSE2_Init()
		{
		}
   */
		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			MaxEvery_sse2_32f(r.src1[0].f, r.dst[0].f);
		}      
		IV REFR(const Fw32f *s,Fw32f *d) const                        // REFR Pixel function
		{         
			MaxEveryC1(s,d);
		}
};


struct MinEvery_16s : public fe2 <Fw16s,C1,Fw16s,C1>
{
		FE_SSE2_REF
	/*	IV SSE2_Init()
		{
		}
   */
		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			MinEvery_sse2_16s(r.src1[0].i, r.dst[0].i);
		}      
		IV REFR(const Fw16s *s,Fw16s *d) const                        // REFR Pixel function
		{         
			MinEveryC1(s,d);
		}
};

struct MinEvery_32s : public fe2 <Fw32s,C1,Fw32s,C1>
{
		FE_SSE2_REF
	/*	IV SSE2_Init()
		{
		}
   */
		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			MinEvery_sse2_32s(r.src1[0].i, r.dst[0].i);
		}      
		IV REFR(const Fw32s *s,Fw32s *d) const                        // REFR Pixel function
		{         
			MinEveryC1(s,d);
		}
};

struct MinEvery_32f : public fe2 <Fw32f,C1,Fw32f,C1>
{
		FE_SSE2_REF
	/*	IV SSE2_Init()
		{
		}
   */
		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			MinEvery_sse2_32f(r.src1[0].f, r.dst[0].f);
		}      
		IV REFR(const Fw32f *s,Fw32f *d) const                        // REFR Pixel function
		{         
			MinEveryC1(s,d);
		}
};

} // namespace OPT_LEVEL

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMaxEvery_16s_I)(const Fw16s* pSrc, Fw16s* pSrcDst, int len)
{ 
	MaxEvery_16s  data;
	return OPT_LEVEL::fe<MaxEvery_16s>( data,pSrc, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMaxEvery_32s_I)(const Fw32s* pSrc, Fw32s* pSrcDst, int len)
{
	MaxEvery_32s  data;
	return OPT_LEVEL::fe<MaxEvery_32s>( data,pSrc, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMaxEvery_32f_I)(const Fw32f* pSrc, Fw32f* pSrcDst, int len)
{
	MaxEvery_32f  data;
	return OPT_LEVEL::fe<MaxEvery_32f>( data,pSrc, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinEvery_16s_I)(const Fw16s* pSrc, Fw16s* pSrcDst, int len)
{ 
	MinEvery_16s  data;
	return OPT_LEVEL::fe<MinEvery_16s>( data,pSrc, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinEvery_32s_I)(const Fw32s* pSrc, Fw32s* pSrcDst, int len)
{
	MinEvery_32s  data;
	return OPT_LEVEL::fe<MinEvery_32s>( data,pSrc, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinEvery_32f_I)(const Fw32f* pSrc, Fw32f* pSrcDst, int len)
{
	MinEvery_32f  data;
	return OPT_LEVEL::fe<MinEvery_32f>( data,pSrc, pSrcDst, len);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
