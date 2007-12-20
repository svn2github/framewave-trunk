/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CMC_FUNCTION_OBJECTS_H__
#define __CMC_FUNCTION_OBJECTS_H__

#include "Object.h"
#include "FunctionSignatures.h"

// --------------------
// Channel to Channel
// --------------------
// Even numbered width buffer (422 Sampling)
template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcDstRoiEvenWidth : public SrcDstRoiGeneric< TS, srcChannels, TD, dstChannels, AdviceRoiEvenWidth >
{
    typedef SrcDstRoiGeneric< TS, srcChannels, TD, dstChannels, AdviceRoiEvenWidth > BaseClass;
public:
    SrcDstRoiEvenWidth( UnitTestCatalogBase & parent, const char *name, typename BaseClass::Fn fn )
        : BaseClass( parent, name, fn )
    {
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcDstRoiEven : public SrcDstRoiGeneric< TS, srcChannels, TD, dstChannels, AdviceRoiEvenWidthEvenHeight >
{
    typedef SrcDstRoiGeneric< TS, srcChannels, TD, dstChannels, AdviceRoiEvenWidthEvenHeight > BaseClass;
public:
    SrcDstRoiEven( UnitTestCatalogBase & parent, const char *name, typename BaseClass::Fn fn )
        : BaseClass( parent, name, fn )
    {
    }
};

// --------------------
// Channel to Planar
// --------------------

// --------------------
// Planar to Channel
// --------------------

// --------------------
// Planar to Planar
// --------------------



//
//// Planar to Planar
//template< typename TS, U32 srcPlanes, typename TD, U32 dstPlanes >
//class PSrcPDstRoiEvenWidth : public PSrcPDstRoiGeneric< TS, srcPlanes, TD, dstPlanes, AdviceRoiEvenWidth >
//{
//    typedef PSrcPDstRoiGeneric< TS, srcPlanes, TD, dstPlanes, AdviceRoiEvenWidth > BaseClass;
//public:
//    PSrcPDstRoiEvenWidth( UnitTestCatalogBase & parent, const char *name, typename BaseClass::Fn fn )
//        : PSrcPDstRoiGeneric< TS, srcPlanes, TD, dstPlanes, AdviceRoiEvenWidth >( parent, name ), m_fn(fn)
//    {
//    }
//};
//
//// --------------------------------
//// Buffer width and height are even
//// --------------------------------
//// Channel to Channel
//template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
//class SrcDstRoiEvenWidth : public SrcDstRoiGeneric< TS, srcChannels, TD, dstChannels, AdviceRoiEvenWidth >
//{
//    typedef SrcDstRoiGeneric< TS, srcChannels, TD, dstChannels, AdviceRoiEvenWidth > BaseClass;
//public:
//    SrcDstRoiEvenWidth( UnitTestCatalogBase & parent, const char *name, typename BaseClass::Fn fn )
//        : SrcDstRoiGeneric< TS, srcChannels, TD, dstChannels, AdviceRoiEvenWidth >( parent, name, fn )
//    {
//    }
//};
//
//// Planar to Planar
//template< typename TS, U32 srcPlanes, typename TD, U32 dstPlanes >
//class PSrcPDstRoiEven : public PSrcPDstRoiGeneric< TS, srcPlanes, TD, dstPlanes, AdviceRoiEvenWidthEvenHeight >
//{
//    typedef PSrcPDstRoiGeneric< TS, srcPlanes, TD, dstPlanes, AdviceRoiEvenWidthEvenHeight > BaseClass;
//public:
//    PSrcPDstRoiEven( UnitTestCatalogBase & parent, const char *name, typename BaseClass::Fn fn )
//        : BaseClass( parent, name ), m_fn(fn)
//    {
//    }
//};


//// --------------------------------
//// Function Signature: (const TS* const[], int, TS*[], int[], FwiSize)
//// --------------------------------
//template< typename TS, U32 srcPlanes, typename TD, U32 dstPlanes, class Advice >
//class PSrcPDstRoiMultipleDstStepGeneric : public PDstRoiBase< TD, dstPlanes, Advice >
//{
//public:
//	typedef FwStatus (STDCALL *Fn)( const TS * const [], int, TD *[], int[], FwiSize );
//    const char ** m_srcInit;
//    Fn m_fn;
//
//    PSrcPDstRoiMultipleDstStepGeneric( UnitTestCatalogBase & parent, const char *name, Fn fn )
//        : PDstRoiBase< TD, dstPlanes, Advice >( parent, name ), m_fn(fn)
//    {
//    }
//
//    void RunTest( const char **srcInit, const char **dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
//    {
//        m_srcInit = srcInit;
//        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
//    }
//
//    virtual void CallFn( const Advice & adv, 
//                         FwStatus & stat, 
//                         PlanarBuffer< TD, dstPlanes >& dst )
//    {
//        int dstStep[dstPlanes];
//        U32 srcAlignments[srcPlanes];
//        for( int plane = 0; plane < srcPlanes; plane++ )
//        {
//            srcAlignments[plane] = adv.Alignment();
//        }
//        for( int plane = 0; plane < dstPlanes; plane++ )
//        {
//            dstStep[plane] = dst.StepBytes();
//        }
//
//		PlanarBuffer< TS, srcPlanes > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), srcAlignments );
//        try
//        {
//            this->timer.Start();
//            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dstStep, adv.Roi() );
//            this->timer.Stop();
//        }
//        catch( ... )
//        {
//            this->timer.Stop();
//            stat = (FwStatus)EXCEPTION_ERR;
//        }
//    }
//};


//template< typename TS, U32 srcPlanes, typename TD, U32 dstPlanes >
//class PSrcPDstRoiMultipleDstStep : public PSrcPDstRoiMultipleDstStepGeneric< TS, srcPlanes, TD, dstPlanes, AdviceRoi >
//{
//    typedef PSrcPDstRoiMultipleDstStepGeneric< TS, srcPlanes, TD, dstPlanes, AdviceRoi > BaseClass;
//public:
//    PSrcPDstRoiMultipleDstStep( UnitTestCatalogBase & parent, const char *name, typename BaseClass::Fn fn )
//        : BaseClass( parent, name, fn )
//    {}
//};

//template< typename TS, U32 srcPlanes, typename TD, U32 dstPlanes >
//class PSrcPDstRoiMultipleDstStepEvenWidth : public PSrcPDstRoiMultipleDstStepGeneric< TS, srcPlanes, TD, dstPlanes, AdviceRoiEvenWidth >
//{
//    typedef PSrcPDstRoiMultipleDstStepGeneric< TS, srcPlanes, TD, dstPlanes, AdviceRoiEvenWidth > BaseClass;
//public:
//    PSrcPDstRoiMultipleDstStepEvenWidth( UnitTestCatalogBase & parent, const char *name, typename BaseClass::Fn fn )
//        : BaseClass( parent, name, fn )
//    {}
//};



#endif // __CMC_FUNCTION_OBJECTS_H__

