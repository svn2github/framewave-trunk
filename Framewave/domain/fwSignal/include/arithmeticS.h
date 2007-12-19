/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __ARITHMETICS_H__
#define __ARITHMETICS_H__

#include "foreach.h"


namespace OPT_LEVEL
{

//cbrt 32s

SYS_INLINE STATIC void Cbrt_32s16s_Sfs( const FnDat & dat, const Fw32s * s, CH, Fw16s * d, CH )
{
	if(s[0] == 0) { d[0] = 0; return ;} 
	int scale  = dat.Get< int >( 0 );
	Fw32s sign = 1;
	Fw32f cbrtf ;

	if (s[0] < 0) sign = -1 ;
	cbrtf = powf((float)sign*s[0], 1/3.0F );
	cbrtf = cbrtf * sign ;

	d[0]= FW_REF::Limits<Fw16s>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale(cbrtf, scale )));
}

SYS_INLINE STATIC void Log10_32s_Sfs( const FnDat & dat, const Fw32s * s, CH, Fw32s * d, CH )
{
	int scale  = dat.Get< int >( 0 );
	
	d[0] = (s[0] <= 0 ) ? FW_REF::Limits<A32S>::MinValue() : FW_REF::Limits<A32S>::Sat(FW_REF::float_int(FW_REF::Scale( (10*log10f((float)s[0])), scale )));
}

SYS_INLINE STATIC void Log10_32s_Sfs_I( const FnDat & dat, Fw32s * d, CH )
{
	int scale  = dat.Get< int >( 0 );
	d[0] = (d[0] <= 0 ) ? FW_REF::Limits<A32S>::MinValue() : FW_REF::Limits<A32S>::Sat(FW_REF::float_int(FW_REF::Scale( (10 *log10f((float)d[0])), scale )));
}

} // namespace OPT_LEVEL

#endif __ARITHMETICS_H__

