#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

# Utility module for Framewave's SCons scripts

import os
import string
import sys


# TODO: Fixup this section to pull flags file correctly
if sys.platform == 'win32':
    from fwflags_msvc import *
elif sys.platform  == 'linux2':
    from fwflags_gcc import *
elif sys.platform  == 'sunos5':
    from fwflags_suncc import *
elif sys.platform == 'darwin':
    from fwflags_gcc import *

class fwFlags:
    def __init__ (self, oEnv, lstBuildFactors, sObjectType):
        self.oEnv = oEnv    # no copy, we need to apply the flags to this environment itself
        if 'msvc'   in lstBuildFactors: self.flags = fwFlags_msvc  (oEnv, lstBuildFactors, sObjectType)
        if 'gcc'    in lstBuildFactors: self.flags = fwFlags_gcc   (oEnv, lstBuildFactors, sObjectType)
        if 'suncc'  in lstBuildFactors: self.flags = fwFlags_suncc (oEnv, lstBuildFactors, sObjectType)
