/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#define THREAD_SW_OFF 536870910		//switch off thread 
#define SIXTEEN_BYTE	128
#define ONE_BYTE		8


namespace OPT_LEVEL
{

#define PIX_MINMAX void (*pix_minmax )(const TDST &,const TDST &,TDST &)

template<class TDST>
SYS_INLINE STATIC void Min_inline(const TDST &param1,const TDST &param2,TDST &dest)
{
	dest =  MIN(param1,param2);
}
template<class TDST>
SYS_INLINE STATIC void Max_inline(const TDST &param1,const TDST &param2,TDST &dest)
{
	dest =  MAX(param1,param2);
}

template<class TDST,int size>
SYS_INLINE	STATIC void SumTemplate_cleanup(FnDat &dat)
{
	TDST sum_inter,*sum1,sum_inter1,sum_inter2;
	TDST *sum  = (TDST*)dat.data[0];
	sum_inter = sum[0];
	sum1 = (TDST*)&dat.xmmData[0];
	sum_inter1 = sum1[0];
	for(int i = 1; i < size ; i++)
	{
		sum_inter1 += sum1[i];
	}

	for(int thread_count =1; thread_count < dat.numThreads; thread_count++)
	{
		sum_inter += sum[thread_count];

		sum1 = (TDST*)&dat.xmmData[thread_count];
		sum_inter2 = sum1[0];
		for(int i = 1; i < size; i++)
		{
			sum_inter2 += sum1[i];
		}
		
		sum_inter1 +=sum_inter2;
	}
	
	sum[0] = sum_inter1 + sum_inter;
}

template<class TDST,int size>
SYS_INLINE	STATIC void SumTemplateC_cleanup(FnDat &dat)
{
	TDST sum_inter,*sum1,sum_inter1,sum_inter2;
	TDST *sum  = (TDST*)dat.data[0];
	sum_inter = sum[0];
	sum1 = (TDST*)&dat.xmmData[0];
	sum_inter1.re = sum1[0].re;
	sum_inter1.im = sum1[0].im;
	for(int i = 1; i < size ; i++)
	{
		sum_inter1 += sum1[i];
	}

	for(int thread_count =1; thread_count < dat.numThreads; thread_count++)
	{
		sum_inter += sum[thread_count];

		sum1 = (TDST*)&dat.xmmData[thread_count];
		sum_inter2.re = sum1[0].re;
		sum_inter2.im = sum1[0].im;
		for(int i = 1; i < size; i++)
		{
			sum_inter2 += sum1[i];
		}
		
		sum_inter1 +=sum_inter2;
	}
	
	sum_inter1 = sum_inter1 + sum_inter;
	sum[0].re = sum_inter1.re;
	sum[0].im = sum_inter1.im;
}


template<class TD,class TDST,class TM, int regCount,PIX_FN1,PIX_SSE,PIX_SSE_SETUP,PIX_SSE_CLEANUP>
SYS_INLINE STATIC FwStatus StdDevTemplate(const TD *pSrc, TDST *pSum,int len, TM &mean,unsigned int minBytesLen=256*256)
{
	FwStatus rest_status;
	TDST sum[XMM_MAX_ITEMS];

	FnDat dat(&sum,&mean);  

	sum[0]= 0;
	
	rest_status =  foreach_1D< TD, C1,regCount,pix_ref,pix_sse,pix_sse_setup,pix_sse_cleanup>(const_cast<TD*>(pSrc), len,dat,minBytesLen ); 
	*pSum = sum[0];

	return rest_status;
}

// Two source type for complex number
template<class TS,class TD,class TDST,int regCount,PIX_FN2,PIX_SSE,PIX_SSE_SETUP,PIX_SSE_CLEANUP>
SYS_INLINE STATIC FwStatus SumTemplateC(const TS *pSrc1,const TD *pSrc2, TDST *pSum,int len,unsigned int minBytesLen=256*256)
{
	FwStatus rest_status;
	TDST sum[XMM_MAX_ITEMS];

	FnDat dat(&sum);  

	sum[0].re = 0;
	sum[0].im = 0;	

	rest_status =  foreach_1D< TS,C1,regCount,TD, C1,pix_ref,pix_sse,pix_sse_setup,pix_sse_cleanup>(const_cast<TS*>(pSrc1),const_cast<TD*>(pSrc2), len,dat,minBytesLen ); 
	pSum->re = sum[0].re;
	pSum->im = sum[0].im;
	return rest_status;
}

//complex data type..
template<class TD,class TDST,int regCount,PIX_FN1,PIX_SSE,PIX_SSE_SETUP,PIX_SSE_CLEANUP>
SYS_INLINE STATIC FwStatus SumTemplateC(const TD *pSrc, TDST *pSum,int len,unsigned int minBytesLen=256*256)
{
	FwStatus rest_status;
	TDST sum[XMM_MAX_ITEMS];

	FnDat dat(&sum);  

	sum[0].re= 0;
	sum[0].im= 0;

	rest_status =  foreach_1D< TD, C1,regCount,pix_ref,pix_sse,pix_sse_setup,pix_sse_cleanup>(const_cast<TD*>(pSrc), len,dat,minBytesLen ); 
	pSum->re = sum[0].re;
	pSum->im = sum[0].im;
	return rest_status;
}




// Two source type.
template<class TS,class TD,class TDST,int regCount,PIX_FN2,PIX_SSE,PIX_SSE_SETUP,PIX_SSE_CLEANUP>
SYS_INLINE STATIC FwStatus SumTemplate(const TS *pSrc1,const TD *pSrc2, TDST *pSum,int len,unsigned int minBytesLen=256*256)
{
	FwStatus rest_status;
	TDST sum[XMM_MAX_ITEMS];
	FnDat dat(&sum);  
	sum[0]= 0;
	rest_status =  foreach_1D< TS,C1,regCount,TD, C1,pix_ref,pix_sse,pix_sse_setup,pix_sse_cleanup>(const_cast<TS*>(pSrc1),const_cast<TD*>(pSrc2), len,dat,minBytesLen ); 
	*pSum = sum[0];
	return rest_status;
}

template<class TD,class TDST,int regCount,PIX_FN1,PIX_SSE,PIX_SSE_SETUP,PIX_SSE_CLEANUP>
SYS_INLINE STATIC FwStatus SumTemplate(const TD *pSrc, TDST *pSum,int len,unsigned int minBytesLen=256*256)
{
	FwStatus rest_status;
	TDST sum[XMM_MAX_ITEMS];

	FnDat dat(&sum);  

	sum[0]= 0;
	
	rest_status =  foreach_1D< TD, C1,regCount,pix_ref,pix_sse,pix_sse_setup,pix_sse_cleanup>(const_cast<TD*>(pSrc), len,dat,minBytesLen ); 
	*pSum = sum[0];
	return rest_status;
}







//Two source type.

template<class TS,class TD,class TDST,int regCount,PIX_FN2,PIX_SSE,PIX_SSE_SETUP,PIX_SSE_CLEANUP>
SYS_INLINE STATIC FwStatus MinOrMax(TDST minmaxval,const TS *pSrc1,const TD *pSrc2, TDST *pMinMax,int len,unsigned int minBytesLen=256*256)
{
	FwStatus rest_status;
	TDST minmax[XMM_MAX_ITEMS];

	if( FW_REF::PtrNotOK ( pMinMax) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	FnDat dat(&minmax);  
	minmax[0]= minmaxval;

	
	rest_status =  foreach_1D< TS,C1,regCount,TD,C1,pix_ref,pix_sse,pix_sse_setup,pix_sse_cleanup>(const_cast<TS*>(pSrc1),const_cast<TD*>(pSrc2), len,dat,minBytesLen ); 
	*pMinMax = minmax[0];
	return rest_status;
}

template<class TD,class TDST,int regCount,PIX_FN1,PIX_SSE,PIX_SSE_SETUP,PIX_SSE_CLEANUP>
SYS_INLINE STATIC FwStatus MinOrMax(TDST minmaxval,const TD *pSrc, TDST *pMinMax,int len,unsigned int minBytesLen=256*256)
{
	FwStatus rest_status;
	TDST minmax[XMM_MAX_ITEMS];

	if( FW_REF::PtrNotOK ( pMinMax) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	FnDat dat(&minmax);  
	minmax[0]= minmaxval;
	
	rest_status =  foreach_1D< TD,C1,regCount,pix_ref,pix_sse,pix_sse_setup,pix_sse_cleanup>(const_cast<TD*>(pSrc), len,dat,minBytesLen ); 

	*pMinMax = minmax[0];
	return rest_status;
}

template<class TDST,PIX_MINMAX> 
SYS_INLINE	STATIC void MinOrMax_cleanup(FnDat &dat)
{
	TDST *minmax  = (TDST*)dat.data[0];
	TDST *minmax1,minmax_inter,minmax_inter1,minmax_inter2;
	minmax_inter = minmax[0];
	minmax1 = (TDST*)&dat.xmmData[0];
	minmax_inter1 = minmax1[0];
	for(int i = 1; i < (128/(sizeof(TDST)*8)); i++)
	{
		pix_minmax(minmax_inter1,minmax1[i],minmax_inter1);
	}

	for(int thread_count =1; thread_count < dat.numThreads; thread_count++)
	{
		pix_minmax(minmax_inter,minmax[thread_count],minmax_inter);
		minmax1 = (TDST*)&dat.xmmData[thread_count];
		minmax_inter2 = minmax1[0];
		for(int i = 1; i < (128/(sizeof(TDST)*8)); i++)
		{
			pix_minmax(minmax_inter2,minmax1[i],minmax_inter2);
		}
		pix_minmax(minmax_inter1,minmax_inter2,minmax_inter1);
	}
	
	pix_minmax(minmax_inter,minmax_inter1,minmax[0]);
}

SYS_INLINE	STATIC void Max_16s_cleanup(FnDat &dat)
{
	XMM128 maxreg;
	Fw16s *minmax  = (Fw16s*)dat.data[0];
	Fw16s *minmax1,minmax_inter,minmax_inter1;
	minmax_inter = minmax[0];
	maxreg.i = dat.xmmData[0].i;
	for(int thread_count =1; thread_count < dat.numThreads; thread_count++)
	{
		minmax_inter = MAX(minmax_inter,minmax[thread_count]);
		maxreg.i = _mm_max_epi16(dat.xmmData[thread_count].i,maxreg.i);
	}

	minmax1 = (Fw16s*)&maxreg;
	minmax_inter1 = minmax1[0];
	for(int i = 1; i < (128/(sizeof(Fw16s)*8)); i++)
	{
		minmax_inter1 = MAX(minmax_inter1,minmax1[i]);
	}

	minmax[0] = MAX(minmax_inter,minmax_inter1);
}



template<class TD,class TDST,int size,int regCount,PIX_MINMAX,PIX_FN1,PIX_SSE,PIX_SSE_SETUP,PIX_SSE_CLEANUP>
SYS_INLINE STATIC FwStatus MinOrMaxIndex(TDST minmaxval,const TD *pSrc, TDST *pMinMax,int *pIndex,int len,unsigned int minBytesLen=256*256)
{
	FwStatus rest_status;
	TDST minmax[XMM_MAX_ITEMS];
	int minmaxindex[XMM_MAX_ITEMS],index[XMM_MAX_ITEMS];
	FnDat dat(&minmax,&minmaxindex,&index);  
	
	minmax[0] =  minmaxval; // reference to compare.
	index[0] = 0;
	minmaxindex[0] = 0;

	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	rest_status =  foreach_1D< TD, C1,regCount,pix_ref,pix_sse,pix_sse_setup,pix_sse_cleanup >(const_cast<TD*>(pSrc), len,dat,minBytesLen ); 
	*pMinMax = minmax[0];
	*pIndex = minmaxindex[0];
	return rest_status;
}


template<class TDST,int size>
SYS_INLINE	STATIC void MinMax_cleanup(FnDat &dat)
{
	TDST *min1,*max1,min_inter,min_inter1,min_inter2,max_inter,max_inter1,max_inter2;	
	TDST *min  = (TDST*)dat.data[0];
	TDST *max  = (TDST*)dat.data[1];
	min_inter = min[0];
	max_inter = max[0];
	min1 = (TDST*)dat.xmmData;
	max1 = (TDST*)dat.xmmData2;
	min_inter1 = min1[0];
	max_inter1 = max1[0];
	for(int i = 1; i < size ; i++)
	{
		min_inter1 = MIN(min_inter1,min1[i]);
		max_inter1 = MAX(max_inter1,max1[i]);
	}

	for(int thread_count =1; thread_count < dat.numThreads; thread_count++)
	{
		min_inter = MIN(min_inter,min[thread_count]);
		max_inter = MAX(max_inter,max[thread_count]);
		min1 = (TDST*)&dat.xmmData[thread_count];
		max1 = (TDST*)&dat.xmmData2[thread_count];
		min_inter2 = min1[0];
		max_inter2 = max1[0];
		for(int i = 1; i < size; i++)
		{
			min_inter2 = MIN(min_inter2,min1[i]);
			max_inter2 = MAX(max_inter2,max1[i]);
		}
		min_inter1 = MIN(min_inter1,min_inter2);
		max_inter1 = MAX(max_inter1,max_inter2);
	}

	min[0] = MIN(min_inter,min_inter1);
	max[0] = MAX(max_inter,max_inter1);
}



template<class TD,class TDST,int regCount,PIX_FN1,PIX_SSE,PIX_SSE_SETUP,PIX_SSE_CLEANUP>
SYS_INLINE STATIC FwStatus MinMax(TDST minval,TDST maxval,const TD *pSrc,TDST *pMin,TDST *pMax,int len,unsigned int minBytesLen=256*256)
{
	FwStatus rest_status;
	TDST min[XMM_MAX_ITEMS];
	TDST max[XMM_MAX_ITEMS];

	FnDat dat(&min,&max);  

	min[0]= minval;
	max[0]= maxval;
	
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	rest_status =  foreach_1D< TD, C1,regCount,pix_ref,pix_sse,pix_sse_setup,pix_sse_cleanup>(const_cast<TD*>(pSrc), len,dat,minBytesLen ); 
	*pMin = min[0];
	*pMax = max[0];
	return rest_status;
}

template<class TD,class TDST,int size,int regCount,PIX_FN1,PIX_SSE,PIX_SSE_SETUP,PIX_SSE_CLEANUP>
SYS_INLINE STATIC FwStatus MinMaxIndex(TDST minval,TDST maxval, const TD *s,TDST *dmin,int *minindex,TDST *dmax,int *maxindex,int len,unsigned int minBytesLen=256*256)
{
	FwStatus rest_status;
	TDST minmax[2];
	int minmaxindex[2],index;
	FnDat dat(&minmax,&minmaxindex,&index);  

	minmax[0] =  minval; // reference to compare to find min.
	minmax[1] =  maxval; // reference to compare to find max.
	index = 0;
	minmaxindex[0]=minmaxindex[1]=0;

	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	rest_status =  foreach_1D< TD, C1,regCount,pix_ref,pix_sse,pix_sse_setup,pix_sse_cleanup >(const_cast<TD*>(s), len,dat,minBytesLen ); 
	*dmin = minmax[0];
	*dmax = minmax[1];
	*minindex = minmaxindex[0];
	*maxindex = minmaxindex[1];
	return rest_status;
}


SYS_INLINE	STATIC void SumTemplateC_cleanup16s16sc32fc(FnDat &dat)
{
	Fw64fc sum_inter,sum_inter1,sum_inter2;
	Fw32fc *sum1;
	Fw64fc *sum  = (Fw64fc*)dat.data[0];
	sum_inter = sum[0];
	sum1 = (Fw32fc*)&dat.xmmData[0];
	sum_inter1.re = sum1[0].re;
	sum_inter1.im = sum1[0].im;

	sum_inter1.re += sum1[1].re;
	sum_inter1.im += sum1[1].im;

	for(int thread_count =1; thread_count < dat.numThreads; thread_count++)
	{
		sum_inter += sum[thread_count];

		sum1 = (Fw32fc*)&dat.xmmData[thread_count];
		sum_inter2.re = sum1[0].re;
		sum_inter2.im = sum1[0].im;

		sum_inter2.re += sum1[1].re;
		sum_inter2.im += sum1[1].im;
		
		sum_inter1 +=sum_inter2;
	}
	
	sum_inter1 = sum_inter1 + sum_inter;
	sum[0].re = sum_inter1.re;
	sum[0].im = sum_inter1.im;
}

} // namespace OPT_LEVEL
