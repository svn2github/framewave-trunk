/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_LIBRARY_H__
#define __CBL_LIBRARY_H__

#include <math.h>
#include "cbl_common_primitives.h"

namespace CBL_LIBRARY
{

#include "cbl_declarations_ref.h"

#ifndef CBL_REF_ONLY
#include "cbl_declarations_sse2.h"
#include "cbl_declarations_sse3.h"
#include "cbl_declarations_gh.h"
#endif // CBL_REF_ONLY

#define CBL_NOT_OPTIMIZED   1000
#define CBL_SSE2_OPTIMIZED  1001
#define CBL_SSE3_OPTIMIZED  1002
#define CBL_GH_OPTIMIZED    1003

#ifdef CBL_OPTIMIZATION_LEVEL
#if CBL_OPTIMIZATION_LEVEL == CBL_NOT_OPTIMIZED
typedef CBL_REF CBL_OPT;
#elif CBL_OPTIMIZATION_LEVEL == CBL_SSE2_OPTIMIZED
typedef CBL_SSE2 CBL_OPT;
#elif CBL_OPTIMIZATION_LEVEL == CBL_SSE3_OPTIMIZED
typedef CBL_SSE3 CBL_OPT;
#elif CBL_OPTIMIZATION_LEVEL == CBL_GH_OPTIMIZED
typedef CBL_GH CBL_OPT;
#else 
#pragma message "WARNING!  Invalid CBL optimization level!"
typedef CBL_SSE2 CBL_OPT;
#endif
#else // !CBL_OPTIMIZATION_LEVEL
#ifdef CBL_REF_ONLY
typedef CBL_REF CBL_OPT;
#else
typedef CBL_SSE2 CBL_OPT;
#endif // CBL_REF_ONLY
#endif // CBL_OPTIMIZATION_LEVEL

#ifndef CBL_REF_ONLY
#include "cbl_support_primitives_sse2.h"
#endif // CBL_REF_ONLY

#include "cbl_shuffle_primitives.h"
#include "cbl_arithmetic_primitives.h"
#include "cbl_logical_primitives.h"
#include "cbl_transpose_primitives.h"
#include "cbl_memory_primitives.h"
#include "cbl_math_primitives.h"
#include "cbl_pack_unpack_primitives.h"

#ifndef CBL_REF_ONLY
#include "cbl_shuffle_primitives_sse2.h"
#include "cbl_arithmetic_primitives_sse2.h"
#include "cbl_logical_primitives_sse2.h"
#include "cbl_transpose_primitives_sse2.h"
#include "cbl_memory_primitives_sse2.h"
#include "cbl_math_primitives_sse2.h"
#include "cbl_pack_unpack_primitives_sse2.h"
#include "cbl_arithmetic_primitives_sse3.h"
#include "cbl_shuffle_primitives_gh.h"
#include "cbl_memory_primitives_gh.h"
#endif // CBL_REF_ONLY

} // namespace CBL_LIBRARY

#endif // __CBL_LIBRARY_H__
