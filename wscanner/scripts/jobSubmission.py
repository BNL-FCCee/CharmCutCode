import abc
import os
import argparse
import copy
from itertools import chain
import shutil
import shlex
import subprocess
import time

class submission:
    def __init__(self, args):
        self.args = args
        self.managerList = []

    def addManager(self, jobManager):
        # store the managers in a list
        self.managerList.append(jobManager)


    def _remergeReplitSubjobs(self, size):
        if(size == -1):
            mergedSubjobList = []
            for job in self.managerList:
                mergedSubjobList.append(job.getJobInfo())
            return mergedSubjobList


        # flatten the list
        flattenJobList = []
        for job in self.managerList:
            flattenJobList += job.getJobInfo()

        # extra the base path
        basePath = flattenJobList[0]["jobOutPath"]
        basePath = basePath[0:basePath.index('sJob') + 4]

        # create new folder for output jobsLIst
        os.system("mkdir -p jobList/%s/%s" % (basePath, "mergeJob"))
        import random
        random.shuffle(flattenJobList)

        # Merge commands a vector to run co-currently 
        counter     = 0
        mergeJobTmp = []
        tmp         = []
        for job in flattenJobList:
            jobName = "source jobList/%s/%s.sh" %(job["jobOutPath"],job["jobOutName"])

            if(counter >= size):
                mergeJobTmp.append(tmp)
                tmp = []
                counter = 0

            counter += 1
            tmp.append(jobName)


        if(len(tmp) > 0): mergeJobTmp.append(tmp)


        # write the new jobs list
        counter = 0
        filePath = "jobList/%s/%s/" % (basePath, "mergeJob")
        mergedSubjobList = []

        for job in mergeJobTmp:
            fileNameBase = "mergeJob" + str(counter)
            fileName = filePath + "/" +  fileNameBase + ".sh"
            fileObj = open(fileName, 'w')
            fileObj.write("#!/bin/bash\n\n")

            for subJob in job:
                fileObj.write(subJob + "\n")

            fileObj.write("\n\n")

            fileObj.close()

            os.system("chmod u+x " + fileName)
            outJob = {}
            outJob["jobOutPath"] = "%s/%s/" % (basePath, "mergeJob")
            outJob["jobOutName"] = fileNameBase

            mergedSubjobList.append(outJob)

            counter += 1

        return [mergedSubjobList]


    def _getJobList(self):
        ## if the user wants to remerge everything then do a new splitting, otherwise do a default split of 1
        if(self.args.remergeResplit):
            print("You are using an 'expert' option that is useful in some cases")
            print("Every subjob you created will be remerged and resplit based on the grouping you gave")
            print("This is useful if you have many small short scans and you dont want to submit each as separate job")
            print("remergeResplitSize controls how many jobs in each subjob")
            finalManagerList = self._remergeReplitSubjobs(self.args.remergeResplitSize)
        else:
            finalManagerList = self._remergeReplitSubjobs(-1)

        return finalManagerList


    @abc.abstractmethod
    def process(self):
        pass

    @abc.abstractmethod
    def submitJob(self):
        pass

    def _cleanFolder(self, path):
        shutil.rmtree(path, ignore_errors=True)

class submitGrid(submission):
    def __init__(self, args):
        submission.__init__(self, args)
        self.gridCmdList = []


    # remerge list based on output folder to simplify submission
    def _remergeListForGrid(self, inList):
        remergeMap = {}
        for job in inList:
            for subJob in job:
                fName = subJob["jobOutPath"].split("/")[0]
                if fName in remergeMap:
                    remergeMap[fName].append(subJob)
                else:
                    remergeMap[fName] = [subJob]

        # organize everything in main list
        remergeList = []
        for l in remergeMap:
            remergeList.append(remergeMap[l])


        # create folder submission of grid
        folderName = "gridJobList"


        gribJobList = []
        # copy the scripts into a nice structure for grid
        for jList in remergeList:
            fName = jList[0]["jobOutPath"].split("/")[0]
            os.system("mkdir -p %s/%s" % (folderName, fName))

            counter = 0

            for subJob in jList:
                jobScript = "jobList/" + subJob["jobOutPath"] + "/" + subJob["jobOutName"] + ".sh"
                outJScript = folderName + "/" + fName + "/job" + str(counter) + "_grid.sh"
                counter += 1
                os.system("cp %s %s" % (jobScript, outJScript))

            gridJob = {}
            gridJob["name"]         = fName
            gridJob["jobFolder"]    = "%s/%s" % (folderName, fName)
            gridJob["nJobs"]        = counter

            gribJobList.append(gridJob)

        return gribJobList


    def _makeTarBall(self):
        import datetime
        import tarfile

        tarFileName = "gridSubmit_"+datetime.datetime.now().strftime('%Y%b%d_%I%M')+".tgz.gz"
        # lets make the tar ball first
        inTarFile = tarfile.open(tarFileName, "w:gz")

        EXCLUDE_FILES = ['.git', '.pyc']

        # for excluding fucntion
        def filter_function(tarinfo):
            for fileName in EXCLUDE_FILES:
                if(fileName in tarinfo.name):
                    return None
            return tarinfo


        # get the path to source folder
        pathToSource = os.path.relpath(os.path.expandvars("${wscanner_data}/../../"))
        # pathToBuild = os.path.relpath(os.path.expandvars("${wscanner_data}/../../../build"))
        inTarFile.add("workspaces", filter=filter_function)
        inTarFile.add("gridJobList", filter=filter_function)
        inTarFile.add(pathToSource, arcname="source/", filter=filter_function)
        # inTarFile.add(pathToBuild, arcname="build/", filter=filter_function)
        inTarFile.add(pathToSource + "/wscanner/scripts/gridRun.sh", arcname="./gridRun.sh", filter=filter_function)

        inTarFile.close()

        return tarFileName


    def _gridJobDef(self, gName, tarName, jobNum):
        # extract the info
        
        # for higgs production role
        _prodRoleList = []
        # _prodRoleList += ["sabidi"]

        command = 'prun --exec "source gridRun.sh %s' %gName["jobFolder"]
        command += ' %RNDM:0"'

        # Outputs
        command += ' --outputs JobSummary.tgz.gz' 

        # setup        
        command += ' --tmpDir /tmp'
        command += ' --inTarBall %s' %tarName

        # input dataset
        command += ' --nJobs %s' %(str(gName["nJobs"]))
        if(self.args.expertSkipScount): command += ' --expertOnly_skipScout'
        
        command += ' --mergeOutput'

        # output DS name
        # For higgs production role
        if(os.environ["USER"] in _prodRoleList):
            command += ' --outDS group.phys-higgs.%s.' %os.environ["USER"]
        else:
            command += ' --outDS user.%s.' %os.environ["USER"]

        command += '%s.' %(gName["name"])
        command += '%i.' %(jobNum)

        command += self.args.uniqueID 

        # for higgs production role
        if(os.environ["USER"] in _prodRoleList):
            command += ' --official --voms=atlas:/atlas/phys-higgs/Role=production'

        return command



    def process(self):
        jList = self._getJobList()

        # get the list for grid submission
        gList = self._remergeListForGrid(jList)

        # make the tarball
        tarBallName = self._makeTarBall()

        counter = 0
        for gJob in gList:
            cmd = self._gridJobDef(gJob, tarBallName, counter)
            counter += 1
            self.gridCmdList.append(cmd)

    def submitJob(self):
        if(self.args.dryRun):
            print("Dry run of your grid jobs. Here are the submit commands, the grid jobs are configured in gridJobList/")
            for cmd in self.gridCmdList:
                print(cmd)
        else:
            for cmd in self.gridCmdList:
                print(cmd)
                os.system(cmd)
            print("grid submission completed")
        pass

class runLocal(submission):
    def __init__(self, args):
        submission.__init__(self, args)
        self.flatJobList = []


    def process(self):
        self.directory = "localRunLogs"

        # flatten the list
        for job in self.managerList:
            self.flatJobList += job.getJobInfo()

        subFolderName = self.directory + "/" + self.flatJobList[0]["jobOutPath"].split("/")[0]

        # remove previous condor scripts
        self._cleanFolder(subFolderName)

    def submitJob(self):

        # actually start the combine process
        processes = set()
        procList = []
        max_processes = self.args.maxProcs

        ## get the unique directories in the flatendList
        uniqueFolders = list(set(dic["jobOutPath"] for dic in self.flatJobList)) 

        ## create only those
        for folder in uniqueFolders:
            os.system("mkdir -p %s/%s" % (self.directory, folder) )

        i = 0
        totalJobs = len(self.flatJobList)
        ## loop through all the subjobs
        for job in self.flatJobList:
            baseDir = self.directory + "/" + job["jobOutPath"]

            jobListPath = "jobList/" + job["jobOutPath"] + job["jobOutName"] +  ".sh"

            # print command
            i = i + 1
            if(i%5 == 0):
                precentDone = float(i)/totalJobs * 100
                print('Running command i: ',i, ' percentage done: ' , precentDone)

            shellCommand = "source %s " %jobListPath
            # shellCommand = "echo %s " %jobListPath
            arguments = shlex.split(shellCommand)
            logFile = baseDir + job["jobOutName"] + ".txt"


            w = open(logFile,'w')

            # start the process
            p = subprocess.Popen(shellCommand, stdout=w, stderr=w, shell=True)


            # store for backup and for the queue
            processes.add(p)

            # check if the current number of processes exceed the maximum number
            if len(processes) >= max_processes:
                #print 'holding since max process exceeded limit'            
                while True:
                    processes.difference_update([p for p in processes if p.poll() is not None])
                    if len(processes) < max_processes:
                        break
                    # sleep for a bit to save polling time
                    time.sleep(10)

        print('Polling for the last few jobs')
        # poll till all the processes are done
        while True:
            processes.difference_update([p for p in processes if p.poll() is not None])
            if len(processes) == 0:
                break
            pass
            # sleep for a bit to save polling time
            time.sleep(5)
        pass
        
        print('Waiting for output files to get written to disk')
        # to wait for the last few files to be written
        time.sleep(5)

class doDryRun(submission):
    def __init__(self, args):
        submission.__init__(self, args)
        self.flatJobList = []

    def process(self):
        # flatten the list
        for job in self.managerList:
            self.flatJobList += job.getJobInfo()    

    def submitJob(self):
        for subJob in self.flatJobList:
            jobList = "jobList/" + subJob["jobOutPath"] + subJob["jobOutName"] + ".sh"
            print(jobList)
               
class submitCondor(submission):
    def __init__(self, args):
        submission.__init__(self, args)
        self.submitCommands = []

    def _makeCondorExecutable(self, subDir):
        executableName = "condor/" + subDir + "/condorExe.sh"

        fileObj = open(executableName, 'w')
        fileObj.write("#!/bin/bash\n\n")
        # cd into the folder
        fileObj.write("cd %s\n" %os.getcwd())

        ##setup root
        ##fileObj.write("source setup.sh\n")

        # run the command which is a input argument
        fileObj.write("source $1\n")

        fileObj.close()

        os.system("chmod u+x " + executableName)

        return executableName 

    def _makeCondorSteer(self, currJob, subDir):
        executableName = "condor/" + subDir + "/condorSteer.sub"

        fileObj = open(executableName, 'w')
        fileObj.write("executable = condor/%s/condorExe.sh \n" % subDir)
        fileObj.write('+JobFlavour = "%s" \n' % self.args.queue)
        fileObj.write("getenv = True \n\n")

        for subJob in currJob:
            condorPath = "condor/" + subJob["jobOutPath"]
            # if the folder doesn't exist create it
            if(not os.path.isdir(condorPath)):
                os.system("mkdir -p %s" % (condorPath))
            
            fileObj.write("arguments = jobList/%s/%s.sh \n" %(subJob["jobOutPath"],subJob["jobOutName"]))
            fileObj.write("output    = condor/%s/%s.out \n" %(subJob["jobOutPath"],subJob["jobOutName"]))
            fileObj.write("error     = condor/%s/%s.err \n" %(subJob["jobOutPath"],subJob["jobOutName"]))
            fileObj.write("log       = condor/%s/%s.log \n" %(subJob["jobOutPath"],subJob["jobOutName"]))
            fileObj.write("queue \n\n")

        os.system("chmod u+x " + executableName)

    # remerge list based on output folder to reduce number of jobs
    def _remergeListForCondor(self, inList):
        remergeMap = {}
        for job in inList:
            for subJob in job:
                fName = subJob["jobOutPath"].split("/")[0]
                if fName in remergeMap:
                    remergeMap[fName].append(subJob)
                else:
                    remergeMap[fName] = [subJob]

        remergeList = []
        for l in remergeMap:
            remergeList.append(remergeMap[l])


        return remergeList

         


    def process(self):

        directory = "condor"
        finalManagerList = self._getJobList()
        finalManagerList = self._remergeListForCondor(finalManagerList)

        print(finalManagerList)

        
        ## The printing of the options to lists
        ## makes the condor steer and exe for each job grouping
        for job in finalManagerList:
            subFolderName = directory + "/" + job[0]["jobOutPath"].split("/")[0]
            ## remove previous condor scripts
            self._cleanFolder(subFolderName)

            subDir = job[0]["jobOutPath"]
            os.system("mkdir -p %s/%s" % (directory, subDir))
            
            self._makeCondorSteer(job, subDir)
            self._makeCondorExecutable(subDir)

            condorCmd = "condor_submit -batch-name " + job[0]["jobOutPath"].split("/")[0] + " " + directory + "/" + subDir + "condorSteer.sub"
            self.submitCommands.append(condorCmd)

        print("Condor submission scripts written in condor/")



    def submitJob(self):
        if(self.args.dryRun):
            print("Dry run of your condor jobs. Here are the submit commands, the condor jobs are configured in condor/")
            for cmd in self.submitCommands:
                print(cmd)

        else:
            for cmd in self.submitCommands:
                os.system(cmd)
            print("Condor submission completed")

class submitSlurm(submission):
    def __init__(self, args):
        submission.__init__(self, args)
        self.submitCommands = []



    def _makeSlurmExecutable(self, subJob, subDir):

        fileName = "slurm/" + subDir + "/" + subJob["jobOutName"] + ".sh"
        fileObj = open(fileName, 'w')
        outFileName = "stdout_" + subJob["jobOutName"] + ".txt"

        CWD = os.getcwd()

        text = """#!/bin/bash    
#SBATCH 
#SBATCH --time=6:00:00
#SBATCH --mem=4000M
"""
        text += """
#SBATCH --job-name %s_%s """ %(subDir.replace("/","_"), subJob["jobOutName"])

        if(os.environ["USER"] == "ciungubi"):
            text += """
#SBATCH --account=def-sinervo""" 
        else:
            text += """
#SBATCH --account=def-psavard""" 
    
        text += """
#SBATCH --output ./slurm/%s/%s""" %(subDir, outFileName)


        text += """
cd ~/
cd %s
module load nixpkgs/16.09 gcc/5.4.0 root/6.14.04 boost cmake
source %s
""" %(CWD, "jobList/" + subDir + "/" + subJob["jobOutName"] + ".sh")
        # text += cmdToRun
        text += """
pwd""" 
        text += """
ls""" 
        fileObj.write(text)


        os.system("chmod u+x " + fileName )

        return fileName



    def process(self):

        directory = "slurm"
        finalManagerList = self._getJobList()

        subFolderName = directory + "/" + finalManagerList[0][0]["jobOutPath"].split("/")[0]

        # remove previous slurm scripts
        self._cleanFolder(subFolderName)

        for job in finalManagerList:

            subDir = job[0]["jobOutPath"]
            os.system("mkdir -p %s/%s" % (directory, subDir))

            for subJob in job:
                submissionFileName =  self._makeSlurmExecutable(subJob, subDir);
                slurmCmd = "sbatch " +  submissionFileName
                self.submitCommands.append(slurmCmd)


        print("Slurm submission scripts written in slurm/")




    def submitJob(self):
        if(self.args.dryRun):
            print("Dry run of your slurm jobs. Here are the submit commands, the slurm jobs are configured in slurm/")
            for cmd in self.submitCommands:
                print(cmd)

        else:
            for cmd in self.submitCommands:
                os.system(cmd)
            print("Slurm submission completed")
        


class submitCedarContainer(submission):
    def __init__(self, args):
        submission.__init__(self, args)
        self.submitCommands = []

    def _makeCedarContainerExecutable(self, subJob, subDir):
        fileName = "slurm/" + subDir + "/" + subJob["jobOutName"] + ".sh"
        fileObj = open(fileName, 'w')
        outFileName = "stdout_" + subJob["jobOutName"] + ".txt"
        CWD = os.getcwd()
        jobList = CWD + "/jobList/" + subDir + "/" + subJob["jobOutName"] + ".sh"
        create_script = "batchScript \"source {}\" -O {}".format(jobList, fileName)
        os.system(create_script)
        os.system("chmod -R 775 " + fileName)

        return fileName

    def process(self):

        directory = "slurm"
        finalManagerList = self._getJobList()

        subFolderName = directory + "/" + finalManagerList[0][0]["jobOutPath"].split("/")[0]

        # remove previous slurm scripts
        self._cleanFolder(subFolderName)


        for job in finalManagerList:
            subDir = job[0]["jobOutPath"]
            os.system("mkdir -p %s/%s" % (directory, subDir))

            for subJob in job:
                submissionFile =  self._makeCedarContainerExecutable(subJob, subDir);
                slurmCmd = "sbatch --time=4:00:00 --mem=8000M {}".format(submissionFile)
                self.submitCommands.append(slurmCmd)

        print("Submission scripts for Cedar container written in slurm/")



    def submitJob(self):
        if(self.args.dryRun):
            print("Dry run of your slurm jobs. Here are the submit commands, the slurm jobs are configured in slurm/")
            for cmd in self.submitCommands:
                print(cmd)

        else:
            for cmd in self.submitCommands:
                os.system(cmd)
            print("Cedar container submission completed")
        



