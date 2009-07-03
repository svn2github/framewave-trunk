/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SCALE_H__
#define __SCALE_H__

#include "fe.h"

namespace OPT_LEVEL
{

/////////////////////////////////////// REF FUNCTIONS ////////////////////////////////////////////

template< class TS, class TD >					// Integer
ISV ScaleI_1(const TS * s, TD * d)
{
	double src_min = (float)FW_REF::Limits<TS>::MinValue();
	double src_max = (float)FW_REF::Limits<TS>::MaxValue();
	double dst_min = (float)FW_REF::Limits<TD>::MinValue();
	double dst_max = (float)FW_REF::Limits<TD>::MaxValue();

	double rd = dst_max - dst_min;
	double rs = src_max - src_min; 

	double k = rd / rs;
	double f = dst_min + k * ( *s - src_min );

	*d = FW_REF::To<TD>::From( f );
}

template<class TS, class TD, CH cs>
ISV ScaleI(const TS * s, TD * d)
{
	for(int i=0;i<cs;i++)
		ScaleI_1(s+i, d+i);	
}

template< class TS, class TD >					// To float
ISV ScaleToF_1(const TS *s, TD *d, const A32F &dst_min, const A32F &dst_max)
{
	float src_min = FW_REF::Limits<TS>::MinValue();
	float src_max = FW_REF::Limits<TS>::MaxValue();
	
	float rd = dst_max - dst_min;
	float rs = src_max - src_min; 

	float k = rd / rs;
	float f = dst_min + k * ( *s - src_min );

	*d = FW_REF::To<TD>::From( f );
}

template< class TS, class TD, CH cs>
ISV ScaleToF(const TS * s, TD * d, const A32F &dst_min, const A32F &dst_max)
{
	for(int i=0;i<cs;i++)
		ScaleToF_1(s+i, d+i, dst_min, dst_max);		
}

template< class TS, class TD >					// From float
ISV ScaleFromF_1(const TS * s, TD * d, const A32F &src_min, const A32F &src_max)
{
	float dst_min = FW_REF::Limits<TD>::MinValue();
	float dst_max = FW_REF::Limits<TD>::MaxValue();

	float rd = dst_max - dst_min;
	float rs = src_max - src_min; 

	float k = rd / rs;
	float f = dst_min + k * ( *s - src_min );
    if( !SYS_FINITE(f) )    // dtoi is not taking care of -ive inf. So this 'if' is put in
        {
        if(f<0) 
            *d = CBL_LIBRARY::Limits<TD>::MinValue();
        else
            *d = CBL_LIBRARY::Limits<TD>::MaxValue();
        return;
        }
	*d = FW_REF::To<TD>::From( f );
}

template< class TS, class TD, CH cs>
ISV ScaleFromF(const TS * s, TD * d, const A32F &src_min, const A32F &src_max)
{
	for(int i=0;i<cs;i++)
		ScaleFromF_1(s+i, d+i, src_min, src_max);
}

template< class TS, class TD >					// Hint
ISV ScaleH_1(const TS *s, TD *d, const FwHintAlgorithm &hint)
{
	hint;										//TODO: Use Hint	
	return ScaleI_1(s, d);				
}

template< class TS, class TD, CH cs>
ISV ScaleH(const TS * s, TD * d, const FwHintAlgorithm &hint)
{
	for(int i=0;i<cs;i++)
		ScaleH_1(s+i, d+i, hint);
}

} // OPT_LEVEL

#endif


