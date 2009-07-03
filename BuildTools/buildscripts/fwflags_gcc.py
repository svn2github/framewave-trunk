#
# Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

import os
from os.path import *
from fwflagsbase import *

# Specify flags in this format;
#
# If you want a flag to be active ONLY if a particular element of a
# build factor is present, add it in the list. If you want the flag
# to be present if any of x elements of a build factor are present
# you can either specify the same factors over again, with first the
# first element of x then the second element of x, etc; optionally,
# you can simply give a list with the x elements as a part of the
# build factor elements list. See the examples below.
#
# Special cases: If a flag is to be applied everywhere, just leave the list empty
#                If a flag is to be applied nowhere, set to it to None

dctCCFlags = {}

dctCCFlags[r'-O2']           = [rel]

dctCCFlags[r'-msse2']        = [ss2]
dctCCFlags[r'-msse3']        = [[ss2,f10]]

dctCCFlags[r'-m32']          = [b32]
dctCCFlags[r'-m64']          = [b64]

dctCCFlags[r'-g']            = [dbg]

dctCCFlags[r'-Werror']       = []

dctCCFlags[r'-DNDEBUG']      = [rel]

dctCCFlags[r'-DLIN32']       = [b32, lin]
dctCCFlags[r'-DLIN64']       = [b64, lin]

dctCCFlags[r'-DSOL32']       = [b32, sol]
dctCCFlags[r'-DSOL64']       = [b64, sol]

dctCCFlags[r'-DMAC32']       = [b32, mac]
dctCCFlags[r'-DMAC64']       = [b64, mac]

dctCCFlags[r'-DBLD_REF_ONLY']= [[ref,spc]]


# Multipass flag
for ol in oOptimizationLevels[:maxImplementedOptimizationLevel]:
    # For every flag, set the requirement to be its corresponding path
    dctCCFlags[r'-D' + ol['DEFVAR']]                = [ol['OPT_LEVEL']]
    dctCCFlags[r'-DOPT_LEVEL='  + ol['OPT_LEVEL']]  = [ol['OPT_LEVEL']]
    dctCCFlags[r'-DOPT_PREFIX=' + ol['OPT_PREFIX']] = [ol['OPT_LEVEL']]
    dctCCFlags[r'-DDISPATCH='    + ol['DISPATCH']]   = [ol['OPT_LEVEL']]



dctLDFlags = {}


dctLDFlags[r'-lrt -pthread -lm -Wl -hlib${PROJECTNAME}.so.1'] = [shr, sol]
dctLDFlags[r'-m32']          = [b32]
dctLDFlags[r'-m64']          = [b64]


dctLDFlags['-install_name /usr/local/lib/${BITNESS}'+ \
           '/lib${PROJECTNAME}.1.dylib'+' '+ \
           '-compatibility_version 1.0 -current_version 1.1.0'] = [shr, mac]


class fwFlags_gcc(fwFlagsBase):
    def __init__ (self, oEnv, lstBuildFactors, sObjectType):
        # Call constructor for base class
        fwFlagsBase.__init__(self, oEnv)

        # Select flags to setup based on what kind of
        # build object are we setting up flags for



        dctFwVars = oEnv['FWVARS']

        dctCCFlags[dctFwVars['REF_CCFLAGS']]   = [ref]
        dctCCFlags[dctFwVars['SSE2_CCFLAGS']]  = [ss2]
        dctCCFlags[dctFwVars['F10H_CCFLAGS']]  = [f10]

        dctLDFlags[r'-l' + dctFwVars['thread'] + ' -Wl,-soname,lib${PROJECTNAME}.so.1'] = [shr, lin]
        if sObjectType=='library':
            self.lstLDFlags = self.determineFlags(lstBuildFactors, dctLDFlags)
            self.lstLDFlags = self.translateFlags(self.lstLDFlags)
        else:
            self.lstCCFlags = self.determineFlags(lstBuildFactors, dctCCFlags)
            self.lstCCFlags = self.translateFlags(self.lstCCFlags)

        self.applyFlags()
