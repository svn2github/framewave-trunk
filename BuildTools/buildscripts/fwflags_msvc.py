#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
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

dctCCFlags[r'/O2']           = [rel]
dctCCFlags[r'/Od']           = [dbg]
dctCCFlags[r'/favor:AMD64']  = [rel, b64, [ss2, ss3, f10]]




dctCCFlags[r'/GS-']          = []
dctCCFlags[r'/EHa']          = []
dctCCFlags[r'/RTC1']         = [dbg]
dctCCFlags[r'/Gz']           = []
dctCCFlags[r'/arch:SSE2']    = [rel, b32, [ss2, ss3, f10]]

dctCCFlags[r'/MT']           = [mtr]
dctCCFlags[r'/MTd']          = [mtd]
dctCCFlags[r'/MD']           = [mdr]
dctCCFlags[r'/MDd']          = [mdd]

dctCCFlags[r'/Zi']           = None
dctCCFlags[r'/ZI']           = None
dctCCFlags[r'/Z7']           = [dbi]

dctCCFlags[r'/W4']           = []
dctCCFlags[r'/WX']           = []

dctCCFlags[r'/D_WINDOWS']    = []
dctCCFlags[r'/D_USRDLL' ]    = []
dctCCFlags[r'/D${PROJECTNAMEU}_EXPORTS'] = [[shr,stc]]

dctCCFlags[r'/DNO_ASSERT']   = [dbg]
dctCCFlags[r'/D_DEBUG'   ]   = [dbg]
dctCCFlags[r'/DNDEBUG']      = [rel]

dctCCFlags[r'/D_WINDLL']     = [shr]
dctCCFlags[r'/D_IS_FW_LIB'] = [stc]

dctCCFlags[r'/DWIN32']       = [b32]
dctCCFlags[r'/DWIN64']       = [b64]

dctCCFlags[r'/DBLD_REF_ONLY']= [[ref,spc]]

dctCCFlags[r'/FD']           = [rel]
dctCCFlags[r'/errorReport:prompt'] = []



# Multipass flag
for ol in oOptimizationLevels[:maxImplementedOptimizationLevel]:
    # For every flag, set the requirement to be its corresponding path
    dctCCFlags[r'/D' + ol['DEFVAR']]                = [ol['OPT_LEVEL']]
    dctCCFlags[r'/DOPT_LEVEL='  + ol['OPT_LEVEL']]  = [ol['OPT_LEVEL']]
    dctCCFlags[r'/DOPT_PREFIX=' + ol['OPT_PREFIX']] = [ol['OPT_LEVEL']]
    dctCCFlags[r'/DDISPATCH='    + ol['DISPATCH']]   = [ol['OPT_LEVEL']]



dctLDFlags = {}

dctLDFlags[r'/OPT:REF']          = [rel]
dctLDFlags[r'/OPT:ICF']          = [rel]
dctLDFlags[r'/DEBUG']            = [dbg]
dctLDFlags[r'/RELEASE']          = [rel]
dctLDFlags[r'/INCREMENTAL:NO']   = [dbg]
dctLDFlags[r'/MANIFEST:NO']      = [shr]
dctLDFlags[r'/PDB:$']            = None
dctLDFlags[r'/PROFILE']          = [dbg]
dctLDFlags[r'/SUBSYSTEM:WINDOWS']= [shr]
dctLDFlags[r'/MACHINE:X86']      = [b32]
dctLDFlags[r'/MACHINE:X64']      = [b64]


class fwFlags_msvc(fwFlagsBase):
    def __init__ (self, oEnv, lstBuildFactors, sObjectType):
        # Call constructor for base class
        fwFlagsBase.__init__(self, oEnv)

        dctFwVars =  oEnv['FWVARS']
        dctCCFlags[dctFwVars['REF_CCFLAGS']]   = [ref]
        dctCCFlags[dctFwVars['SSE2_CCFLAGS']]  = [ss2]
        dctCCFlags[dctFwVars['F10H_CCFLAGS']]  = [f10]

        # Select flags to setup based on what kind of
        # build object are we setting up flags for
        if sObjectType=='library':
            self.lstLDFlags = self.determineFlags(lstBuildFactors, dctLDFlags)
            self.lstLDFlags = self.translateFlags(self.lstLDFlags)
        else:
            self.lstCCFlags = self.determineFlags(lstBuildFactors, dctCCFlags)
            self.lstCCFlags = self.translateFlags(self.lstCCFlags)

        self.applyFlags()
