/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

class TestComplement_32s_C1IR : public SDstRoi< S32, 1 >
{
public:
    TestComplement_32s_C1IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< S32, 1 >( parent, "fwiComplement_32s_C1IR", fwiComplement_32s_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-1 -2 -2147483647 MIN 0 1 2", "-2147483647 -2147483646 -1 MIN 0 1 2" );
    }
};


DEFINE_TEST_TABLE( ComplementTestCatalog )

TEST_ENTRY( TestComplement_32s_C1IR )

END_TEST_TABLE()
