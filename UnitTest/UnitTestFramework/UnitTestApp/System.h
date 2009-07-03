/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <string>
#include <list>

static std::string NoPath( const std::string & file )
{
    size_t pos = file.find_last_of( "/\\" );

    if( std::string::npos == pos )pos  = 0;
    else                          pos += 1;

    return file.substr( pos );
}

#define EXIT_IF( test,  reason )    if( test ) EXIT( reason )
#define EXIT(           reason )    { std::cout << "\nERROR:[" << POS() << "] " << reason << " EXITING!"<< std::endl; std::cout.flush(); exit(-1); }              
#define POS(                   )    NoPath(__FILE__) << "(" << __LINE__ << ")"     

typedef std::list< std::string > StringList;

//
// Operating System Preprocessor Block
//
#if defined( WIN64   ) || defined( _WIN64   ) || \
    defined( WIN32   ) || defined( _WIN32   ) || \
    defined( WINDOWS ) || defined( _WINDOWS )

#define WIN
#undef  LIN
#undef  SOL
#undef  MAC

#elif defined( LIN64   ) || defined( _LIN64 ) || \
      defined( LIN32   ) || defined( _LIN32 ) || \
      defined( LINUX   ) || defined( _LINUX )

#define LIN
#undef  WIN
#undef  SOL
#undef  MAC

#elif defined( SOL64   ) || defined( _SOL64   ) || \
      defined( SOL32   ) || defined( _SOL32   ) || \
      defined( SOLARIS ) || defined( _SOLARIS )

#define SOL
#undef  WIN
#undef  LIN
#undef  MAC

#elif defined( MAC64     ) || defined( _MAC64   ) || \
      defined( MAC32     ) || defined( _MAC32   ) || \
      defined( __APPLE__ )

#define MAC
#undef  WIN
#undef  LIN
#undef  SOL

#endif // Operating System Preprocessor Block

//
// DLL I/O Preprocessor Block
//
template< class T >
static std::string ToStr( const T & val )
{
    std::ostringstream oss; 
    oss << val; 
    return oss.str();
}

std::string pwdir;
void GetLinuxPwd()
{
#if defined( LIN ) || defined( SOL ) || defined( MAC )
    // obtain present working directory
    std::string shell_command = "pwd";

    FILE *inputStream;
    inputStream = popen(shell_command.c_str(), "r");
    EXIT_IF( !inputStream, "Shell input stream could not be opened." );

    char pwdir_c[1024];
    fgets(pwdir_c, 1024, inputStream);
    EXIT_IF( ferror(inputStream), "Shell input stream could not be read." );
    if(strlen(pwdir_c) > 1) if(pwdir_c[strlen(pwdir_c)-1] == '\n') pwdir_c[strlen(pwdir_c)-1] = '\0'; // remove eol charachterd
    pwdir = pwdir_c; pwdir += '/';
    pclose(inputStream);
#else
    pwdir = "";
#endif
}

#if defined( WIN )

#include <windows.h>
#include <direct.h>

#define STRING_COMPARE_NOCASE _stricmp
#define GET_CURRENT_DIR _getcwd

typedef FARPROC DllFnPointer;
typedef HMODULE DllHandle;

#define LOAD_LIBRARY(DLL_FILE) (::LoadLibrary(DLL_FILE))
#define FREE_LIBRARY(DLL_HANDLE) (::FreeLibrary(DLL_HANDLE))
#define GET_PROC_ADDRESS(DLL_HANDLE, FN_NAME) (::GetProcAddress(DLL_HANDLE, FN_NAME))

#define GET_CURRENT_DIRECTORY(DIR_PATH) ::GetCurrentDirectory( MAX_PATH, DIR_PATH )
#define SET_CURRENT_DIRECTORY(DIR_PATH) ::SetCurrentDirectory( DIR_PATH )

#define DLERROR_CHECK


std::string ResolvePath( std::string &searchStr )
{
    if( ('.' == searchStr[0]) && ('.' != searchStr[1]) )
    {
        char buffer[ 1024 ];
        GetCurrentDirectory( sizeof(buffer), buffer );
        searchStr = ToStr(buffer) + searchStr.substr(1);        
    }

    size_t pos = searchStr.find_last_of( "/\\" );
    return searchStr.substr( 0, pos+1 );
}

StringList GetFileList( std::string searchStr ) // Return a list of all files in search path.
{  
    std::string path = ResolvePath( searchStr );
    SET_CURRENT_DIRECTORY( path.c_str() );

    StringList list;
    WIN32_FIND_DATA FindFileData; 
    HANDLE hFind = FindFirstFile( searchStr.c_str(), &FindFileData );

    if( INVALID_HANDLE_VALUE != hFind ) 
    {
        if( FILE_ATTRIBUTE_ARCHIVE == FindFileData.dwFileAttributes )
            list.push_back( path + FindFileData.cFileName );    
        while (FindNextFile(hFind, &FindFileData) != 0) 
        {
            if( FILE_ATTRIBUTE_ARCHIVE == FindFileData.dwFileAttributes )
                list.push_back( path + FindFileData.cFileName );         
        }
        FindClose(hFind);        
    }
    return list;
}

#elif defined( LIN ) || defined( SOL ) || defined (MAC)

#include <dlfcn.h>
#include <unistd.h>

#define STRING_COMPARE_NOCASE strcasecmp
#define GET_CURRENT_DIR getcwd

typedef void * DllFnPointer;
typedef void * DllHandle;

#define LOAD_LIBRARY(DLL_FILE) (dlopen(DLL_FILE, RTLD_NOW))
#define FREE_LIBRARY(DLL_HANDLE) (dlclose(DLL_HANDLE))
#define GET_PROC_ADDRESS(DLL_HANDLE, FN_NAME) (dlsym(DLL_HANDLE, FN_NAME))

#define GET_CURRENT_DIRECTORY(DIR_PATH) 
#define SET_CURRENT_DIRECTORY(DIR_PATH)

#define DLERROR_CHECK    char *errp; 		\
    			 errp = dlerror(); 	\
    			 EXIT_IF( errp,  "Error: Message from dlerror(): " << errp );

StringList GetFileList( std::string searchStr )
{
    StringList list;

    if(searchStr.size() == 0)
        return list;

    if(searchStr[0] != '/')
        searchStr = pwdir + searchStr;

    std::string shell_command = std::string("ls ") + searchStr;
    
    FILE *inputStream;
    inputStream = popen(shell_command.c_str(), "r");
    EXIT_IF( !inputStream, "Shell input stream could not be opened." );

    const unsigned int array_size = 1024 * 1024;
    char *ls_list = 0;
    ls_list = (char *)malloc(array_size);
    EXIT_IF( !ls_list, "Failure in malloc." );
    // Initialize allocated memory
    char *ptr_ls_list = ls_list;
    for( unsigned int i=0; i<array_size; ++i, ++ptr_ls_list )
        *ptr_ls_list = NULL;

    fread(ls_list, 1, array_size, inputStream);
    EXIT_IF( ferror(inputStream), "Shell input stream could not be read." );

    std::string ls_list_s = ls_list;
    free(ls_list);

    std::string white_space = " \t\n\r";
    while(ls_list_s.find_first_not_of(white_space) != std::string::npos)
    {
        ls_list_s.erase(0, ls_list_s.find_first_not_of(white_space.c_str()));
        list.push_back(ls_list_s.substr(0, ls_list_s.find_first_of(white_space.c_str())));
        ls_list_s.erase(0, ls_list_s.find_first_of(white_space.c_str()));
    }

    pclose(inputStream);
    return list;
}

#endif

#endif // __SYSTEM_H__
