#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

include ../Makefileinclude.in
PROJECT_NAME=InFnImageArithmeticAndLogical

OBJS  = InFnImageArithmeticAndLogical.o ut_Abs.o
RELATIVE_INCLUDE_PATHS = -I$(FW_SRC_PATH)/domain/fwBase/include -I$(FW_SRC_PATH)/domain/fwImage/include -I$(FW_SRC_PATH)/domain/common/include -I$(FW_SRC_PATH)/domain/common/include/Arithmetic/Def -I$(FW_SRC_PATH)/domain/common/include/Arithmetic/Impl -I$(FW_SRC_PATH)/sdk

ifeq "$(OSNAME)" "SunOS"
	CXX_FLAGS_SSE = -xtarget=opteron
else
	CXX_FLAGS_SSE = -msse3
endif

EXTRA_CXXFLAGS = $(CXX_FLAGS_SSE) -DOPT_LEVEL=SSE2 -DDISPATCH=DT_SSE2 -DOPT_PREFIX=_sse2_

all:	CreateDirectory $(OBJS)
	$(CXX) -o $(OUTDIR)/lib$(PROJECT_NAME).$(DYN_LIB_EXT) $(CXXFLAGS) $(CONFIGFLAG) $(addprefix $(TMPDIR)/,$(OBJS)) $(SHAREDFLAG) -L$(UNIT_TEST_FRAMEWORK_LIB_PATH) -L$(FW_BIN_PATH) -L$(OUTDIR) -lfwBase -lUnitTestFramework 

%.o: %.cpp $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(CONFIGFLAG) $(PICXXFLAGS) -c -o $(TMPDIR)/$@ $(RELATIVE_INCLUDE_PATHS) $(INCLUDE_PATHS) $<

CreateDirectory:
	$(shell mkdir -p $(OUTDIR))
	$(shell mkdir -p $(TMPDIR))

clean:
	rm -rf $(TMPDIR)
	rm -rf $(OUTDIR)/lib$(PROJECT_NAME).$(DYN_LIB_EXT)


