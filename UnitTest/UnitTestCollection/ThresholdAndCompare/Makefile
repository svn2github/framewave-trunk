#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

include ../Makefileinclude.in
PROJECT_NAME=ThresholdAndCompare

OBJS  = ut_iCompare.o ut_iCompareC.o ut_iCompareEqualEps.o ut_iCompareEqualEpsC.o ut_iThreshold.o ut_iThreshold_GT.o ut_iThreshold_GTVal.o ut_iThreshold_LT.o ut_iThreshold_LTVal.o ut_iThreshold_LTValGTVal.o ut_iThreshold_Val.o ut_sThreshold.o ut_sThreshold_GT.o ut_sThreshold_GTVal.o ut_sThreshold_LT.o ut_sThreshold_LTInv.o ut_sThreshold_LTVal.o ut_sThreshold_LTValGTVal.o ut_ThresholdAndCompare.o 

all:	CreateDirectory $(OBJS)
	$(CXX) -o $(OUTDIR)/lib$(PROJECT_NAME).$(DYN_LIB_EXT) $(CXXFLAGS) $(CONFIGFLAG) $(addprefix $(TMPDIR)/,$(OBJS)) $(SHAREDFLAG) -L$(UNIT_TEST_FRAMEWORK_LIB_PATH) -L$(FW_BIN_PATH) -L$(OUTDIR) -lfwImage -lfwSignal -lfwBase -lUnitTestFramework 

%.o: %.cpp $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) $(CONFIGFLAG) $(PICXXFLAGS) -c -o $(TMPDIR)/$@ $(INCLUDE_PATHS) $<

CreateDirectory:
	$(shell mkdir -p $(OUTDIR))
	$(shell mkdir -p $(TMPDIR))

clean:
	rm -rf $(TMPDIR)
	rm -rf $(OUTDIR)/lib$(PROJECT_NAME).$(DYN_LIB_EXT)


