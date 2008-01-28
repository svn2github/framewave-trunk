#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

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
from fwutil import *
from fwflags import *
from fwparse import constructMultipassCPP, maxOptimizationLevel
from fwObject  import *
from fwLibrary import *

class fwBuildRoot:
    def __init__ (self, pthProjectRoot, pthBuildRoot, pthBuildScripts = None):
        self.dctFwVars                   = {}
        self.dctFwVars['pthProjectRoot'] = pthProjectRoot
        self.dctFwVars['pthDevLabRoot']  = abspath('..')
        self.dctFwVars['pthBuildRoot']   = pthBuildRoot
        self.dctFwVars['pthScratch']     = join(self.dctFwVars['pthBuildRoot'], "tmp")

        self.dctFwVars['variant']        = ARGUMENTS.get('variant',   'debug')
        self.dctFwVars['libtype']        = ARGUMENTS.get('libtype',   'shared')
        self.dctFwVars['bitness']        = ARGUMENTS.get('bitness',   '32')
        self.dctFwVars['debuginfo']      = ARGUMENTS.get('debuginfo', '---')
        self.dctFwVars['toolset']        = ARGUMENTS.get('toolset',   '---')
        self.dctFwVars['wincrt']         = ARGUMENTS.get('wincrt',    '---')

        # Set correct default wincrt
        if self.dctFwVars['wincrt'] == '---':
            if self.dctFwVars['variant']=='debug': self.dctFwVars['wincrt'] = 'mtd'
            else:                                  self.dctFwVars['wincrt'] = 'mt'

        # Set our current toolset if none is set from the command line
        if self.dctFwVars['toolset'] == '---':
            if   sys.platform=='win32':
                self.dctFwVars['toolset'] = 'msvc'
            elif sys.platform=='linux2':
                self.dctFwVars['toolset'] = 'gcc'
            elif sys.platform=='sunos5':
                self.dctFwVars['toolset'] = 'suncc'
            elif sys.platform=='darwin':
                self.dctFwVars['toolset'] = 'gcc'


        # For everything other than msvc, we just go ahead and use the default toolsets
        buildtoolspath = []

        # Based on our toolset, set up the SCons tools
        if   self.dctFwVars['toolset'] == 'msvc':
            buildtools     = ['fwmsvc', 'fwmslink', 'fwmslib']
            if not pthBuildScripts:
                pthBuildScripts = abspath(join(self.dctFwVars['pthProjectRoot'], '..', '..', 'BuildTools', 'buildscripts'))
            buildtoolspath = [pthBuildScripts]
        elif self.dctFwVars['toolset'] == 'suncc':
            buildtools     = ['sunc++', 'suncc', 'sunlink', 'sunar']
        elif self.dctFwVars['toolset'] == 'gcc':
            # Exception on Apple, even if using the gcc, applelink needs to be used as the linker
            if sys.platform == 'darwin':
                buildtools = ['g++', 'gcc', 'applelink', 'ar']
            else:
                buildtools = ['g++', 'gcc', 'gnulink', 'ar']

        if   (self.dctFwVars['debuginfo']=='---') and (self.dctFwVars['variant']=='debug'):
            self.dctFwVars['debuginfo'] = 'on'
        elif (self.dctFwVars['debuginfo']=='---') and (self.dctFwVars['variant']=='release'):
            self.dctFwVars['debuginfo'] = 'off'

        curEnviron = createEnviron()
        self.oEnv = Environment( toolpath=buildtoolspath, tools=buildtools, ENV=curEnviron, FWVARS=self.dctFwVars )

		# fixup paths back to what they were before scons messed them up [BUGBUG: SCons issue, this is the workaround]
        if ((sys.platform=='win32') and (self.dctFwVars['bitness']!='32')):
			self.oEnv['ENV'] = curEnviron

    def subProject(self, sProjectName, lstDependencies = None, altProjectDir = None, libtypeOverride = None):
    	if altProjectDir == None: pthProjectDir = join( self.dctFwVars['pthProjectRoot'], sProjectName )
    	else:                     pthProjectDir = altProjectDir

        libtype = self.dctFwVars['libtype']
        if libtypeOverride:
            if libtypeOverride == 'exe': libtype = 'shared'
            self.dctFwVars['libtypeOverride'+sProjectName] = libtypeOverride

        pthBuildDir = join( self.dctFwVars['pthScratch'],    \
							sProjectName + "_" +              \
							self.dctFwVars['variant'] + "_" +\
							libtype + "_" +                   \
							self.dctFwVars['bitness'] )

        self.dctFwVars[sProjectName+'BuildDir'] = pthBuildDir
        self.oEnv['FWVARS'] = self.dctFwVars  # in case of changes to the internal dictionary
    	oProject = self.oEnv.SConscript( join( pthProjectDir, sProjectName+'.sconscript' ),\
                                         src_dir = pthProjectDir,\
                                         build_dir = pthBuildDir,\
                                         duplicate = 0 )
    	if lstDependencies != None:
    		for dep in lstDependencies:
    			self.oEnv.Depends( oProject, dep )

        return oProject

class fwProject():
    def __init__ (self, oRoot, sProjectName, LIBS = None, LIBPATH = None, pthProjectDir = None, sProgramName = None):
        self.oEnv                        = oRoot.oEnv.Clone()
        self.dctFwVars                  = self.oEnv['FWVARS']

        # First check for a libtype override before any other setup calls
        if self.dctFwVars.has_key('libtypeOverride'+sProjectName):
            self.dctFwVars['libtype'] = self.dctFwVars['libtypeOverride'+sProjectName]

        self.dctFwVars['sProjectName']  = sProjectName
        if sProgramName: self.dctFwVars['sProgramName'] = sProgramName
        if not pthProjectDir: self.dctFwVars['pthProjectDir'] = join(self.dctFwVars['pthProjectRoot'], sProjectName)
        else:                 self.dctFwVars['pthProjectDir'] = pthProjectDir

        self.dctFwVars['pthBuildDir']   = self.dctFwVars[sProjectName+'BuildDir']
        setupIncludePaths(self.oEnv, self.dctFwVars['pthProjectDir'])

        if ( ((sys.platform=='linux2') or (sys.platform=='sunos5')) and (self.dctFwVars['libtype']=='static') ):
            AddObjsFromLibs(self.oEnv, LIBS, LIBPATH)

        if ( sys.platform=='darwin' ):
            self.oEnv['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 'True';
            AddObjsFromLibs(self.oEnv, LIBS, LIBPATH)

        if (sys.platform=='darwin') and LIBS:
            if("ThreadPool" in LIBS):
                LIBS.remove("ThreadPool")
            if("FFTW" in LIBS):
                LIBS.remove("FFTW")
            if("FFTWF" in LIBS):
                LIBS.remove("FFTWF")

        if LIBS: self.dctFwVars['LIBS'] = LIBS
        else:    self.dctFwVars['LIBS'] = []

        if LIBPATH: self.dctFwVars['LIBPATH'] = LIBPATH
        else:       self.dctFwVars['LIBPATH'] = []


        # Make sure that we initialize the dictionary AFTER all other variables are set
        # but BEFORE setting up the flags
        self.dctFwVars['transdict']     = self.initTransdict()

    def initTransdict (self):
        return dict(PROJECTNAME  = self.dctFwVars['sProjectName'],         \
                    PROJECTNAMEU = self.dctFwVars['sProjectName'].upper(), \
                    BUILDDIR     = self.dctFwVars['pthBuildDir'],\
                    BITNESS      = self.dctFwVars['bitness'],\
					PROJECTDIR   = self.dctFwVars['pthProjectDir'])


    def initBuildObjects(self, lstExtraCPPs = None, lstDepends = None, lstDependPaths = None, pthSrcDir=None):
        if not pthSrcDir:
            pthSrcSearch=[join(self.dctFwVars['pthProjectDir'], 'src', '*.cpp'),\
                          join(self.dctFwVars['pthProjectDir'], 'src',  '*.cxx'),\
                          join(self.dctFwVars['pthProjectDir'], 'src', '*.c')]
        else:
            pthSrcSearch=[join(pthSrcDir, '*.cpp'),\
                          join(pthSrcDir, '*.cxx'),\
                          join(pthSrcDir, '*.c')]
            setupIncludePaths(self.oEnv, pthSrcDir)


        self.lstCxxFiles = []
        for sp in pthSrcSearch:
            self.lstCxxFiles += globtree(sp)
        if lstExtraCPPs: self.lstCxxFiles += lstExtraCPPs

        self.oObjectFiles = constructObjects( self.oEnv,
                                              self.dctFwVars['pthProjectDir'],
                                              self.lstCxxFiles )

        # Special SCons handling for MSVC
        specialMSVCHandler(self.oEnv)

        sLibraryName = self.dctFwVars['sProjectName']
        if self.dctFwVars['libtype'] == 'shared':
            createDefFile( self.dctFwVars['pthDevLabRoot' ],\
                           self.dctFwVars['pthProjectRoot'],\
                           self.dctFwVars['pthBuildRoot'  ],\
                           self.dctFwVars['sProjectName'  ] )
            if (sys.platform == 'darwin'):
                if 'EXTRALIBOBJS' in self.dctFwVars:
                    self.oObjectFiles += self.dctFwVars['EXTRALIBOBJS']
        elif self.dctFwVars['libtype'] == 'static':
            if (sys.platform != 'win32'):
                if 'EXTRALIBOBJS' in self.dctFwVars:
                    self.oObjectFiles += self.dctFwVars['EXTRALIBOBJS']
        elif self.dctFwVars['libtype'] == 'exe':
            if self.dctFwVars.has_key('sProgramName'): sLibraryName = self.dctFwVars['sProgramName']
            else: sLibraryName = self.dctFwVars['sProjectName']

        # Create Library object and get the SCons library object from it
        oLibraryObject = fwLibrary(self.oEnv, sLibraryName, self.oObjectFiles)
        oLib = oLibraryObject.getLibrary()

        return oLib

    def initMultipassBuildObjects(self, lstExtraCPPs = None, lstExcludes = None, pthSrcDir=None):
        if pthSrcDir:
            pthSavedProjectDir = self.dctFwVars['pthProjectDir']
            self.dctFwVars['pthProjectDir'] = pthSrcDir
            setupIncludePaths(self.oEnv, pthSrcDir)
        else:
            pthSavedProjectDir = None

        # if this is true, currently this means that we're building the d version of the static lib
        if (pthSavedProjectDir):
            sCPPFile      = self.dctFwVars['sProjectName'][:-1]+'.cpp'
            sOptCPPFile   = self.dctFwVars['sProjectName'][:-1]+'_opt.cpp'
            pthOptHFile   = join('include', self.dctFwVars['sProjectName'][:-1]+'_opt.h'  )
            pthOptCPPFile = join('src',     self.dctFwVars['sProjectName'][:-1]+'_opt.cpp')
            pthBuildHFile = join(self.dctFwVars['pthBuildRoot'], 'include', self.dctFwVars['sProjectName'][:-1]+'.h')
        else:
            sCPPFile      = self.dctFwVars['sProjectName']+'.cpp'
            sOptCPPFile   = self.dctFwVars['sProjectName']+'_opt.cpp'
            pthOptHFile   = join('include', self.dctFwVars['sProjectName']+'_opt.h'  )
            pthOptCPPFile = join('src',     self.dctFwVars['sProjectName']+'_opt.cpp')
            pthBuildHFile = join(self.dctFwVars['pthBuildRoot'], 'include', self.dctFwVars['sProjectName']+'.h')


        # need to create DEF file here everytime, even if its a static library
        if (pthSavedProjectDir):
            createDefFile( self.dctFwVars['pthDevLabRoot' ],\
                           self.dctFwVars['pthProjectRoot'],\
                           self.dctFwVars['pthBuildRoot'  ],\
                           self.dctFwVars['sProjectName'  ][:-1] )
        else:
            createDefFile( self.dctFwVars['pthDevLabRoot' ],\
                           self.dctFwVars['pthProjectRoot'],\
                           self.dctFwVars['pthBuildRoot'  ],\
                           self.dctFwVars['sProjectName'  ] )


        # BUGBUG: <one last hack; can't get rid of this for a bit>
        #if (self.dctFwVars['toolset'] == 'suncc') and (self.dctFwVars['sProjectName'] == 'fwImage'):
        #    pthBuildHFile = appendToFilename(pthBuildHFile, '_sol')
        # </one last hack>

        dctFunctions = constructMultipassCPP( self.dctFwVars['pthProjectDir'],          \
                               self.dctFwVars['sProjectName'],           \
                               pthOptHFile, \
                               pthOptCPPFile, \
                               pthBuildHFile )

        self.lstCxxFiles  = globtree( join(self.dctFwVars['pthProjectDir'], 'src', '*.cpp') )
        self.lstCxxFiles += globtree( join(self.dctFwVars['pthProjectDir'], 'src', '*.cxx') )
        self.lstCxxFiles += globtree( join(self.dctFwVars['pthProjectDir'], 'src', '*.c') )
        if lstExtraCPPs: self.lstCxxFiles += lstExtraCPPs

        if not lstExcludes: lstExcludes = []
        lstExcludes = lstExcludes + [ sOptCPPFile, sCPPFile ]
        if lstExtraCPPs:    lstExcludes = lstExcludes + [basename(cpp) for cpp in lstExtraCPPs]

        self.oObjectFiles = constructMultipassObjects( self.oEnv,                       \
                                                       self.dctFwVars['pthProjectDir'],\
                                                       self.lstCxxFiles )

        # Special SCons handling for MSVC
        specialMSVCHandler(self.oEnv)

        # Create Library object and get the SCons library object from it
        oLibraryObject = fwLibrary(self.oEnv, self.dctFwVars['sProjectName'], self.oObjectFiles)
        oLib = oLibraryObject.getLibrary()

        if pthSavedProjectDir: self.dctFwVars['pthProjectDir'] = pthSavedProjectDir
        return oLib


# ------------- Misc utility global functions defined here -------------

# Create a list of object files from the lstCxxFiles and extras list also
# adding creating SharedObjects in the passed environment
# Note: extras are expected to be found in the FWBASE/Common directory
def constructMultipassObjects ( oEnv, pthProjectDir, lstCxxFiles ):
    lstObjects = []

    # Loop through the Cxx files, create MP objects for each
    # then getting the final SCons objects from them. Final
    # list of SCons objects will be stored in lstObjects
    for cf in lstCxxFiles:
        # Some hacks exist here because of problems with SCons paths
        # Touch this code very, very carefully!
        pthRoot, sExt = os.path.splitext( cf )
        dirname, basename = split( cf )
        pthPrefix = commonprefix([pthProjectDir, cf]).strip("/\\ ")
        pthRoot = src2obj(pthRoot)
        if (pthRoot[:1]==sep): pthRoot = pthRoot[1:]
        pthObjectFileName = (pthRoot + '.obj')[len(pthPrefix)+1:]

        # Create a MP Object
        oMPObject = fwMPObject(oEnv, pthObjectFileName, abspath(cf))
        # Get the created SCons objects from the MP object
        lstObjects += oMPObject.getObjects()

    return lstObjects


# Create a list of object files from the lstCxxFiles and extras list also
# adding creating SharedObjects in the passed environment
# Note: extras are expected to be found in the FWBASE/Common directory
def constructObjects ( oEnv, pthProjectDir, lstCxxFiles ):
    lstObjects = []
    for cf in lstCxxFiles:
        # Get the name for the object file
        pthRoot, sExt = os.path.splitext( cf )
        pthPrefix = commonprefix([pthProjectDir, cf]).strip("/\\ ")
        pthRoot = src2obj(pthRoot)
        if (pthRoot[:1]==sep): pthRoot = pthRoot[1:]
        pthObjectFileName = (pthRoot + '.obj')[len(pthPrefix)+1:]
        oObject = fwObject( oEnv, pthObjectFileName, abspath(cf) )
        lstObjects += oObject.getObject()
    return lstObjects

# Create the def file using the fwHeaderConvert tool
def createDefFile ( pthDevLabRoot, pthProjectRoot, pthBuildRoot, sProjectName ):
    win_sys = 0
    if   sys.platform=='win32':
        exe_name = 'FwHeaderConvert_win.exe'
        win_sys = 1
    elif sys.platform=='linux2':
        exe_name = 'FwHeaderConvert_lin'
    elif sys.platform=='sunos5':
        exe_name = 'FwHeaderConvert_sol'
    elif sys.platform=='darwin':
        exe_name = 'FwHeaderConvert_mac'

    sCommandStr  = '"' + join(pthDevLabRoot,'BuildTools','bin',exe_name) + '"'               # exe name
    sCommandStr += ' -def' # create def file option
    sCommandStr += ' ' + join(pthProjectRoot,sProjectName,'include',sProjectName + '.h')   # input .h file
    sCommandStr += ' ' + join(pthProjectRoot,'common','include','buildnum.h')
    sCommandStr += ' ' + join(pthBuildRoot, 'include', sProjectName + '.h')
    sCommandStr += ' ' + join(pthProjectRoot,sProjectName,sProjectName + '.def')             # finally, the def file name/location
    sCommandStr += ' ' + sProjectName + '.dll'                                               # and the dll name for the def file
    os.system( sCommandStr )

    if win_sys == 0:
        os.remove(join(pthProjectRoot,sProjectName,sProjectName + '.def'))
        #os.system( join('rm ' + pthProjectRoot,sProjectName,sProjectName + '.def') )



def specialMSVCHandler( oEnv ):
    # Get the fw vars dictionary from the environment
    dctFwVars = oEnv['FWVARS']

    # Special handling for msvc; because, well, Microsoft is Microsoft
    if dctFwVars['toolset'] == 'msvc':
        if dctFwVars['debuginfo'] == 'on':
            # translate flags isn't applied on compiler flags setup by SCons, hence get the name
            # from the caller project object's FWVARS dictionary itself
            if dctFwVars.has_key('sProgramName'):
                oEnv.Append( PDB = dctFwVars['sProgramName'] + ".pdb" )
            else:
                oEnv.Append( PDB = dctFwVars['sProjectName'] + ".pdb" )
        if dctFwVars['libtype'] == 'shared': oEnv.Append( WINDOWS_INSERT_DEF = "1" )

