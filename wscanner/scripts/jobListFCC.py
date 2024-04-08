from buildList import *

#=============================================================================
#                                   1D Scans                                 =                          
#=============================================================================
scan1D = []
inputDict = {}
inputDict["poiList"] = [
"mu_HWW:10:0.93:1.07", 
"mu_HZZ:10:0.50:1.50",
"mu_Hbb:10:0.99:1.01",
"mu_Hcc:10:0.90:1.10",
"mu_Hgg:10:0.97:1.03",
"mu_Hss:10:0.00:2.00",
"mu_Htautau:10:0.80:1.20", 
"mu_WW:10:0.999:1.001", 
"mu_ZZ:10:0.995:1.005",
"mu_Zqq:10:0.9993:1.0007",
"mu_qqH:10:0.97:1.03", 
]
inputDict["config"]         = "${wscanner_data}/FCC/ZvvH.txt"
inputDict["scanType"]       = ["statOnly"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["/usatlas/u/ivelisce/FCC/CharmCut/run/wsfolder/WS_combined_wsfolder_model.root"]
inputDict["outputDir"]      = ["WS_combined_ZHvv_v1_model_scan"] 
inputDict["nSplitJob"]      = 1
buildList(inputDict, scan1D)


#=============================================================================
#                                   2D Scans                                 =                          
#=============================================================================
# scan2D = []
# inputDict = {}

# inputDict["poiList"] = [
# "mu_ggF:50:0.5:1.5,mu_VBF:50:0:4.2",
# "mu_ggF:50:0.5:1.5,mu_VH:50:0:8",
# "mu_VBF:50:0:4,mu_VH:50:0:8",
# ]
# inputDict["config"]         = "${wscanner_data}/FCC/ZvvH.txt"
# inputDict["scanType"]       = ["statOnly"]
# inputDict["dimension"]      = "2D"
# inputDict["doData"]         = [False]
# inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V6_model.root"]
# inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V6_model_2D"] 
# inputDict["nSplitJob"]      = 10
# # buildList(inputDict, scan2D)


#=============================================================================
#								    correlationHist							 =							
#=============================================================================
correlationHist = []
inputDict["poiList"] 	    = ["'*'"]
inputDict["config"]         = "${wscanner_data}/FCC/ZvvH.txt"
inputDict["scanType"] 	    = ["statOnly"]
inputDict["dimension"] 	    = "correlationHist"
inputDict["saveCorrHist"]   = True 
inputDict["doData"] 	    = [False]
inputDict["fileName"]       = ["/usatlas/u/ivelisce/FCC/CharmCut/run/wsfolder/WS_combined_wsfolder_model.root"]
inputDict["outputDir"]      = ["WS_combined_ZHvv_v1_model_corr"] 
inputDict["nSplitJob"]      = 1
buildList(inputDict, correlationHist)
