def buildList(inputDict, jobs):
    for i in range(0, len(inputDict["fileName"])):
        for dataSet in inputDict["doData"]:
            for scanType in inputDict["scanType"]:
                for poi in inputDict["poiList"]:
                    jobDef = {}
                    jobDef["poiInfo"]      = poi
                    jobDef["config"]       = inputDict["config"]
                    jobDef["scanType"]     = scanType
                    jobDef["dimension"]    = inputDict["dimension"]
                    jobDef["doData"]       = dataSet
                    if "extraOpts" in inputDict:
                        jobDef["extraOpts"]       = inputDict["extraOpts"]
                    else: 
                        jobDef["extraOpts"]       = []
                    if 'saveCorrHist' in inputDict:
                        jobDef["saveCorrHist"] = inputDict["saveCorrHist"]
                    else:
                        jobDef["saveCorrHist"] = False

                    if 'saveUncondWorkspace' in inputDict:
                        jobDef["saveUncondWorkspace"] = inputDict["saveUncondWorkspace"]
                    else:
                        jobDef["saveUncondWorkspace"] = False

                    if 'adaptiveSampling' in inputDict:
                        jobDef["adaptiveSampling"] = inputDict["adaptiveSampling"]


                    if 'scanToy' in inputDict:
                        jobDef["scanToy"] = inputDict["scanToy"]
                    else:
                        jobDef["scanToy"] = False

                    if 'extraVarSave' in inputDict:
                        jobDef["extraVarSave"] = inputDict["extraVarSave"]
                    else:
                        jobDef["extraVarSave"] = ""

                    if 'nSplitJob' in inputDict:
                        jobDef["nSplitJob"] = inputDict["nSplitJob"]

                    if 'nRepeat' in inputDict:
                        jobDef["nRepeat"] = inputDict["nRepeat"]

                    if 'batchStyle' in inputDict:
                        jobDef["batchStyle"] = inputDict["batchStyle"]

                    if "forceFix" in inputDict:
                        jobDef["forceFix"]       = inputDict["forceFix"]

                    if "forceFree" in inputDict:
                        jobDef["forceFree"]      = inputDict["forceFree"]

                    jobDef["fileName"]     = inputDict["fileName"][i]
                    jobDef["outputDir"]    = inputDict["outputDir"][i]
                    jobs.append(jobDef)

def getfileList(preFix, suffix, number):
    fileList = []

    for i in range(0, number):
        fileList.append(preFix + str(i) + suffix)

    # print fileList

    return fileList

