/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestApp.h"

void GenerateReport( const Logger & logger, std::ostream & out, bool fIncludeMessages )
{
    int testNo = 0;
    std::list<Logger::LogEntry>::const_iterator i;
    for( i = logger.m_logEntries.begin(); i != logger.m_logEntries.end(); i++ )
    {
        const Logger::LogEntry & entry = *i;
        switch( entry.m_type )
        {
        case Logger::LibraryEntry:
            out << "Library: " << entry.m_text << std::endl;
            break;
        case Logger::CatalogEntry:
            out << "\tCatalog: " << entry.m_text << std::endl;
            break;
        case Logger::UnitTest:
            out << "\t\tUnit: " << entry.m_text << std::endl;
            testNo = 0;
            break;
        case Logger::Status:
            testNo++;
            if( entry.m_text.empty() )
            {
                out << "\t\t\tTest " << testNo << "\t\t";
            }
            else
            {
                out << "\t\t\tTest " << testNo << " (" << entry.m_text << ")\t";
            }
            if( entry.m_fPassed )
            {
                out << "passed";
            }
            else
            {
                out << "failed";
            }
            out << std::endl;
            break;
        case Logger::Crash:
            testNo++;
            if( entry.m_text.empty() )
            {
                out << "\t\t\tTest " << testNo << " crashed!";
            }
            else
            {
                out << "\t\t\tTest " << testNo << " (" << entry.m_text << ") crashed!";
            }
            break;
        case Logger::Hang:
            testNo++;
            if( entry.m_text.empty() )
            {
                out << "\t\t\tTest " << testNo << " hanged!";
            }
            else
            {
                out << "\t\t\tTest " << testNo << " (" << entry.m_text << ") hanged!";
            }
            break;
        case Logger::Message:
            if( fIncludeMessages )
            {
                out << entry.m_text << std::endl;
            }
            break;
        default:
            out << "Invalid entry!" << std::endl;
        }
    }
}

void GenerateReportCSV( const Logger & logger, std::ostream & out, bool fNoHeader )
{
    if( !fNoHeader )
    {
        out << "Library, Catalog, Function, Status, TestCase No, Description" << std::endl;
    }

    std::list<Logger::LogEntry>::const_iterator i;
    std::string library, catalog, unit, name, comment;
    for( i = logger.m_logEntries.begin(); i != logger.m_logEntries.end(); i++ )
    {
        const Logger::LogEntry & entry = *i;
        switch( entry.m_type )
        {
        case Logger::LibraryEntry:
            library = entry.m_text;
            break;
        case Logger::CatalogEntry:
            catalog = entry.m_text;
            break;
        case Logger::UnitTest:
            unit = entry.m_text;
            break;
        case Logger::Status:
            out << library << ", " << catalog << ", " << unit << ", ";
            if( entry.m_fPassed )
            {
                out << "passed";
            }
            else
            {
                out << "failed";
            }
            out << ", " << entry.m_num << ", " << entry.m_text;
            out << std::endl;
            break;
        case Logger::Crash:
            out << library << ", " << catalog << ", " << unit << ", ";
            out << "crashed";
            out << ", " << entry.m_num << ", " << entry.m_text;
            out << std::endl;
            break;
        case Logger::Hang:
            out << library << ", " << catalog << ", " << unit << ", ";
            out << "hung";
            out << ", " << entry.m_num << ", " << entry.m_text;
            out << std::endl;
            break;
        case Logger::Message:
            break;
        default:
            out << "Invalid entry!" << std::endl;
        }
    }
}
