from buildList import *

#=============================================================================
#                                   1D Scans                                 =                          
#=============================================================================
scan1D = []
inputDict = {}
inputDict["poiList"] = [
"mu_ggF:50:0.6:1.5", 
"r_ZZ_0jet:50:0.75:1.3",
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys"]
#inputDict["scanType"]       = ["modTheory"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V7_model"] 
inputDict["nSplitJob"]      = 5
# buildList(inputDict, scan1D)

inputDict = {}
inputDict["poiList"] = [
"mu_ggF:50:0.6:1.5", 
"r_ZZ_0jet:50:0.75:1.3",
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys"]
#inputDict["scanType"]       = ["modTheory"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V7_model_5"] 
inputDict["nSplitJob"]      = 5
buildList(inputDict, scan1D)




inputDict["poiList"] = [
"mu_ttH:50:0.7:2,nToys:2:1000", 
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["statOnly"]
inputDict["dimension"]      = "1DToys"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V7_modelToys"] 
inputDict["nSplitJob"]      = 5
# buildList(inputDict, scan1D)






scan2D = []
inputDict = {}
inputDict["poiList"] = [
"mu_ggF:50:0.5:1.5,mu_VBF:50:0:4.2", 
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys"]
inputDict["dimension"]      = "2D"
inputDict["doData"]         = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V7_model_2D_5"] 
inputDict["nSplitJob"]      = 5
buildList(inputDict, scan2D)

inputDict = {}
inputDict["poiList"] = [
"mH_2e2mu:50:124:126,mH_4e:50:124:126", 
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys"]
inputDict["dimension"]      = "2D"
inputDict["doData"]         = [False, True]
inputDict["fileName"]       = ["workspaces/J21_Analytic_All_All_PerBDTBin_fullMC_V12PerChan_workspace.root"]
inputDict["outputDir"]      = ["J21_Analytic_All_All_PerBDTBin_fullMC_V12PerChan_workspace_2D"] 
inputDict["nSplitJob"]      = 5
# buildList(inputDict, scan2D)

inputDict = {}
inputDict["poiList"] = [
"mu_VBF:50:0:4,mu_VH:50:0:8,nToys:2:1000", 
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["statOnly"]
inputDict["dimension"]      = "2DToys"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V7_2DmodelToys"] 
inputDict["nSplitJob"]      = 5
# buildList(inputDict, scan2D)





inputDict = {}
inputDict["poiList"] = [
"mH:50:124:126", 
]
inputDict["config"]         = "${wscanner_data}/H4lMass/H4lMassConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS1D_All_All_noRedBkg_PerChannel_fullMC_workspace.root"]
inputDict["outputDir"]      = ["WS1D_All_All_noRedBkg_PerChannel_fullMC_workspace"] 
inputDict["nSplitJob"]      = 5
# buildList(inputDict, scan1D)

ranking = []
inputDict = {}
inputDict["poiList"]        = ["(.*)"]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "ranking"
inputDict["doData"]         = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V7_model_ranking"] 
inputDict["nSplitJob"]      = 5
buildList(inputDict, ranking)


inputDict = {}
inputDict["poiList"]        = ["(.*)"]
inputDict["config"]         = "${wscanner_data}/H4lMass/H4lMassConfig.txt"
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "ranking"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS1D_All_All_noRedBkg_PerChannel_fullMC_workspace.root"]
inputDict["outputDir"]      = ["WS1D_All_All_noRedBkg_PerChannel_fullMC_workspace_ranking"] 
inputDict["nSplitJob"]      = 5
# buildList(inputDict, ranking)



#=============================================================================
#								    correlationHist							 =							
#=============================================================================
correlationHist = []
inputDict["poiList"] 	    = ["'*'"]
inputDict["config"] 	    = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"] 	    = ["allSys"]
inputDict["dimension"] 	    = "correlationHist"
inputDict["saveCorrHist"]   = True 
inputDict["doData"] 	    = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V7_model_corr"] 
inputDict["nSplitJob"]      = 1
buildList(inputDict, correlationHist)



#=============================================================================
#								    Compatibility							 =							
#=============================================================================
compatibility = []
inputDict = {}
inputDict["poiList"] 		= ["'*'"]
inputDict["config"] 		= "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"] 		= ["singlePOI"]
inputDict["dimension"] 		= "compatibility"
inputDict["doData"] 		= [True, False]
inputDict["fileName"] 		= ["workspaces/WS_combined_XS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"] 		= ["WS_combined_XS_STXS_S0_v23_V7_model_compatibility"]
buildList(inputDict, compatibility)



#=============================================================================
#								    Unconditional ws						 =							
#=============================================================================
# this will make an unconditional workspace
unCondWS = []
inputDict["poiList"] 		= ["'*'"]
inputDict["config"] 		= "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"] 		= ["allSys"]
inputDict["doData"]           = [False, True]
inputDict["dimension"] 		= "saveUncondws"
inputDict["saveUncondWorkspace"] 	= True   
inputDict["fileName"] 		= ["workspaces/WS_combined_XS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"] 		= ["WS_combined_XS_STXS_S0_v23_V7_model_unCondWS"]
buildList(inputDict, unCondWS)



#=============================================================================
#                                dataset Scans                               =                          
#=============================================================================
scanDatasets = []
inputDict = {}



for index in range(0, 10):
      inputDict = {}


      wsName = "workspaces/J21_PER_All_All_PerBDTBin_noNegCDFToys_datasetPart"+str(index)+"_V10_workspace.root"
      outName = "AnalyticFit_"+str(index)+"_simpleToys"

      inputDict["poiList"]        = ["scanDatasets:"+wsName+":25"]
      inputDict["config"]         = "${wscanner_data}/H4lMass/H4lMassConfig.txt"
      inputDict["scanType"]       = ["statOnly"]
      inputDict["dimension"]      = "scanDatasets"
      inputDict["doData"]         = [False]
      inputDict["fileName"]       = ["workspaces/J21_Analytic_All_All_PerBDTBin_fullMC_V14_workspace.root"]
      inputDict["outputDir"]      = [outName] 
      inputDict["nSplitJob"]      = 1
      buildList(inputDict, scanDatasets)





