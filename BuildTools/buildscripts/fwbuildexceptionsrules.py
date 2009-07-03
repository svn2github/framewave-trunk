#
# Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

# Exceptions rules for Framewave's SCons scripts
from fwbuildexceptionshandlers import _gcc_AddSubFix
from fwflagsbase               import *


dctExceptionRules = {}
# Rules work on the same principle that flags do
# The more options you specify, the more specific
# will the be application of the rule
# An empty list will apply to every file being built
# 
# The dctExceptionRules dictionary is indexed by toolset
# The value holds a list, the elements of which are a
# list again, with exactly two elements
# First, a list of build factors (can include wildcards for filenames)
# Second, either None for the name of the handler function
#
# See below for examples

# Don't build these filter files when building with suncc
dctExceptionRules[scc] = []

dctExceptionRules[scc] += [[[r'domain/fwImage/src/FilterMedian.cpp'   ], None]]
dctExceptionRules[scc] += [[[r'domain/fwImage/src/FilterGeneral.cpp'  ], None]]
dctExceptionRules[scc] += [[[r'domain/fwImage/src/FilterBasic.cpp'    ], None]]
dctExceptionRules[scc] += [[[r'domain/fwImage/src/Filter.cpp'         ], None]]
dctExceptionRules[scc] += [[[r'domain/fwImage/src/FilterSeparable.cpp'], None]]
dctExceptionRules[scc] += [[[r'domain/fwImage/src/FilterFixed.cpp'    ], None]]


# Use different flags when building the Add and Sub cpp files for reference with gcc
dctExceptionRules[gcc] = []

dctExceptionRules[gcc] += [[[r'*Add*.cpp', lin, ref], _gcc_AddSubFix]]
dctExceptionRules[gcc] += [[[r'*Sub*.cpp', lin, ref], _gcc_AddSubFix]]


# Some test exceptions with msvc
dctExceptionRules[msv] = []

#dctExceptionRules[msv] += [[[r'*rPredict.cpp', win, ref], None]]

