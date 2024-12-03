from buildList import *

#=============================================================================
#                                   1D Scans                                 =                          
#=============================================================================
scan1D = []
inputDict = {}
inputDict["poiList"] = [
# "mu_HWW:10:0.93:1.07", 
# "mu_HZZ:40:0.50:1.50",
"mu_Hbb:40:0.99:1.01",
"mu_Hcc:40:0.90:1.10",
"mu_Hgg:40:0.8:1.2",
"mu_Hss:40:-25.00:25.00"
# "mu_Htautau:10:0.80:1.20", 
# "mu_WW:10:0.9:1.1", 
# "mu_ZZ:10:0.5:1.5",
# "mu_Zqq:10:0.9:1.1",
# "mu_qqH:10:0.97:1.03", 
]
inputDict["config"]         = "${wscanner_data}/FCC/ZHAllHad.txt"
inputDict["scanType"]       = ["statOnly"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["/usatlas/u/ivelisce/FCCeePostCutCodeDir/run/wsfolder/WS_combined_wsfolder_model.root"]
inputDict["outputDir"]      = ["WS_comb"] 
inputDict["nSplitJob"]      = 1
buildList(inputDict, scan1D)





#=============================================================================
#								    correlationHist							 =							
#=============================================================================
correlationHist = []
inputDict["poiList"] 	    = ["'*'"]
inputDict["config"]         = "${wscanner_data}/FCC/ZHAllHad.txt"
inputDict["scanType"] 	    = ["statOnly"]
inputDict["dimension"] 	    = "correlationHist"
inputDict["saveCorrHist"]   = True 
inputDict["doData"] 	    = [False]
inputDict["fileName"]       = ["/usatlas/u/ivelisce/FCCeePostCutCodeDir/run/wsfolder/WS_combined_wsfolder_model.root"]
inputDict["outputDir"]      = ["WS_comb"] 
inputDict["nSplitJob"]      = 1
buildList(inputDict, correlationHist)
