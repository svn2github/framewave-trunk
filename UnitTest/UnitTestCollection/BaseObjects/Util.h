/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>
#include <sstream>
#include <stdlib.h>
#include <list>
#include "DataTypes.h"

#define COUNT_OF(A) (sizeof(A)/sizeof(A[0]))

#define S8_MIN -128
#define S8_MAX 0x7F
#define U8_MIN 0x0
#define U8_MAX 0xFF
#define S16_MIN -32768
#define S16_MAX 0x7FFF
#define U16_MIN 0x0
#define U16_MAX 0xFFFF
#define S32_MIN 0x80000000l
#define S32_MAX 0x7FFFFFFFl
#define U32_MIN 0x0
#define U32_MAX 0xFFFFFFFFl
#define S64_MIN 0x8000000000000000ll
#define S64_MAX 0x7FFFFFFFFFFFFFFFll
#define U64_MIN 0x0
#define U64_MAX 0xFFFFFFFFFFFFFFFFull
#define F32_MIN -3.402823466e+38F
#define F32_MAX 3.402823466e+38F
#define F64_MIN -1.7976931348623158e+308
#define F64_MAX 1.7976931348623158e+308

typedef std::list< std::string > StringList;

namespace Const
{
    const U32 NAN_32Fi      = 0x7FC00000;
    const U32 INF_32Fi      = 0x7F800000 ;
    const U32 INF_NEG_32Fi  = 0xFF800000 ;
    const U32 IND_32Fi      = 0xFFC00000 ;

    const U32 NAN_64Fi[2]     = {0x0,0x7FF80000};
    const U32 INF_64Fi[2]     = {0x0,0x7FF00000};
    const U32 INF_NEG_64Fi[2] = {0x0,0xFFF00000};
    const U32 IND_64Fi[2]     = {0x0,0xFFF80000};


    const double NAN_64F      = * ( double* ) &NAN_64Fi;
    const double INF_64F      = * ( double* ) &INF_64Fi;
    const double INF_NEG_64F  = * ( double* ) &INF_NEG_64Fi;
    const double IND_64F      = * ( double* ) &IND_64Fi;

    const float NAN_32F       = * ( float*  ) &NAN_32Fi;
    const float INF_32F       = * ( float*  ) &INF_32Fi;
    const float INF_NEG_32F   = * ( float*  ) &INF_NEG_32Fi;
    const float IND_32F       = * ( float*  ) &IND_32Fi;
};

static StringList ToStringList( const std::string & string )
{
	StringList rVal;

	size_t start=0, end=0;      

	while( start<string.length() )
	{
		end = string.find_first_of( "{}[] ", start );

		std::string tmp;
		if( end == start ) tmp = string.substr( start, 1 );
		else               tmp = string.substr( start, end-start);

		if( tmp != " " )
			rVal.push_back( tmp );             
		start = std::max< size_t >( end, end+1 );
	}

	return rVal;
}

//
// To<T1>:From( T2 )
//

template< typename TD >
struct To
{
	template< typename TS >
	static TD From( const TS & s )
    { 
        return static_cast<TD>( s ); 
    }
};

static S64 ToS64( const char * str )
{
	long long val = 0;
	for (; *str; str++) 
		val = 10*val + (*str - '0');
	return val;
}
template< >
struct To< U64 >
{   
	static U64 From( const std::string & s )
	{ 
		if( "MAX" == s ) return U64_MAX;
		if( "MIN" == s ) return U64_MIN;

        return static_cast< U64 >( ToS64( s.c_str() ) );
	}
};

template< > 
struct To< S64 >
{   
	static S64 From( const std::string & s )
	{ 
		if( "MAX" == s ) return S64_MAX;
		if( "MIN" == s ) return S64_MIN;

        return ToS64( s.c_str() );
	}
};

template< > 
struct To< F64 >
{   
	static F64 From( const std::string & s )
	{ 
		if( "MAX" == s ) return F64_MAX;
		if( "MIN" == s ) return F64_MIN;
		if ("INF" == s) return Const::INF_64F;
		if ("INF_NEG" == s) return Const::INF_NEG_64F;

		return atof( s.c_str() );
	}
};

template< > 
struct To< F32 >
{   
	static F32 From( const std::string & s )
	{ 
		if( "MAX" == s ) return F32_MAX;
		if( "MIN" == s ) return F32_MIN;
		if ("INF" == s) return Const::INF_32F;
		if ("INF_NEG" == s) return Const::INF_NEG_32F;

		return static_cast< F32 >( atof( s.c_str() ) );
	}
};

template< > 
struct To< U32 >
{   
	static U32 From( const std::string & s )
	{ 
		if( "MAX" == s ) return U32_MAX;
		if( "MIN" == s ) return U32_MIN;

		return static_cast< U32 >( strtoul(s.c_str(), NULL, 10 ) );
	}
};

template< > 
struct To< S32 >
{   
	static S32 From( const std::string & s )
	{ 
		if( "MAX" == s ) return S32_MAX;
		if( "MIN" == s ) return S32_MIN;

		return static_cast< S32 >( strtol(s.c_str(), NULL, 10) ); 
	} 
};

template< > 
struct To< U16 >
{   
	static U16 From( const std::string & s )
	{ 
		if( "MAX" == s ) return U16_MAX;
		if( "MIN" == s ) return U16_MIN;

		return static_cast< U16 >( strtoul(s.c_str(), NULL, 10 ) );
	}
};

template< > 
struct To< S16 >
{   
	static S16 From( const std::string & s )
	{ 
		if( "MAX" == s ) return S16_MAX;
		if( "MIN" == s ) return S16_MIN;

		return static_cast< S16 >( strtol(s.c_str(), NULL, 10) ); 
	} 
};

template< > 
struct To< U8 >
{   
	static U8 From( const std::string & s )
	{ 
		if( "MAX" == s ) return U8_MAX;
		if( "MIN" == s ) return U8_MIN;

		return static_cast< U8 >( strtoul(s.c_str(), NULL, 10 ) );
	}
};

template< > 
struct To< S8 >
{   
	static S8 From( const std::string & s )
	{ 
		if( "MAX" == s ) return S8_MAX;
		if( "MIN" == s ) return S8_MIN;

		return static_cast< S8 >( strtol(s.c_str(), NULL, 10) ); 
	} 
};

template<>
struct To< Fw16sc >
{   
	static Fw16sc From( const std::string & re, const std::string & im )
	{ 
        Fw16sc rval;
        rval.re = To<S16>::From( re );
        rval.im = To<S16>::From( im );
        return rval;
	}
};

template<>
struct To< Fw32sc >
{   
	static Fw32sc From( const std::string & re, const std::string & im )
	{ 
        Fw32sc rval;
        rval.re = To<S32>::From( re );
        rval.im = To<S32>::From( im );
        return rval;
	}
};

template<>
struct To< Fw64sc >
{   
	static Fw64sc From( const std::string & re, const std::string & im )
	{ 
        Fw64sc rval;
        rval.re = To<S64>::From( re );
        rval.im = To<S64>::From( im );
        return rval;
	}
};

template<>
struct To< Fw32fc >
{   
	static Fw32fc From( const std::string & re, const std::string & im )
	{ 
        Fw32fc rval;
        rval.re = To<F32>::From( re );
        rval.im = To<F32>::From( im );
        return rval;
	}
};

template<>
struct To< Fw64fc >
{   
	static Fw64fc From( const std::string & re, const std::string & im )
	{ 
        Fw64fc rval;
        rval.re = To<F64>::From( re );
        rval.im = To<F64>::From( im );
        return rval;
	}
};

//
// ToStr
//

template < typename T >
static std::string ToStr( const T & val )
{
	std::ostringstream oss;
	oss << val;
	return oss.str();
}

static std::string ToStr( const U8 & val )
{
	std::ostringstream oss;
	oss << static_cast< U16 >( val );
	return oss.str();
}

#endif // __UTIL_H__
