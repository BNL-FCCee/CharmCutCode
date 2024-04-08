#!/usr/bin/env python
# -*- coding:utf-8 -*-

import os
import math

try:
    from wscanner import jobListCouplings, jobListCP, jobListDiff, jobListFCC,jobListFCC_ZHAllHad
except ImportError:
    import jobListCouplings, jobListCP, jobListDiff, jobListFCC,jobListFCC_ZHAllHad

import tarfile
import datetime
import shutil
import os.path
import random
import abc
import sys
#import numpy as np
from collections import OrderedDict
from jobSubmission import *
from jobManager import *



import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--splitJob",       type = int, default = 1,                help = "Split the job into multiple subjobs")
parser.add_argument("--queue",          type = str, default = "longlunch",      help = "Queue to submit to.")
parser.add_argument("--dryRun",         default = False, action='store_true',   help = "Does not submit to anything") 
parser.add_argument("--runLocal",       default = False, action='store_true',   help = "Run in parallel") 
parser.add_argument("--submitCondor",   default = False, action='store_true',   help = "Submit jobs to the batch") 
parser.add_argument("--submitSlurm",    default = False, action='store_true',   help = "Submit jobs to the batch") 
parser.add_argument("--submitCedarContainer",        default = False, action='store_true',   help = "submission for cedarContainer")
parser.add_argument("--maxProcs",       type=int,   default=4,                  help="Number of parallel processed")
parser.add_argument("--scanDir",        type = str,   default="root-files",       help="Default will be root-files")
parser.add_argument("--rankPOIs",       default=False,   action='store_true',    help="Will rank the impact POIs have on each other")
parser.add_argument("--debug",          default=False,   action='store_true',    help="Will run the code with the debug flag on")

parser.add_argument("--remergeResplit",       default = False, action='store_true',   help = "expert option to merge and split jobs")
parser.add_argument("--remergeResplitSize",   type = int, default = 1,   help = "expert option to find how many to merge jobs with")


parser.add_argument("--submitGrid",         default = False, action='store_true',   help = "If true, will submit grid jobs") 
parser.add_argument("--uniqueID",           type = str, default = "test_ID", required='--runGrid' in sys.argv,       help = "Unique ID for the grid")
parser.add_argument("--expertSkipScount",   default = False, action='store_true',   help = "If set, will use a expert option to skip scout for grid jobs")

parser.add_argument("--retry",              default = False, action='store_true',   help = "If true, only retry") 

args = parser.parse_args()


scansToRun = [
    # # Example of jobList structure
    # # ------------------------
    # jobListExample.scan1D, 
    # jobListExample.scan2D,
    # jobListExample.ranking,
    # jobListExample.correlationHist,
    # jobListExample.unCondWS,
    # jobListExample.scan1D_POs

    # # ------------------------
    # # H4l STXS Couplings
    # jobListCouplings.scan1D, 
    # jobListCouplings.scan2D,
    # jobListCouplings.correlationHist,
    # jobListCouplings.ranking,   
    # jobListCouplings.compatibility,   
    # jobListCouplings.scan1D_POs

    # # ------------------------
    # # H4l Diff
    # jobListDiff.scan1D, 
    # jobListDiff.scan2D, 
    # jobListDiff.ranking,
    # jobListDiff.correlationHist,
    # jobListDiff.compatibility,

    ###### FCCee ##########
    jobListFCC_ZHAllHad.scan1D,
    jobListFCC_ZHAllHad.correlationHist,
#     jobListFCC.scan1D,
#     jobListFCC.correlationHist,



]  

def main():

    submissionManager = submissionFactory(args)
    
    ##############################################################
    ##                     Configure the jobs                   ##
    ##############################################################
    ## loop through the different scans the user is submitting, e.g. ranking, correlation, etc.
    for scan in scansToRun:        
        ## For each of those groups, user can have requested multiple jobs (ex. stat and sys scan 1D ranking)
        ## Here we make a job manager for each of those that configures the job
        for job in scan:
            # First check for the existence of the WS
            if(not os.path.isfile(job["fileName"])): 
                print("Workspace", job["fileName"], "not found in folder...proceeding with rest")
                continue

            currJobManger = JOFactory(job, args)  

            currJobManger.process()

            submissionManager.addManager(currJobManger)


    ##############################################################
    ##                      Submit the jobs                     ##
    ##############################################################
    ## This is the actual submission to different systems, e.g. condor, batch, grid, local
    ## Information from the jobManager is re-organized here 
    submissionManager.process()
    submissionManager.submitJob()



## This calls the appropriate class to parse the jobInfo
def JOFactory(job, args):
    jobDimension = job["dimension"]

    if("1DToys" in jobDimension):           return scan1DToys(job, args)
    elif("2DToys" in jobDimension):         return scan2DToys(job, args)
    elif("1" in jobDimension):              return scan1D(job, args)
    elif("2" in jobDimension):              return scan2D(job, args)
    elif("rank" in jobDimension):           return ranking(job, args)
    elif("corr"  in jobDimension 
        or "save" in jobDimension):         return saveCorrUncond(job, args)
    elif("comp" in jobDimension):           return compatibility(job, args)
    elif("scanDataset" in jobDimension):    return scanDatasets(job, args)
    elif("runToys" in jobDimension):        return runToys(job, args)
    elif("minosError" in jobDimension):     return minosError(job, args)

    else:
        print("Dimension", jobDimension, "not recognized")
        exit(1)

def submissionFactory(args):
    if(args.submitSlurm):            return submitSlurm(args)
    elif(args.submitCondor):         return submitCondor(args)
    elif(args.submitCedarContainer): return submitCedarContainer(args)
    elif(args.runLocal):             return runLocal(args)
    elif(args.submitGrid):           return submitGrid(args)
    elif(args.dryRun):               return doDryRun(args)
    else:
        print("You did not enter where to submit to")
        print("A 'dryRun' was selected for you and your commands are printed below")
        print("-------------------------------------------------")
        return doDryRun(args)


# run the main function
if __name__ == "__main__":
    print("Starting submission!")
    main()






