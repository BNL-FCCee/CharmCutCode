#!/usr/bin/env python
import os
import math
import shlex
import subprocess
import time
import ROOT

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--inputDir",   type = str, help = "Path to folder with files")
parser.add_argument("--toyFile",    type = str, default = '', help = "Path to folder with toy run")
parser.add_argument("--extraOpts",    type = str, default = '', help = "Any extra opts to specify for the plotting")
parser.add_argument("--plotDir",    type = str, default='Plots', help = "Default Plots/, but you can modify")
parser.add_argument("--impactPOIs", type = str, default='', help = "If blank, this will be automatically filled with poi if you are plotting a ranking job")
parser.add_argument("--maxProcs",   type=int, default=5, help="Number of parallel processed")
parser.add_argument("--logFile",    type=str, default='logFile_combine.txt', help="Name of the log file")
parser.add_argument("--tmpDir",     type=str, default='tmpDir', help="Directory for temporary saving of the log files")

args = parser.parse_args()

globalScanType = ""
def main():
    # first merge all the scans
    try:
        mergeScan()
    except Exception as e:
        print("Error in merging, gonna try more")

    plottingInfoDict = getPlottingInfoDict()

    fillGlobalMetaData(plottingInfoDict)

    cmdList = []
    print(globalScanType)

    if('rank' in globalScanType or 
       'correlationHist' in globalScanType or 
       "runToys" in globalScanType or 
       "scanDatasets" in globalScanType):
        cmdList = getCommand(plottingInfoDict)
    else:
        uniquePOIList = []
        for plotDict in plottingInfoDict:
            uniquePOIList.append(plotDict["POIName"])
        uniquePOIList = list(set(uniquePOIList))

        for POI in uniquePOIList:
            scanList = getDictSubset(plottingInfoDict, "POIName", POI)
            # print POI, scanList
            cmd = getCommand(scanList, POI)
            cmdList.append(cmd)
            pass

    # perform each command

    for command in cmdList:
        print(command)
        os.system(command)

def fillGlobalMetaData(scanList):
    # get the dimension of the scan to figure out what plotting script to use
    scanTypeList = []
    fileName  = ""
    workspaceName  = ""
    modelConfigName  = ""
    extraVarSave  = ROOT.TString("")
    for scanInfo in scanList:
        # print args.impactPOIs
        print (scanInfo["fileName"])
        cFile = ROOT.TFile.Open(scanInfo["fileName"])
        cTree = cFile.Get('metaData')
        cTree.GetEntry(0)
        dimInfo = getattr(cTree, 'dimension')
        dimInfoStr = "%s" %(dimInfo)
        scanTypeList.append(dimInfoStr)
        fileName = getattr(cTree, 'fileName')
        workspaceName = getattr(cTree, 'workspaceName')
        modelConfigName = getattr(cTree, 'modelConfigName')
        if "extraVarSave" in cTree:
            extraVarSave = getattr(cTree, 'extraVarSave')

    scanTypeList = list(set(scanTypeList))

    if(len(scanTypeList) > 1):
        print ("You are mixing scans with different dimension... I don't know how to deal with this")
        print (scanTypeList)
        exit(1) 


    global globalScanType
    globalScanType = scanTypeList[0]


    if(len(args.impactPOIs) == 0):
        if(not os.path.isfile(fileName.Data()) and 'rank' in globalScanType):
            print("You haven't specified impactPOIs and the workspace at the path doesn't exist. I don't know what to do")
            print(fileName.Data())
            exit(1)
        wsfile = ROOT.TFile.Open(fileName.Data())
        ws = wsfile.Get(workspaceName.Data())
        mc = ws.obj(modelConfigName.Data())        
        poiList = mc.GetParametersOfInterest()
        poiiter = poiList.createIterator()
        var = poiiter.Next()
        potNameList = []
        while var :
            potNameList.append(var.GetName())
            var = poiiter.Next()

        extraVarSave = extraVarSave.ReplaceAll("(.*)", "*")
        varList = ws.allVars().selectByName(extraVarSave.Data())
        poiiter = varList.createIterator()
        var = poiiter.Next()
        while var :
            potNameList.append(var.GetName())
            var = poiiter.Next()


        args.impactPOIs = ','.join(potNameList)




def getCommand(scanList, POIName = None):
    fileNameList = []
    uniqueFolderName = (args.inputDir).split("/")[-1]
    dirName = args.plotDir +"/"+ uniqueFolderName
    os.system("mkdir -vp %s" %dirName)

    if('rank' in globalScanType or 
        'correlationHist' in globalScanType or 
        "runToys" in globalScanType or
        "scanDatasets" in globalScanType):
        for scanInfo in scanList:
            fileNameList.append(scanInfo["fileName"])

        listOfCommands = []
        for file in fileNameList:

            if('rank' in globalScanType):
                runCommand = '../build/wscanner/plotRanking '
                runCommand += ' --impactPOIs %s' %args.impactPOIs
            elif('correlationHist' in globalScanType):
                runCommand = '../build/wscanner/plotCorrHist '
                runCommand += ' --impactPOIs %s' %args.impactPOIs
            elif('runToys' in globalScanType):
                runCommand = '../build/wscanner/plotToys '
            elif('scanDatasets' in globalScanType):
                runCommand = '../build/wscanner/plotDatasetScan '

            runCommand += ' --fileName '    + file
            runCommand += ' --plotDir ' + args.plotDir

            if("AnalysisBase_VERSION" in os.environ):
                runCommand = runCommand.replace("../build/wscanner/", "")

            if(len(args.extraOpts) > 0): runCommand += ' ' + args.extraOpts

            listOfCommands.append(runCommand)

        return listOfCommands
    elif('compatibility' in globalScanType):
        return ""
    else:
        mainOrderList = ['statOnly/data', 'allSys/data', 'statOnly/asimov', 'allSys/asimov', 'modTheory/data', 'modTheory/asimov']
        keyListSort = []

        for orderKey in mainOrderList:
            for i in range(0, len(scanList)):
                if(orderKey in scanList[i]["fileName"]):
                    keyListSort.append(i)
                    break

        reorderedList = [scanList[i] for i in keyListSort]
        keyListSort.sort(reverse = True)
        [scanList.pop(i) for i in keyListSort]
        reorderedList += scanList

        # outFileName = dirName + "/" + POIName
        for scanInfo in reorderedList:
            # if("modTheory" in scanInfo["fileName"]): continue
            # if("stat" in scanInfo["fileName"]): continue
            fileNameList.append(scanInfo["fileName"])

        runCommand = ''
        # we only have 1 scan type
        if('1' in globalScanType):
            runCommand = '../build/wscanner/plot1D '
        elif('2' in globalScanType and "Toy" in globalScanType):
            runCommand = '../build/wscanner/plot2DToys '
        elif('2' in globalScanType):
            runCommand = '../build/wscanner/plot2D '
        else:        
            print ("I don't know how to deal with the input dimension type")
            print (globalScanType)
            exit(1)  

        runCommand += ' --fileName '    + ",".join(fileNameList)
        runCommand += ' --plotDir ' + args.plotDir
        if(len(args.toyFile) > 0):
            runCommand += ' --toyFileName ' + args.toyFile

        if("AnalysisBase_VERSION" in os.environ):
            runCommand = runCommand.replace("../build/wscanner/", "")



        if(len(args.extraOpts) > 0): runCommand += ' ' + args.extraOpts

        return runCommand 


def getDictSubset(inDictList, key, val):
    outDictList = []
    for cDict in inDictList:
        if(val == cDict[key]):
            outDictList.append(cDict)
    return outDictList

def getPlottingInfoDict():
    dirName = args.inputDir
    scanTypeFolderList = getSubFolderList(dirName)

    plottingInfo = []

    for scanTypeFolder in scanTypeFolderList:
        # print scanTypeFolder
        scanFolderPath = dirName + "/" + scanTypeFolder

        runFolderList = getSubFolderList(scanFolderPath)

        # for each run folder, find all POIs to merge
        for runFolder in runFolderList:
            # print runFolder
            runFolderPath = scanFolderPath + "/" + runFolder
            POIOutputFileList = getRootFileList(runFolderPath)
            
            if('rank' in globalScanType):
                scanInfoDict = {}
                scanInfoDict["scanType"] = scanTypeFolder
                scanInfoDict["runType"] = runFolder
                scanInfoDict["fileName"] = args.inputDir + "/" + scanTypeFolder + "/" + runFolder + "/mergedScan.root"
                plottingInfo.append(scanInfoDict)

            else:
                for POIOutputFile in POIOutputFileList:
                    # print POIOutputFile
                    POIname = POIOutputFile.replace(runFolderPath, "").replace(".root", "").replace("/", "")

                    scanInfoDict = {}
                    scanInfoDict["scanType"] = scanTypeFolder
                    scanInfoDict["runType"] = runFolder
                    scanInfoDict["POIName"] = POIname
                    scanInfoDict["fileName"] = POIOutputFile
                    plottingInfo.append(scanInfoDict)

    return plottingInfo 


def getSubFolderList(dirName):
    fileList = []
    for files in os.listdir(dirName):
        if os.path.isdir(dirName + "/" + files):
            fileList.append(files)
            pass
        pass
    
    fileList.sort()
    return fileList

def getRootFileList(dirName):
    fileList = []
    for fileName in os.listdir(dirName):
        if(fileName.endswith(".root")):
            fileList.append(dirName + "/" + fileName);
    return fileList

def getRootFileListSizeCheck(dirName):
    fileList = []
    for fileName in os.listdir(dirName):
        if(fileName.endswith(".root")):
            if(os.stat(dirName + "/" + fileName).st_size < 100): continue
            fileList.append(dirName + "/" + fileName);
    return fileList

def checkIfSubfolder(dirName):
    for fileName in os.listdir(dirName):
        if os.path.isdir(dirName + "/" + fileName):
            return True
    return False

def mergeScan():
    if("XXX" in args.inputDir):
        _mergeScan_scanDataset()
    else:
        _mergeScanOld()

# helpers merging
def _mergeScanOld():
    dirName =  args.inputDir
    scanTypeFolderList = getSubFolderList(dirName)

    scanToMerge = []
    # Merging get all the subFolders inside scanType to merge
    for scanTypeFolder in scanTypeFolderList:
        scanFolderPath = dirName + "/" + scanTypeFolder
        runFolderList = getSubFolderList(scanFolderPath)

        if(not checkIfSubfolder(scanFolderPath + "/" + runFolderList[0] + "/sJob/")):
            for runFolder in runFolderList:
                runFolderPath = scanFolderPath + "/" + runFolder
                runFoldersJobPath = runFolderPath + "/sJob/"
                fileToMerge = getRootFileListSizeCheck(runFoldersJobPath)

                commandCurr = {}
                commandCurr['fileList'] = fileToMerge
                commandCurr['saveName'] = runFolderPath + "/mergedScan.root"
                scanToMerge.append(commandCurr)

        else:
            # for each run folder, find all POIs to merge
            for runFolder in runFolderList:
                runFolderPath = scanFolderPath + "/" + runFolder
                runFoldersJobPath = runFolderPath + "/sJob/"

                ## get the POI list
                POIList = getSubFolderList(runFoldersJobPath)

                # merge all files for each POI
                for POIName in POIList:

                    #output name
                    outFileName = runFolderPath + "/" + POIName + ".root"
                    fileToMerge = getRootFileListSizeCheck(runFoldersJobPath + "/" + POIName)
                    
                    # print "filetoM ", fileToMerge

                    commandCurr = {}
                    commandCurr['fileList'] = fileToMerge
                    commandCurr['saveName'] = outFileName
                    scanToMerge.append(commandCurr)

    mergeRootFiles(scanToMerge) 
    
# helpers mergining
def _mergeScan_scanDataset():
    ## get the base path:
    basePath = "/".join(args.inputDir.split("/")[:-1])
    folderRegex = args.inputDir.split("/")[-1].replace("XXX", "(\S+)")
    outputFolder = basePath + "/" + args.inputDir.split("/")[-1].replace("XXX", "")

    # get the sub folderList
    folderList = getSubFolderList(basePath)
    import re
    # clean up the sub folder list
    folderList = [x for x in folderList if re.match(folderRegex, x)]


    scanToMerge = []
    poiList = {}


    for folderName in folderList:
        folderpath = basePath + "/" + folderName
        scanTypeFolderList = getSubFolderList(folderpath)

        # Merging get all the subFolders inside scanType to merge
        for scanTypeFolder in scanTypeFolderList:
            scanFolderPath = folderpath + "/" + scanTypeFolder
            runFolderList = getSubFolderList(scanFolderPath)

            # for each run folder, find all POIs to merge
            for runFolder in runFolderList:
                runFolderPath = scanFolderPath + "/" + runFolder
                runFoldersJobPath = runFolderPath + "/sJob/"

                ## get the POI list
                fileToMerge = getRootFileListSizeCheck(runFoldersJobPath)

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


    print(scanToMerge)
    mergeRootFiles(scanToMerge) 

    args.inputDir = outputFolder


## To combine .root Files
def combineRootFiles(fileList, saveFile):
    # building the command for combining the files
    command = "hadd -f"
    command += " %s" %saveFile
    for fileName in fileList:
        command += " %s" %fileName
        pass
    # print "command", command
    if len(fileList) == 0:
        return
    return command

# Gets the modification time for the file
def modification_date(filename):
    # default time is zero
    t = 0
    # if the file exist, update the modification time 
    if os.path.isfile(filename):
        t = os.path.getmtime(filename)
    return t 

def mergeRootFiles(commandListFull):
    # all the commands
    commandList = []

    # remove all commands where the output file is newer than the inputs file
    # also remove all commands that have zero input files
    for command in commandListFull:
        # get the creation/modification for the saveFile
        saveFileTime = modification_date(command['saveName'])
        # loop over all the input files and see if there is a newer version of the file
        skipFile = True
        # skipFile = False
        for inputFile in command['fileList']:
           if saveFileTime <= modification_date(inputFile):
               skipFile = False
               break
        # if not skipping this command, append it to another list
        if not skipFile:
            commandList.append(command)


    # actually start the combine process
    processes = set()
    procList = []
    max_processes = args.maxProcs

    # check if the temp directory exists, if it doesn't create it
    createdTmpDir = False
    if not os.path.exists(args.tmpDir):
        os.makedirs(args.tmpDir)
        createdTmpDir = True

    # logfile
    i = 0;
    for command in commandList:
        i = i + 1
        if(i%5 == 0):
            precentDone = float(i)/len(commandList) * 100
            print ('Running command i: %i %s percentage done: %s %.2f'  %(i, bcolors.INFO, bcolors.ENDC, precentDone))

        # get the arguments
        shellCommand = combineRootFiles(command['fileList'], command['saveName'])
        arguments = shlex.split(shellCommand)
        tmpLogFile = args.tmpDir+'/tmpLogFile_combine_'+str(i)+'.txt'

        w = open(tmpLogFile,'w')
        # start the process
        p = subprocess.Popen(arguments, stdout=w, stderr=w)


        # store for backup and for the queue
        processes.add(p)

        procToSave = {}
        procToSave['args'] = shellCommand
        procToSave['logFile'] = tmpLogFile
        procList.append(procToSave)
       
        # check if the current number of processes exceed the maximum number
        if len(processes) >= max_processes:
            #print 'holding since max process exceeded limit'            
            while True:
                processes.difference_update([p for p in processes if p.poll() is not None])
                if len(processes) < max_processes:
                    break

    print ('Polling for the last few jobs')
    # poll till all the processes are done
    while True:
        processes.difference_update([p for p in processes if p.poll() is not None])
        if len(processes) == 0:
            break
        pass
        # sleep for a bit to save polling time
        time.sleep(0)
    pass

    print ('Waiting for output files to get written to disk')
    # to wait for the last few files to be written
    time.sleep(1)

    # for quick error checking
    errorInDownload = False
    # write to log file
    log = open(args.tmpDir+"/"+args.logFile, "w", 1)
    for proc in procList:
        log.write(proc['args'])
        # read the log file
        tmpLogFile = open(proc['logFile'],  "r")
        out = tmpLogFile.read()
        tmpLogFile.close()
        # delete the log file
        # os.remove(proc['logFile'])

        log.write(out)

        # basic error check
        if('Target path' not in out):
            errorInDownload = True
            log.write("ERROR - no output Target file... check the job above\n")
        log.write('\n\n')
    log.close()

    if errorInDownload:
        print ("Potential errors found in output, please check the log file") 


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    INFO = '\033[92m'
    WARNING = '\033[93m'
    ERROR = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'



if __name__ == "__main__":
    main()
