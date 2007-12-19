/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwJPEG.h"
#include <memory.h>

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 100 )
//=======================================
//		fwiGetLibVersion
//=======================================

const FwLibraryVersion* STDCALL fwjGetLibVersion() {

	static FwLibraryVersion s_Lib = *fwGetLibVersion();
	s_Lib.Name = "FwJPEG";
	return &s_Lib;
}

#endif
