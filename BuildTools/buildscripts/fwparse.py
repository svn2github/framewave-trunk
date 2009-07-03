#
# Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

from __future__ import with_statement

# Utility module for Framewave's SCons scripts

import glob
import os
import re
import fileinput

from os.path import *
from string import replace
from fwutil import *


sFwMultipassFunction = r"^[\w\s_]*PREFIX_OPT[\s]*\(OPT_PREFIX[\s]*,[\w\s_]*\)\([\w\s_,*&-\+\[\]]*\)[\s]*;"
reFindFwMPFunction = re.compile( sFwMultipassFunction, re.DOTALL | re.MULTILINE )

def extractMPNames ( lstNames ):
    dctName = {}
    for n in lstNames:
        name = ((re.search(r"[\s,][\w_]*[\s]*\)", str(n))).group()).strip(' \(\)\t\n,')
        dctName[name] = n
    return dctName

def extractParams ( sFunction ):
    sParam = (re.search(r"\([\w\s_,\[\]&*]*\)[\s]*;", sFunction).group()).strip(" ;\(\)")
    sParam = re.sub("\[[\d]*\]","", sParam)

    oParams = re.finditer(r"[\s*&][\w_]*([,\)]|$)", sParam)
    return [(p.group().strip(" *&,")) for p in oParams]
    
def removeOptDecoration ( sFunction, sFunctionName ):
    return re.sub(r"PREFIX_OPT[\s]*\(OPT_PREFIX[\s]*,[\w\s_]*\)", sFunctionName, sFunction)
    
def getMPFuncNames ( sHeaderData ):
    lstFunctions = reFindFwMPFunction.findall( sHeaderData )
    return lstFunctions

def getMarkers ( sFunctionName ):
    lstOptLevels = re.findall(r"OREFR|OSSE2|OF10H", sFunctionName)
    return [l.strip() for l in lstOptLevels]

def getFunctionMarkers( dctFunctionNames ):
    dctFunctionMarkers = {}
    for f in dctFunctionNames:
        dctFunctionMarkers[f] = getMarkers(dctFunctionNames[f])
    return dctFunctionMarkers

def createStub ( sFunctionName, sDecoratedName, lstOpts, lstParamNames, oOptimizationLevels ):
    sParams = ""
    for p in lstParamNames:
        sParams += (p + ", ")
    sParams = sParams.strip(", ")
    
    lstStub   = []
    lstStub  += [r'{']
    lstStub  += [r'    switch( Dispatch::Type<DT_COUNT>() )']
    lstStub  += [r'    {']

    lstOpts = [n.strip() for n in getMarkers(sDecoratedName)]
    
    # code to drop higher optimization levels down to the first available optimization level
    for ol in reversed(oOptimizationLevels[maxImplementedOptimizationLevel:]):
        sOptDisp = ol['DISPATCH']
        lstStub  += [r'    case '+sOptDisp+':']

    for ol in reversed(oOptimizationLevels[:maxImplementedOptimizationLevel]):
        sOptDisp = ol['DISPATCH']
        sOptPrefix = ol['OPT_PREFIX']

        lstStub  += [r'    case '+sOptDisp+':']
        if ol['MARKER'] in lstOpts:
            lstStub  += [r'        return '+sOptPrefix+sFunctionName+'('+sParams+');']

    lstStub  += [r'    default:']
    lstStub  += [r'        assert(false);']
    lstStub  += [r'        return '+oOptimizationLevel_REFR['OPT_PREFIX']+sFunctionName+'('+sParams+');']
    lstStub  += [r'    }']
    lstStub  += [ '}\n']
    
    return lstStub

    
def constructMultipassCPP ( pthProjectDir, sProjectName, pthInHeaderFile, pthOutCPPFile, pthVerifyHeader = None ):
    sHeaderData = readFile( join(pthProjectDir, pthInHeaderFile) )
    if not sHeaderData:
        print "Cannot open " + pthInHeaderFile + "! Exiting.."
        return None

    if pthVerifyHeader:
        sVerifyData = readFile( pthVerifyHeader )
        if not pthVerifyHeader:
            print "Cannot open " + pthVerifyHeader + "! Exiting.."
            return None

    lstMPFunctions = getMPFuncNames( sHeaderData )
    dctFunctions = extractMPNames( lstMPFunctions)
    dctMarkers = getFunctionMarkers( dctFunctions )

    
    # Create CPP
    dirname, sHeaderName = split( pthInHeaderFile )
    sHeader, ext = splitext( sHeaderName )
    sHeader = sHeader.upper()

    lstCPP = []
    for ol in oOptimizationLevels[:maxImplementedOptimizationLevel]:
        lstCPP += ['#define '+ol['DEFVAR']]
        lstCPP += ['#define OPT_LEVEL '+ol['OPT_LEVEL']]
        lstCPP += ['#define OPT_PREFIX '+ol['OPT_PREFIX']]
        lstCPP += ['#undef __'+sHeader+'_H__']
        lstCPP += ['#include "' + sHeaderName + '"']
        lstCPP += ['#undef '+ol['DEFVAR']]
        lstCPP += ['#undef OPT_LEVEL']
        lstCPP += ['#undef OPT_PREFIX\n\n']
        
    lstCPP += ['using namespace REFR;']
    for name in dctFunctions:
        if pthVerifyHeader:
            if not re.search(name, sVerifyData):
                continue
        lstStub = createStub( name, dctFunctions[name], dctMarkers[name], extractParams(dctFunctions[name]), oOptimizationLevels )
        lstCPP += [(removeOptDecoration( dctFunctions[name], name )).strip(" ;")]
        lstCPP += lstStub

    writeLinesToFile( join(pthProjectDir, pthOutCPPFile), lstCPP )
    
    return dctMarkers

def findREinFile ( pthInFile, fname ):
    try:
        file = open(pthInFile, "r")
        for line in file:
            #if fre.search(line):
            if line.find(fname) != -1:
                return 1
    except IOError:
        print "Could not open " + pthInFile  + "!\n"
        return None
    return None


def maxOptimizationLevel( pthCPPFile, dctMarkers ):
    lstOptLevels = []
    sCPPData = readFile(pthCPPFile)
    for f in dctMarkers:
        #findFunction = re.compile(r"PREFIX_OPT[\s]*\(OPT_PREFIX[\s]*,[\s]*("+f+")[\s]*\)")
        if findREinFile(pthCPPFile, f):
            for m in dctMarkers[f]:
               if m not in lstOptLevels: lstOptLevels += [m]
    return lstOptLevels
