#
# Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

# Exceptions handlers for Framewave's SCons scripts
import os
from os.path import *
from fwflagsbase import *

# In this file, you can specify special handlers for exceptions that you want to deal with
# The flags special handler will be a function of the following form;
# Parameters:
# . oEnv                            - The current environment for this object
# . lstBuildFactors                 - List of build factors associated with the object
# . sObjectType                     - A string specifying if the object is an object file or a library
#
# Return value:
# . fwFlagsBase [or derived from]  - This needs to be an instantiated flags object
#
# Not entirely coincidentaly, these are the same parameters that are passed into the
# constructor of a flags class when instantiating a flags object
#
# See the examples below for further clarity


# -----------------------------------------------------------------------------------------------------------
# Add/Sub fix:
# GCC generates incorrect code when compiling the reference code for the Add_16sc and Sub_16sc functions
# with the -O2 flag. For the files that contain these functions, we need to turn off certain flags and leave
# the rest on. Hence, here we restrict the -O2 flag only to SSE* code, and give a different set of flags that
# we've verified as working correctly, for reference code 
# -----------------------------------------------------------------------------------------------------------

def _gcc_AddSubFix( oEnv, lstBuildFactors, sObjectType ):
    return fwFlags_gcc_AddSubFix(oEnv, lstBuildFactors, sObjectType)

class fwFlags_gcc_AddSubFix(fwFlagsBase):
    def __init__ ( self, oEnv, lstBuildFactors, sObjectType ):
        # Pull in the flags from gcc
        from fwflags_gcc import dctCCFlags

        # fix up the flags and add one more, to seperate out the
        # optimizations set on reference code and on SIMD optimized code
        dctCCFlags[r'-O1  -fthread-jumps -fpeephole2 -fcrossjumping -foptimize-sibling-calls  -fdelete-null-pointer-checks  -fcse-follow-jumps   -fcse-skip-blocks   -fgcse   -fgcse-lm   -fexpensive-optimizations  -frerun-cse-after-loop   -fcaller-saves  -fsched-interblock  -fsched-spec  -fregmove  -freorder-blocks  -freorder-functions  -falign-functions  -falign-jumps  -falign-loops  -falign-labels  -ftree-vrp  -ftree-pre'] = [rel, ref]
        dctCCFlags[r'-O2']           = [rel, [ss2,ss3,f10]]


        # Call constructor for base class
        fwFlagsBase.__init__(self, oEnv)

        # Select flags to setup based on what kind of
        # build object are we setting up flags for
        if sObjectType=='library':
            raise exception, "Error: This rule is meant to apply only on object files!"
        else:
            self.lstCCFlags = self.determineFlags(lstBuildFactors, dctCCFlags)
            self.lstCCFlags = self.translateFlags(self.lstCCFlags)

        self.applyFlags()
