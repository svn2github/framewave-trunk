#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

from __future__ import with_statement

# Utility module for Framewave's SCons scripts

import glob
import os
from string import replace
from os.path import *

import sys
sys.path = [ join(sys.prefix, 'Lib', 'site-packages', 'scons-0.97'),
             join(sys.prefix, 'Lib', 'site-packages', 'scons'),
             join(sys.prefix, 'scons-0.97'), join(sys.prefix, 'scons')] + sys.path

from SCons.Script import *

oOptimizationLevel_REFR = { 'DEFVAR':"OPT_REFR", 'OPT_LEVEL':"REFR", 'OPT_PREFIX':"_refr_", 'DISPATCH':'DT_REFR', 'MARKER':'OREFR' }
oOptimizationLevel_SSE2 = { 'DEFVAR':"OPT_SSE2", 'OPT_LEVEL':"SSE2", 'OPT_PREFIX':"_sse2_", 'DISPATCH':'DT_SSE2', 'MARKER':'OSSE2' }
oOptimizationLevel_SSE3 = { 'DEFVAR':"OPT_SSE3", 'OPT_LEVEL':"SSE3", 'OPT_PREFIX':"_sse3_", 'DISPATCH':'DT_SSE3', 'MARKER':'-----' } # no marker allowed for SSE3
oOptimizationLevel_F10H = { 'DEFVAR':"OPT_F10H", 'OPT_LEVEL':"F10H", 'OPT_PREFIX':"_f10h_", 'DISPATCH':'DT_F10H', 'MARKER':'OF10H' }


oOptimizationLevels = [oOptimizationLevel_REFR,\
                       oOptimizationLevel_SSE2,\
                       oOptimizationLevel_F10H,\
                       oOptimizationLevel_SSE3]
maxImplementedOptimizationLevel = 4 # base is 0, max opt + 1

# Similar to glob, except glob's an entire directory tree
def globtree ( pthPath ):
    lstResult = []
    try:
        pthDirName, pthBaseName = os.path.split ( pthPath )
        if not pthDirName: pthDirName = os.curdir

        lstDirs = filter ( os.path.isdir, ( join ( pthDirName, f ) for f in os.listdir ( pthDirName ) ) )

        for d in lstDirs:
            lstResult += globtree ( join ( d, pthBaseName ) )

        lstResult += glob.glob ( pthPath )
    except:
        # if there was an exception, return an empty list
        lstResult = []

    return lstResult

# Finds all the directories which have .h files in it, starting from start path
# and adds them to the given environment
# Note: For current code structure reasons, the pthStartPath is also added as an include path
#       whether or not it has any .h files in it
def setupIncludePaths ( oEnv, pthStartPath, dontRecurse=None ):
    if dontRecurse: lstAllHeaders = [os.path.dirname(pthStartPath)]
    else:           lstAllHeaders = (os.path.dirname(f) for f in globtree(join( pthStartPath, "*.h" )))

    lstIncludes = []
    if oEnv.has_key('CPPPATH'):
        if ('#/'+pthStartPath) not in oEnv['CPPPATH']: lstIncludes = ['#/'+pthStartPath]
    else:
        oEnv.Append( CPPPATH= ['#/'+pthStartPath])

    for f in lstAllHeaders:
        if (('#/'+f) not in lstIncludes) and (('#/'+f) not in oEnv['CPPPATH']):
            lstIncludes += ['#/'+f]
    oEnv.Append( CPPPATH = lstIncludes)

def createEnviron ():
    # Do any environment modifications here
	oEnviron = os.environ
	return oEnviron

def src2obj( pth ):
    pthRootDir, sRootName = split(pth)
    if (len(pthRootDir) >= 3):
        if (pthRootDir[-3:].lower() == "src"):
            pthRootDir = pthRootDir[:-3] + "obj"

    return join(pthRootDir, sRootName)

def buildPath( pthBuildRoot ):
    return join( pthBuildRoot,\
				 ARGUMENTS.get('variant','debug')  + "_" +\
				 ARGUMENTS.get('libtype','shared') + "_" +\
				 ARGUMENTS.get('bitness','32') )


def turnOnZi( oEnv ):
	import SCons.Util
	oEnv['CCPDBFLAGS'] = SCons.Util.CLVar(['${(PDB and "/Gm /ZI /Fd%s" % File(PDB)) or ""}'])

def fixLinuxSharedLib( target = None, source = None, env = None ):
    if target:
        for t in target: os.rename(t.path, t.path+'.1.0.0')

def fixMacSharedLib( target = None, source = None, env = None ):
    if target:
        for t in target: os.rename(t.path, replace(t.path,'.dylib','-1.0.dylib'))

def AddObjsFromLibs( oEnv, LIBS, LIBPATH ):
    # If no libs exist, nothing needs ot be added
    if not LIBS: return

    libprefix      = oEnv['LIBPREFIX']
    libsuffix      = oEnv['LIBSUFFIX']
    pthBuildDir    = oEnv['FWVARS']['pthBuildDir']
    pthDevLabRoot = oEnv['FWVARS']['pthDevLabRoot']

    for l in LIBS:
        lib = None
        if (l[:2]=='fw'): continue # Do not process Framewave static libraries, they will be shipped anyway
        for lp in LIBPATH:
            if exists( join(lp,libprefix+l+libsuffix) ):
                lib =  join(lp,libprefix+l+libsuffix)
                break
        if lib:
            pthObjsOutDir = join(pthBuildDir,'extracted',l)
            os.system(join(pthDevLabRoot,'Framewave','ar_ex.sh')+ " " + lib + " " + pthObjsOutDir)

            oExtraObjs = globtree(join(pthObjsOutDir,'*.o'))+globtree(join(pthObjsOutDir,'*.obj'))
            oExtraObjs = [ oEnv.File('#//'+o) for o in oExtraObjs ]

            if 'EXTRALIBOBJS' in oEnv['FWVARS']: oEnv['FWVARS']['EXTRALIBOBJS'] += oExtraObjs
            else: oEnv['FWVARS']['EXTRALIBOBJS'] = oExtraObjs
        else:
            print "Could not find library " + l + ".."
            print LIBPATH
            sys.exit(0)

def removeD( s, isD ):
   if isD: return s[:-1]
   else:   return s

def isInList( s, lst ):
    bInList = None
    for e in lst:
        if s.find(e) != -1:
            bInList = 1
            break
    return bInList

def isListIn( s, lst ):
    bInList = None
    for e in lst:
        if e.find(s) != -1:
            bInList = 1
            break
    return bInList

def applyPrefixToFilename( pthFile, oPath ):
    # If single pass compile, return original filename
    if not oPath: return pthFile

    # Seperate out the filename from the path
    dirname, basename = split( pthFile )
    return join( dirname, (oPath['OPT_PREFIX'] + basename) )

def readFile ( sInFile ):
    try:
        with open(sInFile, 'r') as f:
            data = f.read()
    except IOError:
        print "Could not open " + sInFile  + "!\n"
        return None
    return data

def writeFile ( sOutFile, lstData ):
    try:
        with open(sOutFile, 'w') as f:
            for d in lstData:
                f.write( d )
    except IOError:
        print "Could not open " + sOutFile + "!\n"

def writeLinesToFile ( sOutFile, lstData ):
    try:
        with open(sOutFile, 'w') as f:
            for d in lstData:
                f.write( d + '\n')
    except IOError:
        print "Could not open " + sOutFile + "!\n"

def getPaths( sInFile ):
    try:
        with open(sInFile, 'r') as f:
            f.seek(-200, os.SEEK_END)
            lines = f.readlines()
    except IOError:
        print "Could not open " + sInFile  + "!\n"
        return None

    lstLines = []
    for f in lines:
        if len(f) > 2: lstLines += [f]

    lstOptLevels = []
    line = lstLines[-1]
    for ol in oOptimizationLevels[:maxImplementedOptimizationLevel]:
        if (line.find(ol['MARKER']) != -1): lstOptLevels += [ol['MARKER']]

    return lstOptLevels

# Check if this element, or if any element in this element list, is present in the given list lst
# This function expects a base element to be a string
def anyInList( e, lst ):
    # Check if this is a single element or a list
    if type(e) == type(""):
        if absPathIfFile(e) in [absPathIfFile(l) for l in lst]:
            return 1
        else:
            return None
    else:
        for elem in list(e):
            if anyInList(elem, lst): return 1
        return None

def allInList( lstToMatch, lstToMatchIn ):
    # Go through the build factors for which this flag should be used
    for f in lstToMatch:
        # If any of the build factors is NOT in the list of build factors
        # for this object, we cannot use this flag
        if not anyInList(f, lstToMatchIn):
            return None
            break

    return 1

def isFile( sElem ):
    if sElem.find('.') == -1: return None
    return 1

def absPathIfFile( sElem ):
    if isFile(sElem): return sElem
    else:             return abspath(sElem)

def appendToFilename( pthFile, sPostfix ):
    # Seperate out the filename and extension
    pthRoot, sExt = os.path.splitext( pthFile )
    return pthRoot+sPostfix+sExt

def removeNones( lstList ):
    lstReturnList = []
    for x in lstList:
        if (x != None): lstReturnList += [x]
    return lstReturnList
