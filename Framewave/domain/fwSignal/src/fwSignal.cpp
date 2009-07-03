/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwSignal.h"
#include <memory.h>

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 100 )
//=======================================
//		fwsGetLibVersion
//=======================================

const FwLibraryVersion* STDCALL fwsGetLibVersion() {
	static FwLibraryVersion lib = *fwGetLibVersion();
	lib.Name = "FwSignal";
	return &lib;	
}

#endif
