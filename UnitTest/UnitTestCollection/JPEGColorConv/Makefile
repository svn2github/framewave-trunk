#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

include ../Makefileinclude.in
PROJECT_NAME=JPEGColorConv

OBJS  = JPEGTest.o T_ColorConv.o T_JPEGColor.o

all:	CreateDirectory $(OBJS)
	$(CXX) -o $(OUTDIR)/lib$(PROJECT_NAME).$(DYN_LIB_EXT) $(CXXFLAGS) $(CONFIGFLAG) $(addprefix $(TMPDIR)/,$(OBJS)) $(SHAREDFLAG) -L$(UNIT_TEST_FRAMEWORK_LIB_PATH) -L$(FW_BIN_PATH) -L$(OUTDIR) -lfwImage -lfwBase -lfwJPEG -lUnitTestFramework 

%.o: %.cpp $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) $(CONFIGFLAG) $(PICXXFLAGS) -c -o $(TMPDIR)/$@ $(INCLUDE_PATHS) $<

CreateDirectory:
	$(shell mkdir -p $(OUTDIR))
	$(shell mkdir -p $(TMPDIR))

clean:
	rm -rf $(TMPDIR)
	rm -rf $(OUTDIR)/lib$(PROJECT_NAME).$(DYN_LIB_EXT)

