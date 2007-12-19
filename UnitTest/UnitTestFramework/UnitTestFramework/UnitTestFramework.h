/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __UNITTESTFRAMEWORK_H__
#define __UNITTESTFRAMEWORK_H__

#include <sstream>
#include <string>
#include <list>

#ifdef _MSC_VER                                         
#define DLL_EXPORT __declspec( dllexport ) 
#else
#define DLL_EXPORT
#endif

class Logger;
class UnitTestsEngineBase;
class UnitTestCatalogBase;

// base class for unit test object
class UnitTestBase
{
    friend class UnitTestCatalogBase;
    friend class UnitTestsEngineBase;

private:
    UnitTestCatalogBase & m_unitTestCatalog;    // reference to the parent object (test catalog)
    Logger * m_pLogger;                         // reference to the logger object
    const std::string m_unitName;               // test object name
    unsigned int m_testCaseNum;                 // test case num

    void RunAll( Logger & logger );             // run all test cases ( calls RunAll() in derived class )

protected:
    UnitTestBase( UnitTestCatalogBase & unitTestCatalog, const std::string & unitName );

    void ReportStatus( bool state, const char * pszComment = NULL, const char * pszCsvComment = NULL );    // report test case status
    void ReportCrash( const char * pszComment = NULL, const char * pszCsvComment = NULL );                 // report test case crash
    void ReportHang( const char * pszComment = NULL, const char * pszCsvComment = NULL );                  // report test case hang
    void ReportException( const char * pszComment = NULL, const char * pszCsvComment = NULL );             // report test case threw an exception
    
    const std::string & GetUnitName() const;                            // get the curent test object name
    const std::string & GetCatalogName() const;                         // get the current catalog (file) name
    const std::string & GetLibraryname() const;                         // get the current test library name
    
    std::stringstream & Log();                                          // get the stream for diagnostic messages

    virtual void RunAll() = 0;                                          // user provided function that runs all test cases
    virtual unsigned int GetTestCaseNum();
};

typedef std::list<UnitTestBase*> UnitTestList;
class UnitTestFileCatalogBase;

// base class for unit test objects catalog
class UnitTestCatalogBase
{
    friend class UnitTestFileCatalogBase;

private:
    UnitTestFileCatalogBase & m_unitTestFileCatalog;    // reference to parent object (file catalog)
    const std::string m_catalogName;                    // name of the catalog object (file)
    UnitTestList m_testsList;                           // list of unit test objects

public:
    void AddTest( UnitTestBase * unitTest );            // add a new unit test object to the catalog
    virtual void AddTests() = 0;                        // initializes/adds all unit test objects to catalog
    const std::string & GetCatalogName() const;         // get the current catalog (file) name
    const std::string & GetLibraryname() const;         // get the current test library name

    UnitTestCatalogBase( UnitTestFileCatalogBase & unitTestFileCatalog, const std::string & catalogName );
    virtual ~UnitTestCatalogBase();
    UnitTestList& GetTests();                           // get the list of unit test objects in the catalog
};

typedef std::list<UnitTestCatalogBase*> UnitTestCatalogList;

// base class for the root catalog
class UnitTestFileCatalogBase
{
    UnitTestCatalogList filesList;      // list of catalogs (files) in this library

private:
    const std::string m_libraryName;    // name of the root catalog (library)

public:
    void AddCatalog( UnitTestCatalogBase * unitTestCatalog ); // adds a new unit test catalog to root catalog
    virtual void AddCatalogs() = 0;                           // initializes/adds all unit test catalogs to root catalog
    const std::string & GetLibraryname() const;               // get the name of this root catalog (library)

    UnitTestFileCatalogBase( const std::string & libraryName );
    virtual ~UnitTestFileCatalogBase();
    UnitTestCatalogList& GetCatalogs();                       // get the list of unit test catalog objects
};

class UnitTestsEngineBase;

// logger class that is used to store the unit test run status and diagnostic messages
class Logger
{
    friend class UnitTestBase;
    friend class UnitTestsEngineBase;

public:
    enum LogEntryType { Invalid, Status, Message, UnitTest, CatalogEntry, LibraryEntry, Crash, Hang, Exception };

    // the status and diagnostic messages are stored as a sequence of LogEntry objects
    class LogEntry
    {
        void Duplicate( const LogEntry& other );

    public:
        LogEntryType m_type;                            // entry type
        bool         m_fPassed;                         // for status entry indicates success or failure
        unsigned int m_num;                             // number for indexing test cases
        std::string m_text;                             // comment/name associated with each entry

        LogEntry();
        LogEntry( const LogEntry& other );
        LogEntry& operator=( const LogEntry& other );
    };

    std::list<LogEntry> m_logEntries;                   // list of entries
    std::stringstream m_stream;                         // string stream to temporary store diagnostic messages

    std::stringstream& Log();                           // returns a reference to the diagnostic messages output stream

    void NewLibrary( const std::string & name );                    // adds a new library entry
    void NewCatalog( const std::string & name );                    // adds a new catalog entry
    void NewUnit( const std::string & name );                       // adds a bew unit object entry
    void ReportStatus( bool state, const int testCaseNum, const std::string & comment, const std::string & csvComment );   // adds a test case status entry
    void ReportCrash( const int testCaseNum, const std::string & comment, const std::string & csvComment );                // adds a test case crash entry
    void ReportHang( const int testCaseNum, const std::string & comment, const std::string & csvComment );                 // adds a test case hang entry
    void ReportException( const int testCaseNum, const std::string & comment, const std::string & csvComment );            // adds a test case exception entry
    void TestCompleted();                                           // indicates that current library testing has completed
    void Reset();                                                   // resets/clears the log entry list

    Logger();
    ~Logger();

private:
    void LogMessage();                                  // adds the diagnostic message to the entry log
};

// provides a base class for the unit test engine for each test dll
class UnitTestsEngineBase
{
public:
    void RunAll( Logger & logger, UnitTestFileCatalogBase & fileCatalogBase );
};

// templetized version of the engine class to handle different types of the root catalog (provided by the test author)
template<class T>
class UnitTestsEngine : public UnitTestsEngineBase
{
    T m_fileCatalogBase;

public:
    void RunAll( Logger & logger )
    {
        return UnitTestsEngineBase::RunAll( logger, m_fileCatalogBase );
    }
};

// macros to simplify coding/adding unit test objects and test cases entries

#define DECLARE_TEST_TABLE( name )                                          \
class name : public UnitTestCatalogBase                                     \
{                                                                           \
public:                                                                     \
    name( UnitTestFileCatalogBase & unitTestFileCatalog );                  \
    virtual void AddTests();                                                \
};

#define DEFINE_TEST_TABLE( name )                                           \
name::name( UnitTestFileCatalogBase & unitTestFileCatalog )                 \
: UnitTestCatalogBase( unitTestFileCatalog, #name )                         \
{                                                                           \
}                                                                           \
                                                                            \
void name::AddTests()                                                       \
{                                                                           

#define TEST_ENTRY( name )                                                  \
    AddTest( new name( *this ) );

#define END_TEST_TABLE()                                                    \
}

// macros to simplify coding/adding unit test catalog objects and RunAll function implementation

#define DEFINE_CATALOG_TABLE( name )                                        \
class name : public UnitTestFileCatalogBase                                 \
{                                                                           \
public:                                                                     \
    name() : UnitTestFileCatalogBase( #name )                               \
    {                                                                       \
    }                                                                       \
                                                                            \
    virtual void AddCatalogs()                                              \
    {

#define CATALOG_ENTRY( name )                                               \
        AddCatalog( new name( *this ) );

#define END_CATALOG_TABLE( name )                                                   \
    }                                                                               \
};                                                                                  \
                                                                                    \
extern "C" DLL_EXPORT void RunAll( Logger * pLogger, const char* configFileName )   \
{                                                                                   \
    static UnitTestsEngine<name> engine;                                            \
    SetConfigFileName( configFileName );                                            \
    if( pLogger != NULL )                                                           \
    {                                                                               \
        Logger & logger = *pLogger;                                                 \
        engine.RunAll( logger );                                                    \
    }                                                                               \
}

void SetConfigFileName( const char* configFileName );
const char* GetConfigFileName();

#endif // __UNITTESTFRAMEWORK_H__
