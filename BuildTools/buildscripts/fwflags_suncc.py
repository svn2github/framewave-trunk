#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

import os
from os.path import *

from fwflagsbase import *



dctCCFlags = {}

dctCCFlags['-xO0']              = [dbg]
dctCCFlags['-xO3']              = [rel]

dctCCFlags['-xtarget=opteron']  = [[ss2,ss3,f10]]
dctCCFlags['-xarch=sse2']       = [ss2]
dctCCFlags['-xarch=sse3']       = [[ss3,f10]]

dctCCFlags['-mt']               = []
dctCCFlags['-m32']              = [b32]
dctCCFlags['-m64']              = [b64]
dctCCFlags['-g']                = [dbg]

dctCCFlags['-DSOL32']           = [b32]
dctCCFlags['-DSOL64']           = [b64]

dctCCFlags[r'-DBLD_REF_ONLY']= [[ref,spc]]


# Multipass flag
for ol in oOptimizationLevels[:maxImplementedOptimizationLevel]:
    # For every flag, set the requirement to be its corresponding path
    dctCCFlags[r'-D' + ol['DEFVAR']]                = [ol['OPT_LEVEL']]
    dctCCFlags[r'-DOPT_LEVEL='  + ol['OPT_LEVEL']]  = [ol['OPT_LEVEL']]
    dctCCFlags[r'-DOPT_PREFIX=' + ol['OPT_PREFIX']] = [ol['OPT_LEVEL']]
    dctCCFlags[r'-DDISPATCH='    + ol['DISPATCH']]   = [ol['OPT_LEVEL']]


dctLDFlags = {}

dctLDFlags['-norunpath']        = []
dctLDFlags['-mt -lpthread']     = []
dctLDFlags['-m32']              = [b32]
dctLDFlags['-m64']              = [b64]

dctLDFlags['-lCrun']                    = [shr]
dctLDFlags['-lrt']                      = [shr]
dctLDFlags['-lCstd']                    = [shr]
dctLDFlags['-hlib${PROJECTNAME}.so.1']  = [shr]


class fwFlags_suncc(fwFlagsBase):
    def __init__ (self, oEnv, lstBuildFactors, sObjectType):
        # Call constructor for base class
        fwFlagsBase.__init__(self, oEnv)
        
        # Select flags to setup based on what kind of
        # build object are we setting up flags for
        if sObjectType=='library':
            self.lstLDFlags = self.determineFlags(lstBuildFactors, dctLDFlags)
            self.lstLDFlags = self.translateFlags(self.lstLDFlags)
        else:
            self.lstCCFlags = self.determineFlags(lstBuildFactors, dctCCFlags)
            self.lstCCFlags = self.translateFlags(self.lstCCFlags)

        self.applyFlags()
