/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


// TestCollection1.cpp
//

#include "UnitTestFramework.h"
#include "SampleTest.h"

class Fn1UnitTest : public UnitTestBase
{
public:
    Fn1UnitTest( UnitTestCatalogBase & parent )
        : UnitTestBase( parent, "Fn1UnitTest" )
    {
    }

    virtual void RunAll()
    {
        TestCase();
    }

    void TestCase()
    {
        ReportStatus( true, "TestCase" );
    }
};

class Fn2UnitTest : public UnitTestBase
{
public:
    Fn2UnitTest( UnitTestCatalogBase & parent )
        : UnitTestBase( parent, "Fn2UnitTest" )
    {
    }

    virtual void RunAll()
    {
        TestCase1();
        TestCase2();
    }

    void TestCase1()
    {
        Log() << "Test 1 ";
        ReportStatus( true, "TestCase1" );
    }

    void TestCase2()
    {
        Log() << "testing logging capabilities";
        ReportStatus( true, "TestCase2" );
    }
};

class Fn3UnitTest : public UnitTestBase
{
public:
    Fn3UnitTest( UnitTestCatalogBase & parent )
        : UnitTestBase( parent, "Fn3UnitTest" )
    {
    }

    virtual void RunAll()
    {
        TestCase1();
        TestCase2();
        TestCase3();
    }

    void TestCase1()
    {
        ReportStatus( false, "TestCase1" );
    }

    void TestCase2()
    {
        ReportStatus( true, "TestCase2" );
    }

    void TestCase3()
    {
        ReportStatus( true, "TestCase3" );
    }
};

DEFINE_TEST_TABLE( UnitTestCatalog1 )
TEST_ENTRY( Fn1UnitTest )
TEST_ENTRY( Fn2UnitTest )
TEST_ENTRY( Fn3UnitTest )
END_TEST_TABLE()
