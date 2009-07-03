/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


// UnitTest.h

#ifndef __UNITTEST_H__
#define __UNITTEST_H__

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include "UnitTestFramework.h"

void GenerateReport( const Logger & logger, std::ostream & out, bool fIncludeMessages = true );
void GenerateReportCSV( const Logger & logger, std::ostream & out, bool fNoHeader = false );

#endif // __UNITTEST_H__
