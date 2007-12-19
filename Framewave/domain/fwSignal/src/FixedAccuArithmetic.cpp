/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

/*
	This file contains all the fixed accuracy arithmetic function calls 
*/
#include "Exp.h"
#include "Ln.h"
#include "DivSqrtCbrt.h"
#include "Power.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;

//Inv

FwStatus PREFIX_OPT(OPT_PREFIX, fwsInv_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Inv_32f data;
	return OPT_LEVEL::fe<Inv_32f >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsInv_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Inv_32f data;
	return OPT_LEVEL::fe<Inv_32f >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsInv_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Inv_32f data;
	return OPT_LEVEL::fe<Inv_32f >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsInv_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Inv_64f data;
	return OPT_LEVEL::fe<Inv_64f >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsInv_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Inv_64f data;
	return OPT_LEVEL::fe<Inv_64f >(data, pSrc, pDst, len);
}

//Div

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDiv_32f_A11 )(const Fw32f* pSrc1, const Fw32f* pSrc2, Fw32f* pDst, int len)
{
	Div_32f data;
	return OPT_LEVEL::fe<Div_32f >(data, pSrc1, pSrc2, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsDiv_32f_A21 )(const Fw32f* pSrc1, const Fw32f* pSrc2, Fw32f* pDst, int len){
	Div_32f data;
	return OPT_LEVEL::fe<Div_32f >(data, pSrc1, pSrc2, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsDiv_32f_A24 )(const Fw32f* pSrc1, const Fw32f* pSrc2, Fw32f* pDst, int len){
	Div_32f data;
	return OPT_LEVEL::fe<Div_32f >(data, pSrc1, pSrc2, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsDiv_64f_A50 )(const Fw64f* pSrc1, const Fw64f* pSrc2, Fw64f* pDst, int len){
	Div_64f data;
	return OPT_LEVEL::fe<Div_64f >(data, pSrc1, pSrc2, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsDiv_64f_A53 )(const Fw64f* pSrc1, const Fw64f* pSrc2, Fw64f* pDst, int len){
	Div_64f data;
	return OPT_LEVEL::fe<Div_64f >(data, pSrc1, pSrc2, pDst, len); 
}

//Sqrt
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqrt_32f_A11 )( const Fw32f* pSrc, Fw32f* pDst, int len )
{
	Sqrt_32f data;
	return OPT_LEVEL::fe<Sqrt_32f >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqrt_32f_A21 )( const Fw32f* pSrc, Fw32f* pDst, int len )
{
	Sqrt_32f data;
	return OPT_LEVEL::fe<Sqrt_32f >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqrt_32f_A24 )( const Fw32f* pSrc, Fw32f* pDst, int len )
{
	Sqrt_32f data;
	return OPT_LEVEL::fe<Sqrt_32f >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqrt_64f_A50 )( const Fw64f* pSrc, Fw64f* pDst, int len )
{
	Sqrt_64f data;
	return OPT_LEVEL::fe<Sqrt_64f >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqrt_64f_A53 )( const Fw64f* pSrc, Fw64f* pDst, int len )
{
	Sqrt_64f data;
	return OPT_LEVEL::fe<Sqrt_64f >(data, pSrc, pDst, len);
}

//InvSqrt

FwStatus PREFIX_OPT(OPT_PREFIX, fwsInvSqrt_32f_A11 )( const Fw32f* pSrc, Fw32f* pDst, int len )
{
	InvSqrt_A11 data;
	return OPT_LEVEL::fe< InvSqrt_A11 >(data, pSrc, pDst, len); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsInvSqrt_32f_A21 )( const Fw32f* pSrc, Fw32f* pDst, int len )
{
	InvSqrt_A21 data;
	return OPT_LEVEL::fe< InvSqrt_A21 >(data, pSrc, pDst, len); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsInvSqrt_32f_A24 )( const Fw32f* pSrc, Fw32f* pDst, int len )
{
	InvSqrt_A24 data;
	return OPT_LEVEL::fe< InvSqrt_A24 >(data, pSrc, pDst, len); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsInvSqrt_64f_A50 )( const Fw64f* pSrc, Fw64f* pDst, int len )
{
	InvSqrt_A50 data;
	return OPT_LEVEL::fe< InvSqrt_A50 >(data, pSrc, pDst, len); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsInvSqrt_64f_A53 )( const Fw64f* pSrc, Fw64f* pDst, int len )
{
	InvSqrt_A53 data;
	return OPT_LEVEL::fe< InvSqrt_A53 >(data, pSrc, pDst, len); 
}


//Cbrt
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCbrt_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Cbrt_32f_A11 data;
	return OPT_LEVEL::fe< Cbrt_32f_A11 >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCbrt_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Cbrt_32f_A21 data;
	return OPT_LEVEL::fe< Cbrt_32f_A21 >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCbrt_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{	
	Cbrt_32f_A24 data;
	return OPT_LEVEL::fe< Cbrt_32f_A24 >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCbrt_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Cbrt_64f_A53 data;
	return OPT_LEVEL::fe< Cbrt_64f_A53 >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCbrt_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Cbrt_64f_A53 data;
	return OPT_LEVEL::fe< Cbrt_64f_A53 >(data, pSrc, pDst, len); 
}

//Inv Cbrt
FwStatus PREFIX_OPT(OPT_PREFIX, fwsInvCbrt_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	InvCbrt_32f_A11 data;
	return OPT_LEVEL::fe< InvCbrt_32f_A11 >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsInvCbrt_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	InvCbrt_32f_A21 data;
	return OPT_LEVEL::fe< InvCbrt_32f_A21 >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsInvCbrt_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{	
	InvCbrt_32f_A24 data;
	return OPT_LEVEL::fe< InvCbrt_32f_A24 >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsInvCbrt_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{	
	InvCbrt_64f_A50 data;
	return OPT_LEVEL::fe< InvCbrt_64f_A50 >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsInvCbrt_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	InvCbrt_64f_A53 data;
	return OPT_LEVEL::fe< InvCbrt_64f_A53 >(data, pSrc, pDst, len); 
}

//Pow
FwStatus PREFIX_OPT(OPT_PREFIX, fwsPow_32f_A11 )(const Fw32f* pSrc1, const Fw32f* pSrc2, Fw32f* pDst, int len)
{
	Pow_32f_A11 data;
	return OPT_LEVEL::fe< Pow_32f_A11 >(data, pSrc1, pSrc2, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsPow_32f_A21 )(const Fw32f* pSrc1, const Fw32f* pSrc2, Fw32f* pDst, int len)
{
	Pow_32f data;
	return OPT_LEVEL::fe< Pow_32f >(data, pSrc1, pSrc2, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsPow_32f_A24 )(const Fw32f* pSrc1, const Fw32f* pSrc2, Fw32f* pDst, int len)
{
	Pow_32f data;
	return OPT_LEVEL::fe< Pow_32f >(data, pSrc1, pSrc2, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsPow_64f_A50 )(const Fw64f* pSrc1, const Fw64f* pSrc2, Fw64f* pDst, int len)
{
	Pow_64f data;
	return OPT_LEVEL::fe< Pow_64f >(data, pSrc1, pSrc2, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsPow_64f_A53 )(const Fw64f* pSrc1, const Fw64f* pSrc2, Fw64f* pDst, int len)
{
	Pow_64f data;
	return OPT_LEVEL::fe< Pow_64f >(data, pSrc1, pSrc2, pDst, len); 
}

//Powx
FwStatus PREFIX_OPT(OPT_PREFIX, fwsPowx_32f_A11 )(const Fw32f* pSrc1, const Fw32f ConstValue, Fw32f* pDst, int len)
{
	if(ConstValue == (int)ConstValue)
	{
		PowXint_32f data(ConstValue);
		return OPT_LEVEL::fe< PowXint_32f >(data, pSrc1, pDst, len);
	}
	else
	{
		Powx_A11 data(ConstValue);
		return OPT_LEVEL::fe< Powx_A11 >(data, pSrc1, pDst, len);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsPowx_32f_A21 )(const Fw32f* pSrc1, const Fw32f ConstValue, Fw32f* pDst, int len)
{
	if(ConstValue == (int)ConstValue)
	{
		PowXint_32f data(ConstValue);
		return OPT_LEVEL::fe< PowXint_32f >(data, pSrc1, pDst, len);
	}
	else
	{
		Powx_32f data(ConstValue);
		return OPT_LEVEL::fe< Powx_32f >(data, pSrc1, pDst, len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsPowx_32f_A24 )(const Fw32f* pSrc1, const Fw32f ConstValue, Fw32f* pDst, int len)
{
	if(ConstValue == (int)ConstValue)
	{
		PowXint_32f data(ConstValue);
		return OPT_LEVEL::fe< PowXint_32f >(data, pSrc1, pDst, len);
	}
	else
	{
		Powx_32f data(ConstValue);
		return OPT_LEVEL::fe< Powx_32f >(data, pSrc1, pDst, len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsPowx_64f_A50 )(const Fw64f* pSrc1, const Fw64f ConstValue, Fw64f* pDst, int len)
{
	if(ConstValue == (Fw64s)ConstValue)
	{
		PowXint_64f data(ConstValue);
		return OPT_LEVEL::fe< PowXint_64f >(data, pSrc1, pDst, len);
	}
	else
	{
		Powx_64f data(ConstValue);
		return OPT_LEVEL::fe< Powx_64f >(data, pSrc1, pDst, len);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsPowx_64f_A53 )(const Fw64f* pSrc1,  const Fw64f ConstValue, Fw64f* pDst, int len)
{
    /* The commented code is much faster for integr inputs.. but less accurate*/
	//if(ConstValue == (Fw64s)ConstValue)
	//{
	//	PowXint_64f data(ConstValue);
	//	return OPT_LEVEL::fe< PowXint_64f >(data, pSrc1, pDst, len);
	//}
	//else
	//{
		Powx_64f data(ConstValue);
		return OPT_LEVEL::fe< Powx_64f >(data, pSrc1, pDst, len);
	//}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Exp_A11 data;
	return OPT_LEVEL::fe< Exp_A11 >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Exp_32f_B2<C1> data;
	return OPT_LEVEL::fe<Exp_32f_B2<C1> >(data, pSrc, pDst, len); 
}   
FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Exp_32f_B2<C1> data;
	return OPT_LEVEL::fe<Exp_32f_B2<C1> >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Exp_64f_B2<C1> data;
	return OPT_LEVEL::fe<Exp_64f_B2<C1> >(data, pSrc, pDst, len);
} 
FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Exp_64f_B2<C1> data;
	return OPT_LEVEL::fe<Exp_64f_B2<C1> >(data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Ln_32f_A11 data;
	return OPT_LEVEL::fe<Ln_32f_A11 >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Ln_32f<C1> data;
	return OPT_LEVEL::fe<Ln_32f<C1> >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Ln_32f<C1> data;
	return OPT_LEVEL::fe<Ln_32f<C1> >(data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Ln_64f_B2<C1> data ;
	return OPT_LEVEL::fe< Ln_64f_B2<C1> >(data, pSrc, pDst, len );
} 
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Ln_64f_B2<C1> data ;
	return OPT_LEVEL::fe< Ln_64f_B2<C1> >(data, pSrc, pDst, len );
} 

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLog10_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Log10_A11 data;
	return OPT_LEVEL::fe< Log10_A11 >(data, pSrc, pDst, len );
} 
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLog10_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Log10_A21 data;
	return OPT_LEVEL::fe< Log10_A21 >(data, pSrc, pDst, len );
} 
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLog10_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Log10_A24 data;
	return OPT_LEVEL::fe< Log10_A24 >(data, pSrc, pDst, len );
} 
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLog10_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Log10_A50 data;
	return OPT_LEVEL::fe< Log10_A50 >(data, pSrc, pDst, len );
} 
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLog10_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Log10_A53 data;
	return OPT_LEVEL::fe< Log10_A53 >(data, pSrc, pDst, len );
} 


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
