from buildList import *

#=============================================================================
#                                   1D Scans                                 =                          
#=============================================================================
scan1D = []
inputDict = {}
inputDict["poiList"] = [
"mu:50:0.6:1.5", 
"r_ZZ_0jet:50:0.75:1.3",
"r_ZZ_1jet:50:0.5:1.5",
"r_ZZ_2jet:50:0.5:1.7",
"r_ttV:50:0.4:4"
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys"]
#inputDict["scanType"]       = ["modTheory"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V6_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V6_model"] 
inputDict["nSplitJob"]      = 5
# buildList(inputDict, scan1D)

inputDict["poiList"] = [
"mu:200:0.6:1.5", 
"r_ZZ_0jet:50:0.75:1.3",
"r_ZZ_1jet:50:0.5:1.5",
"r_ZZ_2jet:50:0.5:1.7",
"r_ttV:50:0.4:4"
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys", "modTheory"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_sigmaMu_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_sigmaMu_STXS_S0_v23_V7_model"] 
inputDict["nSplitJob"]      = 5
#buildList(inputDict, scan1D)

##SO Mu
inputDict["poiList"] = [
"mu_ggF:200:0.6:1.5", 
"mu_VBF:200:0:4.2", 
"mu_VH:200:0:5", 
"mu_ttH:200:0:8",
"r_ZZ_0jet:50:0.75:1.3",
"r_ZZ_1jet:50:0.5:1.5",
"r_ZZ_2jet:50:0.5:1.7",
"r_ttV:50:0.4:4"
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys", "modTheory"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_mu_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_mu_STXS_S0_v23_V7_model"] 
inputDict["nSplitJob"]      = 5
#buildList(inputDict, scan1D)

##SO XS
inputDict["poiList"] = [
"mu_ggF:200:0.6:1.5", 
"mu_VBF:200:0:4.2", 
"mu_VH:200:0:5", 
"mu_ttH:200:0:8",
"r_ZZ_0jet:50:0.75:1.3",
"r_ZZ_1jet:50:0.5:1.5",
"r_ZZ_2jet:50:0.5:1.7",
"r_ttV:50:0.4:4"
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys", "modTheory"]
# inputDict["scanType"]       = ["modTheory"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V7_model"] 
inputDict["nSplitJob"]      = 5
#buildList(inputDict, scan1D)



#S0qqH XS 
inputDict["poiList"] = [
"mu_gg2H:50:0.6:1.5", 
"mu_qqH2qq:50:0:4.2", 
"mu_qq2Hll:50:0:5", 
"mu_ttH:50:0:8",
"r_ZZ_0jet:20:0.75:1.3",
"r_ZZ_1jet:20:0.5:1.5",
"r_ZZ_2jet:20:0.5:1.7",
"r_ttV:20:0.4:4"
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
#inputDict["scanType"]       = ["statOnly", "allSys"]
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0qqH_v23_V5_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0qqH_v23_V5_model"] 
inputDict["nSplitJob"]      = 5
#buildList(inputDict, scan1D) 

#S0qqH Mu
inputDict["poiList"] = [
"mu_gg2H:50:0.6:1.5", 
"mu_qqH2qq:50:0:4.2", 
"mu_qq2Hll:50:0:5", 
"mu_ttH:50:0:8",
"r_ZZ_0jet:20:0.75:1.3",
"r_ZZ_1jet:20:0.5:1.5",
"r_ZZ_2jet:20:0.5:1.7",
"r_ttV:20:0.4:4"
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [True, False]
inputDict["fileName"]       = ["workspaces/WS_combined_mu_STXS_S0qqH_v23_V2_model.root"]
inputDict["outputDir"]      = ["WS_combined_mu_STXS_S0qqH_v23_V2_model"] 
inputDict["nSplitJob"]      = 5
#buildList(inputDict, scan1D) 


#RS1p1 XS
inputDict["poiList"] = [
'mu_ggF_0J_PTH_0_10:200:0.5:1.7',
'mu_ggF_0J_PTH_GT10:200:0.5:1.7',
'mu_ggF_1J_PTH_0_60:200:-1:3.5',
'mu_ggF_1J_PTH_120_200:200:0:3',
'mu_ggF_1J_PTH_60_120:200:-3:6',
'mu_ggF_GE2J:200:-0.5:4',
'mu_ggF_PTH_GT200:200:0:4.5',
'mu_VBF_qq2qq_PTH_GT200:200:-4:7',
'mu_VBF_qq2qq_PTH_LE200:200:0:5',
'mu_VH_Had:200:0:8',
'mu_VH_Lep:200:0:7',
"mu_ttH:200:0:8",
"r_ZZ_0jet:50:0.75:1.3",
"r_ZZ_1jet:50:0.5:1.5",
"r_ZZ_2jet:50:0.5:1.7",
"r_ttV:50:0.4:4"
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys", "modTheory"]
#inputDict["scanType"]       = ["modTheory"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False,True]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_RS1p1_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_RS1p1_v23_V7_model"]
inputDict["nSplitJob"]      = 5
# buildList(inputDict, scan1D) 



# #RS1p1qqH XS
inputDict["poiList"] = [
'mu_gg2H_0J_PTH_0_10:200:0.5:1.7',
'mu_gg2H_0J_PTH_GT10:200:0.5:1.7',
'mu_gg2H_1J_PTH_0_60:200:0:3.5',
'mu_gg2H_1J_PTH_60_120:200:0:3',
'mu_gg2H_1J_PTH_120_200:200:-1.5:6',
'mu_gg2H_GE2J:200:-0.5:4',
'mu_gg2H_PTH_GT200:200:0:4',
'mu_qq2Hqq_mJJ_GT350_PTH_GT200:200:-2:10',
'mu_qq2Hll:200:0:8',
'mu_qqH2qq_VHHad:200:-1:8',
'mu_qqH2qq_rest:200:-1:4',
"mu_ttH:200:0:8",
"r_ZZ_0jet:50:0.75:1.3",
"r_ZZ_1jet:50:0.5:1.5",
"r_ZZ_2jet:50:0.5:1.7",
"r_ttV:50:0.4:4"
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys", "modTheory"]
#inputDict["scanType"]       = ["modTheory"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False,True]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_RS1p1qqH_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_RS1p1qqH_v23_V7_model"]
inputDict["nSplitJob"]      = 5
# buildList(inputDict, scan1D) 


# RS1p1qqH Mu
inputDict["poiList"] = [
'mu_gg2H_0J_PTH_0_10:200:0.5:1.7',
'mu_gg2H_0J_PTH_GT10:200:0.5:1.7',
'mu_gg2H_1J_PTH_0_60:200:-1:3.5',
'mu_gg2H_1J_PTH_60_120:200:0:3',
'mu_gg2H_1J_PTH_120_200:200:-1.5:6',
'mu_gg2H_GE2J:200:-0.5:4',
'mu_gg2H_PTH_GT200:200:0:4.5',
'mu_qq2Hqq_mJJ_GT350_PTH_GT200:200:-2:10',
'mu_qqH2qq_VHHad:200:-3:5',
'mu_qqH2qq_rest:200:0:5',
'mu_qq2Hll:200:0:8',
"mu_ttH:200:0:8",
"r_ZZ_0jet:50:0.75:1.3",
"r_ZZ_1jet:50:0.5:1.5",
"r_ZZ_2jet:50:0.5:1.7",
"r_ttV:20:0.4:4"
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["statOnly", "allSys", "modTheory"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_mu_STXS_RS1p1qqH_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_mu_STXS_RS1p1qqH_v23_V7_model"]
inputDict["nSplitJob"]      = 5
# buildList(inputDict, scan1D) 



inputDict["poiList"] 	    = ["runToys:1000"]
inputDict["config"] 	    = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"] 	    = ["allSys"]
inputDict["dimension"] 	    = "runToys"
inputDict["doData"] 	    = [True, False]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V6_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V6_model_runToys"]
inputDict["nSplitJob"]      = 5
buildList(inputDict, scan1D)


#inputDict["poiList"] = [
#"mu_gg2H:10:0.7:1.3,nToys:100", 
#]
#inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
#inputDict["scanType"]       = ["statOnly"]
#inputDict["dimension"]      = "1DToys"
#inputDict["doData"]         = [False]
#inputDict["fileName"]       = ["workspaces/WS_combined_noSys_STXS_S0_v22t2_V1_model.root"]
#inputDict["outputDir"]      = ["WS_combined_noSys_STXS_S0_v22t2_V1_modelToys"] 
#inputDict["nSplitJob"]      = 5
## just repeat it multiple times to all for more stats
#inputDict["nRepeat"]        = 1
## This option splits the job in a into 1 point per actual run Command
## but then it bunches up the commands into nJobs each with ~ equal number of complexity
## This is to allow the memory to be cleared after each point
## a nessecary evil cuz roofit leaks memory
#inputDict["batchStyle"]     = {"doBatchStyle": True, "nJobs": 10} 
## the scanning code adapts the number of toy per point based on what the exclusion is for that job
## inputDict["adaptiveSampling"] = {"doAdaptiveSampling":True, "inputFile":"Plots/ws_m12m34_expected_mtx_incl_v22t_flavourUniversal_EFT/InputsForAdaptiveSampling/eL_kZZ.root", "histName":"ExclusionCont"}
## buildList(inputDict, scan2D)

# buildList(inputDict, scan1D)



#=============================================================================
#                                   2D Scans                                 =                          
#=============================================================================
scan2D = []
inputDict = {}

inputDict["poiList"] = [
"mu_ggF:50:0.5:1.5,mu_VBF:50:0:4.2",
"mu_ggF:50:0.5:1.5,mu_VH:50:0:8",
"mu_VBF:50:0:4,mu_VH:50:0:8",
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
#inputDict["scanType"]       = ["statOnly", "allSys"]
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "2D"
inputDict["doData"]         = [True]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V6_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V6_model_2D"] 
inputDict["nSplitJob"]      = 10
# buildList(inputDict, scan2D)



inputDict["poiList"] = [
'mu_gg2H_0J_PTH_0_10:50:0:2.4,mu_gg2H_0J_PTH_GT10:50:0.4:2.2',
]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
#inputDict["scanType"]       = ["statOnly", "allSys"]
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "2D"
inputDict["doData"]         = [True]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_RS1p1qqH_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_RS1p1qqH_v23_V7_model_2D"]
inputDict["nSplitJob"]      = 10
# buildList(inputDict, scan2D) 


#=============================================================================
#                                   Ranking                                  =                          
#=============================================================================
ranking = []
inputDict = {}
inputDict["poiList"]        = ["(.*)"]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "ranking"
inputDict["doData"]         = [True, False]
inputDict["fileName"]       = ["workspaces/WS_combined_sigmaMu_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_sigmaMu_STXS_S0_v23_V7_model_ranking"] 
inputDict["nSplitJob"]      = 40
# buildList(inputDict, ranking)

inputDict["poiList"]        = ["(.*)"]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "ranking"
inputDict["doData"]         = [True, False]
inputDict["fileName"]       = ["workspaces/WS_combined_sigmaXS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_sigmaXS_STXS_S0_v23_V7_model_ranking"] 
inputDict["nSplitJob"]      = 40
# buildList(inputDict, ranking)

inputDict["poiList"]        = ["(.*)"] 
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "ranking"
inputDict["doData"]         = [True, False]
inputDict["fileName"]       = [ "workspaces/WS_combined_XS_STXS_S0_v23_V6_model.root"]
# "workspaces/WS_combined_XS_STXS_RS1p1_v23_V7_model.root", "workspaces/WS_combined_XS_STXS_RS1p1qqH_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V6_model_ranking"]
# "WS_combined_XS_STXS_RS1p1_v23_V7_model_ranking", "WS_combined_XS_STXS_RS1p1qqH_v23_V7_model_ranking"]
inputDict["nSplitJob"]      = 40
buildList(inputDict, ranking)


inputDict["poiList"]        = ["(.*)"]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "ranking"
inputDict["doData"]         = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_mu_STXS_S0_v23_V7_model.root", "workspaces/WS_combined_mu_STXS_RS1p1qqH_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_mu_STXS_S0_v23_V7_model_ranking", "WS_combined_mu_STXS_RS1p1qqH_v23_V7_model_ranking"]
inputDict["nSplitJob"]      = 40
# buildList(inputDict, ranking)



#=============================================================================
#								    Compatibility							 =							
#=============================================================================
compatibility = []
inputDict = {}
inputDict["poiList"] 		= ["'*'"]
inputDict["config"] 		= "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"] 		= ["singlePOI"]
inputDict["dimension"] 		= "compatibility"
inputDict["doData"] 		= [True]
inputDict["fileName"] 		= ["workspaces/WS_combined_mu_STXS_RS1p1qqH_v23_V3_model.root"]
inputDict["outputDir"] 		= ["WS_combined_mu_STXS_RS1p1qqH_v23_V3_model.root_compatibility"]
# buildList(inputDict, compatibility)


inputDict = {}
inputDict["poiList"] 		= ["'*'"]
inputDict["config"] 		= "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"] 		= ["singlePOI"]
inputDict["dimension"] 		= "compatibility"
inputDict["doData"] 		= [True]
inputDict["fileName"] 		= ["workspaces/WS_combined_XS_STXS_S0_v23_V6_model.root"]
inputDict["outputDir"] 		= ["WS_combined_XS_STXS_S0_v23_V6_model_compatibility"]
buildList(inputDict, compatibility)


#=============================================================================
#								    correlationHist							 =							
#=============================================================================
correlationHist = []
inputDict["poiList"] 	    = ["'*'"]
inputDict["config"] 	    = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"] 	    = ["allSys"]
inputDict["dimension"] 	    = "correlationHist"
inputDict["saveCorrHist"]   = True 
inputDict["doData"] 	    = [False, True]
inputDict["fileName"]       = ["workspaces/WS_combined_sigmaMu_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_sigmaMu_STXS_S0_v23_V7_model_corr"] 
inputDict["nSplitJob"]      = 1
# buildList(inputDict, correlationHist)

inputDict["poiList"]        = ["'*'"]
inputDict["config"]         = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "correlationHist"
inputDict["saveCorrHist"]   = True 
inputDict["doData"]         = [True, False]
inputDict["fileName"]       = ["workspaces/WS_combined_sigmaXS_STXS_S0_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_sigmaXS_STXS_S0_v23_V7_model_corr"]
inputDict["nSplitJob"]      = 1
# buildList(inputDict, correlationHist)

inputDict["poiList"] 	    = ["'*'"]
inputDict["config"] 	    = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"] 	    = ["allSys"]
inputDict["dimension"] 	    = "correlationHist"
inputDict["saveCorrHist"]   = True   
inputDict["doData"] 	    = [True, False]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V7_model.root", "workspaces/WS_combined_XS_STXS_RS1p1_v23_V7_model.root", "workspaces/WS_combined_XS_STXS_RS1p1qqH_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V7_model_corr","WS_combined_XS_STXS_RS1p1_v23_V7_model_corr", "WS_combined_XS_STXS_RS1p1qqH_v23_V7_model_corr"]
inputDict["nSplitJob"]      = 1
# buildList(inputDict, correlationHist)


inputDict["poiList"]        = ["'*'"]
inputDict["config"]         = "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"]       = ["allSys"]
inputDict["dimension"]      = "correlationHist"
inputDict["saveCorrHist"]   = True   
inputDict["doData"]         = [True, False]
inputDict["fileName"]       = ["workspaces/WS_combined_mu_STXS_S0_v23_V7_model.root", "workspaces/WS_combined_mu_STXS_RS1p1qqH_v23_V7_model.root"]
inputDict["outputDir"]      = ["WS_combined_mu_STXS_S0_v23_V7_model_corr","WS_combined_mu_STXS_RS1p1qqH_v23_V7_model_corr"]
inputDict["nSplitJob"]      = 1
# buildList(inputDict, correlationHist)


inputDict["poiList"] 	    = ["'*'"]
inputDict["config"] 	    = "${wscanner_data}/H4lCoup/XSConfig.txt"
inputDict["scanType"] 	    = ["allSys"]
inputDict["dimension"] 	    = "correlationHist"
inputDict["saveCorrHist"]   = True   
inputDict["doData"] 	    = [True, False]
inputDict["fileName"]       = ["workspaces/WS_combined_XS_STXS_S0_v23_V6_model.root"]
inputDict["outputDir"]      = ["WS_combined_XS_STXS_S0_v23_V6_model_corr"]
inputDict["nSplitJob"]      = 1
buildList(inputDict, correlationHist)


#=============================================================================
#								    Unconditional ws						 =							
#=============================================================================
# this will make an unconditional workspace
unCondWS = []
inputDict["poiList"] 		= ["'*'"]
inputDict["config"] 		= "${wscanner_data}/H4lCoup/muConfig.txt"
inputDict["scanType"] 		= ["allSys"]
inputDict["doData"]         = [False]
inputDict["dimension"] 		= "saveUncondws"
inputDict["saveUncondWorkspace"] 	= True   
inputDict["fileName"] 		= ["workspaces/STXS_mu_v18_S0.root"]
inputDict["outputDir"] 		= ["STXS_mu_v18_S0_unCondWS"]
buildList(inputDict, unCondWS)


# CONF STXS Ranges
# 'mu_gg2H_0J_PTH_0_10:100:0.5:1.7',
# 'mu_gg2H_0J_PTH_GT10:100:0.5:1.7',
# 'mu_gg2H_1J_PTH_0_60:100:0:3.5',
# 'mu_gg2H_1J_PTH_60_120:100:0:3',
# 'mu_gg2H_1J_PTH_120_200:100:-1.5:6',
# 'mu_gg2H_GE2J:100:-0.5:4',
# 'mu_gg2H_PTH_GT200:100:0:4',
# 'mu_VBF_qq2qq_PTH_GT200:100:-2:10',
# 'mu_VBF_qq2qq_PTH_LE200:100:0:5',
# 'mu_VH_Had:100:0:8',
# 'mu_VH_Lep:100:0:7',
# "mu_ttH:100:0:8",
# "r_ZZ_0jet:20:0.75:1.3",
# "r_ZZ_1jet:20:0.5:1.5",
# "r_ZZ_2jet:20:0.5:1.7",
# "r_ttV:20:0.4:4"


