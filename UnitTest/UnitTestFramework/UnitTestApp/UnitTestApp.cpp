/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestApp.h"
#include "System.h"
#include <fstream>

typedef void (*RunAllFn)( Logger * pLogger, const char* configFileName );

bool g_fCSV = false;
bool g_fQuiet = false;
bool g_fNoHeader = false;
bool g_fDiag = false;
std::string g_logPath = "";
std::string g_configFileName = "";

int ParseArgs( int argc, char ** argv )
{
    int n = 1;
    while( n < argc )
    {
        if( STRING_COMPARE_NOCASE( argv[n], "-log:" ) == 0 )
        {
            g_fCSV = true;
            if( n+1 >= argc )
            {
                goto BAD_ARGS;
            }
            n++;
            g_logPath = argv[n];
        }
        else if( STRING_COMPARE_NOCASE( argv[n], "-diag" ) == 0 )
        {
            g_fDiag = true;
        }
        else if( STRING_COMPARE_NOCASE( argv[n], "-quiet" ) == 0 )
        {
            g_fQuiet = true;
        }
        else if( STRING_COMPARE_NOCASE( argv[n], "-noheader" ) == 0 )
        {
            g_fNoHeader = true;
        }
        else if( STRING_COMPARE_NOCASE( argv[n], "-config:" ) == 0 )
        {
            g_configFileName = argv[++n];
        }
        else
        {
            break;
        }
        n++;
    }

    if( n == argc )
    {
        // we already read all parameters, no libraries were specified!
        goto BAD_ARGS;
    }

    return n;

BAD_ARGS:
    std::cerr << "Usage: UnitTest [-log: <log_filename>] [-quiet] [-diag] [-noheader] <UnitTestLibraries>" << std::endl;
    return 0;
}

void Diags( int argc, char ** argv )
{
    char buffer[1024];
    GET_CURRENT_DIR( buffer, sizeof(buffer) );
    std::cerr << "Current dir: " << buffer << std::endl;
    for( int n = 0; n < argc; n++ )
    {
        std::cerr << "argv[" << n << "]: " << argv[n] << std::endl;
    }
}

int main( int argc, char ** argv )
{
    int nLibIndex = ParseArgs( argc, argv );
    if( nLibIndex == 0 )
        return -1;

    if( g_fDiag )
        Diags( argc, argv );

    Logger logger;
    GetLinuxPwd();
    for( ; nLibIndex < argc; nLibIndex++ )
    {
        StringList files = GetFileList( argv[nLibIndex] );
        for( StringList::const_iterator it=files.begin(); it!=files.end(); ++it )
        {
            std::string library = *it;

            std::cout << "Loading Library: " << library << std::endl;
            DllHandle hModule = LOAD_LIBRARY( library.c_str() );
            DLERROR_CHECK;

            if( !hModule )
            {
                std::cout<<"ERROR: Failed to load "<< *it << std::endl; 
                std::cout.flush(); 
                continue;
            }

            RunAllFn pRunAll = (RunAllFn)GET_PROC_ADDRESS( hModule, "RunAll" );
            if( pRunAll == NULL )
            {
                std::cerr << std::endl << "Function RunAll() not found in library " << library << "!" << std::endl;
                FREE_LIBRARY( hModule );
                continue;
            }
            pRunAll( &logger, g_configFileName.c_str() );
            
            FREE_LIBRARY( hModule );
        } // next library
    } // next arg

    if( g_fCSV )
    {
        try 
        {
            std::ofstream log(g_logPath.c_str());
            GenerateReportCSV( logger, log, g_fNoHeader );
        }
        catch(...)
        {
            std::cerr << "Error writing log file " << g_logPath << "!" << std::endl;
        }
    }
    
    std::cout << std::endl;
}
