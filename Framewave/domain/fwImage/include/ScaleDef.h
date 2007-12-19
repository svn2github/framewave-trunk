/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SCALEDEF_H__
#define __SCALEDEF_H__

#include "Scale.h"

namespace OPT_LEVEL
{

// C1R, C3R, C4R
template<class TS, CH CS, class TD, CH CD>
struct ScaleTo_Integer : fe2<TS, CS, TD, CD>
{
	FE_REF

	IV REFR( const TS *s, TD *d ) const								
		{			
			ScaleI<TS, TD, CS>(s, d);
		}
};

// AC4R
template<class TS, CH CS, class TD, CH CD>
struct ScaleTo_Integer_AC4 : fe2<TS, CS, TD, CD>
{
	FE_REF

	IV REFR( const TS *s, TD *d ) const								
		{			
			ScaleI<TS, TD, C3>(s, d);
		}
};

// C1R, C3R, C4R
template<class TS, CH CS, class TD, CH CD>
struct ScaleTo_Float : fe2<TS, CS, TD, CD>
{
	A32F vMin, vMax;

	FE_REF

	ScaleTo_Float(const A32F &Min, const A32F &Max)
		{
			vMin = Min; vMax = Max;
		}

	IV REFR( const TS *s, TD *d ) const								
		{			
			ScaleToF<TS, TD, CS>(s, d, vMin, vMax);
		}
};

template<class TS, CH CS, class TD, CH CD>
struct ScaleTo_Float_AC4 : fe2<TS, CS, TD, CD>
{
	A32F vMin, vMax;

	FE_REF

	ScaleTo_Float_AC4(const A32F &Min, const A32F &Max)
		{
			vMin = Min; vMax = Max;
		}

	IV REFR( const TS *s, TD *d ) const								
		{			
			ScaleToF<TS, TD, C3>(s, d, vMin, vMax);
		}
};

// C1R, C3R, C4R
template<class TS, CH CS, class TD, CH CD>
struct ScaleFrom_Float : fe2<TS, CS, TD, CD>
{
	A32F vMin, vMax;

	FE_REF

	ScaleFrom_Float(const A32F &Min, const A32F &Max)
		{
			vMin = Min; vMax = Max;
		}

	IV REFR( const TS *s, TD *d ) const								
		{			
			ScaleFromF<TS, TD, CS>(s, d, vMin, vMax);
		}
};

// AC4
template<class TS, CH CS, class TD, CH CD>
struct ScaleFrom_Float_AC4 : fe2<TS, CS, TD, CD>
{
	A32F vMin, vMax;

	FE_REF

	ScaleFrom_Float_AC4(const A32F &Min, const A32F &Max)
		{
			vMin = Min; vMax = Max;
		}

	IV REFR( const TS *s, TD *d ) const								
		{			
			ScaleFromF<TS, TD, C3>(s, d, vMin, vMax);
		}
};

// C1R, C3R, C4R
template<class TS, CH CS, class TD, CH CD>
struct Scale_Hint : fe2<TS, CS, TD, CD>
{
	FwHintAlgorithm hint;

	FE_REF

	Scale_Hint(const FwHintAlgorithm &h)
		{
			hint = h;
		}

	IV REFR( const TS *s, TD *d ) const								
		{			
			ScaleH<TS, TD, CS>(s, d, hint);
		}
};

template<class TS, CH CS, class TD, CH CD>
struct Scale_Hint_AC4 : fe2<TS, CS, TD, CD>
{
	FwHintAlgorithm hint;

	FE_REF

	Scale_Hint_AC4(const FwHintAlgorithm &h)
		{
			hint = h;
		}

	IV REFR( const TS *s, TD *d ) const								
		{			
			ScaleH<TS, TD, C3>(s, d, hint);
		}
};

} // OPT_LEVEL

#endif
