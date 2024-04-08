#!/usr/bin/env python
import os
import math
import shlex
import subprocess
import time
from ROOT import *
import re

import plotScans as pS

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--inputDir",   type = str, help = "Path to folder with files, XXX is the regex")
parser.add_argument("--maxProcs",   type=int, default=5, help="Number of parallel processed")
parser.add_argument("--logFile",    type=str, default='logFile_combine.txt', help="Name of the log file")
parser.add_argument("--tmpDir",     type=str, default='tmpDir', help="Directory for temporary saving of the log files")

args = parser.parse_args()

globalScanType = ""
def main():


    ## get the base path:
    basePath = "/".join(args.inputDir.split("/")[:-1])
    folderRegex = args.inputDir.split("/")[-1].replace("XXX", "(\S*)")
    outputFolder = basePath + "/" + args.inputDir.split("/")[-1].replace("XXX", "")

    # get the sub folderList
    folderList = pS.getSubFolderList(basePath)

    # clean up the sub folder list
    folderList = [x for x in folderList if re.match(folderRegex, x)]


    scanToMerge = []
    poiList = {}


    for folderName in folderList:
        folderpath = basePath + "/" + folderName
        scanTypeFolderList = pS.getSubFolderList(folderpath)

        # Merging get all the subFolders inside scanType to merge
        for scanTypeFolder in scanTypeFolderList:
            scanFolderPath = folderpath + "/" + scanTypeFolder
            runFolderList = pS.getSubFolderList(scanFolderPath)

            # for each run folder, find all POIs to merge
            for runFolder in runFolderList:
                runFolderPath = scanFolderPath + "/" + runFolder
                runFoldersJobPath = runFolderPath + "/sJob/"

                ## get the POI list
                fileToMerge = pS.getRootFileListSizeCheck(runFoldersJobPath)

                if(len(fileToMerge) == 0): continue

                POIName = fileToMerge[0].split("_")[-2].split("/")[-1]
                #output name
                outPath  = outputFolder + "/" + scanTypeFolder + "/" + runFolder

                os.system("mkdir -vp %s" %outPath)
                outFileName  = outPath + "/" + POIName + ".root"


                if(POIName in poiList):
                    poiList[POIName]["fileList"] += fileToMerge
                else:
                    commandCurr = {}
                    commandCurr['fileList'] = fileToMerge
                    commandCurr['saveName'] = outFileName

                    poiList[POIName] = commandCurr


    for poiName in poiList:
        commandCurr = {}
        commandCurr['fileList'] = poiList[poiName]["fileList"]
        commandCurr['saveName'] = poiList[poiName]['saveName']
        scanToMerge.append(commandCurr)


    print scanToMerge
    pS.mergeRootFiles(scanToMerge) 


    # print basePath

    # print folderRegex
    # print scanTypeFolderList


    pass


if __name__ == "__main__":
    main()