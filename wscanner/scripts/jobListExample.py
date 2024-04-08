from buildList import *


#=============================================================================
#                                   1D Scans                                 =                          
#=============================================================================
scan1D = []
inputDict = {}
#SO
inputDict["poiList"] = [
"mu_gg2H:50:0.6:1.5", 
"mu_VBF:50:0:4.2", 
"mu_VH:50:0:5", 
"mu_ttH:50:0:8",
"r_ZZ:20:0.8:1.2",
"r_ttV:20:0:10"
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S_S0_r_v19_NN_withSys_model.root", "workspaces/WS_combined_XS_STXS_S_S0_r_LHCP2018_withSys_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S_S0_r_v19_NN_withSys_model", "WS_combined_XS_STXS_S_S0_r_LHCP2018_withSys_model"]
buildList(inputDict, scan1D)

#=============================================================================
#                                   2D Scans                                 =                          
#=============================================================================
scan2D = []
inputDict = {}
inputDict["poiList"] = [
"mu_gg2H:50:0.6:1.5,mu_VBF:50:0:5.5"]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["statOnly"]
inputDict["dimension"]      = "2D"
inputDict["doData"]         = [True,False]
inputDict["fileName"]       = ["workspaces/STXS_mu_v18_S0.root"]
inputDict["outputDir"]      = ["2D_STXS_mu_v18_S0"]
buildList(inputDict, scan2D)


#=============================================================================
#                                   Ranking                                  =                          
#=============================================================================
ranking = []
inputDict = {}
inputDict["poiList"]        = ["(.*)"]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "ranking"
inputDict["doData"]         = [False]
#inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S_S0_r_v19_NN_withSys_model.root", "workspaces/WS_combined_XS_STXS_S_S0_r_LHCP2018_withSys_model.root"] + ["workspaces/WS_combined_XS_STXS_S_RS1_r_LHCP2018_withSys_model.root"] + ["workspaces/WS_combined_XS_STXS_S_VBF_pTH_r_v19_NN_withSys_model.root"]
#inputDict["outputDir"]      = ["WS_combined_XS_STXS_S_S0_r_v19_NN_withSys_model_ranking", "WS_combined_XS_STXS_S_S0_r_LHCP2018_withSys_model_ranking"] + ["WS_combined_XS_STXS_S_RS1_r_LHCP2018_withSys_model_ranking"] + ["WS_combined_XS_STXS_S_VBF_pTH_r_v19_NN_withSys_model_ranking"]
inputDict["fileName"]       = ["workspaces/WS_combined_mu_STXS_S_S0_r_v19_NN_withSys_model.root", "workspaces/WS_combined_mu_STXS_S_S0_r_LHCP2018_withSys_model.root"] + ["workspaces/WS_combined_mu_STXS_S_RS1_r_LHCP2018_withSys_model.root"] + ["workspaces/WS_combined_mu_STXS_S_VBF_pTH_r_v19_NN_withSys_model.root"]
inputDict["outputDir"]      = ["WS_combined_mu_STXS_S_S0_r_v19_NN_withSys_model_ranking", "WS_combined_mu_STXS_S_S0_r_LHCP2018_withSys_model_ranking"] + ["WS_combined_mu_STXS_S_RS1_r_LHCP2018_withSys_model_ranking"] + ["WS_combined_mu_STXS_S_VBF_pTH_r_v19_NN_withSys_model_ranking"]

buildList(inputDict, ranking)


#=============================================================================
#                                   Compatibility                            =                          
#=============================================================================
compatibility = []
inputDict = {}
inputDict["poiList"]        = ["'*'"]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["singlePOI"]
inputDict["dimension"]      = "compatibility"
inputDict["doData"]         = [True,False]
inputDict["fileName"]       = ["workspaces/STXS_mu_v18_S0.root"]
inputDict["outputDir"]      = ["STXS_mu_v18_S0_compatibility"]
buildList(inputDict, compatibility)


#=============================================================================
#                                   correlationHist                          =                          
#=============================================================================
correlationHist = []
inputDict["poiList"]        = ["'*'"]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "correlationHist"
inputDict["saveCorrHist"]   = True   
inputDict["fileName"]       = ["workspaces/STXS_mu_v18_S0.root"]
inputDict["outputDir"]      = ["STXS_mu_v18_S0_corrHist"]
buildList(inputDict, correlationHist)


#=============================================================================
#                                   Unconditional ws                         =                          
#=============================================================================
# this will make an unconditional workspace
savePulls = []
inputDict["poiList"]        = ["'*'"]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "savePulls"
inputDict["doData"]         = [True,False]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V6_model.root"]
inputDict["outputDir"]      = ["testing_savePulls"]
buildList(inputDict, savePulls)


