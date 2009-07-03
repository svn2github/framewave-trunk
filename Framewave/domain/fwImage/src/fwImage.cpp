/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include <memory.h>

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 100 )
//=======================================
//		fwiGetLibVersion
//=======================================

const FwLibraryVersion* STDCALL fwiGetLibVersion() {

	static FwLibraryVersion lib = *fwGetLibVersion();
	lib.Name = "FwImage";
	return &lib;
}

#endif
