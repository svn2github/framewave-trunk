#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

# Class module for Framewave's object files

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

class fwObject:

    # This function will create a list of all the factors that may effect our build
    # These currently include:
    # . src filename
    # . variant, libtype, bitness
    # . toolset (msvc, gcc, suncc)
    # . path building for
    #
    # Additional build factors can be added here
    def getBuildFactors( self, oEnv ):
        # retrieve the fwvars object from the associated environment
        oFWVARS = oEnv['FWVARS']

        # Get variant, libtype and bitness
        variant = oFWVARS['variant']
        libtype = oFWVARS['libtype']
        bitness = oFWVARS['bitness']
        
        # Get platform building on
        # Since there is no cross building yet
        # we can assume that the platform we're
        # building on is also the target platform
        platform = sys.platform
 
        # Get toolset name
        toolset = oFWVARS['toolset']
        
        # We could be building a single pass object, so check if path even exists
        if self.oPath:
            lstBuildFactors = [self.pthSrc, variant, libtype, bitness, platform, toolset, self.oPath['OPT_LEVEL']]
        else:
            # Set the path to the single pass canary token
            lstBuildFactors = [self.pthSrc, variant, libtype, bitness, platform, toolset, 'SiPC']
        
        return lstBuildFactors

    def getEnvForObject( self, oEnv ):
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
        if oExceptFlags: oFlags = oExceptFlags(oNewEnv, lstBuildFactors, "object")
        # Otherwise, there is no special handling;
        # call the appropriate flags handler
        else:            oFlags = fwFlags    (oNewEnv, lstBuildFactors, "object")
        
        # Return our environment; Flags should not have been applied
        return oNewEnv


    # This function will create one fwObject
    def __init__ ( self, oEnv, pthTarget, pthSrc, oPath=None ):
        self.pthSrc = pthSrc
        self.pthTarget = pthTarget # REMEMBER: do not use this path till prefix has been prepended
        self.oPath   = oPath
        self.oEnv   = self.getEnvForObject(oEnv)
        
        # In case we shouldn't be building this object
        if not self.oEnv:
            # Set our saved build object to None and return
            self.oObject = None
            return

        # Pull the libtype from the FWVARS object associated
        # with this enviroment, set the build function based on it
        libtype = self.oEnv['FWVARS']['libtype']
        if (libtype == 'shared'): fObjectCreate = self.oEnv.SharedObject
        else:                     fObjectCreate = self.oEnv.StaticObject # exe's use static objects

        # Get the target name (basicaly original target named prefixed with path prefix)
        self.pthTarget = applyPrefixToFilename(self.pthTarget, oPath)
        
        # Final step; create the build object in this environment
        # (flags have already been applied) and save that object
        self.oObject = fObjectCreate(self.pthTarget, self.pthSrc)

    def getObject( self ):
        if self.oObject: return [self.oObject]
        else:            return []


class fwMPObject:
    def __init__ ( self, oEnv, pthTarget, pthSrc):
        self.oEnv = oEnv
        self.pthSrc = pthSrc
        self.pthTarget = pthTarget
        lstPathObjects = []
        
        # Scan file to get paths (has to the be first line of the file)
        lstPaths = getPaths(pthSrc)
        
        # If no paths are present, compile for single pass
        if not lstPaths:
            lstPathObjects += [fwObject(self.oEnv, pthTarget, pthSrc)]
        else:
            # Interate over all paths. Create an object for paths for which we have markers
            for pth in oOptimizationLevels[:maxImplementedOptimizationLevel]:
                if pth['MARKER'] in lstPaths:
                    # Create an fwObject for this path and store it in the objects list
                    # These need to be created from the object list passed onto us
                    lstPathObjects += [fwObject(self.oEnv, pthTarget, pthSrc, pth)]
                
        # Save away the list of individual path specific objects
        self.lstPathObjects = lstPathObjects

    def getObjects( self ):
        lstObjects = []
        for po in self.lstPathObjects:
            # If the build object exists
            if po.getObject():
                # only then add it to the return list
                lstObjects += [po.getObject()[0]]
            
        return lstObjects
