/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __STATSTESTS_OBJECT_H__
#define __STATSTESTS_OBJECT_H__

#include "../BaseObjects/Object.h"



template <typename TD>
class StatDstLenBase : public UnitTestBase
{
	FwStatus func_stat;
    unsigned int testCaseNum;

protected:
    Timer timer;

public:
	StatDstLenBase( UnitTestCatalogBase & parent, const char *name )
        : UnitTestBase( parent, name ), testCaseNum( 0 )
	{
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

            Buffer< TD > dstCore( dstInit,1 );
            Buffer< TD > expected( dstExpected, 1 );

            bool isGoldSame = true;
            bool isFirst = true;
            Advisor<AdviceLen>::ResetBase();
            while( Advisor<AdviceLen>::AdvanceBase() )
            {
			    const AdviceLen & adv = Advisor<AdviceLen>::Get();
                std::ostringstream oss; 
                oss << "Test " << ++testNum;

                Buffer< TD > dst( dstInit,1, adv.Alignment() );

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

		U32 i;
	    const TD *pDst = dst.Ptr();
	    const TD *pDstExp = expected.Ptr();

		for( i=0; i < dst.Count(); ++i )
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

template< typename TS,typename TD>
class SrcDstLen : public StatDstLenBase< TD >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *); 
public:
    Fn m_fn;
    const char * m_srcInit;
    int BuffLen;

    SrcDstLen( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : StatDstLenBase< TD >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected,int len,TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        BuffLen = len;
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
            stat = m_fn( src.Ptr(), BuffLen, dst.Ptr());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS,typename TD>
class SrcDstLenHint : public StatDstLenBase< TD >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *,FwHintAlgorithm); 
public:
    Fn m_fn;
    const char * m_srcInit;
    int BuffLen;
    FwHintAlgorithm hint;

    SrcDstLenHint( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : StatDstLenBase< TD >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected,int len,FwHintAlgorithm ht= fwAlgHintNone,TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        BuffLen = len;
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
            stat = m_fn( src.Ptr(), BuffLen, dst.Ptr(),hint);
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS1,typename TS2,typename TD>
class SrcSrcDstLen : public StatDstLenBase< TD >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS1 *, const TS2 *,int, TD *); 
public:
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;
    int BuffLen;

    SrcSrcDstLen( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : StatDstLenBase< TD >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2,const char *dstExpected,int len,TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        BuffLen = len;
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
            stat = m_fn( src1.Ptr(),src2.Ptr(), BuffLen, dst.Ptr());
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS,typename TD>
class SrcDstLenScale : public StatDstLenBase< TD >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS *, int, TD *,int); 
public:
    Fn m_fn;
    const char * m_srcInit;
    int BuffLen;
    int ScaleVal;

    SrcDstLenScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : StatDstLenBase< TD >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit, const char *dstExpected,int len,int scale,TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit = srcInit;
        BuffLen = len;
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
            stat = m_fn( src.Ptr(), BuffLen, dst.Ptr(),ScaleVal);
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};

template< typename TS1,typename TS2,typename TD>
class SrcSrcDstLenScale : public StatDstLenBase< TD >
{
protected:
    typedef FwStatus (STDCALL *Fn)( const TS1 *,const TS2 *, int, TD *,int); 
public:
    Fn m_fn;
    const char *m_srcInit1, *m_srcInit2;
    int BuffLen;
    int ScaleValue;

    SrcSrcDstLenScale( UnitTestCatalogBase & parent, const char *name, Fn fn )
        : StatDstLenBase< TD >( parent, name ), m_fn(fn)
    {
    }

    void RunTest( const char *srcInit1, const char *srcInit2,const char *dstExpected,int len,int scale,TD errorMargin = MemOps<TD>::Zero(), const FwStatus expectedReturn = fwStsNoErr )
    {
        m_srcInit1 = srcInit1;
		m_srcInit2 = srcInit2;
        BuffLen = len;
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
            stat = m_fn( src1.Ptr(),src2.Ptr(), BuffLen, dst.Ptr(),ScaleValue);
        }
        catch( ... )
        {
            stat = (FwStatus)EXCEPTION_ERR;
        }
        this->timer.Stop();
    }
};



#endif // __STATSTESTS_OBJECT_H__