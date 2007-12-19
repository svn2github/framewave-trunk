/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "logical.h"
#include "fwImage.h"
//#include "fe.h"

using namespace OPT_LEVEL;

//
// External Functions
//

//Xor

FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C1IR )( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_all::Xor_B2<Fw8u,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2<Fw8u,C1> >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C3IR )( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_all::Xor_B2_C3<Fw8u> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2_C3<Fw8u> >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C4IR )( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_all::Xor_B2<Fw8u,C4> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2<Fw8u,C4> >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_8u_AC4IR)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_8u::Xor_B2_AC4 dat;
	return OPT_LEVEL::fe< Xor_8u::Xor_B2_AC4 >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C1R  )( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_all::Xor_B3<Fw8u,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3<Fw8u,C1> >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C3R  )( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_all::Xor_B3_C3<Fw8u> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3_C3<Fw8u> >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C4R  )( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_all::Xor_B3<Fw8u,C4> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3<Fw8u,C4> >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_8u_AC4R )( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_8u::Xor_B3_AC4 dat;
	return OPT_LEVEL::fe< Xor_8u::Xor_B3_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C1IR )( const Fw16u	*pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_all::Xor_B2<Fw16u,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2<Fw16u,C1> >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C3IR )( const Fw16u	*pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_all::Xor_B2_C3<Fw16u> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2_C3<Fw16u> >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C4IR )( const Fw16u	*pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_all::Xor_B2<Fw16u,C4> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2<Fw16u,C4> >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_16u_AC4IR)( const Fw16u	*pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_16u::Xor_B2_AC4 dat;
	return OPT_LEVEL::fe< Xor_16u::Xor_B2_AC4 >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C1R  )( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_all::Xor_B3<Fw16u,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3<Fw16u,C1> >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C3R  )( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_all::Xor_B3_C3<Fw16u> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3_C3<Fw16u> >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C4R  )( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_all::Xor_B3<Fw16u,C4> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3<Fw16u,C4> >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_16u_AC4R )( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_16u::Xor_B3_AC4 dat;
	return OPT_LEVEL::fe< Xor_16u::Xor_B3_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C1IR )( const Fw32s	*pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_all::Xor_B2<Fw32s,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2<Fw32s,C1> >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C3IR )( const Fw32s	*pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_all::Xor_B2_C3<Fw32s> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2_C3<Fw32s> >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C4IR )( const Fw32s	*pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_all::Xor_B2<Fw32s,C4> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2<Fw32s,C4> >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_32s_AC4IR)( const Fw32s	*pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Xor_32s::Xor_B2_AC4 dat;
	return OPT_LEVEL::fe< Xor_32s::Xor_B2_AC4 >( dat, pSrc, srcStep, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C1R  )( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_all::Xor_B3<Fw32s,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3<Fw32s,C1> >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C3R  )( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_all::Xor_B3_C3<Fw32s> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3_C3<Fw32s> >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C4R  )( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_all::Xor_B3<Fw32s,C4> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3<Fw32s,C4> >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXor_32s_AC4R )( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize)
{ 
	Xor_32s::Xor_B3_AC4 dat;
	return OPT_LEVEL::fe< Xor_32s::Xor_B3_AC4 >( dat, pSrc1, src1Step, pSrc2, src2Step, pDst, dstStep, roiSize ); 
}


//XorC

FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C1IR 	)(       Fw8u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_all::XorC_B1_C1<Fw8u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C1<Fw8u> >( dat, pSrcDst, srcDstStep, roiSize ); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C3IR  )( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_all::XorC_B1_C3<Fw8u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C3<Fw8u> >( dat, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_AC4IR )( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_8u::XorC_B1_AC4 dat(value);
	return OPT_LEVEL::fe< XorC_8u::XorC_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize ); 

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C4IR  )( const Fw8u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_all::XorC_B1_C4<Fw8u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C4<Fw8u> >( dat, pSrcDst, srcDstStep, roiSize ); 

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C1R  	)( const Fw8u   *pSrc, int srcStep,       Fw8u value,    Fw8u *pDst, int dstStep, FwiSize roiSize)
{
	XorC_all::XorC_B2_C1<Fw8u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C1<Fw8u> >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C3R  	)( const Fw8u   *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize)
{
	XorC_all::XorC_B2_C3<Fw8u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C3<Fw8u> >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_AC4R  )( const Fw8u   *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize)
{
	XorC_8u::XorC_B2_AC4 dat(value);
	return OPT_LEVEL::fe< XorC_8u::XorC_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C4R  	)( const Fw8u   *pSrc, int srcStep, const Fw8u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize)
{
	XorC_all::XorC_B2_C4<Fw8u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C4<Fw8u> >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C1IR 	)(       Fw16u value,    Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_all::XorC_B1_C1<Fw16u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C1<Fw16u> >( dat, pSrcDst, srcDstStep, roiSize ); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C3IR  )( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_all::XorC_B1_C3<Fw16u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C3<Fw16u> >( dat, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_AC4IR )( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_16u::XorC_B1_AC4 dat(value);
	return OPT_LEVEL::fe< XorC_16u::XorC_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize ); 

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C4IR  )( const Fw16u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_all::XorC_B1_C4<Fw16u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C4<Fw16u> >( dat, pSrcDst, srcDstStep, roiSize ); 

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C1R  	)( const Fw16u   *pSrc, int srcStep,       Fw16u value,    Fw16u *pDst, int dstStep, FwiSize roiSize)
{
	XorC_all::XorC_B2_C1<Fw16u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C1<Fw16u> >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C3R  	)( const Fw16u   *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize)
{
	XorC_all::XorC_B2_C3<Fw16u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C3<Fw16u> >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_AC4R  )( const Fw16u   *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize)
{
	XorC_16u::XorC_B2_AC4 dat(value);
	return OPT_LEVEL::fe< XorC_16u::XorC_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C4R  	)( const Fw16u   *pSrc, int srcStep, const Fw16u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize)
{
	XorC_all::XorC_B2_C4<Fw16u> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C4<Fw16u> >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C1IR 	)(       Fw32s value,    Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_all::XorC_B1_C1<Fw32s> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C1<Fw32s> >( dat, pSrcDst, srcDstStep, roiSize ); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C3IR  )( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_all::XorC_B1_C3<Fw32s> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C3<Fw32s> >( dat, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_AC4IR )( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_32s::XorC_B1_AC4 dat(value);
	return OPT_LEVEL::fe< XorC_32s::XorC_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize ); 

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C4IR  )( const Fw32s value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	XorC_all::XorC_B1_C4<Fw32s> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C4<Fw32s> >( dat, pSrcDst, srcDstStep, roiSize ); 

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C1R  	)( const Fw32s   *pSrc, int srcStep,       Fw32s value,    Fw32s *pDst, int dstStep, FwiSize roiSize)
{
	XorC_all::XorC_B2_C1<Fw32s> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C1<Fw32s> >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C3R  	)( const Fw32s   *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize)
{
	XorC_all::XorC_B2_C3<Fw32s> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C3<Fw32s> >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_AC4R  )( const Fw32s   *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize)
{
	XorC_32s::XorC_B2_AC4 dat(value);
	return OPT_LEVEL::fe< XorC_32s::XorC_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C4R  	)( const Fw32s   *pSrc, int srcStep, const Fw32s value[4], Fw32s *pDst, int dstStep, FwiSize roiSize)
{
	XorC_all::XorC_B2_C4<Fw32s> dat(value);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C4<Fw32s> >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
