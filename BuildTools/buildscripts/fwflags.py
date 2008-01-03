#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

# Utility module for Framewave's SCons scripts

import os
import string
import sys

class fwFlags:
    def __init__ (self, oEnv, lstBuildFactors, sObjectType):
        self.oEnv = oEnv    # no copy, we need to apply the flags to this environment itself
        if 'msvc'   in lstBuildFactors:
            from fwflags_msvc import fwFlags_msvc
            self.flags = fwFlags_msvc  (oEnv, lstBuildFactors, sObjectType)
        if 'gcc'    in lstBuildFactors:
            from fwflags_gcc import fwFlags_gcc
            self.flags = fwFlags_gcc   (oEnv, lstBuildFactors, sObjectType)
        if 'suncc'  in lstBuildFactors:
            from fwflags_suncc import fwFlags_suncc
            self.flags = fwFlags_suncc (oEnv, lstBuildFactors, sObjectType)
