#
# Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

# Class module for Framewave's library files

import glob
import os
from string import replace
from os.path import *

import sys
sys.path = [ join(sys.prefix, 'Lib', 'site-packages', 'scons-0.97'),
             join(sys.prefix, 'Lib', 'site-packages', 'scons'),
             join(sys.prefix, 'scons-0.97'), join(sys.prefix, 'scons')] + sys.path

from SCons.Script import *
from fwflags import *
from fwutil  import *
from fwbuildexceptions import dctGlobalExceptionsList

class fwLibrary:

    # This function will create a list of all the factors that may effect our build
    # These currently include:
    # . src filename
    # . variant, libtype, bitness
    # . toolset (msvc, gcc, suncc)
    #
    # Additional build factors can be added here
    def getBuildFactors( self, oEnv ):
        # retrieve the fwvars object from the associated environment
        oFWVARS = oEnv['FWVARS']

        # Get variant, libtype and bitness
        variant = oFWVARS['variant']
        libtype = oFWVARS['libtype']
        bitness = oFWVARS['bitness']

        # Set debug information build factor
        if oFWVARS['debuginfo'] == 'off': debuginfo = None
        else:                            debuginfo = 'debuginfo'
        
        # Get platform building on
        # Since there is no cross building yet
        # we can assume that the platform we're
        # building on is also the target platform
        platform = sys.platform
 
        # Get toolset name
        toolset = oFWVARS['toolset']
        
        # Collect the build factors into a list and return
        lstBuildFactors = [self.pthTarget, variant, libtype, bitness, platform, debuginfo, toolset]

        lstBuildFactors = removeNones(lstBuildFactors)
        return lstBuildFactors
        
    def getEnvForLibrary( self, oEnv ):
        lstBuildFactors = self.getBuildFactors(oEnv)
        
        oExceptFlags = None
        toolset = oEnv['FWVARS']['toolset']
        # Check if there is any special handling for this object
        if dctGlobalExceptionsList.has_key(toolset):
            for r in dctGlobalExceptionsList[toolset]:
                if allInList(r[0], lstBuildFactors):
                    oExceptFlags = r[1]
                    # Check if we even should build this object at all
                    if oExceptFlags == None: return None

        # Make an object local copy of the environment
        oNewEnv = oEnv.Clone()
        
        # Initialize a flags object based on our current environment
        # and build factors; at the end of this function, our flags
        # would have been applied onto the environment object passed in
        
        # If there was a hit for a special flags handler in the exception list
        # instantiate that instance. The exception rules should make sure that
        # this handler is appropriate and the correct one for the given toolset
        if oExceptFlags: oFlags = oExceptFlags(oNewEnv, lstBuildFactors, "library")
        # Otherwise, there is no special handling;
        # call the appropriate flags handler
        else:            oFlags = fwFlags    (oNewEnv, lstBuildFactors, "library")
        
        # Return our environment; Flags should not have been applied
        return oNewEnv


    # This function will create one fwObject
    def __init__ ( self, oEnv, pthTarget, lstObjects ):
        self.pthTarget  = pthTarget
        self.oEnv       = self.getEnvForLibrary(oEnv)
        self.dctFwVars = self.oEnv['FWVARS']
        self.lstObjects = lstObjects
        
        # In case we shouldn't be building this library
        if not self.oEnv:
            # Set our saved build object to None and return
            self.oObject = None
            return

        # Pull the libtype from the FWVARS object associated
        # with this enviroment, set the build function based on it
        libtype = self.oEnv['FWVARS']['libtype']
        if   (libtype == 'shared'): fLibraryCreate = self.oEnv.SharedLibrary
        elif (libtype == 'static'): fLibraryCreate = self.oEnv.StaticLibrary
        else:                       fLibraryCreate = self.oEnv.Program

        # Final step; create the build object in this environment
        # (flags have already been applied) and save that object
        self.oLibrary = fLibraryCreate(self.pthTarget, self.lstObjects,
                                       LIBS=self.dctFwVars['LIBS'],
                                       LIBPATH=self.dctFwVars['LIBPATH'])

    def getLibrary( self ):
        return self.oLibrary
