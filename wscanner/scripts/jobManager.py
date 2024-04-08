
import abc
import os
import string
import argparse
import copy
# from RooStats import ModelConfig
import re
import math as math
import random




class baseManager:
    def __init__(self):
        self.outMap = []
        pass

    def getJobInfo(self):
        return self.outMap

class jobManager(baseManager):
    def __init__(self, jobInfo, args):
        baseManager.__init__(self)
        self.jobInfo = jobInfo
        self.args = args
        self._checkConfig()


    def _getPOIRange(self, poiInfo):
        poiStr = poiInfo.split(":")
        poiName = poiStr[0]
        poiSteps = int(poiStr[1])
        poiLwrLim = float(poiStr[2])
        poiUprLim = float(poiStr[3])
        return poiName,poiSteps,poiLwrLim,poiUprLim

    def _getnToysJobs(self, toyInfo):
        toyList = toyInfo.split(":")
        toyName = toyList[0]
        toySteps = int(toyList[1])
        toyNumber = int(toyList[2])
        return toyName, toySteps, toyNumber

    def _roundUp(self, num, divisor):
        if(divisor == 1): 
            return num
        return num - (num%divisor)

    @abc.abstractmethod
    def process(self):
        pass

    def _getCommand(self, job):
        text = "../build/wscanner/runScan"
        if("AnalysisBase_VERSION" in os.environ):
            text = "runScan"

        text += " --config %s" %(job["config"])
        text += " --scanType %s" %(job["scanType"])
        text += " --dimension %s" %(job["dimension"])
        text += " --fileName %s" %(job["fileName"])
        text += " --poiScan %s" %(job["poiInfo"])
        text += " --outputDir %s" %(job["outputDir"])
        text += " --outFileName %s" %(job["outFileName"])

        # outputName = "%s/%s/%s" %(args.scanDir,subJobOutDir, outFileName)

        if (job["doData"] == True):
            text += ' --doData '
        if (job["saveCorrHist"] == True):
            text += ' --saveCorrHist '

        if (len(job["extraVarSave"]) > 0):
            text += ' --extraVarSave \'%s\'' %(",".join(job["extraVarSave"]))
        if  "extraOpts" in job and len(job["extraOpts"]) > 0:
            text += " "+ " ".join(job["extraOpts"])

        if("forceFree" in job):
            text += ' --forceFree \'%s\'' %(job["forceFree"])
        if("forceFix" in job):
            text += ' --forceFix \'%s\'' %(job["forceFix"])

        # if (job["saveUncondWorkspace"] == True):
        #     text += ' --saveUncondWorkspace '
        #     outputName = "unCondWS/%s" %(job["fileName"])

        if (job["dimension"] == "runToys" or job["dimension"] == "2DToys" or job["dimension"] == "1DToys"):
            text += ' --seed %s'  %(str(random.randint(1,100000000000)))
        if(self.args.debug):
            text += ' --debug '

        return text

    def _search_file(self, filename, search_path, pathsep):
        """ Given a search path, find file with requested name """
        for path in string.split(search_path, pathsep):
            candidate = os.path.join(path, filename)
            if os.path.exists(candidate): return os.path.abspath(candidate)
        return None

    def _checkConfig(self):
        # Get full path to config file
        if len(self.jobInfo["config"]):
            if(not self.args.submitGrid):
                # expand the path
                self.jobInfo["config"] = os.path.relpath(os.path.expandvars(self.jobInfo["config"]))

            # find config file - either directly or in DATAPATH
            if os.path.exists(os.path.expandvars(self.jobInfo["config"])):
                return
            # config file is not full path - look in DATAPATH
            if ("DATAPATH" in os.environ):
                data_path = os.environ['DATAPATH']
                configFile = self._search_file(self.jobInfo["config"], data_path, ":")
                if configFile is not None:
                    # replace config with full path to file
                    self.jobInfo["config"] = configFile
                    return
            else:
                print("Error, could not find the config file", self.jobInfo["config"], "in path")
                exit(1)
                
    def _getConfigInfo(self, configFile):
        workspaceName = ""
        modelConfigName = ""
        with open (os.path.expandvars(configFile), 'r') as infile:
            for line in infile:
                if("workspaceName:" in line):
                    workspaceName = line.replace("workspaceName:","")
                    workspaceName = workspaceName.replace(" ","")
                    workspaceName = workspaceName.replace("\t","")
                    workspaceName = workspaceName.replace("\n","")
                if("modelConfigName:" in line):
                    modelConfigName = line.replace("modelConfigName:","")
                    modelConfigName = modelConfigName.replace(" ","")
                    modelConfigName = modelConfigName.replace("\t","")
                    modelConfigName = modelConfigName.replace("\n","")
        infile.close()

        if(len(workspaceName) == 0 or len(modelConfigName) == 0):
            print("Error, please check your configFile and make sure it has a workspaceName and a modelConfigName")
            exit(1)

        return workspaceName, modelConfigName 


    def _getJobSplit(self):
        nSplitJob = self.args.splitJob ## put args. here but we need it passed in
        if("nSplitJob" in self.jobInfo): nSplitJob = self.jobInfo["nSplitJob"]
        return nSplitJob

    def _getDataSetName(self, dataSet):
        if(dataSet == True):
            return "data"
        else:
            return "asimov"

    def _getOutputDir(self, poiName = ""):
        subJobOutDirBaseDir =   self.jobInfo["outputDir"] + "/" + self.jobInfo["scanType"] + "/" + self._getDataSetName(self.jobInfo["doData"])
        fullPath = "%s/"% (subJobOutDirBaseDir) + "/sJob/" 
        if(len(poiName) > 0):
            fullPath += poiName + "/"
        return fullPath

    def _writeJobList(self, jobOutPath, jobOutName, cmd):
        fileName = "jobList/" + jobOutPath + jobOutName + ".sh"
        fileObj = open(fileName, 'w')
        fileObj.write("#!/bin/bash\n\n")
        if(self.args.submitCedarContainer): 
            CWD = os.getcwd()
            fileObj.write("cd " + CWD + "\n")
            fileObj.write("source ../source/H4lMassWorkspace/setup_lxplus.sh \n")
        fileObj.write(cmd)
        fileObj.close()

        os.system("chmod u+x " + fileName)

    def _representsNum(self, s):
        try: 
            float(s)
            return True
        except ValueError:
            return False
        
class scan1D(jobManager):
    def __init__(self, jobInfo, args):
        jobManager.__init__(self, jobInfo, args)
        # print("Initializing a 1DScan jobManager")

    def process(self):
        nJobSplit = self._getJobSplit()

        # Calculate the number of splits
        optionList = self.jobInfo["poiInfo"]
        poiName, poiSteps, poiLwrLim, poiUprLim = self._getPOIRange(optionList)

        # calculate the delta step 
        poiSteps = self._roundUp(poiSteps, nJobSplit);
        deltaPoiStep = (poiUprLim - poiLwrLim) / poiSteps;
        deltaStep = float(poiSteps)/nJobSplit

        counter = 0
        # generate a new subjob object for each split
        for i in range(0, nJobSplit):
            info = {}

            currInfo            = copy.deepcopy(self.jobInfo)
            currLwrLim          = poiLwrLim + i * deltaStep * deltaPoiStep
            currUprLim          = poiLwrLim + (i+1) * deltaStep * deltaPoiStep
            currPoiStr          = str(int(deltaStep)) + ':' + str(currLwrLim) + ':' +  str(currUprLim)
            poiScanInfo         = poiName+ ':' + currPoiStr
            currInfo["poiInfo"] = poiScanInfo

        
            currInfo["outputDir"]   = self.args.scanDir + "/" + self._getOutputDir(poiName) 
            currInfo["outFileName"] = poiName+"_sJob_"+str(counter)+".root"

            info["jobOutPath"] = self._getOutputDir(poiName) 
            info["jobOutName"] = "job" + str(counter) 

            if (i == 0): 
                os.system("mkdir -p %s" % currInfo["outputDir"])
                os.system("mkdir -p jobList/%s" % info["jobOutPath"])


            ## write the jobList to a shellscript
            cmd = self._getCommand(currInfo)
            self._writeJobList(info["jobOutPath"], info["jobOutName"], cmd);

            self.outMap.append(info)


            counter += 1

class scan1DToys(jobManager):
    def __init__(self, jobInfo, args):
        jobManager.__init__(self, jobInfo, args)
        # print("Initializing a 1DScan jobManager")

    def process(self):
        nJobSplit = self._getJobSplit()

        # Calculate the number of splits
        optionList = self.jobInfo["poiInfo"].split(",")
        ## ex. "mu_ttH:50:0.7:2,nToys:2:1000", 

        if(len(optionList) != 2):
            print("You are asking to scan 1D toys, but didn't give enough options")
            exit(1)

        poiName, poiSteps, poiLwrLim, poiUprLim = self._getPOIRange(optionList[0])
        toyName, toySteps, toyNumber = self._getnToysJobs(optionList[1])

        # calculate the delta step 
        poiSteps = self._roundUp(poiSteps, nJobSplit);
        deltaPoiStep = (poiUprLim - poiLwrLim) / poiSteps;
        deltaStep = float(poiSteps)/nJobSplit


        toysPerJob = int(float(toyNumber)/float(toySteps))

        counter = 0
        # generate a new subjob object for each split
        for i in range(0, nJobSplit):

            for j in range(0, toySteps):

                info = {}

                currInfo            = copy.deepcopy(self.jobInfo)
                currLwrLim          = poiLwrLim + i * deltaStep * deltaPoiStep
                currUprLim          = poiLwrLim + (i+1) * deltaStep * deltaPoiStep
                currPoiStr          = str(int(deltaStep)) + ':' + str(currLwrLim) + ':' +  str(currUprLim)
                poiScanInfo         = poiName + ':' + currPoiStr
                poiScanInfo         += "," + toyName + ":" + str(toysPerJob)
                currInfo["poiInfo"] = poiScanInfo

            
                currInfo["outputDir"]   = self.args.scanDir + "/" + self._getOutputDir(poiName) 
                currInfo["outFileName"] = poiName+"_sJob_"+str(counter)+".root"

                info["jobOutPath"] = self._getOutputDir(poiName) 
                info["jobOutName"] = "job" + str(counter) 

                if (i == 0 and j == 0): 
                    os.system("mkdir -p %s" % currInfo["outputDir"])
                    os.system("mkdir -p jobList/%s" % info["jobOutPath"])


                ## write the jobList to a shellscript
                cmd = self._getCommand(currInfo)
                self._writeJobList(info["jobOutPath"], info["jobOutName"], cmd);

                self.outMap.append(info)


                counter += 1

class scan2DToys(jobManager):
    def __init__(self, jobInfo, args):
        jobManager.__init__(self, jobInfo, args)
        # print("Initializing a 1DScan jobManager")


    def process(self):
        nJobSplit = self._getJobSplit()

        # Calculate the number of splits
        optionList = self.jobInfo["poiInfo"].split(",")

        if(len(optionList) != 3):
            print("You are asking to scan 2D toys, but didn't give enough options")
            exit(1)


        poiName1, poiSteps1, poiLwrLim1, poiUprLim1 = self._getPOIRange(optionList[0])
        poiName2, poiSteps2, poiLwrLim2, poiUprLim2 = self._getPOIRange(optionList[1])
        toyName, toySteps, toyNumber = self._getnToysJobs(optionList[2])


        # round up poiSteps to the nearest multiple of the number of subjob to split
        poiSteps1 = self._roundUp(poiSteps1, nJobSplit);
        deltaPoiStep1 = (poiUprLim1 - poiLwrLim1) / poiSteps1;
        deltaStep1 = float(poiSteps1)/nJobSplit

        poiSteps2 = self._roundUp(poiSteps2, nJobSplit);
        deltaPoiStep2 = (poiUprLim2 - poiLwrLim2) / poiSteps2;
        deltaStep2 = float(poiSteps2)/nJobSplit


        toysPerJob = int(float(toyNumber)/float(toySteps))

        counter = 0
        # generate a new subjob object for each split
        for i in range(0, nJobSplit):
            for j in range(0, nJobSplit):
                for k in range(0, toySteps):
                    info = {}

                    currInfo            = copy.deepcopy(self.jobInfo)

                    currLwrLim1 = poiLwrLim1 + i * deltaStep1 * deltaPoiStep1
                    currUprLim1 = poiLwrLim1 + (i+1) * deltaStep1 * deltaPoiStep1

                    currLwrLim2 = poiLwrLim2 + j * deltaStep2 * deltaPoiStep2
                    currUprLim2 = poiLwrLim2 + (j+1) * deltaStep2 * deltaPoiStep2

                    currPoiStr1 = str(int(deltaStep1)) + ':' + str(currLwrLim1) + ':' +  str(currUprLim1)
                    currPoiStr2 = str(int(deltaStep2)) + ':' + str(currLwrLim2) + ':' +  str(currUprLim2)

                    poiScanInfo = poiName1+ ':' + currPoiStr1 + "," + poiName2 + ":" + currPoiStr2
                    poiScanInfo += "," + toyName + ":" + str(toysPerJob)

                    currInfo["poiInfo"] = poiScanInfo
                
                    currInfo["outputDir"]   = self.args.scanDir + "/" + self._getOutputDir(poiName1 + "_" + poiName2) 
                    currInfo["outFileName"] = poiName1 + "_" + poiName2+"_sJob_"+str(counter)+".root"

                    info["jobOutPath"] = self._getOutputDir(poiName1 + "_" + poiName2) 
                    info["jobOutName"] = "job" + str(counter) 

                    if (i == 0 and j == 0 and k == 0): 
                        os.system("mkdir -p %s" % currInfo["outputDir"])
                        os.system("mkdir -p jobList/%s" % info["jobOutPath"])


                    ## write the jobList to a shellscript
                    cmd = self._getCommand(currInfo)
                    self._writeJobList(info["jobOutPath"], info["jobOutName"], cmd);

                    self.outMap.append(info)


                counter += 1


class scan2D(jobManager):
    def __init__(self, jobInfo, args):
        jobManager.__init__(self, jobInfo, args)
        # print("Initializing a 2DScan jobManager")

    def process(self):
        nJobSplit = self._getJobSplit()


        # Calculate the number of splits
        optionList = self.jobInfo["poiInfo"].split(",")
        poiName1, poiSteps1, poiLwrLim1, poiUprLim1 = self._getPOIRange(optionList[0])
        poiName2, poiSteps2, poiLwrLim2, poiUprLim2 = self._getPOIRange(optionList[1])


        # round up poiSteps to the nearest multiple of the number of subjob to split
        poiSteps1 = self._roundUp(poiSteps1, nJobSplit);
        deltaPoiStep1 = (poiUprLim1 - poiLwrLim1) / poiSteps1;
        deltaStep1 = float(poiSteps1)/nJobSplit

        poiSteps2 = self._roundUp(poiSteps2, nJobSplit);
        deltaPoiStep2 = (poiUprLim2 - poiLwrLim2) / poiSteps2;
        deltaStep2 = float(poiSteps2)/nJobSplit


        counter = 0
        # generate a new subjob object for each split
        for i in range(0, nJobSplit):
            for j in range(0, nJobSplit):
                info = {}

                currInfo            = copy.deepcopy(self.jobInfo)

                currLwrLim1 = poiLwrLim1 + i * deltaStep1 * deltaPoiStep1
                currUprLim1 = poiLwrLim1 + (i+1) * deltaStep1 * deltaPoiStep1

                currLwrLim2 = poiLwrLim2 + j * deltaStep2 * deltaPoiStep2
                currUprLim2 = poiLwrLim2 + (j+1) * deltaStep2 * deltaPoiStep2

                currPoiStr1 = str(int(deltaStep1)) + ':' + str(currLwrLim1) + ':' +  str(currUprLim1)
                currPoiStr2 = str(int(deltaStep2)) + ':' + str(currLwrLim2) + ':' +  str(currUprLim2)

                poiScanInfo = poiName1+ ':' + currPoiStr1 + "," + poiName2 + ":" + currPoiStr2

                currInfo["poiInfo"] = poiScanInfo
            
                currInfo["outputDir"]   = self.args.scanDir + "/" + self._getOutputDir(poiName1 + "_" + poiName2) 
                currInfo["outFileName"] = poiName1 + "_" + poiName2+"_sJob_"+str(counter)+".root"

                info["jobOutPath"] = self._getOutputDir(poiName1 + "_" + poiName2) 
                info["jobOutName"] = "job" + str(counter) 

                if (i == 0 and j == 0): 
                    os.system("mkdir -p %s" % currInfo["outputDir"])
                    os.system("mkdir -p jobList/%s" % info["jobOutPath"])


                ## write the jobList to a shellscript
                cmd = self._getCommand(currInfo)
                self._writeJobList(info["jobOutPath"], info["jobOutName"], cmd);

                self.outMap.append(info)


                counter += 1


class ranking(jobManager):
    def __init__(self, jobInfo, args):
        jobManager.__init__(self, jobInfo, args)
        # print("Initializing a ranking jobManager")

    def _splitNP(self, numNP, spliting):
        if(spliting > numNP):
            return 1
        numPerJob = math.floor(float(numNP)/spliting)
        
        return int(numPerJob)

    def process(self):
        from ROOT import TFile, RooWorkspace
        
        nJobSplit = self._getJobSplit()

        workspaceName, modelConfigName = self._getConfigInfo(self.jobInfo["config"])
        wsfile = TFile(self.jobInfo["fileName"])
        ws = wsfile.Get(workspaceName)
        mc = ws.obj(modelConfigName)        
        nitr = mc.GetNuisanceParameters().createIterator()
        var = nitr.Next()

        NPNames = []

        while var:
            NPNames.append(var.GetName())
            var = nitr.Next()

        # Append the POI to this list as well if --rankPOIs is added
        if(self.args.rankPOIs):
            nitr = mc.GetParametersOfInterest().createIterator()
            var = nitr.Next()
            while var:
                NPNames.append(var.GetName())
                var = nitr.Next()

        regexList = []
        if(self.jobInfo["poiInfo"] == "*"):
            regexList = NPNames

        else:
            for NP in NPNames:
                isMatch = re.search(self.jobInfo["poiInfo"], NP)
                if(isMatch):
                    regexList.append(NP)
                    # print NP

        numPerJob = self._splitNP(len(regexList), nJobSplit)

        remaining = len(regexList)
        exitFlag = False
        usedNPName = []
        poiScanList = []

        for i in range(0, len(regexList)):
            poiScanInfo = ""
            tmppoiScanList = []
            startIndex = i * numPerJob
            for j in range(startIndex, startIndex + int(numPerJob)):
                # poiScanInfo += NPNames[j]
                usedNPName.append(regexList[j])
                tmppoiScanList.append(regexList[j])
            poiScanInfo = ",".join(tmppoiScanList)

            poiScanList.append(poiScanInfo)

            remaining -= numPerJob
            if(remaining < numPerJob):
                exitFlag = True     
            if(exitFlag):
                break

        if(usedNPName != regexList):
            notNPName = list(set(regexList).difference(set(usedNPName)))
            poiScanList.append(",".join(notNPName))

        counter = 0
        
        for i in range(0, len(poiScanList)):
            info = {}

            currInfo            = copy.deepcopy(self.jobInfo)
            currInfo["poiInfo"] = poiScanList[i]

        
            currInfo["outputDir"]   = self.args.scanDir + "/" + self._getOutputDir("") 
            currInfo["outFileName"] = "rankJob_sJob_"+str(counter)+".root"

            info["jobOutPath"] = self._getOutputDir("") 
            info["jobOutName"] = "job" + str(counter) 

            if (i == 0): 
                os.system("mkdir -p %s" % currInfo["outputDir"])
                os.system("mkdir -p jobList/%s" % info["jobOutPath"])

            ## write the jobList to a shellscript
            cmd = self._getCommand(currInfo)
            self._writeJobList(info["jobOutPath"], info["jobOutName"], cmd);

            self.outMap.append(info)


            counter += 1


class saveCorrUncond(jobManager):
    def __init__(self, jobInfo, args):
        jobManager.__init__(self, jobInfo, args)

    def process(self):
        from ROOT import TFile, RooWorkspace

        workspaceName, modelConfigName = self._getConfigInfo(self.jobInfo["config"])
        wsfile = TFile(self.jobInfo["fileName"])

        ws = wsfile.Get(workspaceName)
        mc = ws.obj(modelConfigName)        
        poi = mc.GetParametersOfInterest().first()

        poiName = poi.GetName()
        currPoiStr = '0:0:1'
        poiScanInfo = poiName+ ':' + currPoiStr

        info = {}
        currInfo            = copy.deepcopy(self.jobInfo)
        currInfo["poiInfo"] = poiScanInfo

        currInfo["outputDir"]   = self.args.scanDir + "/" + self._getOutputDir("") 
        currInfo["outFileName"] = "fitJob.root"

        info["jobOutPath"] = self._getOutputDir("") 
        info["jobOutName"] = "fitJob"

        os.system("mkdir -p %s" % currInfo["outputDir"])
        os.system("mkdir -p jobList/%s" % info["jobOutPath"])

        ## write the jobList to a shellscript
        cmd = self._getCommand(currInfo)
        self._writeJobList(info["jobOutPath"], info["jobOutName"], cmd);

        self.outMap.append(info)


class compatibility(jobManager):
    def __init__(self, jobInfo, args):
        jobManager.__init__(self, jobInfo, args)

    def process(self):
        info = {}
        currInfo            = copy.deepcopy(self.jobInfo)

        currInfo["outputDir"]   = self.args.scanDir + "/" + self._getOutputDir("") 
        currInfo["outFileName"] = "compatibility.root"

        info["jobOutPath"] = self._getOutputDir("") 
        info["jobOutName"] = "comptJob"

        os.system("mkdir -p %s" % currInfo["outputDir"])
        os.system("mkdir -p jobList/%s" % info["jobOutPath"])

        ## write the jobList to a shellscript
        cmd = self._getCommand(currInfo)
        self._writeJobList(info["jobOutPath"], info["jobOutName"], cmd);

        self.outMap.append(info)


class minosError(jobManager):
    def __init__(self, jobInfo, args):
        jobManager.__init__(self, jobInfo, args)

    def process(self):
        info = {}
        currInfo            = copy.deepcopy(self.jobInfo)

        currInfo["outputDir"]   = self.args.scanDir + "/" + self._getOutputDir("") 
        currInfo["outFileName"] = currInfo["poiInfo"] + ".root"

        info["jobOutPath"] = self._getOutputDir("") 
        info["jobOutName"] = currInfo["poiInfo"]

        os.system("mkdir -p %s" % currInfo["outputDir"])
        os.system("mkdir -p jobList/%s" % info["jobOutPath"])

        ## write the jobList to a shellscript
        cmd = self._getCommand(currInfo)

        self._writeJobList(info["jobOutPath"], info["jobOutName"], cmd);

        self.outMap.append(info)


class scanDatasets(jobManager):
    def __init__(self, jobInfo, args):
        jobManager.__init__(self, jobInfo, args)

    def process(self):
        nJobSplit = self._getJobSplit()


        optionList = self.jobInfo['poiInfo'].split(":")
        if(len(optionList) != 3):
            print("You are asking to scanDatasets toys, but didn't give enough options")
            exit(1)


        scanTypeInfo    = optionList[0]
        wsName          = optionList[1]
        nToys           = int(optionList[2])

        if(".root" in wsName):
            # First check for the existence of the WS
            if(not os.path.isfile(wsName)): 
                print("Workspace", wsName, "not found for scanDatasets, just gonna skip")
                return


        toyPerJob = int(float(nToys)/nJobSplit)

        counter = 0
        # generate a new subjob object for each split
        for i in range(0, nJobSplit):
            info = {}

            currInfo                = copy.deepcopy(self.jobInfo)
            currInfo["poiInfo"]     = scanTypeInfo + ":" + wsName + ":" + str(toyPerJob) + ":" + str(i*toyPerJob)
            currInfo["outputDir"]   = self.args.scanDir + "/" + self._getOutputDir() 
            currInfo["outFileName"] = "ScanDataset_sJob_"+str(counter)+".root"

            info["jobOutPath"]      = self._getOutputDir() 
            info["jobOutName"]      = "job" + str(counter) 

            if (i == 0): 
                os.system("mkdir -p %s" % currInfo["outputDir"])
                os.system("mkdir -p jobList/%s" % info["jobOutPath"])


            ## write the jobList to a shellscript
            cmd = self._getCommand(currInfo)
            self._writeJobList(info["jobOutPath"], info["jobOutName"], cmd);

            self.outMap.append(info)


            counter += 1


class runToys(jobManager):
    def __init__(self, jobInfo, args):
        jobManager.__init__(self, jobInfo, args)

    def process(self):
        nJobSplit = self._getJobSplit()


        optionList = self.jobInfo['poiInfo'].split(":")
        if(len(optionList) != 2 and len(optionList) != 3):
            print("You are asking to scanDatasets toys, but didn't give enough options")
            exit(1)

        if(len(optionList) == 3):
            scanTypeInfo    = optionList[0]
            wsName          = optionList[1]
            nToys           = int(optionList[2])

        if(len(optionList) == 2):
            scanTypeInfo    = optionList[0]
            wsName          = ""
            nToys           = int(optionList[1])


        if(".root" in wsName):
            # First check for the existence of the WS
            if(not os.path.isfile(wsName)): 
                print("Workspace", wsName, "not found for scanDatasets, just gonna skip")
                return


        toyPerJob = int(float(nToys)/nJobSplit)

        counter = 0
        # generate a new subjob object for each split
        for i in range(0, nJobSplit):
            info = {}

            currInfo                = copy.deepcopy(self.jobInfo)
            if(len(wsName) > 0):
                currInfo["poiInfo"] = scanTypeInfo + ":" + wsName + ":" + str(toyPerJob) + ":" + str(i*toyPerJob)
            else:
                currInfo["poiInfo"] = scanTypeInfo + ":" + str(toyPerJob)

            currInfo["outputDir"]   = self.args.scanDir + "/" + self._getOutputDir() 
            currInfo["outFileName"] = "ScanDataset_sJob_"+str(counter)+".root"

            info["jobOutPath"]      = self._getOutputDir() 
            info["jobOutName"]      = "job" + str(counter) 

            if (i == 0): 
                os.system("mkdir -p %s" % currInfo["outputDir"])
                os.system("mkdir -p jobList/%s" % info["jobOutPath"])


            ## write the jobList to a shellscript
            cmd = self._getCommand(currInfo)
            self._writeJobList(info["jobOutPath"], info["jobOutName"], cmd);

            self.outMap.append(info)

            counter += 1
