/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/AddDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddProduct_8u32f_C1IR    )( cA8U  *s1, int s1Step, cA8U *s2,  int s2Step, A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1::AddProduct_8u32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddProduct_8u32f >(data, s1, s1Step, s2, s2Step, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddProduct_8s32f_C1IR    )( const Fw8s  *s1, int s1Step, const Fw8s *s2,  int s2Step, A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1::AddProduct_8s32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddProduct_8s32f >(data, s1, s1Step, s2, s2Step, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddProduct_16u32f_C1IR   )( cA16U *s1, int s1Step, cA16U *s2, int s2Step, A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1::AddProduct_16u32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddProduct_16u32f >(data, s1, s1Step, s2, s2Step, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddProduct_32f_C1IR      )( cA32F *s1, int s1Step, cA32F *s2, int s2Step, A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1::AddProduct_32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddProduct_32f >(data, s1, s1Step, s2, s2Step, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddProduct_8u32f_C1IMR   )( cA8U  *s1, int s1Step, cA8U *s2,  int s2Step, cA8U* m, int mStep, A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1M::AddProduct_8u32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddProduct_8u32f >(data, s1, s1Step, s2, s2Step, m, mStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddProduct_8s32f_C1IMR   )( const Fw8s  *s1, int s1Step, const Fw8s *s2,  int s2Step, cA8U* m, int mStep, A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1M::AddProduct_8s32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddProduct_8s32f >(data, s1, s1Step, s2, s2Step, m, mStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddProduct_16u32f_C1IMR  )( cA16U *s1, int s1Step, cA16U *s2, int s2Step, cA8U* m, int mStep, A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1M::AddProduct_16u32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddProduct_16u32f >(data, s1, s1Step, s2, s2Step, m, mStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddProduct_32f_C1IMR     )( cA32F *s1, int s1Step, cA32F *s2, int s2Step, cA8U* m, int mStep, A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1M::AddProduct_32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddProduct_32f >(data, s1, s1Step, s2, s2Step, m, mStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddSquare_8u32f_C1IR 	   )( cA8U  *s, int sStep, A32F* sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1::AddSquare_8u32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddSquare_8u32f >(data, s, sStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddSquare_8s32f_C1IR 	   )( const Fw8s  *s, int sStep, A32F* sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1::AddSquare_8s32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddSquare_8s32f >(data, s, sStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddSquare_16u32f_C1IR	   )( cA16U *s, int sStep, A32F* sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1::AddSquare_16u32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddSquare_16u32f >(data, s, sStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddSquare_32f_C1IR   	   )( cA32F *s, int sStep, A32F* sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1::AddSquare_32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddSquare_32f >(data, s, sStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddSquare_8u32f_C1IMR    )( cA8U  *s, int sStep, cA8U* m, int mStep, A32F* sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1M::AddSquare_8u32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddSquare_8u32f >(data, s, sStep, m, mStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddSquare_8s32f_C1IMR    )( const Fw8s  *s, int sStep, cA8U* m, int mStep, A32F* sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1M::AddSquare_8s32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddSquare_8s32f >(data, s, sStep, m, mStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddSquare_16u32f_C1IMR   )( cA16U *s, int sStep, cA8U* m, int mStep, A32F* sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1M::AddSquare_16u32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddSquare_16u32f >(data, s, sStep, m, mStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddSquare_32f_C1IMR      )( cA32F *s, int sStep, cA8U* m, int mStep, A32F* sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDX::C1M::AddSquare_32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddSquare_32f >(data, s, sStep, m, mStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_8u32f_C1IR   )( cA8U  *s, int sStep, A32F* sd, int sdStep, ASZ roi, A32F a )
{
	DEF_ADD::ADDX::C1::AddWeighted_8u32f data(a);
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddWeighted_8u32f >(data, s, sStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_8s32f_C1IR   )( const Fw8s  *s, int sStep, A32F* sd, int sdStep, ASZ roi, A32F a )
{
	DEF_ADD::ADDX::C1::AddWeighted_8s32f data(a);
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddWeighted_8s32f >(data, s, sStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_16u32f_C1IR  )( cA16U *s, int sStep, A32F* sd, int sdStep, ASZ roi, A32F a )
{
	DEF_ADD::ADDX::C1::AddWeighted_16u32f data(a);
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddWeighted_16u32f >(data, s, sStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_32f_C1IR     )( cA32F *s, int sStep, A32F* sd, int sdStep, ASZ roi, A32F a )
{
	DEF_ADD::ADDX::C1::AddWeighted_32f data(a);
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddWeighted_32f >(data, s, sStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_8u32f_C1IMR  )( cA8U  *s, int sStep, cA8U* m, int mStep, A32F* sd, int sdStep, ASZ roi, A32F a)
{
	DEF_ADD::ADDX::C1M::AddWeighted_8u32f data(a);
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddWeighted_8u32f >(data, s, sStep, m, mStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_8s32f_C1IMR  )( const Fw8s  *s, int sStep, cA8U* m, int mStep, A32F* sd, int sdStep, ASZ roi, A32F a)
{
	DEF_ADD::ADDX::C1M::AddWeighted_8s32f data(a);
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddWeighted_8s32f >(data, s, sStep, m, mStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_16u32f_C1IMR )( cA16U *s, int sStep, cA8U* m, int mStep, A32F* sd, int sdStep, ASZ roi, A32F a)
{
	DEF_ADD::ADDX::C1M::AddWeighted_16u32f data(a);
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddWeighted_16u32f >(data, s, sStep, m, mStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_32f_C1IMR    )( cA32F *s, int sStep, cA8U* m, int mStep, A32F* sd, int sdStep, ASZ roi, A32F a)
{
	DEF_ADD::ADDX::C1M::AddWeighted_32f data(a);
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1M::AddWeighted_32f >(data, s, sStep, m, mStep, sd, sdStep, roi);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 

