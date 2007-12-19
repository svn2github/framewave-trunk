/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "ThresholdDef.h"	
#include "fwSignal.h"

using namespace OPT_LEVEL;
	
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_16s)(const Fw16s* pSrc, Fw16s* pDst, int len, Fw16s level, Fw16s value)
	{

		DEF_TRES::LT::VAL::Threshold_LTVal_16s data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_16s>(data,pSrc,pDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_32f)(const Fw32f* pSrc, Fw32f* pDst, int len, Fw32f level, Fw32f value)
	{
		DEF_TRES::LT::VAL::Threshold_LTVal_32f data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_32f>(data,pSrc,pDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_64f)(const Fw64f* pSrc, Fw64f* pDst, int len, Fw64f level, Fw64f value)
	{
		DEF_TRES::LT::VAL::Threshold_LTVal_64f data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_64f>(data,pSrc,pDst,len);
	}


	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_16sc)(const Fw16sc* pSrc, Fw16sc* pDst, int len, Fw16s level, Fw16sc value)
	{

		if(level < 0)
			return fwStsThreshNegLevelErr;		

		if (level == 0)
		{
			Threshold_LTVal_L_ZERO<Fw16sc,Fw16sc> data;
			return OPT_LEVEL::fe<Threshold_LTVal_L_ZERO<Fw16sc,Fw16sc> >(data,pSrc,pDst,len);
		}

			DEF_TRES::LT::VAL::Threshold_LTVal_16sc data(level,value);
			return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_16sc>(data,pSrc,pDst,len);
	}


	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_32fc)(const Fw32fc* pSrc, Fw32fc* pDst, int len, Fw32f level, Fw32fc value)
	{
		if(level < 0)
			return fwStsThreshNegLevelErr;		
		
		if (level == 0)
		{
			Threshold_LTVal_L_ZERO<Fw32fc,Fw32fc> data;
			return OPT_LEVEL::fe<Threshold_LTVal_L_ZERO<Fw32fc,Fw32fc> >(data,pSrc,pDst,len);
		}


			DEF_TRES::LT::VAL::Threshold_LTVal_32fc data(level,value);
			return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_32fc>(data,pSrc,pDst,len);	
	}


	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_64fc)(const Fw64fc* pSrc, Fw64fc* pDst, int len, Fw64f level, Fw64fc value)
	{

		if(level < 0)
			return fwStsThreshNegLevelErr;

		if (level == 0)
		{
			Threshold_LTVal_L_ZERO<Fw64fc,Fw64fc> data;
			return OPT_LEVEL::fe<Threshold_LTVal_L_ZERO<Fw64fc,Fw64fc> >(data,pSrc,pDst,len);
		}

		DEF_TRES::LT::VAL::Threshold_LTVal_64fc data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_64fc>(data,pSrc,pDst,len);	
	}


	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_16s_I)(Fw16s* pSrcDst, int len, Fw16s level, Fw16s value)
	{
		DEF_TRES::LT::VAL::Threshold_LTVal_16s_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_16s_I>(data, pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_32f_I)(Fw32f* pSrcDst, int len, Fw32f level, Fw32f value)
	{
		DEF_TRES::LT::VAL::Threshold_LTVal_32f_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_32f_I>(data, pSrcDst,len);	
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_64f_I)(Fw64f* pSrcDst, int len, Fw64f level, Fw64f value)
	{
		DEF_TRES::LT::VAL::Threshold_LTVal_64f_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_64f_I>(data, pSrcDst,len);	
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_16sc_I)(Fw16sc* pSrcDst, int len, Fw16s level, Fw16sc value)
	{

		if(level < 0)
			return fwStsThreshNegLevelErr;

		if (level == 0)
			return fwStsNoErr;

		DEF_TRES::LT::VAL::Threshold_LTVal_16sc_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_16sc_I>(data, pSrcDst,len);	
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_32fc_I)(Fw32fc* pSrcDst, int len, Fw32f level, Fw32fc value)
	{

		if(level < 0)
			return fwStsThreshNegLevelErr;

		if (level == 0)
			return fwStsNoErr;

		DEF_TRES::LT::VAL::Threshold_LTVal_32fc_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_32fc_I>(data, pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTVal_64fc_I)(Fw64fc* pSrcDst, int len, Fw64f level, Fw64fc value)
	{
		if(level < 0)
			return fwStsThreshNegLevelErr;

		if (level == 0)
			return fwStsNoErr;

		DEF_TRES::LT::VAL::Threshold_LTVal_64fc_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::LT::VAL::Threshold_LTVal_64fc_I>(data, pSrcDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_16s)(const Fw16s* pSrc, Fw16s* pDst, int len, Fw16s level, Fw16s value)
	{
		DEF_TRES::GT::VAL::Threshold_GTVal_16s data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_16s>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_32f)(const Fw32f* pSrc, Fw32f* pDst, int len, Fw32f level, Fw32f value)
	{
		DEF_TRES::GT::VAL::Threshold_GTVal_32f data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_32f>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_64f)(const Fw64f* pSrc, Fw64f* pDst, int len, Fw64f level, Fw64f value)
	{
		DEF_TRES::GT::VAL::Threshold_GTVal_64f data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_64f>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_16sc)(const Fw16sc* pSrc, Fw16sc* pDst, int len, Fw16s level, Fw16sc value)
	{

		if(level < 0)
			return fwStsThreshNegLevelErr;

		DEF_TRES::GT::VAL::Threshold_GTVal_16sc data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_16sc>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_32fc)(const Fw32fc* pSrc, Fw32fc* pDst, int len, Fw32f level, Fw32fc value)
	{

		if(level < 0)
			return fwStsThreshNegLevelErr;

		DEF_TRES::GT::VAL::Threshold_GTVal_32fc data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_32fc>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_64fc)(const Fw64fc* pSrc, Fw64fc* pDst, int len, Fw64f level, Fw64fc value)
	{
		if(level < 0)
			return fwStsThreshNegLevelErr;

		DEF_TRES::GT::VAL::Threshold_GTVal_64fc data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_64fc>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_16s_I)(Fw16s* pSrcDst, int len, Fw16s level, Fw16s value)
	{
		DEF_TRES::GT::VAL::Threshold_GTVal_16s_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_16s_I>(data,pSrcDst,len);

	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_32f_I)(Fw32f* pSrcDst, int len, Fw32f level, Fw32f value)
	{
		DEF_TRES::GT::VAL::Threshold_GTVal_32f_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_32f_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_64f_I)(Fw64f* pSrcDst, int len, Fw64f level, Fw64f value)
	{
		DEF_TRES::GT::VAL::Threshold_GTVal_64f_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_64f_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_16sc_I)(Fw16sc* pSrcDst, int len, Fw16s level, Fw16sc value)
	{
		if(level < 0)
		return fwStsThreshNegLevelErr;	
		
		DEF_TRES::GT::VAL::Threshold_GTVal_16sc_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_16sc_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_32fc_I)(Fw32fc* pSrcDst, int len, Fw32f level, Fw32fc value)
	{

		if(level < 0)
			return fwStsThreshNegLevelErr;

		DEF_TRES::GT::VAL::Threshold_GTVal_32fc_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_32fc_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GTVal_64fc_I)(Fw64fc* pSrcDst, int len, Fw64f level, Fw64fc value)
	{
		if(level < 0)
			return fwStsThreshNegLevelErr;

		DEF_TRES::GT::VAL::Threshold_GTVal_64fc_I data(level,value);
		return OPT_LEVEL::fe<DEF_TRES::GT::VAL::Threshold_GTVal_64fc_I>(data,pSrcDst,len);
	}


	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTValGTVal_16s)(const Fw16s* pSrc, Fw16s* pDst, int len, Fw16s levelLT, Fw16s valueLT, Fw16s levelGT, Fw16s valueGT)
	{
		if (levelLT > levelGT)
			return fwStsThresholdErr;

		DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_16s data(levelLT,valueLT,levelGT,valueGT);
		return OPT_LEVEL::fe<DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_16s>(data,pSrc,pDst,len);

	}
	
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTValGTVal_32s)(const Fw32s* pSrc, Fw32s* pDst, int len, Fw32s levelLT, Fw32s valueLT, Fw32s levelGT, Fw32s valueGT)
	{
		if (levelLT > levelGT)
			return fwStsThresholdErr;
		DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_32s data(levelLT,valueLT,levelGT,valueGT);
		return OPT_LEVEL::fe<DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_32s>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTValGTVal_32f)(const Fw32f* pSrc, Fw32f* pDst, int len, Fw32f levelLT, Fw32f valueLT, Fw32f levelGT, Fw32f valueGT)
	{
		if (levelLT > levelGT)
			return fwStsThresholdErr;

		DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_32f data(levelLT,valueLT,levelGT,valueGT);
		return OPT_LEVEL::fe<DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_32f>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTValGTVal_64f)(const Fw64f* pSrc, Fw64f* pDst, int len, Fw64f levelLT, Fw64f valueLT, Fw64f levelGT, Fw64f valueGT)
	{
		if (levelLT > levelGT)
			return fwStsThresholdErr;
		DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_64f data(levelLT,valueLT,levelGT,valueGT);
		return OPT_LEVEL::fe<DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_64f>(data,pSrc,pDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTValGTVal_16s_I)(Fw16s* pSrcDst, int len, Fw16s levelLT, Fw16s valueLT, Fw16s levelGT, Fw16s valueGT)
	{
		if (levelLT > levelGT)
			return fwStsThresholdErr;
		DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_16s_I data(levelLT,valueLT,levelGT,valueGT);
		return OPT_LEVEL::fe<DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_16s_I>(data,pSrcDst,len);

	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTValGTVal_32s_I)(Fw32s* pSrcDst, int len, Fw32s levelLT, Fw32s valueLT, Fw32s levelGT, Fw32s valueGT)
	{
		if (levelLT > levelGT)
			return fwStsThresholdErr;
		DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_32s_I data(levelLT,valueLT,levelGT,valueGT);
		return OPT_LEVEL::fe<DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_32s_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTValGTVal_32f_I)(Fw32f* pSrcDst, int len, Fw32f levelLT, Fw32f valueLT, Fw32f levelGT, Fw32f valueGT)
	{
		if (levelLT > levelGT)
			return fwStsThresholdErr;
		DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_32f_I data(levelLT,valueLT,levelGT,valueGT);
		return OPT_LEVEL::fe<DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_32f_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTValGTVal_64f_I)(Fw64f* pSrcDst, int len, Fw64f levelLT, Fw64f valueLT, Fw64f levelGT, Fw64f valueGT)
	{
		if (levelLT > levelGT)
			return fwStsThresholdErr;
		DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_64f_I data(levelLT,valueLT,levelGT,valueGT);
		return OPT_LEVEL::fe<DEF_TRES::LTVALGTVAL::Threshold_LTValGTVal_64f_I>(data,pSrcDst,len);

	}


	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_16s)(const Fw16s* pSrc, Fw16s* pDst, int len, Fw16s level)
	{
		DEF_TRES::LT::Threshold_LT_16s data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_16s>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_32s)(const Fw32s* pSrc, Fw32s* pDst, int len, Fw32s level)
	{
		DEF_TRES::LT::Threshold_LT_32s data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_32s>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_32f)(const Fw32f* pSrc, Fw32f* pDst, int len, Fw32f level)
	{
		DEF_TRES::LT::Threshold_LT_32f data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_32f>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_64f)(const Fw64f* pSrc, Fw64f* pDst, int len, Fw64f level)
	{
		DEF_TRES::LT::Threshold_LT_64f data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_64f>(data,pSrc,pDst,len);
	}


	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_32fc)(const Fw32fc* pSrc, Fw32fc* pDst, int len, Fw32f level)
	{
		if( level < 0)
			return fwStsThreshNegLevelErr;

		if (level == 0)
		{
			Threshold_LTVal_L_ZERO<Fw32fc,Fw32fc> data;
			return OPT_LEVEL::fe<Threshold_LTVal_L_ZERO<Fw32fc,Fw32fc> >(data,pSrc,pDst,len);
		}

		DEF_TRES::LT::Threshold_LT_32fc data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_32fc>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_64fc)(const Fw64fc* pSrc, Fw64fc* pDst, int len, Fw64f level)
	{

		if (level < 0)
			 return  fwStsThreshNegLevelErr;

		if (level == 0)
		{
			Threshold_LTVal_L_ZERO<Fw64fc,Fw64fc> data;
			return OPT_LEVEL::fe<Threshold_LTVal_L_ZERO<Fw64fc,Fw64fc> >(data,pSrc,pDst,len);
		}

		DEF_TRES::LT::Threshold_LT_64fc data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_64fc>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_16sc)(const Fw16sc* pSrc, Fw16sc* pDst, int len, Fw16s level)
	{
		if( level < 0)
	 		return fwStsThreshNegLevelErr;

		if (level == 0)
		{
			Threshold_LTVal_L_ZERO<Fw16sc,Fw16sc> data;
			return OPT_LEVEL::fe<Threshold_LTVal_L_ZERO<Fw16sc,Fw16sc> >(data,pSrc,pDst,len);
		}

		DEF_TRES::LT::Threshold_LT_16sc data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_16sc>(data,pSrc,pDst,len);

	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_16s_I)(Fw16s* pSrcDst, int len, Fw16s level)
	{
		DEF_TRES::LT::Threshold_LT_16s_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_16s_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_32s_I)(Fw32s* pSrcDst, int len, Fw32s level)
	{
		DEF_TRES::LT::Threshold_LT_32s_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_32s_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_32f_I)(Fw32f* pSrcDst, int len, Fw32f level)
	{
		DEF_TRES::LT::Threshold_LT_32f_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_32f_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_64f_I)(Fw64f* pSrcDst, int len, Fw64f level)
	{
		DEF_TRES::LT::Threshold_LT_64f_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_64f_I>(data,pSrcDst,len);
	}


	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_32fc_I)(Fw32fc* pSrcDst, int len, Fw32f level)
	{

		if (level < 0)
			 return  fwStsThreshNegLevelErr;

		if (level == 0)
			return fwStsNoErr;


		DEF_TRES::LT::Threshold_LT_32fc_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_32fc_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_64fc_I)(Fw64fc* pSrcDst, int len, Fw64f level)
	{

		if (level < 0)
			 return  fwStsThreshNegLevelErr;

		if (level == 0)
			return fwStsNoErr;

		DEF_TRES::LT::Threshold_LT_64fc_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_64fc_I>(data,pSrcDst,len);

	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LT_16sc_I)(Fw16sc* pSrcDst, int len, Fw16s level)
	{
		if( level < 0)
			return fwStsThreshNegLevelErr;

		if (level == 0)
			return fwStsNoErr;

		DEF_TRES::LT::Threshold_LT_16sc_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_16sc_I>(data,pSrcDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_16s)(const Fw16s* pSrc, Fw16s* pDst, int len, Fw16s level)
	{
		DEF_TRES::GT::Threshold_GT_16s data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_16s>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_32s)(const Fw32s* pSrc, Fw32s* pDst, int len, Fw32s level)
	{
		DEF_TRES::GT::Threshold_GT_32s data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_32s>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_32f)(const Fw32f* pSrc, Fw32f* pDst, int len, Fw32f level)
	{
		DEF_TRES::GT::Threshold_GT_32f data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_32f>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_64f)(const Fw64f* pSrc, Fw64f* pDst, int len, Fw64f level)
	{
		DEF_TRES::GT::Threshold_GT_64f data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_64f>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_32fc)(const Fw32fc* pSrc, Fw32fc* pDst, int len, Fw32f level)
	{

		if(level < 0)
			return fwStsThreshNegLevelErr;

		DEF_TRES::GT::Threshold_GT_32fc data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_32fc>(data,pSrc,pDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_64fc)(const Fw64fc* pSrc, Fw64fc* pDst, int len, Fw64f level)
	{

		if(level < 0)
			return fwStsThreshNegLevelErr;

		DEF_TRES::GT::Threshold_GT_64fc data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_64fc>(data,pSrc,pDst,len);

	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_16sc)(const Fw16sc* pSrc, Fw16sc* pDst, int len, Fw16s level)
	{
		if (level < 0)
			return fwStsThreshNegLevelErr;

		DEF_TRES::GT::Threshold_GT_16sc data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_16sc>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_16s_I)(Fw16s* pSrcDst, int len, Fw16s level)
	{
		DEF_TRES::GT::Threshold_GT_16s_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_16s_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_32s_I)(Fw32s* pSrcDst, int len, Fw32s level)
	{
		DEF_TRES::GT::Threshold_GT_32s_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_32s_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_32f_I)(Fw32f* pSrcDst, int len, Fw32f level)
	{
		DEF_TRES::GT::Threshold_GT_32f_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_32f_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_64f_I)(Fw64f* pSrcDst, int len, Fw64f level)
	{
		DEF_TRES::GT::Threshold_GT_64f_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_64f_I>(data,pSrcDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_32fc_I)(Fw32fc* pSrcDst, int len, Fw32f level)
	{
		if( level < 0)
			return fwStsThreshNegLevelErr;

		DEF_TRES::GT::Threshold_GT_32fc_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_32fc_I>(data,pSrcDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_64fc_I)(Fw64fc* pSrcDst, int len, Fw64f level)
	{

		if( level < 0)
			return fwStsThreshNegLevelErr;

		DEF_TRES::GT::Threshold_GT_64fc_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_64fc_I>(data,pSrcDst,len);

	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_GT_16sc_I)(Fw16sc* pSrcDst, int len, Fw16s level)
	{


		if( level < 0)
			return fwStsThreshNegLevelErr;

		DEF_TRES::GT::Threshold_GT_16sc_I data(level );
		return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_16sc_I>(data,pSrcDst,len);
	}


	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_16s)(const Fw16s* pSrc, Fw16s* pDst, int len, Fw16s level, FwCmpOp relOp)
	{

		switch (relOp)
		{
		case fwCmpLess:
			{
				DEF_TRES::LT::Threshold_LT_16s data(level);
				return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_16s>(data,pSrc,pDst,len);
			}
			break;
		case fwCmpGreater:
			{
				DEF_TRES::GT::Threshold_GT_16s data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_16s>(data,pSrc,pDst,len);
			}
			break;
		default: return fwStsNoErr;
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_32f)(const Fw32f* pSrc, Fw32f* pDst, int len, Fw32f level, FwCmpOp relOp)
	{

		switch (relOp)
		{
		case fwCmpLess:
			{
				DEF_TRES::LT::Threshold_LT_32f data(level);
				return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_32f>(data,pSrc,pDst,len);
			}
			break;
		case fwCmpGreater:
			{
				DEF_TRES::GT::Threshold_GT_32f data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_32f>(data,pSrc,pDst,len);
			}
			break;
		default: return fwStsNoErr;
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_64f)(const Fw64f* pSrc, Fw64f* pDst, int len, Fw64f level, FwCmpOp relOp)
	{

		switch (relOp)
		{
		case fwCmpLess:
			{
				DEF_TRES::LT::Threshold_LT_64f data(level);
				return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_64f>(data,pSrc,pDst,len);
			}
			break;
		case fwCmpGreater:
			{
				DEF_TRES::GT::Threshold_GT_64f data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_64f>(data,pSrc,pDst,len);
			}
			break;
		default: return fwStsNoErr;
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_32fc)(const Fw32fc* pSrc, Fw32fc* pDst, int len, Fw32f level, FwCmpOp relOp)
	{
		if (level < 0)
		{
			return fwStsThreshNegLevelErr;;
		}


		switch (relOp)
		{
		case fwCmpLess:

			if (level == 0)
			{
				Threshold_LTVal_L_ZERO<Fw32fc,Fw32fc> data;
				return OPT_LEVEL::fe<Threshold_LTVal_L_ZERO<Fw32fc,Fw32fc> >(data,pSrc,pDst,len);
			}
			else
			{
			DEF_TRES::LT::Threshold_LT_32fc data(level);
			return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_32fc>(data,pSrc,pDst,len);
			}

			break;
		case fwCmpGreater:
			{
				DEF_TRES::GT::Threshold_GT_32fc data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_32fc>(data,pSrc,pDst,len);
			}
			break;
		default: return fwStsNoErr;
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_64fc)(const Fw64fc* pSrc, Fw64fc* pDst, int len, Fw64f level, FwCmpOp relOp)
	{
		if (level < 0)
		{
			return fwStsThreshNegLevelErr;
		}


		switch (relOp)
		{
		case fwCmpLess:
			if (level == 0)
			{
				Threshold_LTVal_L_ZERO<Fw64fc,Fw64fc> data;
				return OPT_LEVEL::fe<Threshold_LTVal_L_ZERO<Fw64fc,Fw64fc> >(data,pSrc,pDst,len);
			}
			{
				DEF_TRES::LT::Threshold_LT_64fc data(level);
				return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_64fc>(data,pSrc,pDst,len);
			}
			break;
		case fwCmpGreater:
			{
				DEF_TRES::GT::Threshold_GT_64fc data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_64fc>(data,pSrc,pDst,len);
			}
			break;
		default: return fwStsNoErr;
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_16sc)(const Fw16sc* pSrc, Fw16sc* pDst, int len, Fw16s level, FwCmpOp relOp)
	{
		if (level < 0)
		{
			return fwStsThreshNegLevelErr;		
			
		}


		switch (relOp)
		{
		case fwCmpLess:
			if (level == 0)
			{
				Threshold_LTVal_L_ZERO<Fw16sc,Fw16sc> data;
				return OPT_LEVEL::fe<Threshold_LTVal_L_ZERO<Fw16sc,Fw16sc> >(data,pSrc,pDst,len);
			}
			{
				DEF_TRES::LT::Threshold_LT_16sc data(level);
				return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_16sc>(data,pSrc,pDst,len);
			}
			break;
		case fwCmpGreater:
			{
				DEF_TRES::GT::Threshold_GT_16sc data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_16sc>(data,pSrc,pDst,len);
			}
			break;
		default: return fwStsNoErr;
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_16s_I)(Fw16s* pSrcDst, int len, Fw16s level, FwCmpOp relOp)
	{

		switch (relOp)
		{
		case fwCmpLess:
			{
				DEF_TRES::LT::Threshold_LT_16s_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_16s_I>(data,pSrcDst,len);
			}
			break;
		case fwCmpGreater:
			{
				DEF_TRES::GT::Threshold_GT_16s_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_16s_I>(data,pSrcDst,len);
			}
			break;
		default: return fwStsNoErr;
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_32f_I)(Fw32f* pSrcDst, int len, Fw32f level, FwCmpOp relOp)
	{

		switch (relOp)
		{
		case fwCmpLess:
			{
				DEF_TRES::LT::Threshold_LT_32f_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_32f_I>(data,pSrcDst,len);
			}
			break;
		case fwCmpGreater:
			{
				DEF_TRES::GT::Threshold_GT_32f_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_32f_I>(data,pSrcDst,len);
			}
		default: return fwStsNoErr;
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_64f_I)(Fw64f* pSrcDst, int len, Fw64f level, FwCmpOp relOp)
	{

		switch (relOp)
		{
		case fwCmpLess:
			{
				DEF_TRES::LT::Threshold_LT_64f_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_64f_I>(data,pSrcDst,len);
			}
			break;
		case fwCmpGreater:
			{
				DEF_TRES::GT::Threshold_GT_64f_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_64f_I>(data,pSrcDst,len);
			}
		default: return fwStsNoErr;
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_32fc_I)(Fw32fc* pSrcDst, int len, Fw32f level, FwCmpOp relOp)
	{


		if (level < 0)
		{
			return fwStsThreshNegLevelErr;;
		}


		switch (relOp)
		{
		case fwCmpLess:
			if (level <= 0)
				return fwStsNoErr;
			{
				DEF_TRES::LT::Threshold_LT_32fc_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_32fc_I>(data,pSrcDst,len);
			}
			break;
		case fwCmpGreater:
			if (level < 0)
				return fwStsNoErr;
			{
				DEF_TRES::GT::Threshold_GT_32fc_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_32fc_I>(data,pSrcDst,len);
			}
			break;
		default: return fwStsNoErr;
		}
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_64fc_I)(Fw64fc* pSrcDst, int len, Fw64f level, FwCmpOp relOp)
	{


		if (level < 0)
		{
			return fwStsThreshNegLevelErr;;
		
		}


		switch (relOp)
		{
		case fwCmpLess:
			if (level <= 0)
				return fwStsNoErr;
			{
				DEF_TRES::LT::Threshold_LT_64fc_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_64fc_I>(data,pSrcDst,len);
			}
			break;
		case fwCmpGreater:
			if (level < 0)
				return fwStsNoErr;
			{
				DEF_TRES::GT::Threshold_GT_64fc_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_64fc_I>(data,pSrcDst,len);
			}
			break;
		default: return fwStsNoErr;
		}
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_16sc_I)(Fw16sc* pSrcDst, int len, Fw16s level, FwCmpOp relOp)
	{
	if (level < 0)
	{
		return fwStsThreshNegLevelErr;
	}

		switch (relOp)
		{
		case fwCmpLess:
			if (level <= 0)
				return fwStsNoErr;
			{
				DEF_TRES::LT::Threshold_LT_16sc_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::LT::Threshold_LT_16sc_I>(data,pSrcDst,len);
			}
			break;
		case fwCmpGreater:
			if (level < 0)
				return fwStsNoErr;
			{
				DEF_TRES::GT::Threshold_GT_16sc_I data(level);
				return OPT_LEVEL::fe<DEF_TRES::GT::Threshold_GT_16sc_I>(data,pSrcDst,len);
			}
			break;
		default: return fwStsNoErr;
		}
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTInv_32f)(const Fw32f* pSrc, Fw32f* pDst, int len, Fw32f level)
	{
		if(level < 0 )
			 return fwStsThreshNegLevelErr;

		if(level == 0)
		{
			DEF_TRES::INV::Threshold_LTInv_32f_LZero data(level);
			OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_32f_LZero>(data,pSrc,pDst,len);
			return data.status;
		}

		DEF_TRES::INV::Threshold_LTInv_32f data(level);
		return OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_32f>(data,pSrc,pDst,len);
	}



	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTInv_64f)(const Fw64f* pSrc, Fw64f* pDst, int len, Fw64f level)
	{
		if(level < 0 )
			 return fwStsThreshNegLevelErr;

		if(level == 0)
		{
			DEF_TRES::INV::Threshold_LTInv_64f_LZero data(level);
			OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_64f_LZero>(data,pSrc,pDst,len);
			return data.status;
		}

		DEF_TRES::INV::Threshold_LTInv_64f data(level);
		return OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_64f>(data,pSrc,pDst,len);
	}

 

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTInv_32fc)(const Fw32fc* pSrc, Fw32fc* pDst, int len, Fw32f level)
	{
		if (level < 0)
			return fwStsThreshNegLevelErr;

		if(level == 0)
		{
			DEF_TRES::INV::Threshold_LTInv_32fc_LZero data(level);
			OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_32fc_LZero>(data,pSrc,pDst,len);
			return data.status;
		}

		DEF_TRES::INV::Threshold_LTInv_32fc data(level);
		return OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_32fc>(data,pSrc,pDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTInv_64fc)(const Fw64fc* pSrc, Fw64fc* pDst, int len, Fw64f level)
	{
		if (level < 0)
			return fwStsThreshNegLevelErr;


		if(level == 0)
		{
			DEF_TRES::INV::Threshold_LTInv_64fc_LZero data(level);
			OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_64fc_LZero>(data,pSrc,pDst,len);
			return data.status;
		}

		DEF_TRES::INV::Threshold_LTInv_64fc data(level);
		return OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_64fc>(data,pSrc,pDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTInv_32f_I)(Fw32f* pSrcDst, int len, Fw32f level)
	{
		if(level < 0 )
			 return fwStsThreshNegLevelErr;

		if(level == 0)
		{
			DEF_TRES::INV::Threshold_LTInv_32f_LZero_I data(level);
			OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_32f_LZero_I>(data,pSrcDst,len);
			return data.status;
		}

		DEF_TRES::INV::Threshold_LTInv_32f_I data(level);
		return OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_32f_I>(data,pSrcDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTInv_64f_I)(Fw64f* pSrcDst, int len, Fw64f level)
	{
		if(level < 0 )
			 return fwStsThreshNegLevelErr;
 
		if(level == 0)
		{
			DEF_TRES::INV::Threshold_LTInv_64f_LZero_I data(level);
			OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_64f_LZero_I>(data,pSrcDst,len);
			return data.status;
		}

		DEF_TRES::INV::Threshold_LTInv_64f_I data(level);
		return OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_64f_I>(data,pSrcDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTInv_32fc_I)(Fw32fc* pSrcDst, int len, Fw32f level)
	{

		if (level < 0)
			return fwStsThreshNegLevelErr;

		if(level == 0)
		{
			DEF_TRES::INV::Threshold_LTInv_32fc_LZero_I data(level);
			OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_32fc_LZero_I>(data,pSrcDst,len);
			return data.status;
		}

		DEF_TRES::INV::Threshold_LTInv_32fc_I data(level);
		return OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_32fc_I>(data,pSrcDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsThreshold_LTInv_64fc_I)(Fw64fc* pSrcDst, int len, Fw64f level)
	{
		if (level < 0)
			return fwStsThreshNegLevelErr;


		if(level == 0)
		{
			DEF_TRES::INV::Threshold_LTInv_64fc_LZero_I data(level);
			OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_64fc_LZero_I>(data,pSrcDst,len);
			return data.status;
		}

		DEF_TRES::INV::Threshold_LTInv_64fc_I data(level);
		return OPT_LEVEL::fe<DEF_TRES::INV::Threshold_LTInv_64fc_I>(data,pSrcDst,len);
	}


	// Magnitude functions
	//

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMagnitude_32f)(const Fw32f* pSrcRe, const Fw32f* pSrcIm, Fw32f* pDst, int len)
	{
		DEF_MAG::Magnitude_32f data;
		return OPT_LEVEL::fe<DEF_MAG::Magnitude_32f>(data,pSrcRe,pSrcIm,pDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMagnitude_64f)(const Fw64f* pSrcRe, const Fw64f* pSrcIm, Fw64f* pDst, int len)
	{
		DEF_MAG::Magnitude_64f data;
		return OPT_LEVEL::fe<DEF_MAG::Magnitude_64f>(data,pSrcRe,pSrcIm,pDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMagnitude_32fc)(const Fw32fc* pSrc, Fw32f* pDst, int len)
	{
		DEF_MAG::Magnitude_32fc data;
		return OPT_LEVEL::fe<DEF_MAG::Magnitude_32fc>(data,pSrc,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMagnitude_64fc)(const Fw64fc* pSrc, Fw64f* pDst, int len)
	{
		DEF_MAG::Magnitude_64fc data;
		return OPT_LEVEL::fe<DEF_MAG::Magnitude_64fc>(data,pSrc,pDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMagnitude_16s32f)(const Fw16s* pSrcRe, const Fw16s* pSrcIm, Fw32f* pDst, int len)
	{
		DEF_MAG::Magnitude_16s32f data;
		return OPT_LEVEL::fe<DEF_MAG::Magnitude_16s32f>(data,pSrcRe,pSrcIm,pDst,len);
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMagnitude_16sc32f)(const Fw16sc* pSrc, Fw32f* pDst, int len)
	{
		DEF_MAG::Magnitude_16sc32f data;
		return OPT_LEVEL::fe<DEF_MAG::Magnitude_16sc32f>(data,pSrc,pDst,len);
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMagnitude_16s_Sfs)(const Fw16s* pSrcRe, const Fw16s* pSrcIm, Fw16s* pDst, int len, int scaleFactor)
	{
		DEF_MAG::Magnitude_16s data(scaleFactor);
		return OPT_LEVEL::fe<DEF_MAG::Magnitude_16s>(data,pSrcRe,pSrcIm,pDst,len);	
	}

	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMagnitude_16sc_Sfs)(const Fw16sc* pSrc, Fw16s* pDst, int len, int scaleFactor)
	{
		DEF_MAG::Magnitude_16sc data(scaleFactor);
		return OPT_LEVEL::fe<DEF_MAG::Magnitude_16sc>(data,pSrc,pDst,len);	
	}
	FwStatus PREFIX_OPT(OPT_PREFIX, fwsMagnitude_32sc_Sfs)(const Fw32sc* pSrc, Fw32s* pDst, int len, int scaleFactor)
	{
		DEF_MAG::Magnitude_32sc data(scaleFactor);
		return OPT_LEVEL::fe<DEF_MAG::Magnitude_32sc>(data,pSrc,pDst,len);	
	}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
