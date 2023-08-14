#!/usr/bin/env python

import os, sys
import argparse
from jobSubmitter import *

parser = argparse.ArgumentParser()

## Analysis options
parser.add_argument("--inputFolder",                default = "/Users/haider/FCC/data/ntuples/zh_vvjj/", type = str,      help = "Path to folder with all the input folder")
parser.add_argument("--outputFolder",               default = "/Users/haider/FCC/CharmCutCode/run/root-files/", type = str,      help = "Path to folder with all the output")
parser.add_argument("--doZHvvjj",                   default = False, action='store_true',   help = "Run jobs for ZHvvjj analysis") 

## Batch options
parser.add_argument("--mergeFile",                  default = 20,    type=int,              help = "Number of files to merge into one job")
parser.add_argument("--dryRun",                     default = False, action='store_true',   help = "Does not submit to anything") 
parser.add_argument("--runLocal",                   default = False, action='store_true',   help = "Run in parallel") 
parser.add_argument("--runInteractive",             default = False, action='store_true',   help = "Run in interactive move") 
parser.add_argument("--submitCondor",               default = False, action='store_true',   help = "Submit jobs to the batch") 
parser.add_argument("--submitSlurm",                default = False, action='store_true',   help = "Submit jobs to the batch") 
parser.add_argument("--submitCedarContainer",       default = False, action='store_true',   help = "submission for cedarContainer")
parser.add_argument("--cedarTime",                  default = "6:00:00", type = str,        help = "Time for a job to run on Cedar. Needs to be in format hh:mm:ss")
parser.add_argument("--maxProcs",                   default = 4,     type=int,              help = "Number of parallel processed")

parser.add_argument("--remergeResplit",             default = False, action='store_true',   help = "expert option to merge and split jobs")
parser.add_argument("--remergeResplitSize",         default = 1,     type = int,            help = "expert option to find how many to merge jobs with")



args = parser.parse_args()


submissionJobList = []



if args.doZHvvjj:
    submissionJobList = [
    {"folderName" : "p8_ee_WW_ecm240",              "processName" : "WW"},
    {"folderName" : "p8_ee_ZZ_ecm240",              "processName" : "ZZ"},
    {"folderName" : "p8_ee_Zqq_ecm240",             "processName" : "Zqq"},
    {"folderName" : "wzp6_ee_nunuH_HWW_ecm240",     "processName" : "HWW"},
    {"folderName" : "wzp6_ee_nunuH_HZZ_ecm240",     "processName" : "HZZ"},
    {"folderName" : "wzp6_ee_nunuH_Hbb_ecm240",     "processName" : "Hbb"},
    {"folderName" : "wzp6_ee_nunuH_Hcc_ecm240",     "processName" : "Hcc"},
    {"folderName" : "wzp6_ee_nunuH_Hgg_ecm240",     "processName" : "Hgg"},
    {"folderName" : "wzp6_ee_nunuH_Hss_ecm240",     "processName" : "Hss"},
    {"folderName" : "wzp6_ee_nunuH_Htautau_ecm240", "processName" : "Htautau"},
    {"folderName" : "wzp6_ee_qqH_ecm240",           "processName" : "qqH"},
    ]

def main():
    submissionManager = submissionFactory(args)

    if(len(submissionJobList) == 0):
        print("No jobs configured. Did you forget an option to this script like --doXXX")
        exit(1)
    ##############################################################
    ##                     Configure the jobs                   ##
    ##############################################################
    for submissionJob in submissionJobList:
        # inputDir
        inputDir = args.inputFolder + "/" + submissionJob["folderName"]
        # get all the root files inside the folder
        rawFileList = getRootFileList(inputDir)
        # string multiple files together to reduce the number of jobs we submit
        fileList = []
        _mergeJobTmp = []
        counter = 0

        for fileName in rawFileList:
            if(counter >= args.mergeFile):
                fileList.append(",".join(_mergeJobTmp))
                _mergeJobTmp = []
                counter = 0

            counter += 1
            _mergeJobTmp.append(fileName)

        if(len(_mergeJobTmp) > 0): fileList.append(",".join(_mergeJobTmp))

        print("final list: ", len(fileList))

        # create the output folder
        outPath = args.outputFolder + "/" + submissionJob["folderName"]
        _createFolder(outPath)

        # loop over each file list and submit a job
        index = 0
        for fileName in fileList:
            jobName = "Job" + str(index)
            index += 1
            currJob = {}
            currJob["inputFileList"] = fileName
            currJob["outputFileName"] = outPath + "/" + jobName + ".root"
            currJob["outputFileName"] = outPath + "/" + jobName + ".root"
            currJob["sampleName"] = submissionJob["folderName"]
            currJob["processName"] = submissionJob["processName"]

            cmd = _getRunCommand("runAnalysis", currJob)
            

            cInfo = {}
            cInfo["jobOutPath"] = submissionJob["folderName"]
            cInfo["jobOutName"] = jobName

            _writeJobList(cInfo["jobOutPath"], cInfo["jobOutName"], cmd)

            submissionManager.addJob(cInfo)


    ##############################################################
    ##                      Submit the jobs                     ##
    ##############################################################
    ## This is the actual submission to different systems, e.g. condor, batch, grid, local
    ## Information from the jobManager is re-organized here 
    submissionManager.process()
    submissionManager.submitJob()

def _writeJobList(jobOutPath, jobOutName, cmd):

    filePath = "jobList/" + jobOutPath
    _createFolder(filePath)

    fileName = filePath + "/" + jobOutName + ".sh"
    fileObj = open(fileName, 'w')
    fileObj.write("#!/bin/bash\n\n")
    if(not (args.runLocal or args.runInteractive)): 
        CWD = os.getcwd()
        fileObj.write("source ../source/setup.sh\n")
        fileObj.write("source ../build/setup.sh\n")
    fileObj.write(cmd)
    fileObj.write("\n")
    fileObj.close()

    os.system("chmod u+x " + fileName)

def submissionFactory(args):
    if(args.submitCondor):           return submitCondor(args)
    elif(args.submitCedarContainer): return submitCedarContainer(args)
    elif(args.runLocal):             return runLocal(args)
    elif(args.runInteractive):       return runInteractive(args)
    elif(args.dryRun):               return doDryRun(args)
    else:
        print("You did not enter where to submit to")
        print("-------------------------------------------------")
        exit(1)

def _getRunCommand(exePath, currJob):
    cmd =  exePath + " "
    cmd += ' --inputFileList %s' %(currJob['inputFileList'])
    cmd += ' --outputFileName %s' %(currJob['outputFileName'])
    cmd += ' --sampleName %s' %(currJob['sampleName'])
    cmd += ' --processName %s' %(currJob['processName'])

    if(args.doZHvvjj):
        cmd += ' --analType vvjj'

    return cmd


def _createFolder(folderPath):
    if(not os.path.isdir(folderPath)):
        os.system("mkdir -p %s" % (folderPath))

def getRootFileList(dirName):
    fileList = []
    for fileName in os.listdir(dirName):
        if(fileName.endswith(".root")):
            fileList.append(dirName + "/" + fileName);
    
    print("looking for", dirName, ": nFile: ", len(fileList))
    return fileList

if __name__ == '__main__':
  sys.exit( main() )
