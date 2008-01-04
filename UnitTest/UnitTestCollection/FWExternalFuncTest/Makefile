#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

include ../Makefileinclude.in
PROJECT_NAME=FWExternalFuncTest

OBJS  = FW_Tests.o T_Abs.o T_AbsDiff.o T_AddC.o T_Add.o T_AddProduct.o T_AddSquare.o T_AddWeighted.o T_Div.o T_DivC.o T_LN.o T_Mul.o T_MulC.o T_MulScale.o T_MulCScale.o T_FilterBox.o T_RotateCenter.o T_Sqr.o T_Sqrt.o T_Exp.o T_SubC.o T_Sub.o T_Complement.o

all:	CreateDirectory $(OBJS)
	$(CXX) -o $(OUTDIR)/lib$(PROJECT_NAME).$(DYN_LIB_EXT) $(CXXFLAGS) $(CONFIGFLAG) $(addprefix $(TMPDIR)/,$(OBJS)) $(SHAREDFLAG) -L$(UNIT_TEST_FRAMEWORK_LIB_PATH) -L$(FW_BIN_PATH) -L$(OUTDIR) -lfwImage -lfwBase -lUnitTestFramework 

%.o: %.cpp $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) $(CONFIGFLAG) $(PICXXFLAGS) -c -o $(TMPDIR)/$@ $(INCLUDE_PATHS) $<

CreateDirectory:
	$(shell mkdir -p $(OUTDIR))
	$(shell mkdir -p $(TMPDIR))

clean:
	rm -rf $(TMPDIR)
	rm -rf $(OUTDIR)/lib$(PROJECT_NAME).$(DYN_LIB_EXT)

