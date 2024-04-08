from buildList import *

# #=============================================================================
# #                                   1D Scans                                 =                          
# #=============================================================================
scan1D = []
inputDict = {}
#########
# Njets
#########
inputDict["poiList"] = [
"sigma_bin0_incl:40:1.0:3", 
"sigma_bin1_incl:40:0.5:1.5", 
"sigma_bin2_incl:40:0:1.5", 
"sigma_bin3_incl:50:0:0.8",
"MuqqZZ0:50:0.5:1.5:",
"MuqqZZ1:50:0.5:1.5",
"MuqqZZ2:50:0.3:2.0",
]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["statOnly", "allSys"]
inputDict["dimension"]          = "1D"
inputDict["doData"]             = [True,False]
inputDict["fileName"]           = ["workspaces/ws_njet_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_njet_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]          = 2
#buildList(inputDict, scan1D)

#########
# Njets Incl
#########
inputDict["poiList"] = [
"sigma_bin0123_incl:40:2.7:4.2", 
"sigma_bin123_incl:40:0.8:1.9", 
"sigma_bin23_incl:40:0.2:1.2", 
"sigma_bin3_incl:50:0:0.8",
"MuqqZZ0:50:0.5:1.5:",
"MuqqZZ1:50:0.5:1.5",
"MuqqZZ2:50:0.3:2.0",
]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["statOnly", "allSys"]
inputDict["dimension"]          = "1D"
inputDict["doData"]             = [True, False]
inputDict["fileName"]           = ["workspaces/ws_njet_inclusive_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_njet_inclusive_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]          = 2
buildList(inputDict, scan1D)


#########
# nbjets 
#########
inputDict["poiList"] = [
"sigma_bin0_incl:50:0:3",
"sigma_bin1_incl:100:0:3",
"sigma_bin2_incl:60:0:0.5",
"MuqqZZ0:60:0:5",
"MuqqZZ1:60:0:5",
 ]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_nbjet_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_nbjet_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)

#########
# ljet pT
#########
inputDict["poiList"] = [
"sigma_bin0_incl:60:1:2.5",
"sigma_bin1_incl:60:0.4:2.0",
"sigma_bin2_incl:60:0.2:1.0",
"sigma_bin3_incl:60:0:0.8",
"MuqqZZ0:60:0:5",
"MuqqZZ1:60:0:5",
"MuqqZZ2:60:0:5",
"MuqqZZ3:80:0:5",
 ]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_ljpt_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_ljpt_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)

#########
# subljet pT
#########
inputDict["poiList"] = [
"sigma_bin0_incl:60:2:4",
"sigma_bin1_incl:60:0.2:1",
"sigma_bin2_incl:60:0:0.5",
"sigma_bin3_incl:60:0:0.5",
"MuqqZZ0:80:0:5",
"MuqqZZ1:50:0:5",
"MuqqZZ2:50:0:5",
 ]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_sjpt_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_sjpt_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)


#########
# phijj 
#########
inputDict["poiList"] = [
"sigma_bin0_incl:50:0:4",
"sigma_bin1_incl:80:0:0.5",
"sigma_bin2_incl:80:0:2",
"sigma_bin3_incl:50:0:1.5",
"sigma_bin4_incl:80:0:1.5",
"MuqqZZ0:60:0:5",
"MuqqZZ1:60:0:5",
"MuqqZZ2:60:0:5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_phijj_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_phijj_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)

#########
# mjj 
#########
inputDict["poiList"] = [
"sigma_bin0_incl:50:2:4",
"sigma_bin1_incl:50:0:0.5",
"sigma_bin2_incl:50:0:0.8",
"sigma_bin3_incl:50:0:0.5",
"MuqqZZ0:80:0:5",
"MuqqZZ1:50:0:5",
"MuqqZZ2:60:0:5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_mjj_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_mjj_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)

#########
# etajj 
#########
inputDict["poiList"] = [
"sigma_bin0_incl:50:1.5:4",
"sigma_bin1_incl:50:0:0.5",
"sigma_bin2_incl:60:0:0.5",
"sigma_bin3_incl:60:0.0:0.5", 
"MuqqZZ0:80:0:5",
"MuqqZZ1:80:0:5",
"MuqqZZ2:60:0:5",
 ]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_etajj_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_etajj_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)

########
# pt4lj 
#########
inputDict["poiList"] = [
"sigma_bin0_incl:50:0.1:3",
"sigma_bin1_incl:50:0.5:2",
"sigma_bin2_incl:50:0.1:0.8",
"sigma_bin3_incl:50:0:0.3",
"MuqqZZ0:80:0:5",
"MuqqZZ1:80:0:5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_pt4lj_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_pt4lj_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)

########
# pt4ljj 
#########
inputDict["poiList"] = [
"sigma_bin0_incl:50:2:4",
"sigma_bin1_incl:50:0:1",
"sigma_bin2_incl:60:0:0.4",
"sigma_bin3_incl:50:0:0.1",
"MuqqZZ0:80:0:5",
"MuqqZZ1:80:0:5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [False,True]
inputDict["fileName"] 		= ["workspaces/ws_pt4ljj_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_pt4ljj_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)

#########
# m4lj 
#########
inputDict["poiList"] = [
"sigma_bin0_incl:50:0:2.6",
"sigma_bin1_incl:50:0:0.4",
"sigma_bin2_incl:50:0.1:0.5",
"sigma_bin3_incl:50:0.2:0.6",
"sigma_bin3_incl:50:0.2:0.6",
"sigma_bin4_incl:60:0:2",
"sigma_bin5_incl:60:0:2",
"sigma_bin6_incl:70:0:2",
"MuqqZZ0:80:0:5",
"MuqqZZ1:70:0:5",
"MuqqZZ2:80:0:5",
 ]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_m4lj_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_m4lj_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)

#########
# m4ljj 
#########
inputDict["poiList"] = [
"sigma_bin0_incl:50:2:3.5",
"sigma_bin1_incl:50:0:0.2",
"sigma_bin2_incl:50:0:0.3",
"sigma_bin3_incl:50:0:0.3",
"sigma_bin4_incl:60:0:0.3",
"sigma_bin5_incl:60:0:0.2",
"MuqqZZ0:70:0:5",
"MuqqZZ1:70:0:5",
"MuqqZZ2:70:0:5",
 ]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_m4ljj_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_m4ljj_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)

#########
# pT
#########
inputDict["poiList"] = [
"sigma_bin0_incl:30:0:1",
"sigma_bin1_incl:30:0:1",
"sigma_bin2_incl:30:0:1",
"sigma_bin3_incl:30:0:1",
"sigma_bin4_incl:30:0:1",
"sigma_bin5_incl:30:0:1",
"sigma_bin6_incl:40:0:1",
"sigma_bin7_incl:40:0:0.6",
"sigma_bin8_incl:40:0:0.3",
"sigma_bin9_incl:40:0:0.1",
"MuqqZZ0:50:0:5",
"MuqqZZ1:50:0:5",
"MuqqZZ2:50:0:5",
"MuqqZZ3:50:0:5",
"MuqqZZ4:50:0:5",
 ]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_pt_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_pt_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]          = 2
#buildList(inputDict, scan1D)

###########
### m12 ###
###########
inputDict["poiList"] = [
"sigma_bin0_incl:60:0:0.7",
"sigma_bin1_incl:60:0:0.6",
"sigma_bin2_incl:60:0:1",
"sigma_bin3_incl:60:1.4:3.4",
"MuqqZZ0:25:0.5:1.6",
"MuqqZZ1:25:0.5:1.6",
"MuqqZZ2:30:0.5:1.3",
]
inputDict["config"]         = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [True,False]
inputDict["fileName"]           = ["workspaces/ws_m12_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_m12_wsys_data_mtx_v23v3"]
#buildList(inputDict, scan1D)


###########
### m34 ###
###########
inputDict["poiList"] = [
"sigma_bin0_incl:50:0:1",
"sigma_bin1_incl:50:0:1",
"sigma_bin2_incl:50:0:1",
"sigma_bin3_incl:50:0:1",
"sigma_bin4_incl:50:0.2:1",
"sigma_bin5_incl:80:0:1",
"sigma_bin6_incl:80:0:2",
"MuqqZZ0:25:0.5:1.5",
"MuqqZZ1:25:0.5:1.5",
"MuqqZZ2:25:0.5:1.5",
]
inputDict["config"]         = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [True,False]
inputDict["fileName"]           = ["workspaces/ws_m34_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_m34_wsys_data_mtx_v23v3"]
#buildList(inputDict, scan1D)

#########
# m12m34 inclusive 
#########
inputDict["poiList"] = [
"sigma_bin0_incl:100:0:0.6",
"sigma_bin1_incl:100:0:0.6",
"sigma_bin2_incl:100:0.2:0.9",
"sigma_bin3_incl:100:0:1.5",
"sigma_bin4_incl:100:0:3",
"MuqqZZ0:30:0.5:1.5",
"MuqqZZ1:39:0.5:1.5",
"MuqqZZ2:30:0.5:1.5",
"MuqqZZ3:30:0.5:1.5",
]
inputDict["config"]         = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [True,False]
inputDict["fileName"]       = ["workspaces/ws_m12m34_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]      = ["ws_m12m34_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]          = 5
#buildList(inputDict, scan1D)

#########
# m12m34 per channel 
#########
inputDict["poiList"] = [
"sigma_bin1_4mu:100:0:0.3",
"sigma_bin1_4e:100:0:0.3",
"sigma_bin1_2mu2e:100:0:0.3",
"sigma_bin1_2e2mu:100:0:0.3",
"sigma_bin2_4mu:100:0:0.4",
"sigma_bin2_4e:100:0:0.4",
"sigma_bin2_2mu2e:100:0:0.4",
"sigma_bin2_2e2mu:100:0:0.4",
"sigma_bin3_4mu:100:0:0.7",
"sigma_bin3_4e:100:0:0.7",
"sigma_bin3_2mu2e:100:0:0.7",
"sigma_bin3_2e2mu:100:0:0.7",
"sigma_bin4_4mu:100:0:0.7",
"sigma_bin4_4e:100:0:0.7",
"sigma_bin4_2mu2e:100:0:0.7",
"sigma_bin4_2e2mu:100:0:0.7",
"MuqqZZ0:25:0.5:1.5",
"MuqqZZ1:25:0.5:1.5",
"MuqqZZ2:25:0.5:1.5",
"MuqqZZ3:25:0.5:1.5",
]
inputDict["config"]         = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]       = ["statOnly"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [True]
inputDict["fileName"]       = ["workspaces/ws_m12m34_perChan_expected_mtx_v20v2.root"]
inputDict["outputDir"]      = ["ws_m12m34_perChan_expected_mtx_v20v2"]
inputDict["nSplitJob"]          = 5
#buildList(inputDict, scan1D)

#########
# comb
#########
inputDict["poiList"] = [
"sigma:50:2.5:4.3",
"MuqqZZ0:25:0.5:1.5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [False,True]
inputDict["fileName"] 		= ["workspaces/ws_m_comb_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_m_comb_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]          = 5
#buildList(inputDict, scan1D)

#########
# 2l2l
#########
inputDict["poiList"] = [
"sigma_2l2l:50:1.2:2.2",
"MuqqZZ0:25:0.5:1.5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [False,True]
inputDict["fileName"] 		= ["workspaces/ws_m_fid2l2l_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_m_fid2l2l_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]          = 5
#buildList(inputDict, scan1D)

#########
# 4l
#########
inputDict["poiList"] = [
"sigma_4l:50:1.2:2.2",
"MuqqZZ0:25:0.5:1.5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [False,True]
inputDict["fileName"] 		= ["workspaces/ws_m_fid4l_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_m_fid4l_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]          = 5
#buildList(inputDict, scan1D)

#########
# channel
#########
inputDict["poiList"] = [
"sigma_4mu:50:0.5:1.2",
"sigma_4e:50:0.3:1.2",
"sigma_2mu2e:50:0.5:1.2",
"sigma_2e2mu:50:0.5:1.4",
"MuqqZZ0:25:0.5:1.5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [False,True]
inputDict["fileName"] 		= ["workspaces/ws_m_fidchannel_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_m_fidchannel_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]          = 5
#buildList(inputDict, scan1D)

#########
# sum
#########
inputDict["poiList"] = [
"sigma_sum:50:2.5:4.0",
"MuqqZZ0:25:0.5:1.5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [False,True]
inputDict["fileName"] 		= ["workspaces/ws_m_fidsum_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_m_fidsum_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]          = 5
#buildList(inputDict, scan1D)

#########
# Total
#########
inputDict["poiList"] = [
"sigma:50:45000:65000",
"MuqqZZ0:25:0.5:1.5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [False,True]
inputDict["fileName"] 		= ["workspaces/ws_m_total_wsys_data_mtx_v23v3_asys.root"]
inputDict["outputDir"] 		= ["ws_m_total_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]          = 5
#buildList(inputDict, scan1D)


################
# ct1 
################
inputDict["poiList"] = [
"sigma_bin0_incl:55:0.15:0.6",
"sigma_bin1_incl:50:0.25:0.7",
"sigma_bin2_incl:50:0.25:0.65",
"sigma_bin3_incl:50:0.25:0.75",
"sigma_bin4_incl:50:0.35:0.75",
"sigma_bin5_incl:50:0:0.75",
"sigma_bin6_incl:50:0.25:0.8",
"sigma_bin7_incl:50:0.1:0.45",
"MuqqZZ0:30:0.5:1.5",
"MuqqZZ1:30:0.5:1.5",
"MuqqZZ2:30:0.5:1.6",
"MuqqZZ3:30:0.5:1.5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_ct1_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_ct1_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 2
#buildList(inputDict, scan1D)

################
# ct2
################
inputDict["poiList"] = [
"sigma_bin0_incl:55:0:0.6",
"sigma_bin1_incl:50:0.1:0.9",
"sigma_bin2_incl:50:0.1:0.9",
"sigma_bin3_incl:50:0.2:1",
"sigma_bin4_incl:50:0.1:1",
"sigma_bin5_incl:50:0.1:0.9",
"sigma_bin6_incl:50:0.1:0.8",
"sigma_bin7_incl:50:0.1:0.7",
"MuqqZZ0:30:0.5:1.5",
"MuqqZZ1:30:0.5:1.5",
"MuqqZZ2:30:0.5:1.6",
"MuqqZZ3:30:0.5:1.5",
]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["statOnly", "allSys"]
inputDict["dimension"]          = "1D"
inputDict["doData"]             = [True,False]
inputDict["fileName"]           = ["workspaces/ws_ct2_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_ct2_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 2
#buildList(inputDict, scan1D)


################
# cts
################
inputDict["poiList"] = [
"sigma_bin0_incl:55:0.2:0.9",
"sigma_bin1_incl:50:0.2:0.9",
"sigma_bin2_incl:50:0.2:0.9",
"sigma_bin3_incl:50:0.2:0.9",
"sigma_bin4_incl:50:0:0.7",
"sigma_bin5_incl:50:0.1:0.8",
"sigma_bin6_incl:50:0.1:0.9",
"sigma_bin7_incl:50:0:0.7",
"MuqqZZ0:30:0.5:1.5",
"MuqqZZ1:30:0.5:1.5",
"MuqqZZ2:30:0.5:1.6",
"MuqqZZ3:30:0.5:1.5",
]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["statOnly", "allSys"]
inputDict["dimension"]          = "1D"
inputDict["doData"]             = [True,False]
inputDict["fileName"]           = ["workspaces/ws_cts_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_cts_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 2
#buildList(inputDict, scan1D)



########
# y4l
#########
inputDict["poiList"] = [
"sigma_bin0_incl:40:0.1:0.6",
"sigma_bin1_incl:40:0.1:0.6",
"sigma_bin2_incl:40:0.1:0.6",
"sigma_bin3_incl:40:0.1:0.6",
"sigma_bin4_incl:40:0.1:0.6",
"sigma_bin5_incl:40:0.1:0.6",
"sigma_bin6_incl:40:0.3:0.8",
"sigma_bin7_incl:40:0.3:0.9",
"sigma_bin8_incl:40:0.1:0.6",
"sigma_bin9_incl:40:0.0:0.4",
"MuqqZZ0:30:0.25:2",
"MuqqZZ1:30:0.25:2",
"MuqqZZ2:30:0.25:2",
"MuqqZZ3:30:0.25:2",
"MuqqZZ4:30:0.25:2",
"MuqqZZ5:30:0.25:2",
"MuqqZZ6:30:0.25:2",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [True,False]
inputDict["fileName"] 		= ["workspaces/ws_y_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_y_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 2
# buildList(inputDict, scan1D)


#########
# phi, phi1
#########
inputDict["poiList"] = [
"sigma_bin0_incl:50:0:0.8",
"sigma_bin1_incl:50:0:0.8",
"sigma_bin2_incl:50:0:0.8",
"sigma_bin3_incl:50:0:0.8",
"sigma_bin4_incl:50:0:0.8",
"sigma_bin5_incl:50:0:0.8",
"sigma_bin6_incl:50:0:0.8",
"sigma_bin7_incl:50:0:1",
"MuqqZZ0:40:0:5",
"MuqqZZ1:40:0:5",
"MuqqZZ2:40:0:5",
"MuqqZZ3:40:0:5",
]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["statOnly", "allSys"]
inputDict["dimension"]          = "1D"
inputDict["doData"]             = [False,True]
inputDict["fileName"]           = ["workspaces/ws_phi_wsys_data_mtx_v23v3.root", "workspaces/ws_phi1_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_phi_wsys_data_mtx_v23v3", "ws_phi1_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)



########
# ptljpt 
#########
inputDict["poiList"] = [
"sigma_bin0_incl:50:1:2.8",
"sigma_bin1_incl:50:0:1.2",
"sigma_bin2_incl:50:0:0.5",
"sigma_bin3_incl:80:0.1:08",
"sigma_bin4_incl:50:0:0.3",
"sigma_bin5_incl:60:0:3",
"sigma_bin6_incl:80:0:3",
"MuqqZZ0:60:0:5",
"MuqqZZ1:60:0:5",
"MuqqZZ2:40:0:5",
]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["statOnly", "allSys"]
inputDict["dimension"] 		= "1D"
inputDict["doData"] 		= [False,True]
inputDict["fileName"] 		= ["workspaces/ws_ptljpt_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"] 		= ["ws_ptljpt_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)


##########
#ljptsjpt
##########
inputDict["poiList"] = [
"sigma_bin0_incl:30:1:2.8",
"sigma_bin1_incl:30:0:2",
"sigma_bin2_incl:30:0:0.7",
"sigma_bin3_incl:30:0:0.7",
"sigma_bin4_incl:30:0:0.65",
"sigma_bin5_incl:30:0:0.45",
"MuqqZZ0:50:0:2",
"MuqqZZ1:40:0:2",
"MuqqZZ2:40:0:2",
]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["statOnly", "allSys"]
inputDict["dimension"]          = "1D"
inputDict["doData"]		= [False,True]
inputDict["fileName"]           = ["workspaces/ws_ljptsjpt_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]		= ["ws_ljptsjpt_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)


##########
#ptnjet
##########
inputDict["poiList"] = [
"sigma_bin0_incl:40:0:2",
"sigma_bin1_incl:40:0:1.3",
"sigma_bin2_incl:40:0:1",
"sigma_bin3_incl:40:0:2",
"sigma_bin4_incl:40:0:1.2",
"sigma_bin5_incl:40:0:0.5",
"sigma_bin6_incl:40:0:0.65",
"sigma_bin7_incl:40:0:0.4",
"sigma_bin8_incl:40:0:0.8",
"sigma_bin9_incl:40:0:0.9",
"sigma_bin10_incl:40:0:0.5",
"sigma_bin11_incl:40:0:0.3",
"MuqqZZ0:50:0:2",
"MuqqZZ1:40:0:2",
"MuqqZZ2:40:0:2",
]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["statOnly", "allSys"]
inputDict["dimension"]          = "1D"
inputDict["doData"]             = [True, False]
inputDict["fileName"]           = ["workspaces/ws_ptnjet_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_ptnjet_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)

##########
#pty
##########
inputDict["poiList"] = [
"sigma_bin0_incl:50:0:2",
"sigma_bin1_incl:50:0:2",
"sigma_bin2_incl:50:0:2",
"sigma_bin3_incl:50:0:2",
"sigma_bin4_incl:50:0:2",
"sigma_bin5_incl:50:0:1",
"sigma_bin6_incl:50:0:1",
"sigma_bin7_incl:50:0:1",
"sigma_bin8_incl:50:0:1",
"sigma_bin9_incl:50:0:1",
"sigma_bin10_incl:50:0:1",
"sigma_bin11_incl:50:0:1",
"MuqqZZ0:50:0:2",
"MuqqZZ1:40:0:2",
"MuqqZZ2:40:0:2",
]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["statOnly", "allSys"]
inputDict["dimension"]          = "1D"
inputDict["doData"]             = [False,True]
inputDict["fileName"]           = ["workspaces/ws_pty_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_pty_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
#buildList(inputDict, scan1D)

##########
# ptptj
##########
inputDict["poiList"] = [
"sigma_bin0_incl:30:0:3",
"sigma_bin1_incl:30:0:2",
"sigma_bin2_incl:30:0:0.4",
"sigma_bin3_incl:30:0:0.5",
"sigma_bin4_incl:30:0:0.4",
"MuqqZZ0:40:0:2",
"MuqqZZ1:30:0:2",
"MuqqZZ2:30:0:2",
]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["statOnly", "allSys"]
inputDict["dimension"]          = "1D"
inputDict["doData"]             = [False,True]
inputDict["fileName"]           = ["workspaces/ws_ptptj_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_ptptj_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)

##########
# ptjm4lj
##########
inputDict["poiList"] = [
"sigma_bin0_incl:30:0:3",
"sigma_bin1_incl:30:0:1",
"sigma_bin2_incl:30:0:1",
"sigma_bin3_incl:30:0:1",
"sigma_bin4_incl:50:0:1",
"MuqqZZ0:50:0:2",
"MuqqZZ1:50:0:2",
]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["statOnly", "allSys"]
inputDict["dimension"]          = "1D"
inputDict["doData"]             = [False,True]
inputDict["fileName"]           = ["workspaces/ws_ptjm4lj_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_ptjm4lj_wsys_data_mtx_v23v3"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)


# #=============================================================================
# #						         Ranking	         					     =							
# #=============================================================================
ranking = []

inputDict = {}
inputDict["poiList"]            = ["(.*)"]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["allSys"]
inputDict["dimension"]          = "ranking"
inputDict["doData"]             = [False,True]
inputDict["extraVarSave"]       = ["MuqqZZ(.*)"]
# inputDict["fileName"]           = ["workspaces/ws_ptptj_wsys_data_mtx_v23v3.root", "workspaces/ws_ptjm4lj_wsys_data_mtx_v23v3.root", "workspaces/ws_ljptsjpt_wsys_data_mtx_v23v3.root", "workspaces/ws_y_wsys_data_mtx_v23v3.root"]
# inputDict["outputDir"]          = ["ws_ptptj_wsys_data_mtx_v23v3_ranking", "ws_ptjm4lj_wsys_data_mtx_v23v3_ranking", "ws_ljptsjpt_wsys_data_mtx_v23v3_ranking", "ws_y_wsys_data_mtx_v23v3_ranking"]

inputDict["fileName"]           = ["workspaces/ws_njet_inclusive_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_njet_inclusive_wsys_data_mtx_v23v3_ranking"]
inputDict["nSplitJob"]          = 10
buildList(inputDict, ranking)

inputDict = {}
inputDict["poiList"]            = ["(.*)"]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["allSys"]
inputDict["dimension"]          = "ranking"
inputDict["doData"]             = [False,True]
inputDict["extraVarSave"]       = ["MuqqZZ(.*)"]
inputDict["fileName"]           = ["workspaces/ws_mjj_wsys_data_mtx_v23v3.root","workspaces/ws_m12_wsys_data_mtx_v23v3.root","workspaces/ws_m34__wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_mjj_wsys_data_mtx_v23v3_ranking", "ws_m12_wsys_data_mtx_v23v3_ranking","ws_m34_wsys_data_mtx_v23v3_ranking"]
inputDict["nSplitJob"]          = 20
#buildList(inputDict, ranking)

inputDict = {}
inputDict["poiList"]            = ["(.*)"]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["allSys"]
inputDict["dimension"]          = "ranking"
inputDict["doData"]             = [False,True]
inputDict["extraVarSave"]       = ["MuqqZZ(.*)"]
inputDict["fileName"]           = ["workspaces/ws_cts_wsys_data_mtx_v23v3.root","workspaces/ws_ct1_wsys_data_mtx_v23v3.root","workspaces/ws_ct2__wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_cts_wsys_data_mtx_v23v3_ranking", "ws_ct1_wsys_data_mtx_v23v3_ranking","ws_ct2_wsys_data_mtx_v23v3_ranking"]
inputDict["nSplitJob"]          = 20
#buildList(inputDict, ranking)

inputDict = {}
inputDict["poiList"]            = ["(.*)"]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["allSys"]
inputDict["dimension"]          = "ranking"
inputDict["doData"]             = [False,True]
inputDict["extraVarSave"]       = ["MuqqZZ(.*)"]
inputDict["fileName"]           = ["workspaces/ws_nbjet_wsys_data_mtx_v23v3.root","workspaces/ws_etajj_wsys_data_mtx_v23v3.root","workspaces/ws_njet__wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_nbjet_wsys_data_mtx_v23v3_ranking", "ws_etajj_wsys_data_mtx_v23v3_ranking","ws_njet_wsys_data_mtx_v23v3_ranking"]
inputDict["nSplitJob"]          = 20
#buildList(inputDict, ranking)

inputDict = {}
inputDict["poiList"]            = ["(.*)"]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["allSys"]
inputDict["dimension"]          = "ranking"
inputDict["doData"]             = [False,True]
inputDict["extraVarSave"]       = ["MuqqZZ(.*)"]
inputDict["fileName"]           = ["workspaces/ws_ljpt_wsys_data_mtx_v23v3.root","workspaces/ws_sjpt_wsys_data_mtx_v23v3.root","workspaces/ws_mjj__wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_ljpt_wsys_data_mtx_v23v3_ranking", "ws_sjpt_wsys_data_mtx_v23v3_ranking","ws_mjj_wsys_data_mtx_v23v3_ranking"]
inputDict["nSplitJob"]          = 20
#buildList(inputDict, ranking)

inputDict = {}
inputDict["poiList"]            = ["(.*)"]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["allSys"]
inputDict["dimension"]          = "ranking"
inputDict["doData"]             = [False,True]
inputDict["extraVarSave"]       = ["MuqqZZ(.*)"]
inputDict["fileName"]           = ["workspaces/ws_phijj_wsys_data_mtx_v23v3.root","workspaces/ws_etajj_wsys_data_mtx_v23v3.root","workspaces/ws_ljptsjpt_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_phijj_wsys_data_mtx_v23v3_ranking", "ws_etajj_wsys_data_mtx_v23v3_ranking","ws_ljptsjpt_wsys_data_mtx_v23v3_ranking"]
inputDict["nSplitJob"]          = 20
#buildList(inputDict, ranking)



# inputDict = {}
# inputDict["poiList"] 		= ["(.*)"]
# inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
# inputDict["scanType"] 		= ["allSys"]
# inputDict["dimension"] 		= "ranking"
# inputDict["doData"] 		= [False, True]
# inputDict["extraVarSave"]       = ["MuqqZZ(.*)"]
# inputDict["fileName"] 		= ["workspaces/ws_pt_wsys_data_cf.root", "workspaces/ws_pt_wsys_data_mtx.root"]
# inputDict["outputDir"] 		= ["ws_pt_wsys_data_cf_ranking", "ws_pt_wsys_data_mtx_ranking"]
# inputDict["nSplitJob"]          = 20
# buildList(inputDict, ranking)

inputDict = {}
inputDict["poiList"] 		= ["(.*)"]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["allSys"]
inputDict["dimension"] 		= "ranking"
inputDict["doData"] 		= [False]
inputDict["extraVarSave"]   = ["MuqqZZ(.*)"]
#inputDict["fileName"]       = ["workspaces/ws_m_fidchannel_wsys_data_mtx_v23v3.root","workspaces/ws_m_fid2l2l_wsys_data_mtx_v23v3.root","workspaces/ws_m_fid4l_wsys_data_mtx_v23v3.root","workspaces/ws_m_fidsum_wsys_data_mtx_v23v3.root","workspaces/ws_m_total_wsys_data_mtx_v23v3.root", "workspaces/ws_m_comb_wsys_data_mtx_v23v3.root"]
#inputDict["outputDir"]      = ["ws_m_fidchannel_wsys_data_mtx_v23v3_ranking", "ws_m_fid2l2l_wsys_data_mtx_v23v3_ranking", "ws_m_fid4l_wsys_data_mtx_v23v3_ranking", "ws_m_fidsum_wsys_data_mtx_v23v3_ranking", "ws_m_total_wsys_data_mtx_v23v3_ranking", "ws_m_comb_wsys_data_mtx_v23v3_ranking"]
#inputDict["fileName"]       = ["workspaces/ws_pt4lj_wsys_data_mtx_v23v3.root", "workspaces/ws_pt4ljj_wsys_data_mtx_v23v3.root"]#, "workspaces/ws_m4ljj_wsys_data_mtx_v23v3.root"]
#inputDict["outputDir"]      = ["ws_pt4lj_wsys_data_mtx_v23v3_ranking","ws_pt4ljj_wsys_data_mtx_v23v3_ranking"]#, "ws_m4ljj_wsys_data_mtx_v23v3_ranking"]
inputDict["fileName"] = ["workspaces/ws_pt4ljj_wsys_expected_mtx_v23v3.root"]
inputDict["outputDir"]= ["ws_pt4ljj_wsys_expected_mtx_v23v3_ranking"]
inputDict["nSplitJob"]          = 20
#buildList(inputDict, ranking)




#=============================================================================
#								    correlationHist							 =							
#=============================================================================
correlationHist = []
inputDict = {}
inputDict["poiList"] 		= ["'*'"]
inputDict["config"] 		= "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"] 		= ["allSys"]
inputDict["dimension"] 		= "correlationHist"
inputDict["saveCorrHist"] 	= True   
inputDict["fileName"]       = ["workspaces/ws_m_fidchannel_wsys_data_mtx_v23v3.root","workspaces/ws_m_fid2l2l_wsys_data_mtx_v23v3.root","workspaces/ws_m_fid4l_wsys_data_mtx_v23v3.root", "workspaces/ws_m_fidsum_wsys_data_mtx_v23v3.root","workspaces/ws_m_total_wsys_data_mtx_v23v3.root", "workspaces/ws_m_comb_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]      = ["ws_m_fidchannel_wsys_data_mtx_v23v3_corr",           "ws_m_fid2l2l_wsys_data_mtx_v23v3_corr",           "ws_m_fid4l_wsys_data_mtx_v23v3_corr",            "ws_m_fidsum_wsys_data_mtx_v23v3_corr",          "ws_m_total_wsys_data_mtx_v23v3_corr", 			"ws_m_comb_wsys_data_mtx_v23v3_corr"]
#buildList(inputDict, correlationHist)

inputDict["poiList"]            = ["'*'"]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["allSys"]
inputDict["dimension"]          = "correlationHist"
inputDict["doData"]             = [True,False]
inputDict["saveCorrHist"]       = True   
inputDict["fileName"]           = ["workspaces/ws_mjj_wsys_data_mtx_v23v3.root", "workspaces/ws_nbjet_wsys_data_mtx_v23v3.root", "workspaces/ws_etajj_wsys_data_mtx_v23v3.root", "workspaces/ws_phijj_wsys_data_mtx_v23v3.root", "workspaces/ws_njet_wsys_data_mtx_v23v3.root", "workspaces/ws_m4lj_wsys_data_mtx_v23v3.root", "workspaces/ws_m4ljj_wsys_data_mtx_v23v3.root", "workspaces/ws_m12_wsys_data_mtx_v23v3.root", "workspaces/ws_m34_wsys_data_mtx_v23v3.root", "workspaces/ws_m12m34_wsys_data_mtx_v23v3.root","workspaces/ws_ljpt_wsys_data_mtx_v23v3.root", "workspaces/ws_ljptsjpt_wsys_data_mtx_v23v3.root", "workspaces/ws_cts_wsys_data_mtx_v23v3.root", "workspaces/ws_ct1_wsys_data_mtx_v23v3.root", "workspaces/ws_ct2_wsys_data_mtx_v23v3.root"]
inputDict["outputDir"]          = ["ws_mjj_wsys_data_mtx_v23v3_corr","ws_nbjet_wsys_data_mtx_v23v3_corr","ws_etajj_wsys_data_mtx_v23v3_corr", "ws_phijj_wsys_data_mtx_v23v3_corr", "ws_njet_wsys_data_mtx_v23v3_corr", "ws_m4lj_wsys_data_mtx_v23v3_corr", "ws_m4ljj_wsys_data_mtx_v23v3_corr", "ws_m12_wsys_data_mtx_v23v3_corr", "ws_m34_wsys_data_mtx_v23v3_corr", "ws_m12m34_wsys_data_mtx_v23v3_corr", "ws_ljpt_wsys_data_mtx_v23v3_corr", "ws_ljptsjpt_wsys_data_mtx_v23v3_corr", "ws_cts_wsys_data_mtx_v23v3_corr", "ws_ct1_wsys_data_mtx_v23v3_corr", "ws_ct2_wsys_data_mtx_v23v3_corr"]
# buildList(inputDict, correlationHist)

inputDict["poiList"]            = ["'*'"]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["allSys"]
inputDict["dimension"]          = "correlationHist"
inputDict["saveCorrHist"]       = True   
inputDict["fileName"] 		= ["workspaces/ws_m_comb_wsys_data.root", "workspaces/ws_m_fid2l2l_wsys_data.root", "workspaces/ws_m_fid4l_wsys_data.root", "workspaces/ws_m_fidchannel_wsys_data.root", "workspaces/ws_m_fidsum_wsys_data.root", "workspaces/ws_m_total_wsys_data.root"]
inputDict["outputDir"] 		= ["ws_m_comb_wsys_data_corr", "ws_m_fid2l2l_wsys_data_corr", "ws_m_fid4l_wsys_data_corr", "ws_m_fidchannel_wsys_data_corr", "ws_m_fidsum_wsys_data_corr", "ws_m_total_wsys_data_corr"]
#buildList(inputDict, correlationHist)



inputDict["poiList"]            = ["'*'"]
inputDict["config"]             = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]           = ["allSys"]
inputDict["dimension"]          = "correlationHist"
inputDict["saveCorrHist"]       = True   
inputDict["fileName"] 		= ["workspaces/ws_m12m34_wsys_data_mtx_sLChan_v23v3.root"]
inputDict["outputDir"] 		= ["ws_m12m34_wsys_data_mtx_sLChan_v23v3_corr"]
buildList(inputDict, correlationHist)

#=============================================================================
#                                   Compatibility                            =                          
#=============================================================================
compatibility = []
inputDict = {}
inputDict["poiList"]        = ["'*'"]
inputDict["config"]         = "${wscanner_data}/H4lDiff/H4lDiffConfig.txt"
inputDict["scanType"]       = ["singlePOI"]
inputDict["dimension"]      = "compatibility"
inputDict["doData"]         = [True, False]
inputDict["fileName"]       = ["workspaces/ws_m_fidchannel_wsys_data.root"]
inputDict["outputDir"]      = ["ws_m_fidchannel_wsys_data_compatibility"]
buildList(inputDict, compatibility)

