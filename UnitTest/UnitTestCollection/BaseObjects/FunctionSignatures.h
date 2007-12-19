/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __FUNCTION_SIGNATURES_H__
#define __FUNCTION_SIGNATURES_H__

#include "Object.h"

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, class Advice >
class SrcDstRoiGeneric : public DstRoiBase< TD, dstChannels, Advice >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *, int, FwiSize ); 
public:
    Fn m_fn;
    const char * m_srcInit;

    SrcDstRoiGeneric( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, Advice >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const Advice & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcDstRoi : public SrcDstRoiGeneric< TS, srcChannels, TD, dstChannels, AdviceRoi >
{
    typedef SrcDstRoiGeneric< TS, srcChannels, TD, dstChannels, AdviceRoi > BaseClass;
public:
    SrcDstRoi( UnitTestCatalogBase & parent, const char *name, typename BaseClass::Fn fn )
        : SrcDstRoiGeneric< TS, srcChannels, TD, dstChannels, AdviceRoi >( parent, name, fn )
    {
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcDstRoiMask : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *, int, FwiSize, const Fw8u*, int); 
    Fn m_fn;
    const char * m_srcInit;
    const char * m_pMask;

public:
    SrcDstRoiMask( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, const char * pMask, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_pMask = pMask;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer < Fw8u, dstChannels > mask( m_pMask, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try 
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), mask.Ptr(), mask.StepBytes() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcSDstRoi : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *, int, FwiSize );
    Fn m_fn;
    const char * m_srcInit;

public:
    SrcSDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *srcDstInit, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcSDstRoiScale : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *, int, FwiSize, int );
    Fn m_fn;
    const char * m_srcInit;
    int m_scaleFactor;

public:
    SrcSDstRoiScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *srcDstInit, const char *dstExpected, int scaleFactor, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_scaleFactor = scaleFactor;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_scaleFactor );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcDstRoiScale : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *, int, FwiSize, int );
    Fn m_fn;
    const char * m_srcInit;
    int m_scaleFactor;

public:
    SrcDstRoiScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, int scaleFactor, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_scaleFactor = scaleFactor;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_scaleFactor );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcSrcDstRoiScale : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, const TS *, int, TD *, int, FwiSize, int );
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;
    int m_scaleFactor;

public:
    SrcSrcDstRoiScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2, const char *dstExpected, int scaleFactor, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        m_scaleFactor = scaleFactor;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src1( m_srcInit1, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer< TS, srcChannels > src2( m_srcInit2, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2.Ptr(), src2.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_scaleFactor );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcSrcDstRoi : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, const TS *, int, TD *, int, FwiSize);
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;

public:
    SrcSrcDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst)
    {
		ChannelBuffer< TS, srcChannels > src1( m_srcInit1, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer< TS, srcChannels > src2( m_srcInit2, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2.Ptr(), src2.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcValDstRoiScale : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, const TS, TD *, int, FwiSize, int );
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;
    int m_scaleFactor;

public:
    SrcValDstRoiScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2, const char *dstExpected, int scaleFactor, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        m_scaleFactor = scaleFactor;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src1( m_srcInit1, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer< TS, srcChannels > src2( m_srcInit2, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2[0], dst.Ptr(), dst.StepBytes(), adv.Roi(), m_scaleFactor );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};


template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcPValDstRoiScale : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, const TS *, TD *, int, FwiSize, int );
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;
    int m_scaleFactor;

public:
    SrcPValDstRoiScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2, const char *dstExpected, int scaleFactor, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        m_scaleFactor = scaleFactor;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src1( m_srcInit1, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer< TS, srcChannels > src2( m_srcInit2, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2.Ptr(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_scaleFactor );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcValDstRoi : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, const TS, TD *, int, FwiSize);
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;

public:
    SrcValDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst)
    {
		ChannelBuffer< TS, srcChannels > src1( m_srcInit1, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer< TS, srcChannels > src2( m_srcInit2, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2[0], dst.Ptr(), dst.StepBytes(), adv.Roi());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcPValDstRoi : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, const TS *, TD *, int, FwiSize);
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;

public:
    SrcPValDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst)
    {
		ChannelBuffer< TS, srcChannels > src1( m_srcInit1, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer< TS, srcChannels > src2( m_srcInit2, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2.Ptr(), dst.Ptr(), dst.StepBytes(), adv.Roi());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcPlanes, typename TD, U32 dstPlanes >
class PSrcPDstRoi : public PDstRoiBase< TD, dstPlanes, AdviceRoi >
{
public:
	typedef FwStatus (STDCALL *Fn)( const TS * const [], int, TD *[], int, FwiSize );
    const char ** m_srcInit;
    Fn m_fn;

    PSrcPDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : PDstRoiBase< TD, dstPlanes, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char **srcInit, const char **dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         PlanarBuffer< TD, dstPlanes >& dst )
    {
        U32 srcAlignments[srcPlanes];
        for( int plane = 0; plane < srcPlanes; plane++ )
        {
            srcAlignments[plane] = adv.Alignment();
        }

		PlanarBuffer< TS, srcPlanes > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), srcAlignments );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstPlanes >
class SrcPDstRoi : public PDstRoiBase< TD, dstPlanes, AdviceRoi >
{
public:
	typedef FwStatus (STDCALL *Fn)( const TS *, int, TD * const [], int, FwiSize );
    const char * m_srcInit;
    Fn m_fn;

    SrcPDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : PDstRoiBase< TD, dstPlanes, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char **dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         PlanarBuffer< TD, dstPlanes >& dst )
    {
        ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcPlanes, typename TD, U32 dstChannels >
class PSrcDstRoi : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
public:
	typedef FwStatus (STDCALL *Fn)( const TS * const[], int, TD *, int, FwiSize );
    const char ** m_srcInit;
    Fn m_fn;

    PSrcDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char **srcInit, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        U32 srcAlignments[srcPlanes];
        for( int plane = 0; plane < srcPlanes; plane++ )
        {
            srcAlignments[plane] = adv.Alignment();
        }

		PlanarBuffer< TS, srcPlanes > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), srcAlignments );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template<typename TS, U32 srcChannels >
class ValSDstRoiScale : public DstRoiBase< TS, srcChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( TS, TS *, int, FwiSize, int );
    Fn m_fn;
    const char * m_valInit;
    int m_scaleFactor;

public:
    ValSDstRoiScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TS, srcChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *valInit, const char *srcDstInit, const char *dstExpected, int scaleFactor, TS errorMargin = MemOps<TS>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_valInit = valInit;
        m_scaleFactor = scaleFactor;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TS, srcChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_valInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src[0], dst.Ptr(), dst.StepBytes(), adv.Roi(), m_scaleFactor );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template<typename TS, U32 srcChannels >
class PValSDstRoiScale : public DstRoiBase< TS, srcChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, TS *, int, FwiSize, int );
    Fn m_fn;
    const char * m_valInit;
    int m_scaleFactor;

public:
    PValSDstRoiScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TS, srcChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *valInit, const char *srcDstInit, const char *dstExpected, int scaleFactor, TS errorMargin = MemOps<TS>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_valInit = valInit;
        m_scaleFactor = scaleFactor;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TS, srcChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_valInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_scaleFactor );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template<typename TS, U32 srcChannels >
class ValSDstRoi : public DstRoiBase< TS, srcChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( TS, TS *, int, FwiSize );
    Fn m_fn;
    const char * m_valInit;

public:
    ValSDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TS, srcChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *valInit, const char *srcDstInit, const char *dstExpected, TS errorMargin = MemOps<TS>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_valInit = valInit;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TS, srcChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_valInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src[0], dst.Ptr(), dst.StepBytes(), adv.Roi());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template<typename TS, U32 srcChannels >
class PValSDstRoi : public DstRoiBase< TS, srcChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, TS *, int, FwiSize );
    Fn m_fn;
    const char * m_valInit;

public:
    PValSDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TS, srcChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *valInit, const char *srcDstInit, const char *dstExpected, TS errorMargin = MemOps<TS>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_valInit = valInit;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TS, srcChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_valInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), dst.Ptr(), dst.StepBytes(), adv.Roi());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TD, U32 dstChannels >
class SDstRoi : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
public:
	typedef FwStatus (STDCALL *Fn)( TD *, int, FwiSize ); 
    Fn m_fn;

    SDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        ExecuteTest( srcInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

//Val is fixed to int
template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcDstRoiValI : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *, int, FwiSize, int);
    Fn m_fn;
    const char *m_srcInit, *m_valInit;

public:
    SrcDstRoiValI( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *valInit, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
		m_valInit = valInit;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst)
    {
		ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer< int, 1 > val( m_valInit, 1, 1, 0 );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), val[0]);
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcDstRoiVal : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *, int, FwiSize, TS);
    Fn m_fn;
    const char *m_srcInit, *m_valInit;

public:
    SrcDstRoiVal( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *valInit, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
		m_valInit = valInit;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst)
    {
		ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer< TS, 1 > val( m_valInit, 1, 1, 0 );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), val[0]);
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcSrcSDstRoi : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, const TS *, int, TD *, int, FwiSize);
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;

public:
    SrcSrcSDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2,  const char *srcDstInit,  const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst)
    {
		ChannelBuffer< TS, srcChannels > src1( m_srcInit1, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer< TS, srcChannels > src2( m_srcInit2, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2.Ptr(), src2.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcSrcSDstRoiMask : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, const TS *, int, const Fw8u*, int,  TD *, int, FwiSize ); 
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;
    const char * m_pMask;

public:
    SrcSrcSDstRoiMask( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest(const char *srcInit1, const char *srcInit2, const char * pMask, const char *srcDstInit, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
	    m_srcInit2 = srcInit2;
        m_pMask = pMask;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src1( m_srcInit1, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
    	ChannelBuffer< TS, srcChannels > src2( m_srcInit2, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        ChannelBuffer < Fw8u, dstChannels > mask( m_pMask, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2.Ptr(), src2.StepBytes(), mask.Ptr(), mask.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TD, U32 dstChannels >
class SDstRoiScale : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
public:
	typedef FwStatus (STDCALL *Fn)( TD *, int, FwiSize, int ); 
    Fn m_fn;
	int m_scaleFactor;

    SDstRoiScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, int scaleFactor, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
		m_scaleFactor = scaleFactor;
        ExecuteTest( srcInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi(), m_scaleFactor );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};
template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcSDstRoiMask : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, const Fw8u*, int,  TD *, int, FwiSize ); 
    Fn m_fn;
    const char *m_srcInit;
    const char * m_pMask;

public:
    SrcSDstRoiMask( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest(const char *srcInit, const char * pMask, const char *srcDstInit, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
	    m_srcInit = srcInit;
        m_pMask = pMask;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
    	ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        ChannelBuffer < Fw8u, dstChannels > mask( m_pMask, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), mask.Ptr(), mask.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};
template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcSDstRoiWeighted : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *, int, FwiSize, Fw32f );
    Fn m_fn;
    const char * m_srcInit;
    Fw32f m_alpha;

public:
    SrcSDstRoiWeighted( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *srcDstInit, const char *dstExpected, Fw32f alpha, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_alpha = alpha;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_alpha );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcSDstRoiMaskWeighted : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, const Fw8u*, int,  TD *, int, FwiSize , Fw32f); 
    Fn m_fn;
    const char *m_srcInit;
    const char * m_pMask;
    Fw32f m_alpha;

public:
    SrcSDstRoiMaskWeighted( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest(const char *srcInit, const char * pMask, const char *srcDstInit, const char *dstExpected, Fw32f alpha, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
	    m_srcInit = srcInit;
        m_pMask = pMask;
        m_alpha=alpha;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
    	ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        ChannelBuffer < Fw8u, dstChannels > mask( m_pMask, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), mask.Ptr(), mask.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_alpha );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};
template<typename TD, U32 dstChannels >
class ValDstRoi: public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( TD, TD *,int, FwiSize );
    Fn m_fn;
    const char * m_valInit;

public:
    ValDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest(const char *valInit, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_valInit = valInit;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TD, dstChannels > src( m_valInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src[0], dst.Ptr(), dst.StepBytes(), adv.Roi() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template<typename TD, U32 dstChannels >
class PValDstRoi : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const TD *, TD *, int, FwiSize );
    Fn m_fn;
    const char * m_valInit;

public:
    PValDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *valInit, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_valInit = valInit;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TD, dstChannels > src( m_valInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), dst.Ptr(), dst.StepBytes(), adv.Roi() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template<typename TD, U32 dstChannels >
class ValDstRoiMask : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( TD, TD *,int, FwiSize, const Fw8u*, int );
    Fn m_fn;
    const char * m_valInit;
    const char * m_pMask;

public:
    ValDstRoiMask( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest(const char *valInit, const char *dstExpected, const char * pMask, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_valInit = valInit;
        m_pMask = pMask;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TD, dstChannels > src( m_valInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
    	ChannelBuffer < Fw8u, dstChannels > mask( m_pMask, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src[0], dst.Ptr(), dst.StepBytes(), adv.Roi(), mask.Ptr(), mask.StepBytes() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template<typename TD, U32 dstChannels >
class PValDstRoiMask : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const TD *, TD *, int, FwiSize, const Fw8u*, int );
    Fn m_fn;
    const char * m_valInit;
    const char * m_pMask;

public:
    PValDstRoiMask( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *valInit, const char *dstExpected, const char * pMask, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_valInit = valInit;
        m_pMask = pMask;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TD, dstChannels > src( m_valInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        ChannelBuffer < Fw8u, dstChannels > mask( m_pMask, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), dst.Ptr(), dst.StepBytes(), adv.Roi(), mask.Ptr(), mask.StepBytes() );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcValuDstRoi : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, Fw32u, TD *, int, FwiSize);
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;

public:
    SrcValuDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst)
    {
		ChannelBuffer< TS, srcChannels > src1( m_srcInit1, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer< Fw32u, 1 > src2( m_srcInit2, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2[0], dst.Ptr(), dst.StepBytes(), adv.Roi());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels >
class SrcPValuDstRoi : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( const TS *, int, const Fw32u *, TD *, int, FwiSize);
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;

public:
    SrcPValuDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TD, dstChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2, const char *dstExpected, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        ExecuteTest( 0, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst)
    {
		ChannelBuffer< TS, srcChannels > src1( m_srcInit1, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
		ChannelBuffer< Fw32u, srcChannels > src2( m_srcInit2, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2.Ptr(), dst.Ptr(), dst.StepBytes(), adv.Roi());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template<typename TS, U32 srcChannels >
class ValuSDstRoi : public DstRoiBase< TS, srcChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)( Fw32u, TS *, int, FwiSize );
    Fn m_fn;
    const char * m_valInit;

public:
    ValuSDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TS, srcChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *valInit, const char *srcDstInit, const char *dstExpected, TS errorMargin = MemOps<TS>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_valInit = valInit;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TS, srcChannels >& dst )
    {
		ChannelBuffer< Fw32u, 1 > src( m_valInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src[0], dst.Ptr(), dst.StepBytes(), adv.Roi());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template<typename TS, U32 srcChannels >
class PValuSDstRoi : public DstRoiBase< TS, srcChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const Fw32u *, TS *, int, FwiSize );
    Fn m_fn;
    const char * m_valInit;

public:
    PValuSDstRoi( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase< TS, srcChannels, AdviceRoi >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *valInit, const char *srcDstInit, const char *dstExpected, TS errorMargin = MemOps<TS>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_valInit = valInit;
        ExecuteTest( srcDstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TS, srcChannels >& dst )
    {
		ChannelBuffer< Fw32u, srcChannels > src( m_valInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), dst.Ptr(), dst.StepBytes(), adv.Roi());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

#endif // __FUNCTION_SIGNATURES_H__

