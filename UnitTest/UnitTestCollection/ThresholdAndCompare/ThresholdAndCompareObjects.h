/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __THRESHOLD_AND_COMPARE_OBJECTS_H__
#define __THRESHOLD_AND_COMPARE_OBJECTS_H__

#include "../BaseObjects/Object.h"

template< typename TD, typename TDT >
class SDstLenThreshold : public DstLenBase< TD >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, TDT, FwCmpOp); 
    Fn m_fn;
    TDT m_threshold;
    FwCmpOp m_fwCmpOp;

public:
    SDstLenThreshold( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstLenBase<TD>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, TDT threshold, FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_threshold = threshold;
        m_fwCmpOp = fwCmpOp;
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer< TD >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), adv.BufferLength(), m_threshold, m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TD, typename TDT >
class SDstLenThreshold_NoCmpOp : public DstLenBase< TD >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, TDT); 
    Fn m_fn;
    TDT m_threshold;

public:
    SDstLenThreshold_NoCmpOp( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstLenBase<TD>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, TDT threshold, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_threshold = threshold;
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer< TD >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), adv.BufferLength(), m_threshold );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TD, typename TDT >
class SDstLenThreshold_NoCmpOp_Val : public DstLenBase< TD >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, TDT, TD); 
    Fn m_fn;
    TDT m_threshold;
    TD m_value;

public:
    SDstLenThreshold_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstLenBase<TD>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, TDT threshold, TD value, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_threshold = threshold;
        m_value = value;
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer< TD >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), adv.BufferLength(), m_threshold, m_value );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TD, typename TDT >
class SDstLenThreshold_Double_NoCmpOp_Val : public DstLenBase< TD >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, TDT, TD, TDT, TD); 
    Fn m_fn;
    TDT m_threshold_a;
    TD m_value_a;
    TDT m_threshold_b;
    TD m_value_b;

public:
    SDstLenThreshold_Double_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstLenBase<TD>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, TDT threshold_a, TD value_a, TDT threshold_b, TD value_b, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_threshold_a = threshold_a;
        m_value_a = value_a;
        m_threshold_b = threshold_b;
        m_value_b = value_b;
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer< TD >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), adv.BufferLength(), m_threshold_a, m_value_a, m_threshold_b, m_value_b );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, typename TD, typename TDT >
class SrcDstLenThreshold : public DstLenBase< TD >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, TD *, int, TDT, FwCmpOp); 
    Fn m_fn;
    const char * m_srcInit;
    TDT m_threshold;
    FwCmpOp m_fwCmpOp;

public:
    SrcDstLenThreshold( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstLenBase<TD>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TDT threshold, FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_threshold = threshold;
        m_fwCmpOp = fwCmpOp;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer< TD >& dst )
    {
        Buffer<TS> src( m_srcInit, adv.BufferLength(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), dst.Ptr(), adv.BufferLength(), m_threshold, m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, typename TD, typename TDT >
class SrcDstLenThreshold_NoCmpOp : public DstLenBase< TD >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, TD *, int, TDT); 
    Fn m_fn;
    const char * m_srcInit;
    TDT m_threshold;

public:
    SrcDstLenThreshold_NoCmpOp( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstLenBase<TD>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TDT threshold, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_threshold = threshold;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer< TD >& dst )
    {
        Buffer<TS> src( m_srcInit, adv.BufferLength(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), dst.Ptr(), adv.BufferLength(), m_threshold );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, typename TD, typename TDT >
class SrcDstLenThreshold_NoCmpOp_Val : public DstLenBase< TD >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, TD *, int, TDT, TD); 
    Fn m_fn;
    const char * m_srcInit;
    TDT m_threshold;
    TD m_value;

public:
    SrcDstLenThreshold_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstLenBase<TD>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TDT threshold, TD value, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_threshold = threshold;
        m_value = value;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer< TD >& dst )
    {
        Buffer<TS> src( m_srcInit, adv.BufferLength(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), dst.Ptr(), adv.BufferLength(), m_threshold, m_value );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, typename TD, typename TDT >
class SrcDstLenThreshold_Double_NoCmpOp_Val : public DstLenBase< TD >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, TD *, int, TDT, TD, TDT, TD); 
    Fn m_fn;
    const char * m_srcInit;
    TDT m_threshold_a;
    TD m_value_a;
    TDT m_threshold_b;
    TD m_value_b;

public:
    SrcDstLenThreshold_Double_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstLenBase<TD>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TDT threshold_a, TD value_a, TDT threshold_b, TD value_b, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_threshold_a = threshold_a;
        m_value_a = value_a;
        m_threshold_b = threshold_b;
        m_value_b = value_b;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer< TD >& dst )
    {
        Buffer<TS> src( m_srcInit, adv.BufferLength(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), dst.Ptr(), adv.BufferLength(), m_threshold_a, m_value_a, m_threshold_b, m_value_b );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

// ==================
// ==================
// ==================

template< typename TD >
class SDstRoiThresholdSingleChannel : public DstRoiBase< TD, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, FwiSize, TD, FwCmpOp); 
    Fn m_fn;
    TD m_threshold;
    FwCmpOp m_fwCmpOp;

public:
    SDstRoiThresholdSingleChannel( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, TD threshold, FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_threshold = threshold;
        m_fwCmpOp = fwCmpOp;
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, 1 >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, typename TD >
class SrcDstRoiThresholdSingleChannel : public DstRoiBase< TD, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, int, TD *, int, FwiSize, TD, FwCmpOp); 
    Fn m_fn;
    const char * m_srcInit;
    TD m_threshold;
    FwCmpOp m_fwCmpOp;

public:
    SrcDstRoiThresholdSingleChannel( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TD threshold, FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_threshold = threshold;
        m_fwCmpOp = fwCmpOp;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, 1 >& dst )
    {
		ChannelBuffer< TS, 1 > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TD, U32 dstChannels, U32 constChannels >
class SDstRoiThresholdMultiChannel : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, FwiSize, const TD [], FwCmpOp); 
    Fn m_fn;
    TD m_threshold[constChannels];
    FwCmpOp m_fwCmpOp;

public:
    SDstRoiThresholdMultiChannel( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,dstChannels, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, const TD threshold[constChannels], FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        for(unsigned int i=0; i<constChannels; i++)
        {
            m_threshold[i] = threshold[i];
        }
        m_fwCmpOp = fwCmpOp;
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcDstRoiThresholdMultiChannel_Internal : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
protected:
	typedef FwStatus (STDCALL *Fn)(const TS *, int, TD *, int, FwiSize, const TD [], FwCmpOp); 

private:
    Fn m_fn;
    const char * m_srcInit;
    TD m_threshold[constChannels];
    FwCmpOp m_fwCmpOp;

public:
    SrcDstRoiThresholdMultiChannel_Internal( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,dstChannels, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void StoreValues(const char *srcInit, const TD threshold[constChannels], FwCmpOp fwCmpOp)
    {
        m_srcInit = srcInit;
        for(unsigned int i=0; i<constChannels; i++)
        {
            m_threshold[i] = threshold[i];
        }
        m_fwCmpOp = fwCmpOp;
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcDstRoiThresholdMultiChannel : public SrcDstRoiThresholdMultiChannel_Internal< TS, srcChannels, TD, dstChannels, constChannels >
{
    typedef SrcDstRoiThresholdMultiChannel_Internal< TS, srcChannels, TD, dstChannels, constChannels > Base;

public:
    SrcDstRoiThresholdMultiChannel( UnitTestCatalogBase & parent, const char *name, typename Base::Fn fn )
        : Base( parent, name, fn )
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, const TD threshold[constChannels], FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        StoreValues(srcInit, threshold, fwCmpOp);
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcSDstRoiThresholdMultiChannel : public SrcDstRoiThresholdMultiChannel_Internal< TS, srcChannels, TD, dstChannels, constChannels >
{
    typedef SrcDstRoiThresholdMultiChannel_Internal< TS, srcChannels, TD, dstChannels, constChannels > Base;

public:
    SrcSDstRoiThresholdMultiChannel( UnitTestCatalogBase & parent, const char *name, typename Base::Fn fn )
        : Base( parent, name, fn )
    {
    }

    void RunTest( const char *srcInit, const char *dstInit, const char *dstExpected, const TD threshold[constChannels], FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        StoreValues(srcInit, threshold, fwCmpOp);
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }
};

// ==================

template< typename TD >
class SDstRoiThresholdSingleChannel_NoCmpOp : public DstRoiBase< TD, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, FwiSize, TD); 
    Fn m_fn;
    TD m_threshold;

public:
    SDstRoiThresholdSingleChannel_NoCmpOp( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, TD threshold, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_threshold = threshold;
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, 1 >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, typename TD >
class SrcDstRoiThresholdSingleChannel_NoCmpOp : public DstRoiBase< TD, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, int, TD *, int, FwiSize, TD); 
    Fn m_fn;
    TD m_threshold;
    const char * m_srcInit;

public:
    SrcDstRoiThresholdSingleChannel_NoCmpOp( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TD threshold, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_threshold = threshold;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, 1 >& dst )
    {
        ChannelBuffer< TS, 1 > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TD, U32 dstChannels, U32 constChannels >
class SDstRoiThresholdMultiChannel_NoCmpOp : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, FwiSize, const TD []); 
    Fn m_fn;
    TD m_threshold[constChannels];

public:
    SDstRoiThresholdMultiChannel_NoCmpOp( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,dstChannels, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, const TD threshold[constChannels], TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        for(unsigned int i=0; i<constChannels; i++)
        {
            m_threshold[i] = threshold[i];
        }
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcDstRoiThresholdMultiChannel_NoCmpOp_Internal : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
protected:
	typedef FwStatus (STDCALL *Fn)(const TS *, int, TD *, int, FwiSize, const TD []); 

private:
    Fn m_fn;
    TD m_threshold[constChannels];
    const char * m_srcInit;

public:
    SrcDstRoiThresholdMultiChannel_NoCmpOp_Internal( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,dstChannels, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void StoreValues(const char *srcInit, const TD threshold[constChannels])
    {
        m_srcInit = srcInit;
        for(unsigned int i=0; i<constChannels; i++)
        {
            m_threshold[i] = threshold[i];
        }
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcDstRoiThresholdMultiChannel_NoCmpOp : public SrcDstRoiThresholdMultiChannel_NoCmpOp_Internal< TS, srcChannels, TD, dstChannels, constChannels >
{
    typedef SrcDstRoiThresholdMultiChannel_NoCmpOp_Internal< TS, srcChannels, TD, dstChannels, constChannels > Base;

public:
    SrcDstRoiThresholdMultiChannel_NoCmpOp( UnitTestCatalogBase & parent, const char *name, typename Base::Fn fn )
        : Base( parent, name, fn )
    {}

    void RunTest( const char *srcInit, const char *dstExpected, const TD threshold[constChannels], TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        StoreValues(srcInit, threshold);
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcSDstRoiThresholdMultiChannel_NoCmpOp : public SrcDstRoiThresholdMultiChannel_NoCmpOp_Internal< TS, srcChannels, TD, dstChannels, constChannels >
{
    typedef SrcDstRoiThresholdMultiChannel_NoCmpOp_Internal< TS, srcChannels, TD, dstChannels, constChannels > Base;

public:
    SrcSDstRoiThresholdMultiChannel_NoCmpOp( UnitTestCatalogBase & parent, const char *name, typename Base::Fn fn )
        : Base( parent, name, fn )
    {}

    void RunTest( const char *srcInit, const char *dstInit, const char *dstExpected, const TD threshold[constChannels], TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        StoreValues(srcInit, threshold);
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }
};

// ==================

template< typename TD >
class SDstRoiThresholdSingleChannel_Val : public DstRoiBase< TD, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, FwiSize, TD, TD, FwCmpOp); 
    Fn m_fn;
    TD m_threshold;
    TD m_value;
    FwCmpOp m_fwCmpOp;

public:
    SDstRoiThresholdSingleChannel_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, TD threshold, TD value, FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_threshold = threshold;
        m_value = value;
        m_fwCmpOp = fwCmpOp;
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, 1 >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_value, m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, typename TD >
class SrcDstRoiThresholdSingleChannel_Val : public DstRoiBase< TD, 1, AdviceRoi >
{
protected:
	typedef FwStatus (STDCALL *Fn)(const TS *, int, TD *, int, FwiSize, TD, TD, FwCmpOp); 

private:
    Fn m_fn;
    TD m_threshold;
    TD m_value;
    FwCmpOp m_fwCmpOp;
    const char * m_srcInit;

public:
    SrcDstRoiThresholdSingleChannel_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TD threshold, TD value, FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_threshold = threshold;
        m_value = value;
        m_fwCmpOp = fwCmpOp;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, 1 >& dst )
    {
        ChannelBuffer< TS, 1 > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_value, m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TD, U32 dstChannels, U32 constChannels >
class SDstRoiThresholdMultiChannel_Val : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, FwiSize, const TD [], const TD [], FwCmpOp); 
    Fn m_fn;
    TD m_threshold[constChannels];
    TD m_value[constChannels];
    FwCmpOp m_fwCmpOp;

public:
    SDstRoiThresholdMultiChannel_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,dstChannels, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, const TD threshold[constChannels], const TD value[constChannels], FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_fwCmpOp = fwCmpOp;
        for(unsigned int i=0; i<constChannels; i++)
        {
            m_threshold[i] = threshold[i];
            m_value[i] = value[i];
        }
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_value, m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcDstRoiThresholdMultiChannel_Val_Internal : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
protected:
	typedef FwStatus (STDCALL *Fn)(const TS *, int, TD *, int, FwiSize, const TD [], const TD [], FwCmpOp); 

private:
    Fn m_fn;
    TD m_threshold[constChannels];
    TD m_value[constChannels];
    FwCmpOp m_fwCmpOp;
    const char * m_srcInit;

public:
    SrcDstRoiThresholdMultiChannel_Val_Internal( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,dstChannels, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void StoreValues(const char *srcInit, const TD threshold[constChannels], const TD value[constChannels], FwCmpOp fwCmpOp)
    {
        m_srcInit = srcInit;
        m_fwCmpOp = fwCmpOp;
        for(unsigned int i=0; i<constChannels; i++)
        {
            m_value[i] = value[i];
            m_threshold[i] = threshold[i];
        }
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_value, m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcDstRoiThresholdMultiChannel_Val : public SrcDstRoiThresholdMultiChannel_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels >
{
    typedef SrcDstRoiThresholdMultiChannel_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels > Base;

public:
    SrcDstRoiThresholdMultiChannel_Val( UnitTestCatalogBase & parent, const char *name, typename Base::Fn fn )
        : Base( parent, name, fn )
    {}

    void RunTest( const char *srcInit, const char *dstExpected, const TD threshold[constChannels], const TD value[constChannels], FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        StoreValues(srcInit, threshold, value, fwCmpOp);
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcSDstRoiThresholdMultiChannel_Val : public SrcDstRoiThresholdMultiChannel_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels >
{
    typedef SrcDstRoiThresholdMultiChannel_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels > Base;

public:
    SrcSDstRoiThresholdMultiChannel_Val( UnitTestCatalogBase & parent, const char *name, typename Base::Fn fn )
        : Base( parent, name, fn )
    {}

    void RunTest( const char *srcInit, const char *dstInit, const char *dstExpected, const TD threshold[constChannels], const TD value[constChannels], FwCmpOp fwCmpOp, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        StoreValues(srcInit, threshold, value, fwCmpOp);
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }
};

// ==================

template< typename TD >
class SDstRoiThresholdSingleChannel_NoCmpOp_Val : public DstRoiBase< TD, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, FwiSize, TD, TD); 
    Fn m_fn;
    TD m_threshold;
    TD m_value;

public:
    SDstRoiThresholdSingleChannel_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, TD threshold, TD value, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_threshold = threshold;
        m_value = value;
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, 1 >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_value );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, typename TD >
class SrcDstRoiThresholdSingleChannel_NoCmpOp_Val : public DstRoiBase< TD, 1, AdviceRoi >
{
protected:
	typedef FwStatus (STDCALL *Fn)(const TS *, int, TD *, int, FwiSize, TD, TD); 

private:
    Fn m_fn;
    TD m_threshold;
    TD m_value;
    const char * m_srcInit;

public:
    SrcDstRoiThresholdSingleChannel_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TD threshold, TD value, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_threshold = threshold;
        m_value = value;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, 1 >& dst )
    {
        ChannelBuffer< TS, 1 > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_value );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TD, U32 dstChannels, U32 constChannels >
class SDstRoiThresholdMultiChannel_NoCmpOp_Val : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, FwiSize, const TD [], const TD []); 
    Fn m_fn;
    TD m_threshold[constChannels];
    TD m_value[constChannels];

public:
    SDstRoiThresholdMultiChannel_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,dstChannels, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, const TD threshold[constChannels], const TD value[constChannels], TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        for(unsigned int i=0; i<constChannels; i++)
        {
            m_threshold[i] = threshold[i];
            m_value[i] = value[i];
        }
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_value );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcDstRoiThresholdMultiChannel_NoCmpOp_Val_Internal : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
protected:
	typedef FwStatus (STDCALL *Fn)(const TS *, int, TD *, int, FwiSize, const TD [], const TD []); 

private:
    Fn m_fn;
    TD m_threshold[constChannels];
    TD m_value[constChannels];
    const char * m_srcInit;

public:
    SrcDstRoiThresholdMultiChannel_NoCmpOp_Val_Internal( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,dstChannels, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void StoreValues(const char *srcInit, const TD threshold[constChannels], const TD value[constChannels])
    {
        m_srcInit = srcInit;
        for(unsigned int i=0; i<constChannels; i++)
        {
            m_value[i] = value[i];
            m_threshold[i] = threshold[i];
        }
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold, m_value );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcDstRoiThresholdMultiChannel_NoCmpOp_Val : public SrcDstRoiThresholdMultiChannel_NoCmpOp_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels >
{
    typedef SrcDstRoiThresholdMultiChannel_NoCmpOp_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels > Base;

public:
    SrcDstRoiThresholdMultiChannel_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, typename Base::Fn fn )
        : Base( parent, name, fn )
    {}

    void RunTest( const char *srcInit, const char *dstExpected, const TD threshold[constChannels], const TD value[constChannels], TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        StoreValues(srcInit, threshold, value);
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcSDstRoiThresholdMultiChannel_NoCmpOp_Val : public SrcDstRoiThresholdMultiChannel_NoCmpOp_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels >
{
    typedef SrcDstRoiThresholdMultiChannel_NoCmpOp_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels > Base;

public:
    SrcSDstRoiThresholdMultiChannel_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, typename Base::Fn fn )
        : Base( parent, name, fn )
    {}

    void RunTest( const char *srcInit, const char *dstInit, const char *dstExpected, const TD threshold[constChannels], const TD value[constChannels], TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        StoreValues(srcInit, threshold, value);
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }
};

// ==================

template< typename TD >
class SDstRoiThresholdSingleChannel_Double_NoCmpOp_Val : public DstRoiBase< TD, 1, AdviceRoi >
{
    typedef FwStatus (STDCALL *Fn)(TD *, int, FwiSize, TD, TD, TD, TD); 
    Fn m_fn;
    TD m_threshold_a;
    TD m_value_a;
    TD m_threshold_b;
    TD m_value_b;

public:
    SDstRoiThresholdSingleChannel_Double_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, TD threshold_a, TD value_a, TD threshold_b, TD value_b, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_threshold_a = threshold_a;
        m_value_a = value_a;
        m_threshold_b = threshold_b;
        m_value_b = value_b;
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, 1 >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold_a, m_value_a, m_threshold_b, m_value_b );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, typename TD >
class SrcDstRoiThresholdSingleChannel_Double_NoCmpOp_Val : public DstRoiBase< TD, 1, AdviceRoi >
{
    typedef FwStatus (STDCALL *Fn)(const TS *, int, TD *, int, FwiSize, TD, TD, TD, TD); 
    Fn m_fn;
    TD m_threshold_a;
    TD m_value_a;
    TD m_threshold_b;
    TD m_value_b;
    const char * m_srcInit;

public:
    SrcDstRoiThresholdSingleChannel_Double_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TD threshold_a, TD value_a, TD threshold_b, TD value_b, TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_threshold_a = threshold_a;
        m_value_a = value_a;
        m_threshold_b = threshold_b;
        m_value_b = value_b;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, 1 >& dst )
    {
        ChannelBuffer< TS, 1 > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold_a, m_value_a, m_threshold_b, m_value_b );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TD, U32 dstChannels, U32 constChannels >
class SDstRoiThresholdMultiChannel_Double_NoCmpOp_Val : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(TD *, int, FwiSize, const TD [], const TD [], const TD [], const TD []); 
    Fn m_fn;
    TD m_threshold_a[constChannels];
    TD m_value_a[constChannels];
    TD m_threshold_b[constChannels];
    TD m_value_b[constChannels];

public:
    SDstRoiThresholdMultiChannel_Double_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,dstChannels, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *dstInit, const char *dstExpected, const TD threshold_a[constChannels], const TD value_a[constChannels], const TD threshold_b[constChannels], const TD value_b[constChannels], TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        for(unsigned int i=0; i<constChannels; i++)
        {
            m_threshold_a[i] = threshold_a[i];
            m_value_a[i] = value_a[i];
            m_threshold_b[i] = threshold_b[i];
            m_value_b[i] = value_b[i];
        }
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        this->timer.Start();
        try
        {
            stat = m_fn( dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold_a, m_value_a, m_threshold_b, m_value_b );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcDstRoiThresholdMultiChannel_Double_NoCmpOp_Val_Internal : public DstRoiBase< TD, dstChannels, AdviceRoi >
{
protected:
	typedef FwStatus (STDCALL *Fn)(const TS *, int, TD *, int, FwiSize, const TD [], const TD [], const TD [], const TD []); 

private:
    Fn m_fn;
    TD m_threshold_a[dstChannels];
    TD m_value_a[dstChannels];
    TD m_threshold_b[dstChannels];
    TD m_value_b[dstChannels];
    const char * m_srcInit;

public:
    SrcDstRoiThresholdMultiChannel_Double_NoCmpOp_Val_Internal( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<TD,dstChannels, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void StoreValues(const char *srcInit, const TD threshold_a[dstChannels], const TD value_a[dstChannels], const TD threshold_b[dstChannels], const TD value_b[dstChannels])
    {
        m_srcInit = srcInit;
        for(unsigned int i=0; i<dstChannels; i++)
        {
            m_threshold_a[i] = threshold_a[i];
            m_value_a[i] = value_a[i];
            m_threshold_b[i] = threshold_b[i];
            m_value_b[i] = value_b[i];
        }
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst )
    {
        ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_threshold_a, m_value_a, m_threshold_b, m_value_b );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcDstRoiThresholdMultiChannel_Double_NoCmpOp_Val : public SrcDstRoiThresholdMultiChannel_Double_NoCmpOp_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels >
{
    typedef SrcDstRoiThresholdMultiChannel_Double_NoCmpOp_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels > Base;

public:
    SrcDstRoiThresholdMultiChannel_Double_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, typename Base::Fn fn )
        : Base( parent, name, fn )
    {}

    void RunTest( const char *srcInit, const char *dstExpected, const TD threshold_a[constChannels], const TD value_a[constChannels], const TD threshold_b[constChannels], const TD value_b[constChannels], TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        StoreValues(srcInit, threshold_a, value_a, threshold_b, value_b);
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }
};

template< typename TS, U32 srcChannels, typename TD, U32 dstChannels, U32 constChannels >
class SrcSDstRoiThresholdMultiChannel_Double_NoCmpOp_Val : public SrcDstRoiThresholdMultiChannel_Double_NoCmpOp_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels >
{
    typedef SrcDstRoiThresholdMultiChannel_Double_NoCmpOp_Val_Internal< TS, srcChannels, TD, dstChannels, constChannels > Base;

public:
    SrcSDstRoiThresholdMultiChannel_Double_NoCmpOp_Val( UnitTestCatalogBase & parent, const char *name, typename Base::Fn fn )
        : Base( parent, name, fn )
    {}

    void RunTest( const char *srcInit, const char *dstInit, const char *dstExpected, const TD threshold_a[constChannels], const TD value_a[constChannels], const TD threshold_b[constChannels], const TD value_b[constChannels], TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        StoreValues(srcInit, threshold_a, value_a, threshold_b, value_b);
        ExecuteTest( dstInit, dstExpected, errorMargin, expectedReturn );
    }
};

// ==================
// ==================
// ==================

template< typename TS, U32 srcChannels >
class SrcDstRoiCompare : public DstRoiBase< U8, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, int, const TS *, int, U8 *, int, FwiSize, FwCmpOp); 
    Fn m_fn;
    const char * m_src1Init;
    const char * m_src2Init;
    FwCmpOp m_fwCmpOp;

public:
    SrcDstRoiCompare( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<U8,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *src1Init, const char *src2Init, const char *dstExpected, FwCmpOp fwCmpOp, U8 errorMargin = MemOps<U8>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_src1Init = src1Init;
        m_src2Init = src2Init;
        m_fwCmpOp = fwCmpOp;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< U8, 1 >& dst )
    {
		ChannelBuffer< TS, srcChannels > src1( m_src1Init, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        ChannelBuffer< TS, srcChannels > src2( m_src2Init, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2.Ptr(), src2.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS >
class SrcDstRoiCompareCSingleChannel : public DstRoiBase< U8, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, int, TS, U8 *, int, FwiSize, FwCmpOp); 
    Fn m_fn;
    const char * m_srcInit;
    TS m_value;
    FwCmpOp m_fwCmpOp;

public:
    SrcDstRoiCompareCSingleChannel( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<U8,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TS value, FwCmpOp fwCmpOp, U8 errorMargin = MemOps<U8>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_value = value;
        m_fwCmpOp = fwCmpOp;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< U8, 1 >& dst )
    {
		ChannelBuffer< TS, 1 > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), m_value, dst.Ptr(), dst.StepBytes(), adv.Roi(), m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, U32 constChannels >
class SrcDstRoiCompareCMultiChannel : public DstRoiBase< U8, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, int, const TS [], U8 *, int, FwiSize, FwCmpOp); 
    Fn m_fn;
    const char * m_srcInit;
    TS m_value[constChannels];
    FwCmpOp m_fwCmpOp;

public:
    SrcDstRoiCompareCMultiChannel( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<U8,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, const TS value[constChannels], FwCmpOp fwCmpOp, U8 errorMargin = MemOps<U8>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        for(unsigned int i=0; i<constChannels; i++)
        {
            m_value[i] = value[i];
        }
        m_fwCmpOp = fwCmpOp;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< U8, 1 >& dst )
    {
		ChannelBuffer< TS, srcChannels > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), m_value, dst.Ptr(), dst.StepBytes(), adv.Roi(), m_fwCmpOp );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels >
class SrcDstRoiCompareEqualEps : public DstRoiBase< U8, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, int, const TS *, int, U8 *, int, FwiSize, TS); 
    Fn m_fn;
    const char * m_src1Init;
    const char * m_src2Init;
    TS m_eps;

public:
    SrcDstRoiCompareEqualEps( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<U8,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *src1Init, const char *src2Init, const char *dstExpected, TS eps, U8 errorMargin = MemOps<U8>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_src1Init = src1Init;
        m_src2Init = src2Init;
        m_eps = eps;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< U8, 1 >& dst )
    {
		ChannelBuffer< TS, srcChannels > src1( m_src1Init, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        ChannelBuffer< TS, srcChannels > src2( m_src2Init, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src1.Ptr(), src1.StepBytes(), src2.Ptr(), src2.StepBytes(), dst.Ptr(), dst.StepBytes(), adv.Roi(), m_eps );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS >
class SrcDstRoiCompareEqualEpsCSingleChannel : public DstRoiBase< U8, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, int, TS, U8 *, int, FwiSize, TS); 
    Fn m_fn;
    const char * m_srcInit;
    TS m_value;
    TS m_eps;

public:
    SrcDstRoiCompareEqualEpsCSingleChannel( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<U8,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, TS value, TS eps, U8 errorMargin = MemOps<U8>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        m_value = value;
        m_eps = eps;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< U8, 1 >& dst )
    {
		ChannelBuffer< TS, 1 > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), m_value, dst.Ptr(), dst.StepBytes(), adv.Roi(), m_eps );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS, U32 srcChannels, U32 constChannels >
class SrcDstRoiCompareEqualEpsCMultiChannel : public DstRoiBase< U8, 1, AdviceRoi >
{
	typedef FwStatus (STDCALL *Fn)(const TS *, int, const TS [], U8 *, int, FwiSize, TS); 
    Fn m_fn;
    const char * m_srcInit;
    TS m_value[constChannels];
    TS m_eps;

public:
    SrcDstRoiCompareEqualEpsCMultiChannel( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : DstRoiBase<U8,1, AdviceRoi>( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected, const TS value[constChannels], TS eps, U8 errorMargin = MemOps<U8>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        for(unsigned int i=0; i<constChannels; i++)
        {
            m_value[i] = value[i];
        }
        m_eps = eps;
        ExecuteTest( NULL, dstExpected, errorMargin, expectedReturn );
    }

    virtual void CallFn( const AdviceRoi & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< U8, 1 >& dst )
    {
		ChannelBuffer< TS, 1 > src( m_srcInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
        this->timer.Start();
        try
        {
            stat = m_fn( src.Ptr(), src.StepBytes(), m_value, dst.Ptr(), dst.StepBytes(), adv.Roi(), m_eps );
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

#endif // __THRESHOLD_AND_COMPARE_OBJECTS_H__
