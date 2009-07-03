/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "Support.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

Fw8u* fwiMalloc_8u_C1		(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A8U,C1>(widthPixels, heightPixels, pStepBytes); }
Fw8u* fwiMalloc_8u_C2		(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A8U,C2>(widthPixels, heightPixels, pStepBytes); }
Fw8u* fwiMalloc_8u_C3		(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A8U,C3>(widthPixels, heightPixels, pStepBytes); }
Fw8u* fwiMalloc_8u_C4		(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A8U,C4>(widthPixels, heightPixels, pStepBytes); }
Fw8u* fwiMalloc_8u_AC4	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A8U,AC4>(widthPixels, heightPixels, pStepBytes); }

Fw16u* fwiMalloc_16u_C1	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A16U,C1>(widthPixels, heightPixels, pStepBytes); }
Fw16u* fwiMalloc_16u_C2	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A16U,C2>(widthPixels, heightPixels, pStepBytes); }
Fw16u* fwiMalloc_16u_C3	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A16U,C3>(widthPixels, heightPixels, pStepBytes); }
Fw16u* fwiMalloc_16u_C4	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A16U,C4>(widthPixels, heightPixels, pStepBytes); }
Fw16u* fwiMalloc_16u_AC4	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A16U,AC4>(widthPixels, heightPixels, pStepBytes); }

Fw16s* fwiMalloc_16s_C1	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A16S,C1>(widthPixels, heightPixels, pStepBytes); }
Fw16s* fwiMalloc_16s_C2	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A16S,C2>(widthPixels, heightPixels, pStepBytes); }
Fw16s* fwiMalloc_16s_C3	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A16S,C3>(widthPixels, heightPixels, pStepBytes); }
Fw16s* fwiMalloc_16s_C4	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A16S,C4>(widthPixels, heightPixels, pStepBytes); }
Fw16s* fwiMalloc_16s_AC4	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A16S,AC4>(widthPixels, heightPixels, pStepBytes); }

Fw32s* fwiMalloc_32s_C1	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32S,C1>(widthPixels, heightPixels, pStepBytes); }
Fw32s* fwiMalloc_32s_C2	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32S,C2>(widthPixels, heightPixels, pStepBytes); }
Fw32s* fwiMalloc_32s_C3	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32S,C3>(widthPixels, heightPixels, pStepBytes); }
Fw32s* fwiMalloc_32s_C4	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32S,C4>(widthPixels, heightPixels, pStepBytes); }
Fw32s* fwiMalloc_32s_AC4	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32S,AC4>(widthPixels, heightPixels, pStepBytes); }

Fw32f* fwiMalloc_32f_C1	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32F,C1>(widthPixels, heightPixels, pStepBytes); }
Fw32f* fwiMalloc_32f_C2	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32F,C2>(widthPixels, heightPixels, pStepBytes); }
Fw32f* fwiMalloc_32f_C3	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32F,C3>(widthPixels, heightPixels, pStepBytes); }
Fw32f* fwiMalloc_32f_C4	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32F,C4>(widthPixels, heightPixels, pStepBytes); }
Fw32f* fwiMalloc_32f_AC4	(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32F,AC4>(widthPixels, heightPixels, pStepBytes); }

Fw32sc* fwiMalloc_32sc_C1 (int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32SC,C1>(widthPixels, heightPixels, pStepBytes); }
Fw32sc* fwiMalloc_32sc_C2 (int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32SC,C2>(widthPixels, heightPixels, pStepBytes); }
Fw32sc* fwiMalloc_32sc_C3 (int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32SC,C3>(widthPixels, heightPixels, pStepBytes); }
Fw32sc* fwiMalloc_32sc_C4 (int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32SC,C4>(widthPixels, heightPixels, pStepBytes); }
Fw32sc* fwiMalloc_32sc_AC4(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32SC,AC4>(widthPixels, heightPixels, pStepBytes); }

Fw32fc* fwiMalloc_32fc_C1 (int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32FC,C1>(widthPixels, heightPixels, pStepBytes); }
Fw32fc* fwiMalloc_32fc_C2 (int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32FC,C2>(widthPixels, heightPixels, pStepBytes); }
Fw32fc* fwiMalloc_32fc_C3 (int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32FC,C3>(widthPixels, heightPixels, pStepBytes); }
Fw32fc* fwiMalloc_32fc_C4 (int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32FC,C4>(widthPixels, heightPixels, pStepBytes); }
Fw32fc* fwiMalloc_32fc_AC4(int widthPixels, int heightPixels, int* pStepBytes) { return ImgMalloc<A32FC,AC4>(widthPixels, heightPixels, pStepBytes); }

void fwiFree(void* ptr)	{	fwFree(ptr); }

