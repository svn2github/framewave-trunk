/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __DATATYPES_H__
#define __DATATYPES_H__

//=======================================
//				Types
//=======================================
typedef signed char			S8;	
typedef unsigned char		U8;
typedef unsigned short		U16;
typedef unsigned int		U32;
typedef unsigned long long	U64;
typedef signed short		S16;
typedef signed int			S32;
typedef signed long long	S64;
typedef float				F32;
typedef double				F64;

//=======================================
//				Complex Types
//=======================================

inline bool operator== ( const Fw16sc & a, const Fw16sc b ){ return (a.re==b.re && a.im==b.im); }
inline bool operator== ( const Fw32sc a, const Fw32sc b ){ return (a.re==b.re && a.im==b.im); }
inline bool operator== ( const Fw32fc a, const Fw32fc b ){ return (a.re==b.re && a.im==b.im); }
inline bool operator== ( const Fw64fc a, const Fw64fc b ){ return (a.re==b.re && a.im==b.im); }
inline bool operator== ( const Fw64sc a, const Fw64sc b ){ return (a.re==b.re && a.im==b.im); }

inline bool operator!= ( const Fw16sc a, const Fw16sc b ){ return !(a == b); }
inline bool operator!= ( const Fw32sc a, const Fw32sc b ){ return !(a == b); }
inline bool operator!= ( const Fw32fc a, const Fw32fc b ){ return !(a == b); }
inline bool operator!= ( const Fw64fc a, const Fw64fc b ){ return !(a == b); }
inline bool operator!= ( const Fw64sc a, const Fw64sc b ){ return !(a == b); }

template< typename T >
static T & operator<< ( T & stream, const Fw16sc b ){ stream << "{" << b.re << "," << b.im << "}"; return stream; }
template< typename T >
static T & operator<< ( T & stream, const Fw32sc b ){ stream << "{" << b.re << "," << b.im << "}"; return stream; }
template< typename T >
static T & operator<< ( T & stream, const Fw64sc b ){ stream << "{" << b.re << "," << b.im << "}"; return stream; }
template< typename T >
static T & operator<< ( T & stream, const Fw32fc b ){ stream << "{" << b.re << "," << b.im << "}"; return stream; }
template< typename T >
static T & operator<< ( T & stream, const Fw64fc b ){ stream << "{" << b.re << "," << b.im << "}"; return stream; }

#endif // __DATATYPES_H__
