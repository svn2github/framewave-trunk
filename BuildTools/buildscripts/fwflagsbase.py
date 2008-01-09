#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

# Utility module for Framewave's SCons scripts

import os
import string

from os.path import *
from fwutil import *


# Short cuts to the various build factors strings
dbg = 'debug'
rel = 'release'

b32 = '32'
b64 = '64'

shr = 'shared'
stc = 'static'
exe = 'exe'

lin = 'linux2'
win = 'win32'
sol = 'sunos5'
mac = 'darwin'

ref = 'REFR'
ss2 = 'SSE2'
ss3 = 'SSE3'
f10 = 'F10H'
spc = 'SiPC'


gcc = 'gcc'
msv = 'msvc'
scc = 'suncc'

# Shortcut for the debug info build factor
dbi = 'debuginfo'

# Shortcuts for the win crt build factors
mtr = 'wincrtmt'
mtd = 'wincrtmtd'
mdr = 'wincrtmd'
mdd = 'wincrtmdd'


class fwFlagsBase:
    def __init__ (self, oEnv):
        self.lstCCFlags = []
        self.lstLDFlags = []
        
        self.oEnv = oEnv    # no copy, we need to apply the flags to this environment itself
        self.dctFwVars = oEnv['FWVARS']
        
        self.variant    = self.dctFwVars['variant']
        self.libtype    = self.dctFwVars['libtype']
        self.bitness    = self.dctFwVars['bitness']
        self.transdict  = self.dctFwVars['transdict']
        
    def translateFlags (self, lstFlags):
    	lstResult = []
    	for f in lstFlags:
            st = string.Template( f )
            lstResult += [ st.safe_substitute(self.transdict) ]
    	return lstResult

    def applyFlags (self):
        for c in self.lstCCFlags: self.oEnv.Append( CCFLAGS  = ' ' + c )
        for l in self.lstLDFlags: self.oEnv.Append( LINKFLAGS = ' ' + l )

    def determineFlags( self, lstBuildFactors, dctFlags ):
        lstFlags = []
        # Go through all the CC flags available for the given toolset
        for k in dctFlags:
            # Check if this flag is to be used at all
            if dctFlags[k] != None:
                # If all the build factors for the flag exist in the list of build factors
                # for this object, then go ahead and use this flag for the current object
                if allInList(dctFlags[k], lstBuildFactors): lstFlags += [k]
        return lstFlags
