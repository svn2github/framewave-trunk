/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "ConversionDef.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;

	//
	// Phase functions
	//

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsPhase_64fc)(cA64FC *pSrc, A64F *pDst, int len)
	{
		Phase_gen<cA64FC,A64F> data;
		return OPT_LEVEL::fe<Phase_gen<cA64FC,A64F> >(data,pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsPhase_32fc)(cA32FC *pSrc, A32F *pDst, int len)
	{
		Phase_gen<cA32FC,A32F> data;
		return OPT_LEVEL::fe<Phase_gen<cA32FC,A32F> >(data,pSrc, pDst, len );		
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsPhase_16sc32f)(cA16SC *pSrc, A32F *pDst, int len)
	{
		Phase_gen<cA16SC,A32F> data;
		return OPT_LEVEL::fe<Phase_gen<cA16SC,A32F> >(data,pSrc, pDst, len );	
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsPhase_16sc_Sfs)(cA16SC *pSrc, A16S *pDst, int len, int scaleFactor)
	{
		
		Phase_gen_scale<cA16SC,A16S> data(scaleFactor);
		return OPT_LEVEL::fe<Phase_gen_scale<cA16SC,A16S> >(data,pSrc, pDst, len );	
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsPhase_32sc_Sfs)(cA32SC *pSrc, A32S *pDst, int len, int scaleFactor)
	{
		Phase_gen_scale<cA32SC,A32S> data(scaleFactor);
		return OPT_LEVEL::fe<Phase_gen_scale<cA32SC,A32S> >(data,pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsPhase_64f)(cA64F *pSrcRe, cA64F *pSrcIm, A64F* pDst, int len)
	{
		Phase_gen2<cA64F,cA64F,A64F> data;
		return OPT_LEVEL::fe<Phase_gen2<cA64F,cA64F,A64F> >(data,pSrcRe,pSrcIm, pDst, len );	
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsPhase_32f)(cA32F* pSrcRe, cA32F* pSrcIm, A32F* pDst, int len)
	{
		Phase_gen2<cA32F,cA32F,A32F> data;
		return OPT_LEVEL::fe<Phase_gen2<cA32F,cA32F,A32F> >(data,pSrcRe,pSrcIm, pDst, len );	
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsPhase_16s32f)(cA16S* pSrcRe, cA16S *pSrcIm, A32F* pDst, int len)
	{
		Phase_gen2<cA16S,cA16S,A32F> data;
		return OPT_LEVEL::fe<Phase_gen2<cA16S,cA16S,A32F> >(data,pSrcRe,pSrcIm, pDst, len );	
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsPhase_16s_Sfs)(cA16S* pSrcRe, cA16S* pSrcIm, A16S* pDst, int len, int scaleFactor)
	{

		Phase_gen2_scale<cA16S,cA16S,A16S> data(scaleFactor);
		return OPT_LEVEL::fe<Phase_gen2_scale<cA16S,cA16S,A16S> >(data,pSrcRe,pSrcIm, pDst, len );	
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_8s16s)(const Fw8s* pSrc,Fw16s* pDst, int len)
	{
		Convert_8s16s data;
		return OPT_LEVEL::fe<Convert_8s16s>( data,pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_8u16s)(const Fw8u* pSrc,Fw16s* pDst, int len)
	{
		Convert_8u16s data;
		return OPT_LEVEL::fe<Convert_8u16s>( data,pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_8s32f)(const Fw8s* pSrc, Fw32f* pDst, int len)
	{
		Convert_8s32f data;
		return OPT_LEVEL::fe<Convert_8s32f>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_8u32f)(const Fw8u* pSrc, Fw32f* pDst, int len)
	{
		Convert_8u32f data;
		return OPT_LEVEL::fe<Convert_8u32f>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_16s32s)(const Fw16s* pSrc, Fw32s* pDst, int len)
	{
		Convert_16s32s data;
		return OPT_LEVEL::fe<Convert_16s32s>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_16u32s)(const Fw16u* pSrc, Fw32s* pDst, int len)
	{
		Convert_16u32s data;
		return OPT_LEVEL::fe<Convert_16u32s>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_16s32f)(const Fw16s* pSrc, Fw32f* pDst, int len)
	{
		Convert_16s32f data;
		return OPT_LEVEL::fe<Convert_16s32f>(data, pSrc, pDst, len );
	}



	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_16u32f)(const Fw16u* pSrc, Fw32f* pDst, int len)
	{
		Convert_16u32f data;
		return OPT_LEVEL::fe<Convert_16u32f>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32s16s)(const Fw32s* pSrc, Fw16s* pDst, int len)
	{
		Convert_32s16s data;
		return OPT_LEVEL::fe<Convert_32s16s>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32u16s)(const Fw32u* pSrc, Fw16s* pDst, int len)
	{
		Convert_32u16s data;
		return OPT_LEVEL::fe<Convert_32u16s>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32s32f)(const Fw32s* pSrc, Fw32f* pDst, int len)
	{
		Convert_32s32f data;
		return OPT_LEVEL::fe<Convert_32s32f>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32u32f)(const Fw32u* pSrc, Fw32f* pDst, int len)
	{
		Convert_32u32f data;
		return OPT_LEVEL::fe<Convert_32u32f>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32s64f)(const Fw32s* pSrc, Fw64f* pDst, int len)
	{
		Convert_32s64f data;
		return OPT_LEVEL::fe<Convert_32s64f>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32u64f)(const Fw32u* pSrc, Fw64f* pDst, int len)
	{
		Convert_32u64f data;
		return OPT_LEVEL::fe<Convert_32u64f>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32f64f)(const Fw32f* pSrc, Fw64f* pDst, int len)
	{
		Convert_32f64f data;
		return OPT_LEVEL::fe<Convert_32f64f>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_64f32f)(const Fw64f* pSrc, Fw32f* pDst, int len)
	{
		Convert_64f32f data;
		return OPT_LEVEL::fe<Convert_64f32f>(data, pSrc, pDst, len );
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_16s32f_Sfs)(const Fw16s* pSrc, Fw32f* pDst, int len, int scaleFactor)
	{
		Convert_16s32f_Sfs data(scaleFactor);
		return OPT_LEVEL::fe<Convert_16s32f_Sfs>( data,pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_16u32f_Sfs)(const Fw16u* pSrc, Fw32f* pDst, int len, int scaleFactor)
	{
		Convert_16u32f_Sfs data(scaleFactor);
		return OPT_LEVEL::fe<Convert_16u32f_Sfs>( data,pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_16s64f_Sfs)(const Fw16s* pSrc, Fw64f* pDst, int len, int scaleFactor)
	{
		Convert_16s64f_Sfs data(scaleFactor);
		return OPT_LEVEL::fe<Convert_16s64f_Sfs>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32s16s_Sfs)(const Fw32s* pSrc, Fw16s* pDst, int len, int scaleFactor)
	{

		if(0 == scaleFactor)
		{
			Convert_32s16s_Sfs_noscale data;
			return OPT_LEVEL::fe<Convert_32s16s_Sfs_noscale>(data, pSrc, pDst, len );
		}
		Convert_32s16s_Sfs data(scaleFactor);
		return OPT_LEVEL::fe<Convert_32s16s_Sfs>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32s32f_Sfs)(const Fw32s* pSrc, Fw32f* pDst, int len, int scaleFactor)
	{
		Convert_32s32f_Sfs data(scaleFactor);
		return OPT_LEVEL::fe<Convert_32s32f_Sfs>(data, pSrc, pDst, len );
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32s64f_Sfs)(const Fw32s* pSrc, Fw64f* pDst, int len, int scaleFactor)
	{
		Convert_32s64f_Sfs data(scaleFactor);
		return OPT_LEVEL::fe<Convert_32s64f_Sfs>(data, pSrc, pDst, len );
	}
	// Round mode convert functions
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32f8s_Sfs)(const Fw32f* pSrc, Fw8s* pDst, int len, FwRoundMode rndMode, int scaleFactor)
	{
		if (rndMode == fwRndNear)
		{
			Convert_32f8s_Sfs data(scaleFactor);
			return OPT_LEVEL::fe<Convert_32f8s_Sfs>(data, pSrc, pDst, len );
		}
		else
		{
			Convert_32f8s_Sfs_RndZero data(scaleFactor);
			return OPT_LEVEL::fe<Convert_32f8s_Sfs_RndZero>(data, pSrc, pDst, len );
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32f8u_Sfs)(const Fw32f* pSrc, Fw8u* pDst, int len, FwRoundMode rndMode, int scaleFactor)
	{

		if (rndMode == fwRndNear)
		{
			Convert_32f8u_Sfs data(scaleFactor);
			return OPT_LEVEL::fe<Convert_32f8u_Sfs>(data, pSrc, pDst, len );
		}
		else
		{
			Convert_32f8u_Sfs_RndZero data(scaleFactor);
			return OPT_LEVEL::fe<Convert_32f8u_Sfs_RndZero>(data, pSrc, pDst, len );
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32f16s_Sfs)(const Fw32f* pSrc, Fw16s* pDst, int len, FwRoundMode rndMode, int scaleFactor)
	{
		if (rndMode == fwRndNear)
		{
			Convert_32f16s_Sfs data(scaleFactor);
			return OPT_LEVEL::fe<Convert_32f16s_Sfs>(data, pSrc, pDst, len );
		}
		else
		{
			Convert_32f16s_Sfs_RndZero data(scaleFactor);
			return OPT_LEVEL::fe<Convert_32f16s_Sfs_RndZero>(data, pSrc, pDst, len );
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32f16u_Sfs)(const Fw32f* pSrc, Fw16u* pDst, int len, FwRoundMode rndMode, int scaleFactor)
	{
		if (rndMode == fwRndNear)
		{
			Convert_32f16u_Sfs data(scaleFactor);
			return OPT_LEVEL::fe<Convert_32f16u_Sfs>( data,pSrc, pDst, len );
		}
		else
		{
			Convert_32f16u_Sfs_RndZero data(scaleFactor);
			return OPT_LEVEL::fe<Convert_32f16u_Sfs_RndZero>(data, pSrc, pDst, len );
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_32f32s_Sfs)(const Fw32f* pSrc, Fw32s* pDst, int len, FwRoundMode rndMode, int scaleFactor)
	{
		if (rndMode == fwRndNear)
		{
			Convert_32f32s_Sfs data(scaleFactor);
			return OPT_LEVEL::fe<Convert_32f32s_Sfs>(data, pSrc, pDst, len );
		}
		else
		{
			Convert_32f32s_Sfs_RndZero data(scaleFactor);
			return OPT_LEVEL::fe<Convert_32f32s_Sfs_RndZero>(data, pSrc, pDst, len );
		}
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_64s32s_Sfs)(const Fw64s* pSrc, Fw32s* pDst, int len, FwRoundMode rndMode, int scaleFactor)
	{

		if( FW_REF::PtrNotOK ( pSrc,pDst ) ) return fwStsNullPtrErr;	
		if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;

		Fw64f scale = pow(2.0,-scaleFactor);
		Fw64f scaleVal;
		if(rndMode == fwRndNear)
		{
			Convert_64s32s_Sfs data(scaleFactor);
			return OPT_LEVEL::fe<Convert_64s32s_Sfs>(data, pSrc, pDst, len );
		}
		else
		{
				int i=0;
				int count = (len - (len %4));
				while( i < count )
				{	
                    scaleVal = (double)pSrc[i]*scale;
					pDst[i] = FW_REF::Limits<S32>::Sat(scaleVal);
					i++;
					scaleVal = (double)pSrc[i]*scale;
					pDst[i] = FW_REF::Limits<S32>::Sat(scaleVal);
					i++;
					scaleVal = (double)pSrc[i]*scale;
					pDst[i] = FW_REF::Limits<S32>::Sat(scaleVal);
					i++;
					scaleVal = (double)pSrc[i]*scale;
					pDst[i] = FW_REF::Limits<S32>::Sat(scaleVal);
					i++;
				}

				switch(len%4)
				{
					case 3:	scaleVal = (double)pSrc[i]*scale;
							pDst[i] = FW_REF::Limits<S32>::Sat(scaleVal);
							i++;
					case 2:	scaleVal = (double)pSrc[i]*scale;
							pDst[i] = FW_REF::Limits<S32>::Sat(scaleVal);
							i++;
					case 1:	scaleVal = (double)pSrc[i]*scale;
							pDst[i] = FW_REF::Limits<S32>::Sat(scaleVal);
				}
			
				return fwStsNoErr;

		}

		
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsConvert_64f32s_Sfs)(const Fw64f* pSrc, Fw32s* pDst, int len, FwRoundMode rndMode, int scaleFactor)
	{
		if (rndMode == fwRndNear)
		{
			Convert_64f32s_Sfs data(scaleFactor);
			return OPT_LEVEL::fe<Convert_64f32s_Sfs>(data, pSrc, pDst, len );
		}
		else
		{
			Convert_64f32s_Sfs_RndZero data(scaleFactor);
			return OPT_LEVEL::fe<Convert_64f32s_Sfs_RndZero>(data, pSrc, pDst, len );
		}
	}



	// Copy functions
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsCopy_8u)(const Fw8u* pSrc, Fw8u* pDst, int len)
	{

		Copy2<const Fw8u,Fw8u> data;
		return OPT_LEVEL::fe<Copy2<const Fw8u,Fw8u> >(data,pSrc,pDst, len);		
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsCopy_16s)(const Fw16s* pSrc, Fw16s* pDst, int len)
	{
		Copy2<const Fw16s,Fw16s> data;
		return OPT_LEVEL::fe<Copy2<const Fw16s,Fw16s> >(data,pSrc,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsCopy_16sc)(const Fw16sc* pSrc, Fw16sc* pDst, int len)
	{
		Copy2<const Fw16sc,Fw16sc> data;
		return OPT_LEVEL::fe<Copy2<const Fw16sc,Fw16sc> >(data,pSrc,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsCopy_32f)(const Fw32f* pSrc, Fw32f* pDst, int len)
	{
		Copy2<const Fw32f,Fw32f> data;
		return OPT_LEVEL::fe<Copy2<const Fw32f,Fw32f> >(data,pSrc,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsCopy_32fc)(const Fw32fc* pSrc, Fw32fc* pDst, int len)
	{
		Copy2<const Fw32fc,Fw32fc> data;
		return OPT_LEVEL::fe<Copy2<const Fw32fc,Fw32fc> >(data,pSrc,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsCopy_64f)(const Fw64f* pSrc, Fw64f* pDst, int len)
	{
		Copy2<const Fw64f,Fw64f> data;
		return OPT_LEVEL::fe<Copy2<const Fw64f,Fw64f> >(data,pSrc,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsCopy_64fc)(const Fw64fc* pSrc, Fw64fc* pDst, int len)
	{
		Copy2<const Fw64fc,Fw64fc> data;
		return OPT_LEVEL::fe<Copy2<const Fw64fc,Fw64fc> >(data,pSrc,pDst, len);
	}




	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMove_8u)(const Fw8u* pSrc, Fw8u* pDst, int len)
	{
		Copy2<const Fw8u,Fw8u> data;
		return OPT_LEVEL::fe<Copy2<const Fw8u,Fw8u> >(data,pSrc,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMove_16s)(const Fw16s* pSrc, Fw16s* pDst, int len)
	{
		Copy2<const Fw16s,Fw16s> data;
		return OPT_LEVEL::fe<Copy2<const Fw16s,Fw16s> >(data,pSrc,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMove_16sc)(const Fw16sc* pSrc, Fw16sc* pDst, int len)
	{
		Copy2<const Fw16sc,Fw16sc> data;
		return OPT_LEVEL::fe<Copy2<const Fw16sc,Fw16sc> >(data,pSrc,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMove_32f)(const Fw32f* pSrc, Fw32f* pDst, int len)
	{
		Copy2<const Fw32f,Fw32f> data;
		return OPT_LEVEL::fe<Copy2<const Fw32f,Fw32f> >(data,pSrc,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMove_32fc)(const Fw32fc* pSrc, Fw32fc* pDst, int len)
	{
		Copy2<const Fw32fc,Fw32fc> data;
		return OPT_LEVEL::fe<Copy2<const Fw32fc,Fw32fc> >(data,pSrc,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMove_64f)(const Fw64f* pSrc, Fw64f* pDst, int len)
	{
		Copy2<const Fw64f,Fw64f> data;
		return OPT_LEVEL::fe<Copy2<const Fw64f,Fw64f> >(data,pSrc,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMove_64fc)(const Fw64fc* pSrc, Fw64fc* pDst, int len)
	{
		Copy2<const Fw64fc,Fw64fc> data;
		return OPT_LEVEL::fe<Copy2<const Fw64fc,Fw64fc> >(data,pSrc,pDst, len);
	}


	// Set
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsSet_8u)(Fw8u val, Fw8u* pDst, int len)
	{
	
		Set1<Fw8u> data(val);
		return OPT_LEVEL::fe<Set1<Fw8u> >(data,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsSet_16s)(Fw16s val, Fw16s* pDst, int len)
	{
		Set1<Fw16s> data(val);
		return OPT_LEVEL::fe<Set1<Fw16s> >(data,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsSet_16sc)(Fw16sc val, Fw16sc* pDst, int len)
	{
		Set1<Fw16sc> data(val);
		return OPT_LEVEL::fe<Set1<Fw16sc> >(data,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsSet_32s)(Fw32s val, Fw32s* pDst, int len)
	{
		Set1<Fw32s> data(val);
		return OPT_LEVEL::fe<Set1<Fw32s> >(data,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsSet_32f)(Fw32f val, Fw32f* pDst, int len)
	{
		Set1<Fw32f> data(val);
		return OPT_LEVEL::fe<Set1<Fw32f> >(data,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsSet_32sc)(Fw32sc val, Fw32sc* pDst, int len)
	{
		Set1<Fw32sc> data(val);
		return OPT_LEVEL::fe<Set1<Fw32sc> >(data,pDst, len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsSet_32fc)(Fw32fc val, Fw32fc* pDst, int len)
	{
		Set1<Fw32fc> data(val);
		return OPT_LEVEL::fe<Set1<Fw32fc> >(data,pDst, len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsSet_64s)(Fw64s val, Fw64s* pDst, int len)
	{
		Set1<Fw64s> data(val);
		return OPT_LEVEL::fe<Set1<Fw64s> >(data,pDst, len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsSet_64f)(Fw64f val, Fw64f* pDst, int len)
	{
		Set1<Fw64f> data(val);
		return OPT_LEVEL::fe<Set1<Fw64f> >(data,pDst, len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsSet_64sc)(Fw64sc val, Fw64sc* pDst, int len)
	{
		Set1<Fw64sc> data(val);
		return OPT_LEVEL::fe<Set1<Fw64sc> >(data,pDst, len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsSet_64fc)(Fw64fc val, Fw64fc* pDst, int len)
	{
		Set1<Fw64fc> data(val);
		return OPT_LEVEL::fe<Set1<Fw64fc> >(data,pDst, len);
	}



	// Zero 
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsZero_8u)(Fw8u* pDst, int len)
	{
		Zero1<Fw8u> data;
		return OPT_LEVEL::fe<Zero1<Fw8u> >(data, pDst, len ); 
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsZero_16s)(Fw16s* pDst, int len)
	{
		Zero1<Fw16s> data;
		return OPT_LEVEL::fe<Zero1<Fw16s> >(data, pDst, len ); 
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsZero_16sc)(Fw16sc* pDst, int len)
	{
		Zero1<Fw16sc> data;
		return OPT_LEVEL::fe<Zero1<Fw16sc> >(data, pDst, len ); 
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsZero_32f)(Fw32f* pDst, int len)
	{
		Zero1<Fw32f> data;
		return OPT_LEVEL::fe<Zero1<Fw32f> >(data, pDst, len ); 
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsZero_32fc)(Fw32fc* pDst, int len)
	{
		Zero1<Fw32fc> data;
		return OPT_LEVEL::fe<Zero1<Fw32fc> >(data, pDst, len ); 
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsZero_64f)(Fw64f* pDst, int len)
	{
		Zero1<Fw64f> data;
		return OPT_LEVEL::fe<Zero1<Fw64f> >(data, pDst, len ); 
	}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
