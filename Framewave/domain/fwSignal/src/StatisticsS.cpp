/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "StatisticsS.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;




FwStatus PREFIX_OPT(OPT_PREFIX, fwsMean_32f)(const Fw32f* pSrc, int len, Fw32f* pMean,FwHintAlgorithm hint)
{

	if( FW_REF::PtrNotOK ( pSrc,pMean ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;
	Fw64f sum = 0;
	int i;
	hint;
	for( i = 0;i < len;i++)
	{	
		sum = sum + pSrc[i];
	}
	*pMean = FW_REF::Limits<F32>::Sat(sum/len);
	return fwStsNoErr;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMean_32fc)(const Fw32fc* pSrc, int len, Fw32fc* pMean,FwHintAlgorithm hint)
{
	if( FW_REF::PtrNotOK ( pSrc,pMean ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;
	Fw64fc sum = {0,0};
	int i;
	hint;
	for( i = 0;i < len;i++)
	{	
		sum.re = sum.re + pSrc[i].re;
		sum.im = sum.im + pSrc[i].im;
	}

	pMean->re = FW_REF::Limits<F32>::Sat(sum.re/len);
	pMean->im = FW_REF::Limits<F32>::Sat(sum.im/len);
	return fwStsNoErr;
}




FwStatus PREFIX_OPT(OPT_PREFIX, fwsMean_64f)(const Fw64f* pSrc, int len, Fw64f* pMean)
{
	FwStatus retStatus;
	StatSum_64f data;
	retStatus = OPT_LEVEL::fex<StatSum_64f>(data,pSrc,len,THREAD_SW_OFF);
	*pMean = data.sum/len;
	return retStatus;
}



FwStatus PREFIX_OPT(OPT_PREFIX, fwsMean_64fc)(const Fw64fc* pSrc, int len, Fw64fc* pMean)
{
	FwStatus retStatus;
	StatSum_64fc data;
	retStatus = OPT_LEVEL::fex<StatSum_64fc>(data,pSrc,len,THREAD_SW_OFF);
	pMean->re = data.sum.re/len;
	pMean->im = data.sum.im/len;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMean_16s_Sfs)(const Fw16s* pSrc, int len, Fw16s* pMean, int scaleFactor)
{

	FwStatus retStatus = fwStsNoErr;
	Fw64f mean_global = 0;
	int loop_size = FW_REF::Limits<U8>::MaxValue() * FW_REF::Limits<U8>::MaxValue();
	int repeat = len / loop_size;
	int residue = len % loop_size;
	int i;

	const Fw16s* pSrcTemp = pSrc;
	for( i = 0; i < repeat ; i++)
	{
		StatMean_16s_Sfs data;
		retStatus = OPT_LEVEL::fex<StatMean_16s_Sfs >(data,pSrcTemp,loop_size,THREAD_SW_OFF);
		mean_global = mean_global + data.sum;
		pSrcTemp = pSrcTemp + loop_size;
	}
	if (residue > 0)
	{
		StatMean_16s_Sfs data;
		retStatus = OPT_LEVEL::fex<StatMean_16s_Sfs >(data,pSrcTemp,residue,THREAD_SW_OFF);
		mean_global = mean_global + data.sum;
	}
	*pMean = FW_REF::Limits<S16>::Sat(CBL_LIBRARY::DoubleToLong(FW_REF::Scale(mean_global/len, scaleFactor)));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMean_16sc_Sfs)(const Fw16sc* pSrc, int len, Fw16sc* pMean, int scaleFactor)
{
	FwStatus retStatus = fwStsNoErr;
	Fw64fc mean_global = {0,0};
	int loop_size = FW_REF::Limits<U8>::MaxValue() * FW_REF::Limits<U8>::MaxValue();
	int repeat = len / loop_size;
	int residue = len % loop_size;
	int i;

	const Fw16sc* pSrcTemp = pSrc;
	for( i = 0; i < repeat ; i++)
	{
		StatMean_16sc_Sfs data;
		retStatus = OPT_LEVEL::fex<StatMean_16sc_Sfs >(data,pSrcTemp,loop_size,THREAD_SW_OFF);
		mean_global.im = mean_global.im + data.sum.im;
		mean_global.re = mean_global.re + data.sum.re;
		pSrcTemp = pSrcTemp + loop_size;
	}
	if (residue > 0)
	{
		StatMean_16sc_Sfs data;
		retStatus = OPT_LEVEL::fex<StatMean_16sc_Sfs >(data,pSrcTemp,residue,THREAD_SW_OFF);
		mean_global.im = mean_global.im + data.sum.im;
		mean_global.re = mean_global.re + data.sum.re;
	}

	pMean->re = FW_REF::Limits<S16>::Sat(CBL_LIBRARY::DoubleToLong(FW_REF::Scale(mean_global.re/len, scaleFactor)));
	pMean->im = FW_REF::Limits<S16>::Sat(CBL_LIBRARY::DoubleToLong(FW_REF::Scale(mean_global.im/len, scaleFactor)));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSum_32f)(const Fw32f* pSrc, int len, Fw32f* pSum, FwHintAlgorithm hint)
{
	hint;
	FwStatus retStatus = fwStsNoErr;
	StatSum_32f data;
	retStatus = OPT_LEVEL::fex<StatSum_32f>(data,pSrc,len,THREAD_SW_OFF);
	*pSum = FW_REF::Limits<F32>::Sat(data.sum);

	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSum_32fc)(const Fw32fc* pSrc, int len, Fw32fc* pSum, FwHintAlgorithm hint)
{
	hint;
	FwStatus retStatus;
	StatSum_32fc data;
	retStatus = OPT_LEVEL::fex<StatSum_32fc>(data,pSrc,len,THREAD_SW_OFF);
	pSum->re = FW_REF::Limits<F32>::Sat(data.sum.re);
	pSum->im = FW_REF::Limits<F32>::Sat(data.sum.im);
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSum_64f)(const Fw64f* pSrc, int len, Fw64f* pSum)
{
	FwStatus retStatus;
	StatSum_64f data;
	retStatus = OPT_LEVEL::fex<StatSum_64f>(data,pSrc,len,THREAD_SW_OFF);
	*pSum = data.sum;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSum_64fc)(const Fw64fc* pSrc, int len, Fw64fc* pSum)
{
	FwStatus retStatus;
	StatSum_64fc data;
	retStatus = OPT_LEVEL::fex<StatSum_64fc>(data,pSrc,len,THREAD_SW_OFF);
	*pSum = data.sum;
	return retStatus;

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSum_16s_Sfs)(const Fw16s* pSrc, int len, Fw16s* pSum, int scaleFactor)
{
	FwStatus retStatus;
	StatSum_16s_Sfs data;
	retStatus = OPT_LEVEL::fex<StatSum_16s_Sfs>(data,pSrc,len,THREAD_SW_OFF);
	*pSum =  FW_REF::Limits<S16>::Sat((FW_REF::Scale(data.sum,scaleFactor)));
	return retStatus;

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSum_32s_Sfs)(const Fw32s* pSrc, int len, Fw32s* pSum, int scaleFactor)
{
	FwStatus retStatus;
	StatSum_32s_Sfs data;
	retStatus = OPT_LEVEL::fex<StatSum_32s_Sfs>(data,pSrc,len,THREAD_SW_OFF);
	*pSum =  FW_REF::Limits<S32>::Sat(CBL_LIBRARY::DoubleToLong(FW_REF::Scale(data.sum,scaleFactor)));
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSum_16s32s_Sfs)(const Fw16s* pSrc, int len, Fw32s* pSum, int scaleFactor)
{
	FwStatus retStatus;
	StatSum_16s32s_Sfs data;
	retStatus = OPT_LEVEL::fex<StatSum_16s32s_Sfs>(data,pSrc,len,THREAD_SW_OFF);
	*pSum =  FW_REF::Limits<S32>::Sat(CBL_LIBRARY::DoubleToLong(FW_REF::Scale(data.sum,scaleFactor)));
	return retStatus;

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSum_16sc_Sfs)(const Fw16sc* pSrc, int len, Fw16sc* pSum, int scaleFactor)
{
	FwStatus retStatus;
	StatSum_16sc_Sfs data;
	retStatus = OPT_LEVEL::fex<StatSum_16sc_Sfs>(data,pSrc,len,THREAD_SW_OFF);
	pSum->re =  FW_REF::Limits<S16>::Sat((FW_REF::Scale(data.sum.re,scaleFactor)));
	pSum->im =  FW_REF::Limits<S16>::Sat((FW_REF::Scale(data.sum.im,scaleFactor)));
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSum_16sc32sc_Sfs)(const Fw16sc* pSrc, int len, Fw32sc* pSum, int scaleFactor)
{
	if( FW_REF::PtrNotOK ( pSrc,pSum ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;
	int repeat = len - len%4;
	Fw64sc sum = {0,0};

	int i;
	for( i = 0;i < repeat;i++)
	{	

		sum.im = sum.im + pSrc[i].im;
		sum.re = sum.re + pSrc[i].re;
		i++;
		sum.im = sum.im + pSrc[i].im;
		sum.re = sum.re + pSrc[i].re;
		i++;
		sum.im = sum.im + pSrc[i].im;
		sum.re = sum.re + pSrc[i].re;
		i++;
		sum.im = sum.im + pSrc[i].im;
		sum.re = sum.re + pSrc[i].re;
		
	}

	switch(len%4)
	{
	case 3:		sum.im = sum.im + pSrc[i].im;
				sum.re = sum.re + pSrc[i].re;
				i++;

	case 2:		sum.im = sum.im + pSrc[i].im;
				sum.re = sum.re + pSrc[i].re;
				i++;

	case 1:		sum.im = sum.im + pSrc[i].im;
				sum.re = sum.re + pSrc[i].re;
	}
	
	pSum->re =  FW_REF::Limits<S32>::Sat((FW_REF::Scale(sum.re,scaleFactor)));
	pSum->im =  FW_REF::Limits<S32>::Sat((FW_REF::Scale(sum.im,scaleFactor)));
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsStdDev_32f)(const Fw32f* pSrc, int len, Fw32f* pStdDev,FwHintAlgorithm hint)
{
	if( FW_REF::PtrNotOK ( pSrc,pStdDev ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;
	if(len <= 1 ) return fwStsSizeErr;

    Fw64f mean;
    FwStatus retStatus = fwStsNoErr;
	hint;

    StatSum_32f data;
	retStatus = OPT_LEVEL::fex<StatSum_32f>(data,pSrc,len,THREAD_SW_OFF);
    mean = (data.sum/len);

    Fw64f sum = 0;
    int rem = len %4,i;
    int len2 = len - rem;
    Fw64f imRes;
	for( i = 0;i < len2;i++)
	{	
		imRes = pSrc[i] - mean;
		imRes = imRes * imRes;
		sum = sum + imRes;
        i++;
		imRes = pSrc[i] - mean;
		imRes = imRes * imRes;
		sum = sum + imRes;
        i++;
		imRes = pSrc[i] - mean;
		imRes = imRes * imRes;
		sum = sum + imRes;
        i++;
		imRes = pSrc[i] - mean;
		imRes = imRes * imRes;
		sum = sum + imRes;
	}

    switch(rem)
    {
        case 3: imRes = pSrc[i] - mean;
                imRes = imRes * imRes;
                sum = sum + imRes;
                i++;

        case 2: imRes = pSrc[i] - mean;
                imRes = imRes * imRes;
                sum = sum + imRes;
                i++;
        case 1: imRes = pSrc[i] - mean;
                imRes = imRes * imRes;
                sum = sum + imRes;
    }



	*pStdDev = FW_REF::Limits<F32>::Sat(sqrt(sum/(len-1)));

   /*  StatStdDev_32f data1(mean);

   retStatus = OPT_LEVEL::fex<StatStdDev_32f>(data1,pSrc,len,THREAD_SW_OFF);
	*pStdDev = FW_REF::Limits<F32>::Sat(sqrt(data1.sum/(len-1)));
*/
	return fwStsNoErr;

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsStdDev_64f)(const Fw64f* pSrc, int len, Fw64f* pStdDev)
{

	if( FW_REF::PtrNotOK ( pSrc,pStdDev ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;
	if(len <= 1 ) return fwStsSizeErr;

	Fw64f mean;

    FwStatus retStatus = fwStsNoErr;
    StatSum_64f data;
	retStatus = OPT_LEVEL::fex<StatSum_64f>(data,pSrc,len,THREAD_SW_OFF);
    mean =   (data.sum/len);

    StatStdDev_64f data1(mean);

    retStatus = OPT_LEVEL::fex<StatStdDev_64f>(data1,pSrc,len,THREAD_SW_OFF);

	*pStdDev = sqrt(data1.sum/(len-1));

	return retStatus;
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsStdDev_16s32s_Sfs)(const Fw16s* pSrc, int len, Fw32s* pStdDev,int scaleFactor)
{
	if( FW_REF::PtrNotOK ( pSrc,pStdDev ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;
	if(len <= 1 ) return fwStsSizeErr;

    FwStatus retStatus = fwStsNoErr;
	Fw32s  mean;

    StatStdDevMean_16s data;
    retStatus = OPT_LEVEL::fex<StatStdDevMean_16s>(data,pSrc,len,THREAD_SW_OFF);
    
    mean = CBL_LIBRARY::FloatToInt(data.sum/len);

    StatStdDev_16s data1(mean);

    retStatus = OPT_LEVEL::fex<StatStdDev_16s>(data1,pSrc,len,THREAD_SW_OFF);


	*pStdDev = FW_REF::Limits<S16>::Sat(CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt(data1.sum/(len-1)),scaleFactor)));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsStdDev_16s_Sfs)(const Fw16s* pSrc, int len, Fw16s* pStdDev, int scaleFactor)
{
	if( FW_REF::PtrNotOK ( pSrc,pStdDev ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;
	if(len <= 1 ) return fwStsSizeErr;
    Fw32s mean_global = 0;
	FwStatus retStatus = fwStsNoErr;
    StatStdDevMean_16s data;
    retStatus = OPT_LEVEL::fex<StatStdDevMean_16s>(data,pSrc,len,THREAD_SW_OFF);
   
    mean_global = CBL_LIBRARY::FloatToInt(data.sum/len);

    StatStdDev_16s data1(mean_global);

    retStatus = OPT_LEVEL::fex<StatStdDev_16s>(data1,pSrc,len,THREAD_SW_OFF);


	*pStdDev = FW_REF::Limits<S16>::Sat(CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt(data1.sum/(len-1)),scaleFactor)));
	return retStatus;
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
