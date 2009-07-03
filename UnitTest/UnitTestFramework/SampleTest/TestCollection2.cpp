/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


// TestCollection2.cpp
//

#include "UnitTestFramework.h"
#include "SampleTest.h"


class FnAUnitTest : public UnitTestBase
{
public:
    FnAUnitTest( UnitTestCatalogBase & parent )
        : UnitTestBase( parent, "FnAUnitTest" )
    {
    }

    virtual void RunAll()
    {
        Test1();
        Test2();
    }

    void Test1()
    {
    }

    void Test2()
    {
        ReportStatus( true, "Test2" );
    }
};

class FnBUnitTest : public UnitTestBase
{
public:
    FnBUnitTest( UnitTestCatalogBase & parent )
        : UnitTestBase( parent, "FnBUnitTest" )
    {
    }

    virtual void RunAll()
    {
        Test1();
        Test2();
        Test3();
    }

    void Test1()
    {
        ReportStatus( false, "Test1" );
    }

    void Test2()
    {
        ReportStatus( true );
    }

    void Test3()
    {
        ReportStatus( false );
    }
};

class FnCUnitTest : public UnitTestBase
{
public:
    FnCUnitTest( UnitTestCatalogBase & parent )
        : UnitTestBase( parent, "FnCUnitTest" )
    {
    }

    virtual void RunAll()
    {
        Test1();
        Test2();
        Test3();
    }

    void Test1()
    {
        ReportStatus( true, "Test1" );
    }

    void Test2()
    {
        Log() << "Injecting custom failure message!";
    }

    void Test3()
    {
        ReportStatus( true, "Test3" );
    }
};

DEFINE_TEST_TABLE( UnitTestCatalog2 )
TEST_ENTRY( FnAUnitTest )
TEST_ENTRY( FnBUnitTest )
TEST_ENTRY( FnCUnitTest )
END_TEST_TABLE()
