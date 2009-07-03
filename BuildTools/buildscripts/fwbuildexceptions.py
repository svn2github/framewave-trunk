#
# Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

# Exceptions list module for Framewave's SCons scripts
import os
from string import replace
from os.path import *

import sys
sys.path = [ join(sys.prefix, 'Lib', 'site-packages', 'scons-0.97'),
             join(sys.prefix, 'Lib', 'site-packages', 'scons'),
             join(sys.prefix, 'scons-0.97'), join(sys.prefix, 'scons')] + sys.path

from SCons.Script import *
from fwutil      import *

from fwbuildexceptionsrules    import dctExceptionRules
from fwbuildexceptionshandlers import *

dctGlobalExceptionsList = {}

# Expand all file names to their full paths
# File names will be searched for recursively
# using the Framewave directory as the base

# Iterate through the toolsets
for ts in dctExceptionRules:
    # Go through all the rules in the given toolset
    dctGlobalExceptionsList[ts] = []
    for r in dctExceptionRules[ts]:
        lstMatches = None
        
        sFileBuildFactor = ""
        # Go through all the build factors in the first element of the rule
        for e in r[0]:
            # If the build factor is a filename (if it has a . or a wildcard, either indicates that its a filename)
            # To list a cpp or library file, one of the two is absolutely required
            if (e.find('.') != -1) or (e.find('*') != -1) or (e.find('?') != -1):
                sf = e
                # Find the filename, or all matches
                lstMatches = globtree(e)
                sFileBuildFactor = e

        # Only if files were found matching the build exception rule
        if lstMatches:
            # Go through the list of files and create rules in the actual Global Exceptions list
            for f in lstMatches:
                # List of all the build factors, except the one that matches the filename build factor that
                # we had determined was the file build factor. Instead prepend the file from the matches list
                lstBuildFactors = [abspath(f)] + [bf for bf in r[0] if bf != sFileBuildFactor]
                
                # Add the new rule with the rule's build factors list[with changed filename] and the rule action
                dctGlobalExceptionsList[ts] += [[lstBuildFactors, r[1]]]
