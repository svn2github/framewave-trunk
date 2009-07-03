#
# Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

# Bitness and Output directories
ifeq "$(BITNESS)" "64"
	BITNESS=64
else
	BITNESS=32
endif

# OS compiler settings
ifndef $(OSNAME)
	OSNAME = $(shell uname -s)
endif
ifeq "$(OSNAME)" "Linux"			# Solaris
	CXX = g++
	PICXXFLAGS = -DLIN -fPIC
	DYNLIBFLAGS = -shared -Wl,-soname,libSampleTest.so
	DYN_LIB_EXT = so
else 
	ifeq "$(OSNAME)" "Darwin"		# Mac OS
		CXX = g++
		PICXXFLAGS = -DMAC -fPIC
		DYNLIBFLAGS = -dynamiclib -install_name libSampleTest.dylib 
		DYN_LIB_EXT = dylib
	else
		ifeq "$(OSNAME)" "SunOS"	# Sun Studio
			CXX = CC
			PICXXFLAGS = -DSOL -KPIC
			DYNLIBFLAGS = -G -norunpath -hlibSampleTest.so
			DYN_LIB_EXT = so
		else
			abort
		endif
	endif
endif

ifeq "$(CONFIG)" "release"
	CONFIG_TYPE = release
	ifeq "$(OSNAME)" "SunOS"	# Sun Studio
		CONFIGFLAG := -xO3
	else
		CONFIGFLAG := -O2 -DNDEBUG
	endif
else
	CONFIG_TYPE = debug
	CONFIGFLAG := -g
endif 

PWD = $(shell pwd)
ifndef $(OUTDIR)
	OUTDIR=$(PWD)/bin/$(CONFIG_TYPE)_$(BITNESS)
endif

ifndef $(TMPDIR)
	TMPDIR=$(PWD)/tmp/$(CONFIG_TYPE)_$(BITNESS)
endif
ifndef $(SRC_DIR)
	SRC_DIR=./
endif

CXXFLAGS := -m$(BITNESS)

all:	CreateDirectory $(OUTDIR)/UnitTestApp $(OUTDIR)/libSampleTest $(OUTDIR)/libUnitTestFramework.a

UnitTestFramework:	CreateDirectory $(OUTDIR)/libUnitTestFramework.a
UnitTestApp:	CreateDirectory $(OUTDIR)/UnitTestApp
SampleTest:	CreateDirectory $(OUTDIR)/libSampleTest

CreateDirectory:
	$(shell mkdir -p $(OUTDIR))
	$(shell mkdir -p $(TMPDIR))

# UnitTestFramework
$(OUTDIR)/libUnitTestFramework.a: $(TMPDIR)/UnitTestFramework.o
	ar rcs $(OUTDIR)/libUnitTestFramework.a $(TMPDIR)/UnitTestFramework.o

$(TMPDIR)/UnitTestFramework.o: $(SRC_DIR)/UnitTestFramework/UnitTestFramework.h $(SRC_DIR)/UnitTestFramework/print.h
	$(CXX) $(CXXFLAGS) $(CONFIGFLAG) $(PICXXFLAGS) -c -o $(TMPDIR)/UnitTestFramework.o $(SRC_DIR)/UnitTestFramework/UnitTestFramework.cpp

# UnitTest Application
$(OUTDIR)/UnitTestApp: $(TMPDIR)/UnitTestApp.o $(TMPDIR)/Report.o $(OUTDIR)/libUnitTestFramework.a
	$(CXX) $(CXXFLAGS) $(CONFIGFLAG) $(PICXXFLAGS) -o $(OUTDIR)/UnitTestApp $(TMPDIR)/UnitTestApp.o $(TMPDIR)/Report.o -L$(OUTDIR) -lUnitTestFramework -ldl

$(TMPDIR)/UnitTestApp.o: $(OUTDIR)/libUnitTestFramework.a $(SRC_DIR)/UnitTestApp/UnitTestApp.h $(SRC_DIR)/UnitTestApp/System.h
	$(CXX) $(CXXFLAGS) $(CONFIGFLAG) $(PICXXFLAGS) -c -o $(TMPDIR)/UnitTestApp.o -I$(SRC_DIR)/UnitTestFramework $(SRC_DIR)/UnitTestApp/UnitTestApp.cpp

$(TMPDIR)/Report.o: $(OUTDIR)/libUnitTestFramework.a $(SRC_DIR)/UnitTestApp/UnitTestApp.h $(SRC_DIR)/UnitTestApp/System.h
	$(CXX) $(CXXFLAGS) $(CONFIGFLAG) $(PICXXFLAGS) -c -o $(TMPDIR)/Report.o -I$(SRC_DIR)/UnitTestFramework $(SRC_DIR)/UnitTestApp/Report.cpp

# SampleTest
$(OUTDIR)/libSampleTest: $(TMPDIR)/SampleTest.o $(TMPDIR)/TestCollection1.o $(TMPDIR)/TestCollection2.o $(OUTDIR)/libUnitTestFramework.a
	$(CXX) $(CXXFLAGS) $(CONFIGFLAG) $(DYNLIBFLAGS) -o $(OUTDIR)/libSampleTest.$(DYN_LIB_EXT) $(TMPDIR)/SampleTest.o $(TMPDIR)/TestCollection1.o $(TMPDIR)/TestCollection2.o -L$(OUTDIR) -lUnitTestFramework 

$(TMPDIR)/SampleTest.o: $(OUTDIR)/libUnitTestFramework.a  $(SRC_DIR)/SampleTest/SampleTest.h
	$(CXX) $(CXXFLAGS) $(CONFIGFLAG) $(PICXXFLAGS) -c -o $(TMPDIR)/SampleTest.o -I$(SRC_DIR)/UnitTestFramework $(SRC_DIR)/SampleTest/SampleTest.cpp

$(TMPDIR)/TestCollection1.o: $(OUTDIR)/libUnitTestFramework.a  $(SRC_DIR)/SampleTest/SampleTest.h
	$(CXX) $(CXXFLAGS) $(CONFIGFLAG) $(PICXXFLAGS) -c -o $(TMPDIR)/TestCollection1.o -I$(SRC_DIR)/UnitTestFramework $(SRC_DIR)/SampleTest/TestCollection1.cpp

$(TMPDIR)/TestCollection2.o: $(OUTDIR)/libUnitTestFramework.a  $(SRC_DIR)/SampleTest/SampleTest.h
	$(CXX) $(CXXFLAGS) $(CONFIGFLAG) $(PICXXFLAGS) -c -o $(TMPDIR)/TestCollection2.o -I$(SRC_DIR)/UnitTestFramework $(SRC_DIR)/SampleTest/TestCollection2.cpp

clean:
	rm -rf $(TMPDIR)
	rm -f $(OUTDIR)/UnitTestApp $(OUTDIR)/libUnitTestFramework.a $(OUTDIR)/libSampleTest.$(DYN_LIB_EXT)

