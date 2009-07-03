/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
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



/////////////
template <typename TD>
class SDstLenBase : public UnitTestBase
{
	FwStatus func_stat;
    unsigned int testCaseNum;

protected:
    Timer timer;
    int BuffLength;

public:
	SDstLenBase( UnitTestCatalogBase & parent, const char *name )
        : UnitTestBase( parent, name ), testCaseNum( 0 )
	{
        BuffLength = 0;
    }

    void setBufflengh(int bufflen)
    { 
        BuffLength = bufflen;
    }

    int  getBufflengh()
    { 
        return BuffLength;
    }

    virtual void RunAll() = 0;

protected:
    void ExecuteTest( const char * dstInit, const char * dstExpected, TD errorMargin, const FwStatus expectedReturn )
    {
        testCaseNum++;
        if( dstInit == NULL )
            dstInit = "";

        bool isExpectedAvailable = true;
        if( dstExpected == NULL )
        {
            isExpectedAvailable = false;
            dstExpected = "";
        }

        unsigned testNum = 0;
		Advisor<AdviceLen>::ResetCore();
		while( Advisor<AdviceLen>::AdvanceCore() )
		{
            const AdviceLenCore & advCore = Advisor<AdviceLen>::GetCore();

            Buffer< TD > dstCore( dstInit, advCore.BufferLength() );
            Buffer< TD > expected( dstExpected, advCore.BufferLength() );

            bool isGoldSame = true;
            bool isFirst = true;
            Advisor<AdviceLen>::ResetBase();
            while( Advisor<AdviceLen>::AdvanceBase() )
            {
			    const AdviceLen & adv = Advisor<AdviceLen>::Get();
                std::ostringstream oss; 
                oss << "Test " << ++testNum;

                Buffer< TD > dst( dstInit, adv.BufferLength(), adv.Alignment() );

			    CallFn( adv, func_stat, dst );

                std::string msg = oss.str() + std::string(" ") + adv.GetDescription() + timer.GetDescription();

                if( func_stat == EXCEPTION_ERR )
                {
                    ReportException ( oss.str().c_str(), msg.c_str() );
                }
                else
                {
                    bool isExpectedSame = true;
                    if(isExpectedAvailable)
                    {
                        isExpectedSame = Eval( adv, dst, expected, expectedReturn, errorMargin );
                    }
                    else
                    {
                        if(isFirst)
                        {
                            dstCore = dst;
                        }
                        else
                        {
                            isGoldSame = Eval( adv, dst, dstCore, expectedReturn, errorMargin );
                        }
                    }

                    ReportStatus( isGoldSame && isExpectedSame, oss.str().c_str(), msg.c_str() );
                }
                isFirst = false;
            }
		}
	}
	
	virtual bool Eval(const AdviceLen & adv, 
                      const Buffer< TD >& dst,
                      const Buffer< TD >& expected,
                      const FwStatus expectedReturn,
                      TD errorMargin )
	{
		if (func_stat != expectedReturn) {
			Log() << "Status Error, Expected Status: " << expectedReturn << 
				", Returned Status: " << func_stat << std::endl;
			return false;
		}
		if (func_stat==fwStsNullPtrErr) return true;

		int i;
	    const TD *pDst = dst.Ptr();
	    const TD *pDstExp = expected.Ptr();

		for( i=0; i < BuffLength ; ++i )
		{
            if( !MemOps<TD>::Compare( dst[i], pDstExp[i], errorMargin ) )
			{
				Log() << std::endl << "Buffer mismatch at index: " << i << std::endl;
                Log() << "Actual: " << ::ToStr(dst[i]) << std::endl;
				Log() << "Expected: " << ::ToStr(pDstExp[i]) << std::endl;
                return false;
			}
		}

        if( !dst.TestIntegrity() )
        {
            Log() << std::endl << "Memory corruption detected around the destination buffer!" << std::endl;
            return false;
		}
        return true;
	}

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer< TD >& dst ) = 0;

    virtual unsigned int GetTestCaseNum()
    {
        return testCaseNum;
    }
};

/////////////



template< typename TS,typename TD>
class SrcDstLen : public SDstLenBase< TD >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS *, TD *,int); 
public:
    Fn m_fn;
    const char * m_srcInit;
    int BuffLen;

    SrcDstLen( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : SDstLenBase< TD >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected,int len,TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        SDstLenBase<TD>::setBufflengh(len);
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer<TD>& dst)
    {
		Buffer< TS > src( m_srcInit, adv.BufferLength());

        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), dst.Ptr(),SDstLenBase<TD>::getBufflengh());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};




template< typename TS,typename TD>
class SrcConstDstLen : public SDstLenBase< TD >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS *, const TS , TD *,int); 
public:
    Fn m_fn;
    const char * m_srcInit;
    int BuffLen;
    TS ConstVal;

    SrcConstDstLen( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : SDstLenBase< TD >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit,TS const cVal, const char *dstExpected,int len,TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        SDstLenBase<TD>::setBufflengh(len);
        ConstVal = cVal;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer<TD>& dst)
    {
		Buffer< TS > src( m_srcInit, adv.BufferLength());

        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(),ConstVal, dst.Ptr(),SDstLenBase<TD>::getBufflengh());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};


template< typename TS,typename TD>
class SrcDstLenHint : public SDstLenBase< TD >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *,int ,FwHintAlgorithm); 
public:
    Fn m_fn;
    const char * m_srcInit;
    int BuffLen;
    FwHintAlgorithm hint;

    SrcDstLenHint( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : SDstLenBase< TD >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected,int len,FwHintAlgorithm ht= fwAlgHintNone,TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        SDstLenBase<TD>::setBufflengh(len);
        hint = ht;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer<TD>& dst)
    {
		Buffer< TS > src( m_srcInit, adv.BufferLength());

        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(),dst.Ptr(),SDstLenBase<TD>::getBufflengh(),hint);
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS1,typename TS2,typename TD>
class SrcSrcDstLen : public SDstLenBase< TD >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS1 *, const TS2 *,TD *,int); 
public:
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;
    int BuffLen;

    SrcSrcDstLen( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : SDstLenBase< TD >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2,const char *dstExpected,int len,TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        SDstLenBase<TD>::setBufflengh(len);
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer<TD>& dst)
    {

		Buffer< TS1 > src1( m_srcInit1, adv.BufferLength());
        Buffer< TS2 > src2( m_srcInit2, adv.BufferLength());

        this->timer.Start();
        try
        {
           
            stat = m_fn( src1.Ptr(),src2.Ptr(), dst.Ptr(),SDstLenBase<TD>::getBufflengh());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS,typename TD>
class SrcDstLenScale : public SDstLenBase< TD >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS *,TD *,int,int); 
public:
    Fn m_fn;
    const char * m_srcInit;
    int BuffLen;
    int ScaleVal;

    SrcDstLenScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : SDstLenBase< TD >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected,int len,int scale,TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        SDstLenBase<TD>::setBufflengh(len);
        ScaleVal = scale;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer<TD>& dst)
    {
		Buffer< TS > src( m_srcInit, adv.BufferLength());

        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), dst.Ptr(),SDstLenBase<TD>::getBufflengh(),ScaleVal);
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS1,typename TS2,typename TD>
class SrcSrcDstLenScale : public SDstLenBase< TD >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS1 *,const TS2 *,TD *,int,int); 
public:
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;
    int BuffLen;
    int ScaleValue;

    SrcSrcDstLenScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : SDstLenBase< TD >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2,const char *dstExpected,int len,int scale,TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        SDstLenBase<TD>::setBufflengh(len);
        ScaleValue = scale;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer<TD>& dst)
    {
		Buffer< TS1 > src1( m_srcInit1, adv.BufferLength());
        Buffer< TS2 > src2( m_srcInit2, adv.BufferLength());

        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(),src2.Ptr(),dst.Ptr(),SDstLenBase<TD>::getBufflengh(),ScaleValue);
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};


#endif // __FUNCTION_SIGNATURES_H__

