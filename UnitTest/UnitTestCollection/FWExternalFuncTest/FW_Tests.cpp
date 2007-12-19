/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"
#include <sstream>

DEFINE_CATALOG_TABLE( ImageTestCatalog )

#if !defined(SOL32) && !defined(SOL64)
CATALOG_ENTRY( FilterBoxTestCatalog )
#endif

CATALOG_ENTRY( RotateCenterTestCatalog )
CATALOG_ENTRY( AddTestCatalog )
CATALOG_ENTRY( AddCTestCatalog )
CATALOG_ENTRY( AddSquareTestCatalog )
CATALOG_ENTRY( AddProductTestCatalog )
CATALOG_ENTRY( AddWeightedTestCatalog )
CATALOG_ENTRY( MulTestCatalog )
CATALOG_ENTRY( MulCTestCatalog )
CATALOG_ENTRY( MulScaleTestCatalog )
CATALOG_ENTRY( MulCScaleTestCatalog )
CATALOG_ENTRY( SubTestCatalog )
CATALOG_ENTRY( SubCTestCatalog )
CATALOG_ENTRY( DivTestCatalog )
CATALOG_ENTRY( DivCTestCatalog )
CATALOG_ENTRY( AbsTestCatalog )
CATALOG_ENTRY( AbsDiffTestCatalog )
CATALOG_ENTRY( SqrTestCatalog )
CATALOG_ENTRY( SqrtTestCatalog )
CATALOG_ENTRY( LnTestCatalog )
CATALOG_ENTRY( ExpTestCatalog )
CATALOG_ENTRY( ComplementTestCatalog )

END_CATALOG_TABLE( ImageTestCatalog )
