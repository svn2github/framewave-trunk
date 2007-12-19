/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "ImageJaehne.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 9999 )

FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8u_C1R)(Fw8u* pDst, int dstStep, FwiSize roiSize)		{ return JaehneU<A8U,C1> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8u_C3R)(Fw8u* pDst, int dstStep, FwiSize roiSize)		{ return JaehneU<A8U,C3> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8u_C4R)(Fw8u* pDst, int dstStep, FwiSize roiSize)		{ return JaehneU<A8U,C4> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8u_AC4R)(Fw8u* pDst, int dstStep, FwiSize roiSize)		{ return JaehneU<A8U,AC4> (pDst, dstStep, roiSize); }

FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8s_C1R)(Fw8s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A8S,C1> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8s_C3R)(Fw8s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A8S,C3> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8s_C4R)(Fw8s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A8S,C4> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8s_AC4R)(Fw8s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A8S,AC4> (pDst, dstStep, roiSize); }

FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16u_C1R)(Fw16u* pDst, int dstStep, FwiSize roiSize)		{ return JaehneU<A16U,C1> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16u_C3R)(Fw16u* pDst, int dstStep, FwiSize roiSize)		{ return JaehneU<A16U,C3> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16u_C4R)(Fw16u* pDst, int dstStep, FwiSize roiSize)		{ return JaehneU<A16U,C4> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16u_AC4R)(Fw16u* pDst, int dstStep, FwiSize roiSize)		{ return JaehneU<A16U,AC4> (pDst, dstStep, roiSize); }

FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16s_C1R)(Fw16s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A16S,C1> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16s_C3R)(Fw16s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A16S,C3> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16s_C4R)(Fw16s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A16S,C4> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16s_AC4R)(Fw16s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A16S,AC4> (pDst, dstStep, roiSize); }

FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32s_C1R)(Fw32s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A32S,C1> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32s_C3R)(Fw32s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A32S,C3> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32s_C4R)(Fw32s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A32S,C4> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32s_AC4R)(Fw32s* pDst, int dstStep, FwiSize roiSize)		{ return JaehneS<A32S,AC4> (pDst, dstStep, roiSize); }

FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32f_C1R)(Fw32f* pDst, int dstStep, FwiSize roiSize)		{ return JaehneF<A32F,C1> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32f_C3R)(Fw32f* pDst, int dstStep, FwiSize roiSize)		{ return JaehneF<A32F,C3> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32f_C4R)(Fw32f* pDst, int dstStep, FwiSize roiSize)		{ return JaehneF<A32F,C4> (pDst, dstStep, roiSize); }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32f_AC4R)(Fw32f* pDst, int dstStep, FwiSize roiSize)		{ return JaehneF<A32F,AC4> (pDst, dstStep, roiSize); }

#endif


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
