/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"
#include <sstream>

DEFINE_CATALOG_TABLE( ThresholdAndCompareCatalog )

CATALOG_ENTRY( Catalog_iCompare )
CATALOG_ENTRY( Catalog_iCompareC )
CATALOG_ENTRY( Catalog_iCompareEqualEps )
CATALOG_ENTRY( Catalog_iCompareEqualEpsC )
CATALOG_ENTRY( Catalog_iThreshold )
CATALOG_ENTRY( Catalog_iThreshold_LT )
CATALOG_ENTRY( Catalog_iThreshold_GT )
CATALOG_ENTRY( Catalog_iThreshold_Val )
CATALOG_ENTRY( Catalog_iThreshold_LTVal )
CATALOG_ENTRY( Catalog_iThreshold_GTVal )
CATALOG_ENTRY( Catalog_iThreshold_LTValGTVal )
CATALOG_ENTRY( Catalog_sThreshold )
CATALOG_ENTRY( Catalog_sThreshold_LT )
CATALOG_ENTRY( Catalog_sThreshold_GT )
CATALOG_ENTRY( Catalog_sThreshold_LTInv )
CATALOG_ENTRY( Catalog_sThreshold_GTVal )
CATALOG_ENTRY( Catalog_sThreshold_LTVal )
CATALOG_ENTRY( Catalog_sThreshold_LTValGTVal )

END_CATALOG_TABLE( ThresholdAndCompareCatalog )
