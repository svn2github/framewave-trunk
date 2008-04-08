/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "StatisticsMinMax.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMax_8s)(const Fw8s* pSrc, int len, Fw8s* pMax)
{
	if (len == 1) { *pMax = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMax_8s data;
	retStatus = OPT_LEVEL::fex<StatMax_8s>(data,pSrc,len); 
	*pMax = data.max;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMax_16s)(const Fw16s* pSrc, int len, Fw16s* pMax)
{
	if (len == 1) { *pMax = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMax_16s data;
	retStatus = OPT_LEVEL::fex<StatMax_16s>(data,pSrc,len); 
	*pMax = data.max;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMax_32s)(const Fw32s* pSrc, int len, Fw32s* pMax)
{
	if (len == 1) { *pMax = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;	
	StatMax_32s data;
	retStatus = OPT_LEVEL::fex<StatMax_32s>(data,pSrc,len); 
	*pMax = data.max;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMax_64u)(const Fw64u* pSrc, int len, Fw64u* pMax)
{
	if (len == 1) { *pMax = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;	
	StatMax_64u data;
	retStatus = OPT_LEVEL::fex<StatMax_64u>(data,pSrc,len); 
	*pMax = data.max;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMax_32f)(const Fw32f* pSrc, int len, Fw32f* pMax)
{
	if (len == 1) { *pMax = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;	
	StatMax_32f data;
	retStatus = OPT_LEVEL::fex<StatMax_32f>(data,pSrc,len); 
	*pMax = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMax_64f)(const Fw64f* pSrc, int len, Fw64f* pMax)
{
	if (len == 1) { *pMax = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;	
	StatMax_64f data;
	retStatus = OPT_LEVEL::fex<StatMax_64f>(data,pSrc,len); 
	*pMax = data.max;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMin_8s)(const Fw8s* pSrc, int len, Fw8s* pMin)
{
	if (len == 1) { *pMin = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMin_8s data;
	retStatus = OPT_LEVEL::fex<StatMin_8s>(data,pSrc,len); 
	*pMin = data.min;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMin_16s)(const Fw16s* pSrc, int len, Fw16s* pMin)
{
	if (len == 1) { *pMin = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMin_16s data;
	retStatus = OPT_LEVEL::fex<StatMin_16s>(data,pSrc,len); 
	*pMin = data.min;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMin_32s)(const Fw32s* pSrc, int len, Fw32s* pMin)
{
	if (len == 1) { *pMin = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMin_32s data;
	retStatus = OPT_LEVEL::fex<StatMin_32s>(data,pSrc,len); 
	*pMin = data.min;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMin_64u)(const Fw64u* pSrc, int len, Fw64u* pMin)
{
	if (len == 1) { *pMin = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;	
	StatMin_64u data;
	retStatus = OPT_LEVEL::fex<StatMin_64u>(data,pSrc,len); 
	*pMin = data.min;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMin_32f)(const Fw32f* pSrc, int len, Fw32f* pMin)
{
	if (len == 1) { *pMin = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMin_32f data;
	retStatus = OPT_LEVEL::fex<StatMin_32f>(data,pSrc,len); 
	*pMin = data.min;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMin_64f)(const Fw64f* pSrc, int len, Fw64f* pMin)
{
	if (len == 1) { *pMin = pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMin_64f data;
	retStatus = OPT_LEVEL::fex<StatMin_64f>(data,pSrc,len); 
	*pMin = data.min;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMax_8u)(const Fw8u* pSrc, int len, Fw8u* pMin, Fw8u* pMax)
{
	if (len == 1) { *pMin = *pMax =  pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMinMax_8u data;
	retStatus = OPT_LEVEL::fex<StatMinMax_8u>(data,pSrc,len); 
	*pMin = data.min;
	*pMax = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMax_16u)(const Fw16u* pSrc, int len, Fw16u* pMin, Fw16u* pMax)
{
	if (len == 1) { *pMin = *pMax =  pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMinMax_16u data;
	retStatus = OPT_LEVEL::fex<StatMinMax_16u>(data,pSrc,len); 
	*pMin = data.min;
	*pMax = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMax_16s)(const Fw16s* pSrc, int len, Fw16s* pMin, Fw16s* pMax)
{
	if (len == 1) { *pMin = *pMax =  pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMinMax_16s data;
	retStatus = OPT_LEVEL::fex<StatMinMax_16s>(data,pSrc,len); 
	*pMin = data.min;
	*pMax = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMax_32u)(const Fw32u* pSrc, int len, Fw32u* pMin, Fw32u* pMax)
{
	if (len == 1) { *pMin = *pMax =  pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMinMax_32u data;
	retStatus = OPT_LEVEL::fex<StatMinMax_32u>(data,pSrc,len); 
	*pMin = data.min;
	*pMax = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMax_32s)(const Fw32s* pSrc, int len, Fw32s* pMin, Fw32s* pMax)
{
	if (len == 1) { *pMin = *pMax =  pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMinMax_32s data;
	retStatus = OPT_LEVEL::fex<StatMinMax_32s>(data,pSrc,len); 
	*pMin = data.min;
	*pMax = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMax_32f)(const Fw32f* pSrc, int len, Fw32f* pMin, Fw32f* pMax)
{
	if (len == 1) { *pMin = *pMax =  pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMinMax_32f data;
	retStatus = OPT_LEVEL::fex<StatMinMax_32f>(data,pSrc,len);
	*pMin = data.min;
	*pMax = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMax_64f)(const Fw64f* pSrc, int len, Fw64f* pMin, Fw64f* pMax)
{
	if (len == 1) { *pMin = *pMax =  pSrc[0]; return fwStsNoErr;}
	FwStatus retStatus;
	StatMinMax_64f data;
	retStatus = OPT_LEVEL::fex<StatMinMax_64f>(data,pSrc,len); 
	*pMin = data.min;
	*pMax = data.max;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMaxAbs_16s)(const Fw16s* pSrc, int len, Fw16s* pMaxAbs)
{
	FwStatus retStatus= fwStsNoErr;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatMinMax_16s data;
	retStatus = OPT_LEVEL::fex<StatMinMax_16s>(data,pSrc,len); 

	if(data.min == (Fw16s)FW_REF::Limits<S16>::MinValue())	*pMaxAbs = (Fw16s)FW_REF::Limits<S16>::MaxValue();	
	else 	*pMaxAbs = MAX<Fw16s>(FW_REF::ABS_S<S16>::ABS( data.min ),FW_REF::ABS_S<S16>::ABS( data.max ));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMaxAbs_32s)(const Fw32s* pSrc, int len, Fw32s* pMaxAbs)
{
	if (len == 1) 
	{ 
		Fw32s complement, temp  ;		//Finding the absolute value
		temp = (Fw32s)*pSrc >> 31;
		complement = *pSrc ^ temp;  //one's complement of src
		complement = complement - temp;
		temp  = (Fw32u)complement >> 31;
		*pMaxAbs = complement - temp;
		return fwStsNoErr;
	}

	FwStatus retStatus;
	StatMaxAbs_32s data;
	retStatus = OPT_LEVEL::fex<StatMaxAbs_32s>(data,pSrc,len);
	*pMaxAbs = data.max;
	return retStatus;	
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinAbs_16s)(const Fw16s* pSrc, int len, Fw16s* pMinAbs)
{
	FwStatus retStatus;
	StatMinAbs_16s data;
	retStatus = OPT_LEVEL::fex<StatMinAbs_16s>(data,pSrc,len); 

    Fw16s complement, sign  ;		//Finding the absolute value
    sign = (Fw16s)data.min >> 15;
    complement = data.min ^ sign;  //one's complement of src
    complement = complement - sign;
    sign  = (Fw16u)complement >> 15;
    *pMinAbs = complement - sign;
	return retStatus;	
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinAbs_32s)(const Fw32s* pSrc, int len, Fw32s* pMinAbs)
{
	FwStatus retStatus;
	StatMinAbs_32s data;
	retStatus = OPT_LEVEL::fex<StatMinAbs_32s>(data,pSrc,len);
   
    Fw32s complement, sign  ;		//Finding the absolute value
    sign = (Fw32s)data.min >> 31;
    complement = data.min ^ sign;  //one's complement of src
    complement = complement - sign;
    sign  = (Fw32u)complement >> 31;
    *pMinAbs = complement - sign;

	return retStatus;	
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsMaxIndx_16s)(const Fw16s* pSrc, int len, Fw16s* pMax, int* pIndx)
{
	FwStatus retStatus;

	if( FW_REF::PtrNotOK ( pMax	  ) ) return fwStsNullPtrErr;	

	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	

	StatMax_16s data;
	retStatus = OPT_LEVEL::fex<StatMax_16s>(data,pSrc,len); 
	*pMax = data.max;

	int i;
	for( i=0;i<len;i++)
		if (pSrc[i] == *pMax) {	*pIndx = i; break;}

	return retStatus;
	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMaxIndx_32s)(const Fw32s* pSrc, int len, Fw32s* pMax, int* pIndx)
{

	if( FW_REF::PtrNotOK ( pSrc,pMax,pIndx ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;

	int i=0;
	int count = (len - (len %4));
	Fw32s max=pSrc[0];
	int index=0;

	while(i < count)
	{
		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
		i++;
		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
		i++;
		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
		i++;
		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
		i++;

		if(max == FW_REF::Limits<S32>::MaxValue()) 
		{ 
			break;
		}
	}


	switch(len%4)
	{
		case 3:		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
					i++;
		case 2:		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
					i++;
		case 1:		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
	}
	*pIndx = index;
	*pMax = max;
	
	return fwStsNoErr;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMaxIndx_32f)(const Fw32f* pSrc, int len, Fw32f* pMax, int* pIndx)
{
	if( FW_REF::PtrNotOK ( pSrc,pMax,pIndx ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;

	int i=0;
	int count = (len - (len %4));
	Fw32f max=pSrc[0];
	int index=0;

	while(i < count)
	{
		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
		i++;
		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
		i++;
		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
		i++;
		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
		i++;

	}


	switch(len%4)
	{
		case 3:		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
					i++;
		case 2:		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
					i++;
		case 1:		if(max < pSrc[i]) { max = pSrc[i]; index = i;}
	}
	*pIndx = index;
	*pMax = max;
	
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMaxIndx_64f)(const Fw64f* pSrc, int len, Fw64f* pMax, int* pIndx)
{
	FwStatus retStatus;
	StatMaxIndx_64f data;
	retStatus = OPT_LEVEL::fex<StatMaxIndx_64f>(data,pSrc,len,THREAD_SW_OFF);
	*pMax = data.max;
	*pIndx = data.maxindex;
	return retStatus;	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinIndx_16s)(const Fw16s* pSrc, int len, Fw16s* pMin, int* pIndx)
{
	FwStatus retStatus;

	if( FW_REF::PtrNotOK ( pMin	  ) ) return fwStsNullPtrErr;	

	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatMin_16s data;
	retStatus = OPT_LEVEL::fex<StatMin_16s>(data,pSrc,len); 
	*pMin = data.min;

	int i;
	for( i=0;i<len;i++)
		if (pSrc[i] == *pMin) {	*pIndx = i; break;}

	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinIndx_32s)(const Fw32s* pSrc, int len, Fw32s* pMin, int* pIndx)
{

	if( FW_REF::PtrNotOK ( pSrc,pMin,pIndx ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;

	int i=0;
	int count = (len - (len %4));
	Fw32s min=pSrc[0];
	int index=0;

	while(i < count)
	{
		if(min>pSrc[i]) { min = pSrc[i]; index = i;}
		i++;
		if(min>pSrc[i]) { min = pSrc[i]; index = i;}
		i++;
		if(min>pSrc[i]) { min = pSrc[i]; index = i;}
		i++;
		if(min>pSrc[i]) { min = pSrc[i]; index = i;}
		i++;

		if(min == FW_REF::Limits<S32>::MinValue()) 
		{ 
			break;
		}
	}


	switch(len%4)
	{
		case 3:		if(min>pSrc[i]) { min = pSrc[i]; index = i;}
					i++;
		case 2:		if(min>pSrc[i]) { min = pSrc[i]; index = i;}
					i++;
		case 1:		if(min>pSrc[i]) { min = pSrc[i]; index = i;}
	}
	*pIndx = index;
	*pMin = min;
	
	return fwStsNoErr;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinIndx_32f)(const Fw32f* pSrc, int len, Fw32f* pMin, int* pIndx)
{
	if (len == 1) { *pMin = pSrc[0]; pIndx = 0; return fwStsNoErr;}
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;	


	StatMin_32f data;
	retStatus = OPT_LEVEL::fex<StatMin_32f>(data,pSrc,len); 
	*pMin = data.min;

	int i;
	for( i=0;i<len;i++)
		if (pSrc[i] == *pMin) {	*pIndx = i; break;}

	return retStatus;

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinIndx_64f)(const Fw64f* pSrc, int len, Fw64f* pMin, int* pIndx)
{
	if (len == 1) { *pMin = pSrc[0]; pIndx = 0; return fwStsNoErr;}
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatMin_64f data;
	retStatus = OPT_LEVEL::fex<StatMin_64f>(data,pSrc,len); 
	*pMin = data.min;

	int i;
	for( i=0;i<len;i++)
		if (pSrc[i] == *pMin) {	*pIndx = i; break;}

	return retStatus;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMaxAbsIndx_16s)(const Fw16s* pSrc, int len, Fw16s* pMaxAbs, int* pIndx)
{


	Fw16s pMin[1], pMax[1];
	FwStatus retStatus= fwStsNoErr;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatMinMax_16s data;
	retStatus = OPT_LEVEL::fex<StatMinMax_16s>(data,pSrc,len);
	*pMin = data.min;
	*pMax = data.max;

	if(*pMin == (Fw16s)FW_REF::Limits<S16>::MinValue())
	{for( int i=0;i<len;i++)
			if ( pSrc[i] == (Fw16s)FW_REF::Limits<S16>::MinValue()) {	*pMaxAbs = (Fw16s)FW_REF::Limits<S16>::MaxValue(); *pIndx = i; break;}
	}
	
	else 
	{
		*pMaxAbs = MAX<Fw16s>(FW_REF::ABS_S<S16>::ABS( *pMin ),FW_REF::ABS_S<S16>::ABS( *pMax ));
	
		for( int i=0;i<len;i++)
			if (*pMaxAbs == FW_REF::ABS_S<S16>::ABS( pSrc[i] )) {	*pIndx = i; break;}
	}


	return retStatus;

}



FwStatus PREFIX_OPT(OPT_PREFIX, fwsMaxAbsIndx_32s)(const Fw32s* pSrc, int len, Fw32s* pMaxAbs, int* pIndx)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatMaxAbsIndx_32s data;
	retStatus = OPT_LEVEL::fex<StatMaxAbsIndx_32s>(data,pSrc,len,THREAD_SW_OFF);
	*pMaxAbs = data.max;
    *pIndx = data.maxindex;
	Fw32s complement, temp  ;		//Finding the absolute value
	temp = (Fw32s)*pMaxAbs >> 31;
	complement = *pMaxAbs ^ temp;  //one's complement of src
	complement = complement - temp;
	temp  = (Fw32u)complement >> 31;
	*pMaxAbs = complement - temp;
	return  retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinAbsIndx_16s)(const Fw16s* pSrc, int len, Fw16s* pMinAbs, int* pIndx)
{

	if( FW_REF::PtrNotOK ( pSrc,pMinAbs,pIndx ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;

	int i=0;
	int count = (len - (len %4));
	Fw32s max=MIN<Fw32s>((Fw32s)pSrc[0],0-(Fw32s)pSrc[0]);
	int index=0;
	Fw32s max1;

	while( i < count )
	{	
		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
		if(max<max1) { max = max1; index = i;}
		i++;
		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
		if(max<max1) { max = max1; index = i;}
		i++;
		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
		if(max<max1) { max = max1; index = i;}
		i++;
		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
		if(max<max1) { max = max1; index = i;}
		i++;
		if(max == 0) { break;}
	}


	switch(len%4)
	{
		case 3:		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
					if(max<max1) { max = max1; index = i;}
					i++;
		case 2:		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
					if(max<max1) { max = max1; index = i;}
					i++;
		case 1:		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
					if(max<max1) { max = max1; index = i;}
	}

	*pIndx = index;
	*pMinAbs = FW_REF::Limits<S16>::Sat(FW_REF::ABS_S<S32>::ABS( max ));
	
	return fwStsNoErr;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinAbsIndx_32s)(const Fw32s* pSrc, int len, Fw32s* pMinAbs, int* pIndx)
{

	if( FW_REF::PtrNotOK ( pSrc,pMinAbs,pIndx ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;

	int i=0;
	int count = (len - (len %4));
	Fw32s max=MIN<Fw32s>((Fw32s)pSrc[0],0-(Fw32s)pSrc[0]);
	int index=0;
	Fw32s max1;

	while( i < count )
	{	
		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
		if(max<max1) { max = max1; index = i;}
		i++;
		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
		if(max<max1) { max = max1; index = i;}
		i++;
		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
		if(max<max1) { max = max1; index = i;}
		i++;
		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
		if(max<max1) { max = max1; index = i;}
		i++;
		if(max == 0) { break;}
	}


	switch(len%4)
	{
		case 3:		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
					if(max<max1) { max = max1; index = i;}
					i++;
		case 2:		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
					if(max<max1) { max = max1; index = i;}
					i++;
		case 1:		max1 = MIN<Fw32s>((Fw32s)pSrc[i],0-(Fw32s)pSrc[i]);
					if(max<max1) { max = max1; index = i;}
	}

	*pIndx = index;
	if(max == (Fw32s)FW_REF::Limits<S32>::MinValue()) 
		*pMinAbs = (Fw32s)FW_REF::Limits<S32>::MaxValue();
	else	
		*pMinAbs = FW_REF::Limits<S32>::Sat(FW_REF::ABS_S<S32>::ABS( max ));
	
	return fwStsNoErr;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMaxIndx_8u)(const Fw8u* pSrc, int len, Fw8u *pMin, int* pMinIndx, Fw8u* pMax, int* pMaxIndx)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;	

	StatMinMax_8u data;
	retStatus = OPT_LEVEL::fex<StatMinMax_8u>(data,pSrc,len); 
	*pMin = data.min;
	*pMax = data.max;

	int i;
	for( i=0;i<len;i++)
		if (pSrc[i] == *pMin) {	*pMinIndx = i; break;}

	for(i=0;i<len;i++)
		if (pSrc[i] == *pMax) {	*pMaxIndx = i; break;}

	return retStatus;

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMaxIndx_16u)(const Fw16u* pSrc, int len, Fw16u* pMin, int* pMinIndx, Fw16u* pMax, int* pMaxIndx)
{

	FwStatus retStatus;
	int i;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatMinMax_16u data;
	retStatus = OPT_LEVEL::fex<StatMinMax_16u>(data,pSrc,len); 
	*pMin = data.min;
	*pMax = data.max;

	for( i=0;i<len;i++)
		if (pSrc[i] == *pMin) {	*pMinIndx = i; break;}

	for(i=0;i<len;i++)
		if (pSrc[i] == *pMax) {	*pMaxIndx = i; break;}
	return retStatus;	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMaxIndx_16s)(const Fw16s* pSrc, int len, Fw16s* pMin, int* pMinIndx, Fw16s* pMax, int* pMaxIndx)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	
	StatMinMax_16s data;
	retStatus = OPT_LEVEL::fex<StatMinMax_16s>(data,pSrc,len);
	*pMin = data.min;
	*pMax = data.max;

	int i;
	for( i=0;i<len;i++)
		if (pSrc[i] == *pMin) {	*pMinIndx = i; break;}

	for(i=0;i<len;i++)
		if (pSrc[i] == *pMax) {	*pMaxIndx = i; break;}

	return retStatus;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMaxIndx_32u)(const Fw32u* pSrc, int len, Fw32u* pMin, int* pMinIndx, Fw32u* pMax, int* pMaxIndx)
{
	if( FW_REF::PtrNotOK ( pSrc,pMin,pMinIndx,pMax,pMaxIndx ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;

	int i, repeat = (len - (len%4));
	register Fw32u min = pSrc[0], max = pSrc[0];
	register int minIndx = 0, maxIndx = 0;
	for( i=0;i<repeat;i++)
	{
		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
		i++;
		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
		i++;
		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
		i++;
		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
	}
	switch(len%4)
	{	
		case 3:	if(min>pSrc[i])			min= pSrc[i];		
				else if(max<pSrc[i])	max = pSrc[i];
				i++;
		case 2: if(min>pSrc[i])			min= pSrc[i];		
				else if(max<pSrc[i])	max = pSrc[i];
				i++;
		case 1: if(min>pSrc[i])			min= pSrc[i];		
				else if(max<pSrc[i])	max = pSrc[i];
	}

	*pMin = min;
	*pMax = max;
	*pMinIndx = minIndx;
	*pMaxIndx = maxIndx;
	
	for( i=0;i<len;i++)
		if (pSrc[i] == *pMin) {	*pMinIndx = i; break;}

	for(i=0;i<len;i++)
		if (pSrc[i] == *pMax) {	*pMaxIndx = i; break;}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMaxIndx_32s)(const Fw32s* pSrc, int len, Fw32s* pMin, int* pMinIndx, Fw32s* pMax, int* pMaxIndx)
{
	if( FW_REF::PtrNotOK ( pSrc,pMin,pMinIndx,pMax,pMaxIndx ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;

	int i, repeat = (len - (len%4));
	register Fw32s min = pSrc[0], max = pSrc[0];
	register int minIndx = 0, maxIndx = 0;
	for( i=0;i<repeat;i++)
	{
		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
		i++;
		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
		i++;
		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
		i++;
		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
	}
	switch(len%4)
	{	
		case 3:	if(min>pSrc[i])			min= pSrc[i];		
				else if(max<pSrc[i])	max = pSrc[i];
				i++;
		case 2: if(min>pSrc[i])			min= pSrc[i];		
				else if(max<pSrc[i])	max = pSrc[i];
				i++;
		case 1: if(min>pSrc[i])			min= pSrc[i];		
				else if(max<pSrc[i])	max = pSrc[i];
	}

	*pMin = min;
	*pMax = max;
	*pMinIndx = minIndx;
	*pMaxIndx = maxIndx;
	
	for( i=0;i<len;i++)
		if (pSrc[i] == *pMin) {	*pMinIndx = i; break;}

	for(i=0;i<len;i++)
		if (pSrc[i] == *pMax) {	*pMaxIndx = i; break;}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMaxIndx_32f)(const Fw32f* pSrc, int len, Fw32f* pMin, int* pMinIndx, Fw32f* pMax, int* pMaxIndx)
{
	FwStatus retStatus;
	if( FW_REF::PtrNotOK ( pSrc,pMin,pMinIndx,pMax,pMaxIndx ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;


	StatMinMax_32f data;
	retStatus = OPT_LEVEL::fex<StatMinMax_32f>(data,pSrc,len); 
	*pMin = data.min;
	*pMax = data.max;
	
	int i;
	for( i=0;i<len;i++)
		if (pSrc[i] == *pMin) {	*pMinIndx = i; break;}

	for(i=0;i<len;i++)
		if (pSrc[i] == *pMax) {	*pMaxIndx = i; break;}

	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMinMaxIndx_64f)(const Fw64f* pSrc, int len, Fw64f* pMin, int* pMinIndx, Fw64f* pMax, int* pMaxIndx)
{

	if( FW_REF::PtrNotOK ( pSrc,pMin,pMinIndx,pMax,pMaxIndx ) ) return fwStsNullPtrErr;	
	if( FW_REF::SizeNotOK( len ) ) return fwStsSizeErr;

	int i, repeat = (len - (len%4));
	register Fw64f min = pSrc[0], max = pSrc[0];
	register int minIndx = 0, maxIndx = 0;

	for( i=0;i<repeat;i++)
	{

		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
		i++;
		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
		i++;
		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
		i++;
		if(min>pSrc[i])			min= pSrc[i];		
		else if(max<pSrc[i])	max = pSrc[i];
	}
	switch(len%4)
	{	
		case 3:	if(min>pSrc[i])			min= pSrc[i];		
				else if(max<pSrc[i])	max = pSrc[i];
				i++;
		case 2: if(min>pSrc[i])			min= pSrc[i];		
				else if(max<pSrc[i])	max = pSrc[i];
				i++;
		case 1: if(min>pSrc[i])			min= pSrc[i];		
				else if(max<pSrc[i])	max = pSrc[i];
	}

	*pMin = min;
	*pMax = max;
	*pMinIndx = minIndx;
	*pMaxIndx = maxIndx;
	
	for( i=0;i<len;i++)
		if (pSrc[i] == *pMin) {	*pMinIndx = i; break;}

	for(i=0;i<len;i++)
		if (pSrc[i] == *pMax) {	*pMaxIndx = i; break;}

	return fwStsNoErr;
}

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
