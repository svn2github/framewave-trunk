/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __FW_TESTS_H__
#define __FW_TESTS_H__

#if !defined(SOL32) && !defined(SOL64)
DECLARE_TEST_TABLE( FilterBoxTestCatalog )
#endif

DECLARE_TEST_TABLE( RotateCenterTestCatalog )
DECLARE_TEST_TABLE( AddTestCatalog )
DECLARE_TEST_TABLE( AddCTestCatalog )
DECLARE_TEST_TABLE( AddSquareTestCatalog )
DECLARE_TEST_TABLE( AddProductTestCatalog )
DECLARE_TEST_TABLE( AddWeightedTestCatalog )
DECLARE_TEST_TABLE( MulTestCatalog )
DECLARE_TEST_TABLE( MulCTestCatalog )
DECLARE_TEST_TABLE( MulScaleTestCatalog )
DECLARE_TEST_TABLE( MulCScaleTestCatalog )
DECLARE_TEST_TABLE( SubTestCatalog )
DECLARE_TEST_TABLE( SubCTestCatalog )
DECLARE_TEST_TABLE( DivTestCatalog )
DECLARE_TEST_TABLE( DivCTestCatalog )
DECLARE_TEST_TABLE( AbsTestCatalog )
DECLARE_TEST_TABLE( AbsDiffTestCatalog )
DECLARE_TEST_TABLE( SqrTestCatalog )
DECLARE_TEST_TABLE( SqrtTestCatalog )
DECLARE_TEST_TABLE( LnTestCatalog )
DECLARE_TEST_TABLE( ExpTestCatalog )
DECLARE_TEST_TABLE( ComplementTestCatalog )

#endif // __FW_TESTS_H__
