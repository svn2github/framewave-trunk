/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include <stdio.h>
#include <string>
#include "UnitTestFramework.h"

//////////////////////////////////////////////////////////////////////////////
// UnitTestBase implementation
//////////////////////////////////////////////////////////////////////////////

UnitTestBase::UnitTestBase( UnitTestCatalogBase & unitTestCatalog, const std::string & unitName )
: m_unitName( unitName ), m_unitTestCatalog( unitTestCatalog )
{
    m_pLogger = NULL;
    m_testCaseNum = 0;
}

// called from RunAll to indicate a success or failure of the unit test.
// state - [IN] true indicates success, false otherwise
// pszComment - [IN] optional comment or test case description
void UnitTestBase::ReportStatus( bool state, const char * pszComment, const char *pszCsvComment )
{
    if( pszComment == NULL )
        pszComment = "";

    if( pszCsvComment == NULL )
        pszCsvComment = "";

    m_pLogger->ReportStatus( state, this->GetTestCaseNum(), pszComment, pszCsvComment );
}

// called from RunAll to indicate a unit test crash/exception.
// pszComment - [IN] optional comment or test case description
void UnitTestBase::ReportCrash( const char * pszComment, const char *pszCsvComment )
{
    if( pszComment == NULL )
        pszComment = "";

    if( pszCsvComment == NULL )
        pszCsvComment = "";

    m_pLogger->ReportCrash( this->GetTestCaseNum(), pszComment, pszCsvComment );
}

// called from RunAll to indicate a unit test hang.
// pszComment - [IN] optional comment or test case description
void UnitTestBase::ReportHang( const char * pszComment, const char *pszCsvComment )
{
    if( pszComment == NULL )
        pszComment = "";

    if( pszCsvComment == NULL )
        pszCsvComment = "";

    m_pLogger->ReportHang( this->GetTestCaseNum(), pszComment, pszCsvComment );
}

// called from RunAll to indicate a unit test threw an exception.
// pszComment - [IN] optional comment or test case description
void UnitTestBase::ReportException( const char * pszComment, const char *pszCsvComment )
{
    if( pszComment == NULL )
        pszComment = "";

    if( pszCsvComment == NULL )
        pszCsvComment = "";

    m_pLogger->ReportException( this->GetTestCaseNum(), pszComment, pszCsvComment );
}

// called from RunAll to obtain the name of the current unit test object.
// return value: string containing the unit test object name
const std::string & UnitTestBase::GetUnitName() const
{
    return m_unitName;
}

// called from RunAll to obtain the name of the current unit test catalog.
// return value: string containing the unit test catalog name
const std::string & UnitTestBase::GetCatalogName() const
{
    return m_unitTestCatalog.GetCatalogName();
}

// called from RunAll to obtain the name of the current unit library.
// return value: string containing the unit test library name
const std::string & UnitTestBase::GetLibraryname() const
{
    return m_unitTestCatalog.GetLibraryname();
}

// called from RunAll to obtain the output stream for diagnostic/custom messages
// return value: reference to a stringstream object
std::stringstream & UnitTestBase::Log()
{
    return m_pLogger->Log();
}

// called by the framework to execute the tests for this unit test object.
// This function will call the virtual RunAll method provided in the UnitTestBase derived class.
// logger - reference to a Logger object used to log the results and diagnostic information
void UnitTestBase::RunAll( Logger & logger )
{
    m_pLogger = &logger;
    RunAll();
    m_pLogger = NULL;
}


unsigned int UnitTestBase::GetTestCaseNum()
{
    return ++m_testCaseNum;
}

//////////////////////////////////////////////////////////////////////////////
// UnitTestCatalogBase implementation
//////////////////////////////////////////////////////////////////////////////

UnitTestCatalogBase::UnitTestCatalogBase( UnitTestFileCatalogBase & unitTestFileCatalog, const std::string & catalogName )
    : m_unitTestFileCatalog( unitTestFileCatalog ), m_catalogName( catalogName )
{
}

// virtual destructor - will delete all unit test objects from the list
UnitTestCatalogBase::~UnitTestCatalogBase()
{
    while( !m_testsList.empty() )
    {
        UnitTestList::iterator i = m_testsList.begin();
        delete *i;
        m_testsList.erase( i );
    }
}

// Called by the framework to obtain a list of all unit test objects in this catalog
// return value: reference to list<UnitTestBase> object
UnitTestList& UnitTestCatalogBase::GetTests()
{
    AddTests();
    return m_testsList;
}

// called to add a unit test object to the catalog.
// this function is typically called from AddTests method in the user provided class derived from UnitTestCatalogBase
// pUnitTest - pointer to an instance of the user provided unit test object (derived from UnitTestBase)
void UnitTestCatalogBase::AddTest( UnitTestBase * pUnitTest )
{
    if( pUnitTest != NULL )
        m_testsList.push_back( pUnitTest );
}

// Obtain the name of the current unit test catalog.
// return value: string containing the unit test catalog name
const std::string & UnitTestCatalogBase::GetCatalogName() const
{
    return m_catalogName;
}

// Obtain the name of the current unit library.
// return value: string containing the unit test library name
const std::string & UnitTestCatalogBase::GetLibraryname() const
{
    return m_unitTestFileCatalog.GetLibraryname();
}

//////////////////////////////////////////////////////////////////////////////
// UnitTestFileCatalogBase implementation
//////////////////////////////////////////////////////////////////////////////

UnitTestFileCatalogBase::UnitTestFileCatalogBase( const std::string & libraryName )
    : m_libraryName( libraryName )
{
}

// virtual destructor - will remove all catalog objects from the file catalog list
UnitTestFileCatalogBase::~UnitTestFileCatalogBase()
{
    while( !filesList.empty() )
    {
        UnitTestCatalogList::iterator i = filesList.begin();
        delete *i;
        filesList.erase( i );
    }
}

// Called by the framework to obtain a list of all unit test catalogs in the current library
// return value: reference to list<UnitTestCatalogList> object
UnitTestCatalogList& UnitTestFileCatalogBase::GetCatalogs()
{
    AddCatalogs();
    return filesList;
}

// called to add a unit test catalog to the library.
// this function is typically called from AddCatalogs method in the user provided class derived from UnitTestFileCatalogBase
// pUnitTestCatalog - pointer to an instance of the user provided unit test catalog object (derived from UnitTestCatalogBase)
void UnitTestFileCatalogBase::AddCatalog( UnitTestCatalogBase * unitTestCatalog )
{
    if( unitTestCatalog != NULL )
        filesList.push_back( unitTestCatalog );
}

// Obtain the name of the current unit library.
// return value: string containing the unit test library name
const std::string & UnitTestFileCatalogBase::GetLibraryname() const
{
    return m_libraryName;
}

//////////////////////////////////////////////////////////////////////////////
// UnitTestsEngineBase implementation
//////////////////////////////////////////////////////////////////////////////

// called by the framework to iterate and execute all unit tests in this library.
// logger - reference to a Logger object used to collect the test results and diagnostic information
// fileCatalogBase - reference to a user provided object (derived from UnitTestFileCatalogBase) that
//                   implements the file catalog for this library
void UnitTestsEngineBase::RunAll( Logger & logger, UnitTestFileCatalogBase & fileCatalogBase )
{
    // log the library name
    logger.NewLibrary( fileCatalogBase.GetLibraryname() );
    printf( "Library: %s\n", fileCatalogBase.GetLibraryname().c_str() );

    // get the list of files/catalog objects in this library
    UnitTestCatalogList& catalogList = fileCatalogBase.GetCatalogs();

    // iterate through all files/catalogs
    UnitTestCatalogList::iterator i;
    for( i = catalogList.begin(); i != catalogList.end(); i++ )
    {
        UnitTestCatalogBase * pUnitTestCatalog = *i;

        // log the catalog name
        logger.NewCatalog( pUnitTestCatalog->GetCatalogName() );
        printf( "\tCatalog: %s\n", pUnitTestCatalog->GetCatalogName().c_str() );

        // get the list of all unit test objects in the current catalog
        UnitTestList& testsList = pUnitTestCatalog->GetTests();

        // iterate through all unit test objects
        UnitTestList::iterator j;
        for( j = testsList.begin(); j != testsList.end(); j++ )
        {
            UnitTestBase* pUnitTest = *j;

            // log the current unit test object name
            logger.NewUnit( pUnitTest->GetUnitName() );
            printf( "\t\tFunction: %s\n", pUnitTest->GetUnitName().c_str() );

            // execute the unit tests in the current unit test object
            pUnitTest->RunAll( logger );
        }
    }

    // indicate the end of the unit tests in the current library
    logger.TestCompleted();
}

//////////////////////////////////////////////////////////////////////////////
// Logger::LogEntry implementation
//////////////////////////////////////////////////////////////////////////////

Logger::LogEntry::LogEntry()
{
    m_type = Invalid;
    m_fPassed = false;
}

Logger::LogEntry::LogEntry( const Logger::LogEntry& other )
{
    Duplicate( other );
}

Logger::LogEntry& Logger::LogEntry::operator=( const Logger::LogEntry& other )
{
    Duplicate( other );
    return *this;
}

void Logger::LogEntry::Duplicate( const Logger::LogEntry& other )
{
    if( this != &other )
    {
        m_type = other.m_type;
        m_fPassed = other.m_fPassed;
        m_num = other.m_num;
        m_text = other.m_text;
    }
}

//////////////////////////////////////////////////////////////////////////////
// Logger implementation
//////////////////////////////////////////////////////////////////////////////

Logger::Logger()
{
}

Logger::~Logger()
{
}

// clears the list of LogEntry objects
void Logger::Reset()
{
    m_logEntries.clear();
}

// returns reference to the stringstream object used to log diagnostic information
std::stringstream& Logger::Log()
{
    return m_stream;
}

// adds a new log entry to indicate a new unit test library
// name - [IN] name of the library
void Logger::NewLibrary( const std::string & name )
{
    LogMessage();
    LogEntry newEntry;
    newEntry.m_type = LibraryEntry;
    newEntry.m_text = name;

    m_logEntries.push_back( newEntry );
}

// adds a new log entry to indicate a new unit test catalog
// name - [IN] name of the catalog
void Logger::NewCatalog( const std::string & name )
{
    LogMessage();
    LogEntry newEntry;
    newEntry.m_type = CatalogEntry;
    newEntry.m_text = name;

    m_logEntries.push_back( newEntry );
}

// adds a new log entry to indicate a new unit test object
// name - [IN] name of the unit test object
void Logger::NewUnit( const std::string & name )
{
    LogMessage();
    LogEntry newEntry;
    newEntry.m_type = UnitTest;
    newEntry.m_text = name;

    m_logEntries.push_back( newEntry );
}

// called to indicate the end of tests for the current library
// it is used to flush the diagnostic messages from the m_stream object 
// and add them as a new entry in the log entry list
void Logger::TestCompleted()
{
    LogMessage();
}

// checks if there is any diagnostic data in the m_stream object.  
// Flushes it out and adds to the log entry list.
void Logger::LogMessage()
{
    if( m_stream.str().empty() )
        return;

    LogEntry newEntry;
    newEntry.m_type = Message;
    newEntry.m_text = m_stream.str();

    m_logEntries.push_back( newEntry );
    m_stream.str("");
    printf( "%s\n", newEntry.m_text.c_str() );
}

// adds a new log entry to indicate the status of a unit test case
// state - [IN] indicates if the test case passed (true) or failed (false)
// name - [IN] description of the unit test case
void Logger::ReportStatus( bool state, const int testCaseNum, const std::string & description, const std::string & csvDescription )
{
    LogMessage();
    LogEntry newEntry;
    newEntry.m_type = Status;
    newEntry.m_fPassed = state;
    newEntry.m_num = testCaseNum;
    newEntry.m_text = csvDescription;

    m_logEntries.push_back( newEntry );
    if( description.length() > 0 )
    {
        printf( "\t\t\tTestCase %d (%s) %s\n", testCaseNum, description.c_str(), state ? "succeeded." : "failed!" );
    }
    else
    {
        printf( "\t\t\tTestCase %d %s\n", testCaseNum, state ? "succeeded." : "failed!" );
    }
}

// adds a new log entry to indicate a crash of a unit test case
// name - [IN] description of the unit test case
void Logger::ReportCrash( const int testCaseNum, const std::string & description, const std::string & csvDescription )
{
    LogMessage();
    LogEntry newEntry;
    newEntry.m_type = Crash;
    newEntry.m_num = testCaseNum;
    newEntry.m_text = csvDescription;

    m_logEntries.push_back( newEntry );
    printf( "\t\t\tTest %d crashed!\n", testCaseNum );
}

// adds a new log entry to indicate a hang of a unit test case
// name - [IN] description of the unit test case
void Logger::ReportHang( const int testCaseNum, const std::string & description, const std::string & csvDescription )
{
    LogMessage();
    LogEntry newEntry;
    newEntry.m_type = Hang;
    newEntry.m_num = testCaseNum;
    newEntry.m_text = csvDescription;

    m_logEntries.push_back( newEntry );
    printf( "\t\t\tTest %d hung!\n", testCaseNum );
}

// adds a new log entry to indicate an exception of a unit test case
// name - [IN] description of the unit test case
void Logger::ReportException( const int testCaseNum, const std::string & description, const std::string & csvDescription )
{
    LogMessage();
    LogEntry newEntry;
    newEntry.m_type = Exception;
    newEntry.m_num = testCaseNum;
    newEntry.m_text = csvDescription;

    m_logEntries.push_back( newEntry );
    printf( "\t\t\tTest %d threw an exception!\n", testCaseNum );
}


const char *ut_configFileName;
void SetConfigFileName( const char* configFileName )
{
    ut_configFileName = configFileName;
}
const char* GetConfigFileName()
{
    return ut_configFileName;
}

