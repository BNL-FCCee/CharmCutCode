from buildList import *


scan1D = []
scan2D = []
inputDict = {}

#########################################################################
# Flags for 4l decay 1d and 2d scans

# Note 1: One may 'remove' a scan from being perfomed by commenting out
#  the 'buildList' line, e.g.     buildList(inputDict1, scan2D)

# Note 2: 2d observables, e.g. OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1 is
#  commented out below. To do 1d or 2d coupling scans, need to uncomment
#  in each section for the scans e.g. "if do4lcHBtilvscHWBtil:", etc.
#########################################################################

do4lcHBtilvscHWBtil = True
do4lcHBtilvscHWBtil = False
do4lcHWtilvscHBtil  = True
do4lcHWtilvscHBtil  = False
do4lcHWtilvscHWBtil = True
do4lcHWtilvscHWBtil = False

do4lcHWtil1D  = True
do4lcHWtil1D  = False
do4lcHBtil1D  = True
do4lcHBtil1D  = False
do4lcHWBtil1D = True
do4lcHWBtil1D = False
do4ldTilde1D = True
do4ldTilde1D = False
# higgs basis for 4l: (to be completed)
do4ltCaa1D  = True
do4ltCaa1D  = False
do4ltCza1D  = True
do4ltCza1D  = False
do4ltCzz1D  = True
do4ltCzz1D  = False



# If you setup your workspaces directory as "xxxxx_workspaces":
ws_prefix = ""
# ws_prefix = "h48bin_"

ws_postfix = ""
if do4ldTilde1D:
    ws_postfix = "_dTilde"


# 3d 4l workspace names:
ws_3d_4l_OO1_4l_tCzz1_name = ws_prefix + "workspaces/WS_combined_CP_v25_ggF_cHWtil_cHBtil_cHWBtil_OO1_4l_tCzz1_opt_v1_filtered" + ws_postfix + ".root"
ws_3d_4l_OO1_4l_tCza1_name = ws_prefix + "workspaces/WS_combined_CP_v25_ggF_cHWtil_cHBtil_cHWBtil_OO1_4l_tCza1_opt_v1_filtered" + ws_postfix + ".root"
ws_3d_4l_OO1_4l_tCaa1_name = ws_prefix + "workspaces/WS_combined_CP_v25_ggF_cHWtil_cHBtil_cHWBtil_OO1_4l_tCaa1_opt_v1_filtered" + ws_postfix + ".root"
ws_3d_4l_OO1_4l_cHWtil1_name = ws_prefix + "workspaces/WS_combined_CP_v25_ggF_cHWtil_cHBtil_cHWBtil_OO1_4l_cHWtil1_opt_v1_filtered" + ws_postfix + ".root"
ws_3d_4l_OO1_4l_cHBtil1_name = ws_prefix + "workspaces/WS_combined_CP_v25_ggF_cHWtil_cHBtil_cHWBtil_OO1_4l_cHBtil1_opt_v1_filtered" + ws_postfix + ".root"
ws_3d_4l_OO1_4l_cHWBtil1_name = ws_prefix + "workspaces/WS_combined_CP_v25_ggF_cHWtil_cHBtil_cHWBtil_OO1_4l_cHWBtil1_opt_v1_filtered" + ws_postfix + ".root"
ws_3d_4l_OO1_4l_dTilde1_name = ws_prefix + "workspaces/WS_combined_CP_v25_ggF_cHWtil_cHBtil_cHWBtil_OO1_4l_dTilde1_opt_v1_filtered" + ws_postfix + ".root"
ws_3d_4l_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1 = ws_prefix + "workspaces/WS_combined_CP_v25_ggF_cHWtil_cHBtil_cHWBtil_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1_opt_v1_filtered" + ws_postfix + ".root"
ws_3d_4l_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1 = ws_prefix + "workspaces/WS_combined_CP_v25_ggF_cHWtil_cHBtil_cHWBtil_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1_opt_v1_filtered" + ws_postfix + ".root"
ws_3d_4l_OO1_4l_cHBtil1_VS_OO1_4l_cHWBtil1 = ws_prefix + "workspaces/WS_combined_CP_v25_ggF_cHWtil_cHBtil_cHWBtil_OO1_4l_cHBtil1_VS_OO1_4l_cHWBtil1_opt_v1_filtered" + ws_postfix + ".root"


# 1d 4l/ggF scans: config and output files - add the floating POIs to config and output files for mu_qqZZ
# using 3d workspace

# *** MUST SET ONLY ONE OF THE THREE config/output file names ***

# just scanning axis
# floating_pois_4l = ""
# mu_qqZZ floating
floating_pois_4l = "_float_qqZZ"


#################################
# Flags for prod 1d and 2d scans
#################################

doProdtCzzvstCza = True
doProdtCzzvstCza = False
doProd1DtCzz     = True
doProd1DtCzz     = False
doProd1DtCza     = True
doProd1DtCza     = False
doProd1DcHWtil   = True
doProd1DcHWtil   = False

# If you setup your workspaces directory as "xxxxx_workspaces":
ws_prefix = ""
# ws_prefix = "test_"


# 2d jj/VBF workspace names:
ws_2d_jj_OO1_jj_tCzz1_name    = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_tCzz_tCza_OO1_jj_tCzz1_reco.root"
ws_2d_jj_OO1_jj_tCza1_name    = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_tCzz_tCza_OO1_jj_tCza1_reco.root"
ws_2d_jj_OO1_jj_tCaa1_name    = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_tCzz_tCza_OO1_jj_tCaa1_reco.root"
ws_2d_jj_OO1_jj_cHWtil1_name  = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_tCzz_tCza_OO1_jj_cHWtil1_reco.root"
ws_2d_jj_OO1_jj_cHBtil1_name  = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_tCzz_tCza_OO1_jj_cHBtil1_reco.root"
ws_2d_jj_OO1_jj_cHWBtil1_name = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_tCzz_tCza_OO1_jj_cHWBtil1_reco.root"
ws_2d_jj_dijet_deltaphi_name  = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_tCzz_tCza_dijet_deltaphi_reco.root"
# Test for 2d observable, used below in 2d prod scan
# ws_2d_jj_OO1_jj_tCzz1_VS_OO1_jj_tCza1_name    = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_tCzz_tCza_OO1_jj_tCzz1_VS_OO1_jj_tCza1_reco.root"

# 1d jj/VBF workspace names:
ws_1d_jj_OO1_jj_tCzz1_name    = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_cHWtil_OO1_jj_tCzz1_reco.root"
ws_1d_jj_OO1_jj_tCza1_name    = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_cHWtil_OO1_jj_tCza1_reco.root"
ws_1d_jj_OO1_jj_tCaa1_name    = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_cHWtil_OO1_jj_tCaa1_reco.root"
ws_1d_jj_OO1_jj_cHWtil1_name  = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_cHWtil_OO1_jj_cHWtil1_reco.root"
ws_1d_jj_OO1_jj_cHBtil1_name  = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_cHWtil_OO1_jj_cHBtil1_reco.root"
ws_1d_jj_OO1_jj_cHWBtil1_name = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_cHWtil_OO1_jj_cHWBtil1_reco.root"
ws_1d_jj_dijet_deltaphi_name  = ws_prefix + "workspaces/WS_combined_CP_v25_VBF_cHWtil_dijet_deltaphi_reco.root"

# using 2d workspace

# *** MUST SET ONLY ONE OF THE THREE config/output file names ***

# just scanning axis
# floating_pois_jj = ""
# mu_qqZZ floating
# floating_pois_jj = "_float_qqZZ"
# mu_qqZZ and mu_ggF floating
floating_pois_jj = "_float_qqZZ_ggF"



###############################################################################
# Cannot do more than one scan at a time - exit if more than one is requested
###############################################################################

nscan = 0
if do4lcHBtilvscHWBtil:
    nscan += 1
if do4lcHWtilvscHBtil:
    nscan += 1
if do4lcHWtilvscHWBtil:
    nscan += 1
if do4lcHWtil1D:
    nscan += 1
if do4lcHBtil1D:
    nscan += 1
if do4lcHWBtil1D:
    nscan += 1
if do4ldTilde1D:
    nscan += 1
if do4ltCaa1D:
    nscan += 1
if do4ltCza1D:
    nscan += 1
if do4ltCzz1D:
    nscan += 1
if doProdtCzzvstCza:
    nscan += 1
if doProd1DtCzz:
    nscan += 1
if doProd1DtCza:
    nscan += 1
if doProd1DcHWtil:
    nscan += 1

    
if nscan > 1:
    print("Cannot do more than ONE type of scan:")
    print("4l cHBtil vs cHWBtil: {}".format(do4lcHBtilvscHWBtil))
    print("4l cHWtil vs cHBtil : {}".format(do4lcHWtilvscHBtil))
    print("4l cHWtil vs cHWBtil: {}".format(do4lcHWtilvscHWBtil))
    print("4l cHWtil  1D:{}".format(do4lcHWtil1D))
    print("4l cHBtil  1D:{}".format(do4lcHBtil1D))
    print("4l cHWBtil 1D:{}".format(do4lcHWBtil1D))
    print("4l dTilde 1D:{}".format(do4ldTilde1D))
    print("4l tCaa  1D:{}".format(do4ltCaa1D))
    print("4l tCza  1D:{}".format(do4ltCza1D))
    print("4l tCzz  1D:{}".format(do4ltCzz1D))
    print("prod tCzz vs tCza:{}".format(doProdtCzzvstCza))
    print("prod 1D tCzz:{}".format(doProd1DtCzz))
    print("prod 1D tCza:{}".format(doProd1DtCza))
    print("prod 1D cHtil:{}".format(doProd1DcHWtil))
    exit(1)

    
#=======================
# 4l cHBtil vs cHWBtil
#=======================

# OO1_4l 2D:

# 3d
if do4lcHBtilvscHWBtil:
    inputDict1 = {}
    inputDict1["poiList"] = [
        # "cHBtil:50:-4.0:4.0,cHWBtil:50:-6.0:6.0",
        "cHBtil:50:-8.0:8.0,cHWBtil:50:-12.0:12.0",
        ]
    inputDict1["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil_cHWBtil"  + floating_pois_4l + ".txt"
    inputDict1["scanType"]       = ["statOnly"]
    inputDict1["dimension"]      = "2D"
    inputDict1["doData"]         = [False]
    inputDict1["fileName"]       = [ws_3d_4l_OO1_4l_tCzz1_name]
    inputDict1["outputDir"]      = ["ws_scan_v25_ggF_cHBtil_cHWBtil_fixed_cHWtil" + floating_pois_4l + "_OO1_4l_tCzz1_reco"]
    buildList(inputDict1, scan2D)
    
    inputDict2 = {}
    inputDict2["poiList"] = [
        # "cHBtil:50:-4.0:4.0,cHWBtil:50:-6.0:6.0",
        # "cHBtil:50:-8.0:8.0,cHWBtil:50:-12.0:12.0",
        "cHBtil:50:-4.0:4.0,cHWBtil:50:-7.0:7.0",
        ]
    inputDict2["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict2["scanType"]       = ["statOnly"]
    inputDict2["dimension"]      = "2D"
    inputDict2["doData"]         = [False]
    inputDict2["fileName"]       = [ws_3d_4l_OO1_4l_tCza1_name]
    inputDict2["outputDir"]      = ["ws_scan_v25_ggF_cHBtil_cHWBtil_fixed_cHWtil" + floating_pois_4l + "_OO1_4l_tCza1_reco"]
    buildList(inputDict2, scan2D)
    
    inputDict3 = {}
    inputDict3["poiList"] = [
        # "cHBtil:50:-4.0:4.0,cHWBtil:50:-6.0:6.0",
        # "cHBtil:50:-8.0:8.0,cHWBtil:50:-12.0:12.0",
        "cHBtil:50:-5.0:5.0,cHWBtil:50:-8.0:8.0",
        ]
    inputDict3["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict3["scanType"]       = ["statOnly"]
    inputDict3["dimension"]      = "2D"
    inputDict3["doData"]         = [False]
    inputDict3["fileName"]       = [ws_3d_4l_OO1_4l_tCaa1_name]
    inputDict3["outputDir"]      = ["ws_scan_v25_ggF_cHBtil_cHWBtil_fixed_cHWtil" + floating_pois_4l + "_OO1_4l_tCaa1_reco"]
    buildList(inputDict3, scan2D)
    
    inputDict4 = {}
    inputDict4["poiList"] = [
        "cHBtil:50:-3.0:3.0,cHWBtil:50:-4.0:4.0",
        # "cHBtil:50:-4.0:4.0,cHWBtil:50:-6.0:6.0",
        # "cHBtil:50:-8.0:8.0,cHWBtil:50:-12.0:12.0",
        ]
    inputDict4["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict4["scanType"]       = ["statOnly"]
    inputDict4["dimension"]      = "2D"
    inputDict4["doData"]         = [False]
    inputDict4["fileName"]       = [ws_3d_4l_OO1_4l_cHWtil1_name]
    inputDict4["outputDir"]      = ["ws_scan_v25_ggF_cHBtil_cHWBtil_fixed_cHWtil" + floating_pois_4l + "_OO1_4l_cHWtil1_reco"]
    buildList(inputDict4, scan2D)
    
    inputDict5 = {}
    inputDict5["poiList"] = [
        "cHBtil:50:-3.0:3.0,cHWBtil:50:-4.0:4.0",
        # "cHBtil:50:-4.0:4.0,cHWBtil:50:-6.0:6.0",
        # "cHBtil:50:-8.0:8.0,cHWBtil:50:-12.0:12.0",
        ]
    inputDict5["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict5["scanType"]       = ["statOnly"]
    inputDict5["dimension"]      = "2D"
    inputDict5["doData"]         = [False]
    inputDict5["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_name]
    inputDict5["outputDir"]      = ["ws_scan_v25_ggF_cHBtil_cHWBtil_fixed_cHWtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_reco"]
    buildList(inputDict5, scan2D)
    
    
    inputDict6 = {}
    inputDict6["poiList"] = [
        "cHBtil:50:-3.0:3.0,cHWBtil:50:-4.0:4.0",
        # "cHBtil:50:-4.0:4.0,cHWBtil:50:-6.0:6.0",
        # "cHBtil:50:-8.0:8.0,cHWBtil:50:-12.0:12.0",
        ]
    inputDict6["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict6["scanType"]       = ["statOnly"]
    inputDict6["dimension"]      = "2D"
    inputDict6["doData"]         = [False]
    inputDict6["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_name]
    inputDict6["outputDir"]      = ["ws_scan_v25_ggF_cHBtil_cHWBtil_fixed_cHWtil" + floating_pois_4l + "_OO1_4l_cHBtil1_reco"]
    buildList(inputDict6, scan2D)

    inputDict7 = {}
    inputDict7["poiList"] = [
        "cHBtil:50:-3.0:3.0,cHWBtil:50:-4.0:4.0",
        # "cHBtil:50:-4.0:4.0,cHWBtil:50:-6.0:6.0",
        # "cHBtil:50:-8.0:8.0,cHWBtil:50:-12.0:12.0",
        ]
    inputDict7["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict7["scanType"]       = ["statOnly"]
    inputDict7["dimension"]      = "2D"
    inputDict7["doData"]         = [False]
    inputDict7["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1]
    inputDict7["outputDir"]      = ["ws_scan_v25_ggF_cHBtil_cHWBtil_fixed_cHWtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1_reco"]
    buildList(inputDict7, scan2D)
    
    # inputDict8 = {}
    # inputDict8["poiList"] = [
    #     "cHBtil:50:-3.0:3.0,cHWBtil:50:-4.0:4.0",
    #     # "cHBtil:50:-4.0:4.0,cHWBtil:50:-6.0:6.0",
    #     # "cHBtil:50:-8.0:8.0,cHWBtil:50:-12.0:12.0",
    #     ]
    # inputDict8["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil_cHWBtil" + floating_pois_4l + ".txt"
    # inputDict8["scanType"]       = ["statOnly"]
    # inputDict8["dimension"]      = "2D"
    # inputDict8["doData"]         = [False]
    # inputDict8["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1]
    # inputDict8["outputDir"]      = ["ws_scan_v25_ggF_cHBtil_cHWBtil_fixed_cHWtil" + floating_pois_4l + "_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1_reco"]
    # buildList(inputDict8, scan2D)
    
    # inputDict9 = {}
    # inputDict9["poiList"] = [
    #     "cHBtil:50:-3.0:3.0,cHWBtil:50:-4.0:4.0",
    #     # "cHBtil:50:-4.0:4.0,cHWBtil:50:-6.0:6.0",
    #     # "cHBtil:50:-8.0:8.0,cHWBtil:50:-12.0:12.0",
    #     ]
    # inputDict9["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil_cHWBtil" + floating_pois_4l + ".txt"
    # inputDict9["scanType"]       = ["statOnly"]
    # inputDict9["dimension"]      = "2D"
    # inputDict9["doData"]         = [False]
    # inputDict9["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_VS_OO1_4l_cHWBtil1]
    # inputDict9["outputDir"]      = ["ws_scan_v25_ggF_cHBtil_cHWBtil_fixed_cHWtil" + floating_pois_4l + "_OO1_4l_cHBtil1_VS_OO1_4l_cHWBtil1_reco"]
    # buildList(inputDict9, scan2D)
    


#=====================
# 4l cHWtil vs cHBtil
#=====================

if do4lcHWtilvscHBtil:

    inputDict1 = {}
    inputDict1["poiList"] = [
        "cHWtil:50:-6.0:6.0,cHBtil:50:-4.0:4.0",
        ]
    inputDict1["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHBtil" + floating_pois_4l + ".txt"
    inputDict1["scanType"]       = ["statOnly"]
    inputDict1["dimension"]      = "2D"
    inputDict1["doData"]         = [False]
    inputDict1["fileName"]       = [ws_3d_4l_OO1_4l_tCzz1_name]
    inputDict1["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHBtil_fixed_cHWBtil" + floating_pois_4l + "_OO1_4l_tCzz1_reco"]
    buildList(inputDict1, scan2D)
    
    inputDict2 = {}
    inputDict2["poiList"] = [
        "cHWtil:50:-6.0:6.0,cHBtil:50:-4.0:4.0",
        ]
    inputDict2["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHBtil" + floating_pois_4l + ".txt"
    inputDict2["scanType"]       = ["statOnly"]
    inputDict2["dimension"]      = "2D"
    inputDict2["doData"]         = [False]
    inputDict2["fileName"]       = [ws_3d_4l_OO1_4l_tCza1_name]
    inputDict2["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHBtil_fixed_cHWBtil" + floating_pois_4l + "_OO1_4l_tCza1_reco"]
    buildList(inputDict2, scan2D)
    
    inputDict3 = {}
    inputDict3["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHBtil:50:-4.0:4.0",
        "cHWtil:50:-7.0:7.0,cHBtil:50:-3.0:3.0",
        ]
    inputDict3["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHBtil" + floating_pois_4l + ".txt"
    inputDict3["scanType"]       = ["statOnly"]
    inputDict3["dimension"]      = "2D"
    inputDict3["doData"]         = [False]
    inputDict3["fileName"]       = [ws_3d_4l_OO1_4l_tCaa1_name]
    inputDict3["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHBtil_fixed_cHWBtil" + floating_pois_4l + "_OO1_4l_tCaa1_reco"]
    buildList(inputDict3, scan2D)
    
    inputDict4 = {}
    inputDict4["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHBtil:50:-4.0:4.0",
        "cHWtil:50:-5.0:5.0,cHBtil:50:-2.0:2.0",
        ]
    inputDict4["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHBtil" + floating_pois_4l + ".txt"
    inputDict4["scanType"]       = ["statOnly"]
    inputDict4["dimension"]      = "2D"
    inputDict4["doData"]         = [False]
    inputDict4["fileName"]       = [ws_3d_4l_OO1_4l_cHWtil1_name]
    inputDict4["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHBtil_fixed_cHWBtil" + floating_pois_4l + "_OO1_4l_cHWtil1_reco"]
    buildList(inputDict4, scan2D)
    
    inputDict5 = {}
    inputDict5["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHBtil:50:-4.0:4.0",
        "cHWtil:50:-5.0:5.0,cHBtil:50:-2.0:2.0",
        ]
    inputDict5["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHBtil" + floating_pois_4l + ".txt"
    inputDict5["scanType"]       = ["statOnly"]
    inputDict5["dimension"]      = "2D"
    inputDict5["doData"]         = [False]
    inputDict5["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_name]
    inputDict5["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHBtil_fixed_cHWBtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_reco"]
    buildList(inputDict5, scan2D)
    
    inputDict6 = {}
    inputDict6["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHBtil:50:-4.0:4.0",
        "cHWtil:50:-5.0:5.0,cHBtil:50:-2.0:2.0",
        ]
    inputDict6["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHBtil" + floating_pois_4l + ".txt"
    inputDict6["scanType"]       = ["statOnly"]
    inputDict6["dimension"]      = "2D"
    inputDict6["doData"]         = [False]
    inputDict6["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_name]
    inputDict6["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHBtil_fixed_cHWBtil" + floating_pois_4l + "_OO1_4l_cHBtil1_reco"]
    buildList(inputDict6, scan2D)


    inputDict7 = {}
    inputDict7["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHBtil:50:-4.0:4.0",
        "cHWtil:50:-5.0:5.0,cHBtil:50:-2.0:2.0",
        ]
    inputDict7["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHBtil" + floating_pois_4l + ".txt"
    inputDict7["scanType"]       = ["statOnly"]
    inputDict7["dimension"]      = "2D"
    inputDict7["doData"]         = [False]
    inputDict7["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1]
    inputDict7["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHBtil_fixed_cHWBtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1_reco"]
    buildList(inputDict7, scan2D)
    
    # inputDict8 = {}
    # inputDict8["poiList"] = [
    #     # "cHWtil:50:-6.0:6.0,cHBtil:50:-4.0:4.0",
    #     "cHWtil:50:-5.0:5.0,cHBtil:50:-2.0:2.0",
    #     ]
    # inputDict8["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHBtil" + floating_pois_4l + ".txt"
    # inputDict8["scanType"]       = ["statOnly"]
    # inputDict8["dimension"]      = "2D"
    # inputDict8["doData"]         = [False]
    # inputDict8["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1]
    # inputDict8["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHBtil_fixed_cHWBtil" + floating_pois_4l + "_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1_reco"]
    # buildList(inputDict8, scan2D)
    
    # inputDict9 = {}
    # inputDict9["poiList"] = [
    #     # "cHWtil:50:-6.0:6.0,cHBtil:50:-4.0:4.0",
    #     "cHWtil:50:-5.0:5.0,cHBtil:50:-2.0:2.0",
    #     ]
    # inputDict9["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHBtil" + floating_pois_4l + ".txt"
    # inputDict9["scanType"]       = ["statOnly"]
    # inputDict9["dimension"]      = "2D"
    # inputDict9["doData"]         = [False]
    # inputDict9["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_VS_OO1_4l_cHWBtil1]
    # inputDict9["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHBtil_fixed_cHWBtil" + floating_pois_4l + "_OO1_4l_cHBtil1_VS_OO1_4l_cHWBtil1_reco"]
    # buildList(inputDict9, scan2D)
    
    
    
    

#=====================
# 4l cHWtil vs cHWBtil
#=====================

if do4lcHWtilvscHWBtil:
    inputDict1 = {}
    inputDict1["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHWBtil:50:-6.0:6.0",
        # "cHWtil:50:-8.0:8.0,cHWBtil:50:-6.0:6.0",
        "cHWtil:50:-10.0:10.0,cHWBtil:50:-8.0:8.0",
        ]
    inputDict1["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict1["scanType"]       = ["statOnly"]
    inputDict1["dimension"]      = "2D"
    inputDict1["doData"]         = [False]
    inputDict1["fileName"]       = [ws_3d_4l_OO1_4l_tCzz1_name]
    inputDict1["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHWBtil_fixed_cHBtil" + floating_pois_4l + "_OO1_4l_tCzz1_reco"]
    buildList(inputDict1, scan2D)
    
    inputDict2 = {}
    inputDict2["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHWBtil:50:-6.0:6.0",
        # "cHWtil:50:-8.0:8.0,cHWBtil:50:-6.0:6.0",
        "cHWtil:50:-10.0:10.0,cHWBtil:50:-8.0:8.0",
        ]
    inputDict2["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict2["scanType"]       = ["statOnly"]
    inputDict2["dimension"]      = "2D"
    inputDict2["doData"]         = [False]
    inputDict2["fileName"]       = [ws_3d_4l_OO1_4l_tCza1_name]
    inputDict2["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHWBtil_fixed_cHBtil" + floating_pois_4l + "_OO1_4l_tCza1_reco"]
    buildList(inputDict2, scan2D)
    
    inputDict3 = {}
    inputDict3["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHWBtil:50:-6.0:6.0",
        # "cHWtil:50:-8.0:8.0,cHWBtil:50:-6.0:6.0",
        "cHWtil:50:-10.0:10.0,cHWBtil:50:-8.0:8.0",
        ]
    inputDict3["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict3["scanType"]       = ["statOnly"]
    inputDict3["dimension"]      = "2D"
    inputDict3["doData"]         = [False]
    inputDict3["fileName"]       = [ws_3d_4l_OO1_4l_tCaa1_name]
    inputDict3["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHWBtil_fixed_cHBtil" + floating_pois_4l + "_OO1_4l_tCaa1_reco"]
    buildList(inputDict3, scan2D)
    
    inputDict4 = {}
    inputDict4["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHWBtil:50:-6.0:6.0",
        # "cHWtil:50:-8.0:8.0,cHWBtil:50:-6.0:6.0",
        "cHWtil:50:-8.0:8.0,cHWBtil:50:-5.0:5.0",
        ]
    inputDict4["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict4["scanType"]       = ["statOnly"]
    inputDict4["dimension"]      = "2D"
    inputDict4["doData"]         = [False]
    inputDict4["fileName"]       = [ws_3d_4l_OO1_4l_cHWtil1_name]
    inputDict4["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHWBtil_fixed_cHBtil" + floating_pois_4l + "_OO1_4l_cHWtil1_reco"]
    buildList(inputDict4, scan2D)
    
    inputDict5 = {}
    inputDict5["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHWBtil:50:-6.0:6.0",
        # "cHWtil:50:-8.0:8.0,cHWBtil:50:-6.0:6.0",
        "cHWtil:50:-8.0:8.0,cHWBtil:50:-5.0:5.0",
        ]
    inputDict5["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict5["scanType"]       = ["statOnly"]
    inputDict5["dimension"]      = "2D"
    inputDict5["doData"]         = [False]
    inputDict5["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_name]
    inputDict5["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHWBtil_fixed_cHBtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_reco"]
    buildList(inputDict5, scan2D)
    
    inputDict6 = {}
    inputDict6["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHWBtil:50:-6.0:6.0",
        # "cHWtil:50:-8.0:8.0,cHWBtil:50:-6.0:6.0",
        "cHWtil:50:-8.0:8.0,cHWBtil:50:-5.0:5.0",
        ]
    inputDict6["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict6["scanType"]       = ["statOnly"]
    inputDict6["dimension"]      = "2D"
    inputDict6["doData"]         = [False]
    inputDict6["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_name]
    inputDict6["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHWBtil_fixed_cHBtil" + floating_pois_4l + "_OO1_4l_cHBtil1_reco"]
    buildList(inputDict6, scan2D)

    inputDict7 = {}
    inputDict7["poiList"] = [
        # "cHWtil:50:-6.0:6.0,cHWBtil:50:-6.0:6.0",
        # "cHWtil:50:-8.0:8.0,cHWBtil:50:-6.0:6.0",
        "cHWtil:50:-8.0:8.0,cHWBtil:50:-5.0:5.0",
        ]
    inputDict7["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHWBtil" + floating_pois_4l + ".txt"
    inputDict7["scanType"]       = ["statOnly"]
    inputDict7["dimension"]      = "2D"
    inputDict7["doData"]         = [False]
    inputDict7["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1]
    inputDict7["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHWBtil_fixed_cHBtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1_reco"]
    buildList(inputDict7, scan2D)
    
    # inputDict8 = {}
    # inputDict8["poiList"] = [
    #     # "cHWtil:50:-6.0:6.0,cHWBtil:50:-6.0:6.0",
    #     # "cHWtil:50:-8.0:8.0,cHWBtil:50:-6.0:6.0",
    #     "cHWtil:50:-8.0:8.0,cHWBtil:50:-5.0:5.0",
    #     ]
    # inputDict8["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHWBtil" + floating_pois_4l + ".txt"
    # inputDict8["scanType"]       = ["statOnly"]
    # inputDict8["dimension"]      = "2D"
    # inputDict8["doData"]         = [False]
    # inputDict8["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1]
    # inputDict8["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHWBtil_fixed_cHBtil" + floating_pois_4l + "_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1_reco"]
    # buildList(inputDict8, scan2D)
    
    # inputDict9 = {}
    # inputDict9["poiList"] = [
    #     # "cHWtil:50:-6.0:6.0,cHWBtil:50:-6.0:6.0",
    #     # "cHWtil:50:-8.0:8.0,cHWBtil:50:-6.0:6.0",
    #     "cHWtil:50:-8.0:8.0,cHWBtil:50:-5.0:5.0",
    #     ]
    # inputDict9["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil_cHWBtil" + floating_pois_4l + ".txt"
    # inputDict9["scanType"]       = ["statOnly"]
    # inputDict9["dimension"]      = "2D"
    # inputDict9["doData"]         = [False]
    # inputDict9["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_VS_OO1_4l_cHWBtil1]
    # inputDict9["outputDir"]      = ["ws_scan_v25_ggF_cHWtil_cHWBtil_fixed_cHBtil" + floating_pois_4l + "_OO1_4l_cHBtil1_VS_OO1_4l_cHWBtil1_reco"]
    # buildList(inputDict9, scan2D)
    
#===========
# 4l cHWtil
#===========

if do4lcHWtil1D:

    inputDict1 = {}
    inputDict1["poiList"] = [
        "cHWtil:200:-4.0:4.0",
    ]
    inputDict1["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_4l + ".txt"
    inputDict1["scanType"]       = ["statOnly"]
    inputDict1["dimension"]      = "1D"
    inputDict1["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict1["fileName"]       = [ws_3d_4l_OO1_4l_tCzz1_name]
    inputDict1["outputDir"]      = ["ws_scan_v25_4l_cHWtil" + floating_pois_4l + "_OO1_4l_tCzz1_reco"]
    buildList(inputDict1, scan1D)

    inputDict2 = {}
    inputDict2["poiList"] = [
        "cHWtil:200:-4.0:4.0",
    ]
    inputDict2["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_4l + ".txt"
    inputDict2["scanType"]       = ["statOnly"]
    inputDict2["dimension"]      = "1D"
    inputDict2["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict2["fileName"]       = [ws_3d_4l_OO1_4l_tCza1_name]
    inputDict2["outputDir"]      = ["ws_scan_v25_4l_cHWtil" + floating_pois_4l + "_OO1_4l_tCza1_reco"]
    buildList(inputDict2, scan1D)

    inputDict3 = {}
    inputDict3["poiList"] = [
        "cHWtil:200:-4.0:4.0",
    ]
    inputDict3["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_4l + ".txt"
    inputDict3["scanType"]       = ["statOnly"]
    inputDict3["dimension"]      = "1D"
    inputDict3["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict3["fileName"]       = [ws_3d_4l_OO1_4l_tCaa1_name]
    inputDict3["outputDir"]      = ["ws_scan_v25_4l_cHWtil" + floating_pois_4l + "_OO1_4l_tCaa1_reco"]
    buildList(inputDict3, scan1D)

    inputDict4 = {}
    inputDict4["poiList"] = [
        "cHWtil:200:-4.0:4.0",
    ]
    inputDict4["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_4l + ".txt"
    inputDict4["scanType"]       = ["statOnly"]
    inputDict4["dimension"]      = "1D"
    inputDict4["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict4["fileName"]       = [ws_3d_4l_OO1_4l_cHWtil1_name]
    inputDict4["outputDir"]      = ["ws_scan_v25_4l_cHWtil" + floating_pois_4l + "_OO1_4l_cHWtil1_reco"]
    buildList(inputDict4, scan1D)

    inputDict5 = {}
    inputDict5["poiList"] = [
        "cHWtil:200:-4.0:4.0",
    ]
    inputDict5["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_4l + ".txt"
    inputDict5["scanType"]       = ["statOnly"]
    inputDict5["dimension"]      = "1D"
    inputDict5["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict5["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_name]
    inputDict5["outputDir"]      = ["ws_scan_v25_4l_cHWtil" + floating_pois_4l + "_OO1_4l_cHBtil1_reco"]
    buildList(inputDict5, scan1D)

    inputDict6 = {}
    inputDict6["poiList"] = [
        "cHWtil:200:-4.0:4.0",
    ]
    inputDict6["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_4l + ".txt"
    inputDict6["scanType"]       = ["statOnly"]
    inputDict6["dimension"]      = "1D"
    inputDict6["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict6["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_name]
    inputDict6["outputDir"]      = ["ws_scan_v25_4l_cHWtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_reco"]
    buildList(inputDict6, scan1D)

    inputDict7 = {}
    inputDict7["poiList"] = [
        "cHWtil:200:-4.0:4.0",
    ]
    inputDict7["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_4l + ".txt"
    inputDict7["scanType"]       = ["statOnly"]
    inputDict7["dimension"]      = "1D"
    inputDict7["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict7["fileName"]       = [ws_3d_4l_OO1_4l_dTilde1_name]
    inputDict7["outputDir"]      = ["ws_scan_v25_4l_cHWtil" + floating_pois_4l + "_OO1_4l_dTilde1_reco"]
    buildList(inputDict7, scan1D)

    inputDict8 = {}
    inputDict8["poiList"] = [
        "cHWtil:200:-4.0:4.0",
    ]
    inputDict8["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_4l + ".txt"
    inputDict8["scanType"]       = ["statOnly"]
    inputDict8["dimension"]      = "1D"
    inputDict8["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict8["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1]
    inputDict8["outputDir"]      = ["ws_scan_v25_4l_cHWtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1_reco"]
    buildList(inputDict8, scan1D)

    # inputDict9 = {}
    # inputDict9["poiList"] = [
    #     "cHWtil:200:-4.0:4.0",
    # ]
    # inputDict9["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_4l + ".txt"
    # inputDict9["scanType"]       = ["statOnly"]
    # inputDict9["dimension"]      = "1D"
    # inputDict9["doData"]         = [False]
    # # OO1_4l 1D: 
    # # Use 3d reco workspace
    # inputDict9["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1]
    # inputDict9["outputDir"]      = ["ws_scan_v25_4l_cHWtil" + floating_pois_4l + "_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1_reco"]
    # buildList(inputDict9, scan1D)


#===========
# 4l cHBtil
#===========

if do4lcHBtil1D:

    inputDict1 = {}
    inputDict1["poiList"] = [
        "cHBtil:200:-2.0:2.0",
    ]
    inputDict1["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil" + floating_pois_4l + ".txt"
    inputDict1["scanType"]       = ["statOnly"]
    inputDict1["dimension"]      = "1D"
    inputDict1["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict1["fileName"]       = [ws_3d_4l_OO1_4l_tCzz1_name]
    inputDict1["outputDir"]      = ["ws_scan_v25_4l_cHBtil" + floating_pois_4l + "_OO1_4l_tCzz1_reco"]
    buildList(inputDict1, scan1D)

    inputDict2 = {}
    inputDict2["poiList"] = [
        "cHBtil:200:-2.0:2.0",
    ]
    inputDict2["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil" + floating_pois_4l + ".txt"
    inputDict2["scanType"]       = ["statOnly"]
    inputDict2["dimension"]      = "1D"
    inputDict2["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict2["fileName"]       = [ws_3d_4l_OO1_4l_tCza1_name]
    inputDict2["outputDir"]      = ["ws_scan_v25_4l_cHBtil" + floating_pois_4l + "_OO1_4l_tCza1_reco"]
    buildList(inputDict2, scan1D)

    inputDict3 = {}
    inputDict3["poiList"] = [
        "cHBtil:200:-2.0:2.0",
    ]
    inputDict3["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil" + floating_pois_4l + ".txt"
    inputDict3["scanType"]       = ["statOnly"]
    inputDict3["dimension"]      = "1D"
    inputDict3["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict3["fileName"]       = [ws_3d_4l_OO1_4l_tCaa1_name]
    inputDict3["outputDir"]      = ["ws_scan_v25_4l_cHBtil" + floating_pois_4l + "_OO1_4l_tCaa1_reco"]
    buildList(inputDict3, scan1D)

    inputDict4 = {}
    inputDict4["poiList"] = [
        "cHBtil:200:-2.0:2.0",
    ]
    inputDict4["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil" + floating_pois_4l + ".txt"
    inputDict4["scanType"]       = ["statOnly"]
    inputDict4["dimension"]      = "1D"
    inputDict4["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict4["fileName"]       = [ws_3d_4l_OO1_4l_cHWtil1_name]
    inputDict4["outputDir"]      = ["ws_scan_v25_4l_cHBtil" + floating_pois_4l + "_OO1_4l_cHWtil1_reco"]
    buildList(inputDict4, scan1D)

    inputDict5 = {}
    inputDict5["poiList"] = [
        "cHBtil:200:-2.0:2.0",
    ]
    inputDict5["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil" + floating_pois_4l + ".txt"
    inputDict5["scanType"]       = ["statOnly"]
    inputDict5["dimension"]      = "1D"
    inputDict5["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict5["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_name]
    inputDict5["outputDir"]      = ["ws_scan_v25_4l_cHBtil" + floating_pois_4l + "_OO1_4l_cHBtil1_reco"]
    buildList(inputDict5, scan1D)

    inputDict6 = {}
    inputDict6["poiList"] = [
        "cHBtil:200:-2.0:2.0",
    ]
    inputDict6["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil" + floating_pois_4l + ".txt"
    inputDict6["scanType"]       = ["statOnly"]
    inputDict6["dimension"]      = "1D"
    inputDict6["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict6["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_name]
    inputDict6["outputDir"]      = ["ws_scan_v25_4l_cHBtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_reco"]
    buildList(inputDict6, scan1D)

    inputDict7 = {}
    inputDict7["poiList"] = [
        "cHBtil:200:-2.0:2.0",
    ]
    inputDict7["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil" + floating_pois_4l + ".txt"
    inputDict7["scanType"]       = ["statOnly"]
    inputDict7["dimension"]      = "1D"
    inputDict7["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict7["fileName"]       = [ws_3d_4l_OO1_4l_dTilde1_name]
    inputDict7["outputDir"]      = ["ws_scan_v25_4l_cHBtil" + floating_pois_4l + "_OO1_4l_dTilde1_reco"]
    buildList(inputDict7, scan1D)

    # inputDict8 = {}
    # inputDict8["poiList"] = [
    #     "cHBtil:200:-2.0:2.0",
    # ]
    # inputDict8["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil" + floating_pois_4l + ".txt"
    # inputDict8["scanType"]       = ["statOnly"]
    # inputDict8["dimension"]      = "1D"
    # inputDict8["doData"]         = [False]
    # # OO1_4l 1D: 
    # # Use 3d reco workspace
    # inputDict8["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1]
    # inputDict8["outputDir"]      = ["ws_scan_v25_4l_cHBtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1_reco"]
    # buildList(inputDict8, scan1D)

    # inputDict9 = {}
    # inputDict9["poiList"] = [
    #     "cHBtil:200:-2.0:2.0",
    # ]
    # inputDict9["config"]         = "wscanner/H4lCP/allAxesConfig_cHBtil" + floating_pois_4l + ".txt"
    # inputDict9["scanType"]       = ["statOnly"]
    # inputDict9["dimension"]      = "1D"
    # inputDict9["doData"]         = [False]
    # # OO1_4l 1D: 
    # # Use 3d reco workspace
    # inputDict9["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1]
    # inputDict9["outputDir"]      = ["ws_scan_v25_4l_cHBtil" + floating_pois_4l + "_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1_reco"]
    # buildList(inputDict9, scan1D)



    
#===========
# 4l cHWBtil
#===========

if do4lcHWBtil1D:
    inputDict1 = {}
    inputDict1["poiList"] = [
        "cHWBtil:200:-3.5:3.5",
    ]
    inputDict1["config"]         = "wscanner/H4lCP/allAxesConfig_cHWBtil" + floating_pois_4l + ".txt"
    inputDict1["scanType"]       = ["statOnly"]
    inputDict1["dimension"]      = "1D"
    inputDict1["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict1["fileName"]       = [ws_3d_4l_OO1_4l_tCzz1_name]
    inputDict1["outputDir"]      = ["ws_scan_v25_4l_cHWBtil" + floating_pois_4l + "_OO1_4l_tCzz1_reco"]
    buildList(inputDict1, scan1D)

    inputDict2 = {}
    inputDict2["poiList"] = [
        "cHWBtil:200:-3.5:3.5",
    ]
    inputDict2["config"]         = "wscanner/H4lCP/allAxesConfig_cHWBtil" + floating_pois_4l + ".txt"
    inputDict2["scanType"]       = ["statOnly"]
    inputDict2["dimension"]      = "1D"
    inputDict2["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict2["fileName"]       = [ws_3d_4l_OO1_4l_tCza1_name]
    inputDict2["outputDir"]      = ["ws_scan_v25_4l_cHWBtil" + floating_pois_4l + "_OO1_4l_tCza1_reco"]
    buildList(inputDict2, scan1D)

    inputDict3 = {}
    inputDict3["poiList"] = [
        "cHWBtil:200:-3.5:3.5",
    ]
    inputDict3["config"]         = "wscanner/H4lCP/allAxesConfig_cHWBtil" + floating_pois_4l + ".txt"
    inputDict3["scanType"]       = ["statOnly"]
    inputDict3["dimension"]      = "1D"
    inputDict3["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict3["fileName"]       = [ws_3d_4l_OO1_4l_tCaa1_name]
    inputDict3["outputDir"]      = ["ws_scan_v25_4l_cHWBtil" + floating_pois_4l + "_OO1_4l_tCaa1_reco"]
    buildList(inputDict3, scan1D)

    inputDict4 = {}
    inputDict4["poiList"] = [
        "cHWBtil:200:-3.5:3.5",
    ]
    inputDict4["config"]         = "wscanner/H4lCP/allAxesConfig_cHWBtil" + floating_pois_4l + ".txt"
    inputDict4["scanType"]       = ["statOnly"]
    inputDict4["dimension"]      = "1D"
    inputDict4["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict4["fileName"]       = [ws_3d_4l_OO1_4l_cHWtil1_name]
    inputDict4["outputDir"]      = ["ws_scan_v25_4l_cHWBtil" + floating_pois_4l + "_OO1_4l_cHWtil1_reco"]
    buildList(inputDict4, scan1D)

    inputDict5 = {}
    inputDict5["poiList"] = [
        "cHWBtil:200:-3.5:3.5",
    ]
    inputDict5["config"]         = "wscanner/H4lCP/allAxesConfig_cHWBtil" + floating_pois_4l + ".txt"
    inputDict5["scanType"]       = ["statOnly"]
    inputDict5["dimension"]      = "1D"
    inputDict5["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict5["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_name]
    inputDict5["outputDir"]      = ["ws_scan_v25_4l_cHWBtil" + floating_pois_4l + "_OO1_4l_cHBtil1_reco"]
    buildList(inputDict5, scan1D)

    inputDict6 = {}
    inputDict6["poiList"] = [
        "cHWBtil:200:-3.5:3.5",
    ]
    inputDict6["config"]         = "wscanner/H4lCP/allAxesConfig_cHWBtil" + floating_pois_4l + ".txt"
    inputDict6["scanType"]       = ["statOnly"]
    inputDict6["dimension"]      = "1D"
    inputDict6["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict6["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_name]
    inputDict6["outputDir"]      = ["ws_scan_v25_4l_cHWBtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_reco"]
    buildList(inputDict6, scan1D)

    inputDict7 = {}
    inputDict7["poiList"] = [
        "cHWBtil:200:-3.5:3.5",
    ]
    inputDict7["config"]         = "wscanner/H4lCP/allAxesConfig_cHWBtil" + floating_pois_4l + ".txt"
    inputDict7["scanType"]       = ["statOnly"]
    inputDict7["dimension"]      = "1D"
    inputDict7["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict7["fileName"]       = [ws_3d_4l_OO1_4l_dTilde1_name]
    inputDict7["outputDir"]      = ["ws_scan_v25_4l_cHWBtil" + floating_pois_4l + "_OO1_4l_dTilde1_reco"]
    buildList(inputDict7, scan1D)

    inputDict8 = {}
    inputDict8["poiList"] = [
        "cHWBtil:200:-3.5:3.5",
    ]
    inputDict8["config"]         = "wscanner/H4lCP/allAxesConfig_cHWBtil" + floating_pois_4l + ".txt"
    inputDict8["scanType"]       = ["statOnly"]
    inputDict8["dimension"]      = "1D"
    inputDict8["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict8["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1]
    inputDict8["outputDir"]      = ["ws_scan_v25_4l_cHWBtil" + floating_pois_4l + "_OO1_4l_cHWBtil1_VS_OO1_4l_cHWtil1_reco"]
    buildList(inputDict8, scan1D)

    # inputDict9 = {}
    # inputDict9["poiList"] = [
    #     "cHWBtil:200:-3.5:3.5",
    # ]
    # inputDict9["config"]         = "wscanner/H4lCP/allAxesConfig_cHWBtil" + floating_pois_4l + ".txt"
    # inputDict9["scanType"]       = ["statOnly"]
    # inputDict9["dimension"]      = "1D"
    # inputDict9["doData"]         = [False]
    # # OO1_4l 1D: 
    # # Use 3d reco workspace
    # inputDict9["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1]
    # inputDict9["outputDir"]      = ["ws_scan_v25_4l_cHWBtil" + floating_pois_4l + "_OO1_4l_cHBtil1_VS_OO1_4l_cHWtil1_reco"]
    # buildList(inputDict9, scan1D)

    

#===========
# 4l dTilde
#===========

if do4ldTilde1D:
    inputDict1 = {}
    inputDict1["poiList"] = [
        "dTilde:200:-2.0:2.0",
    ]
    inputDict1["config"]         = "wscanner/H4lCP/allAxesConfig_dTilde" + floating_pois_4l + ".txt"
    inputDict1["scanType"]       = ["statOnly"]
    inputDict1["dimension"]      = "1D"
    inputDict1["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict1["fileName"]       = [ws_3d_4l_OO1_4l_tCzz1_name]
    inputDict1["outputDir"]      = ["ws_scan_v25_4l_dTilde" + floating_pois_4l + "_OO1_4l_tCzz1_reco"]
    buildList(inputDict1, scan1D)

    inputDict2 = {}
    inputDict2["poiList"] = [
        "dTilde:200:-2.0:2.0",
    ]
    inputDict2["config"]         = "wscanner/H4lCP/allAxesConfig_dTilde" + floating_pois_4l + ".txt"
    inputDict2["scanType"]       = ["statOnly"]
    inputDict2["dimension"]      = "1D"
    inputDict2["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict2["fileName"]       = [ws_3d_4l_OO1_4l_tCza1_name]
    inputDict2["outputDir"]      = ["ws_scan_v25_4l_dTilde" + floating_pois_4l + "_OO1_4l_tCza1_reco"]
    buildList(inputDict2, scan1D)

    inputDict3 = {}
    inputDict3["poiList"] = [
        "dTilde:200:-2.0:2.0",
    ]
    inputDict3["config"]         = "wscanner/H4lCP/allAxesConfig_dTilde" + floating_pois_4l + ".txt"
    inputDict3["scanType"]       = ["statOnly"]
    inputDict3["dimension"]      = "1D"
    inputDict3["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict3["fileName"]       = [ws_3d_4l_OO1_4l_tCaa1_name]
    inputDict3["outputDir"]      = ["ws_scan_v25_4l_dTilde" + floating_pois_4l + "_OO1_4l_tCaa1_reco"]
    buildList(inputDict3, scan1D)

    inputDict4 = {}
    inputDict4["poiList"] = [
        "dTilde:200:-2.0:2.0",
    ]
    inputDict4["config"]         = "wscanner/H4lCP/allAxesConfig_dTilde" + floating_pois_4l + ".txt"
    inputDict4["scanType"]       = ["statOnly"]
    inputDict4["dimension"]      = "1D"
    inputDict4["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict4["fileName"]       = [ws_3d_4l_OO1_4l_cHWtil1_name]
    inputDict4["outputDir"]      = ["ws_scan_v25_4l_dTilde" + floating_pois_4l + "_OO1_4l_cHWtil1_reco"]
    buildList(inputDict4, scan1D)

    inputDict5 = {}
    inputDict5["poiList"] = [
        "dTilde:200:-2.0:2.0",
    ]
    inputDict5["config"]         = "wscanner/H4lCP/allAxesConfig_dTilde" + floating_pois_4l + ".txt"
    inputDict5["scanType"]       = ["statOnly"]
    inputDict5["dimension"]      = "1D"
    inputDict5["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict5["fileName"]       = [ws_3d_4l_OO1_4l_cHBtil1_name]
    inputDict5["outputDir"]      = ["ws_scan_v25_4l_dTilde" + floating_pois_4l + "_OO1_4l_cHBtil1_reco"]
    buildList(inputDict5, scan1D)

    inputDict6 = {}
    inputDict6["poiList"] = [
        "dTilde:200:-2.0:2.0",
    ]
    inputDict6["config"]         = "wscanner/H4lCP/allAxesConfig_dTilde" + floating_pois_4l + ".txt"
    inputDict6["scanType"]       = ["statOnly"]
    inputDict6["dimension"]      = "1D"
    inputDict6["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict6["fileName"]       = [ws_3d_4l_OO1_4l_cHWBtil1_name]
    inputDict6["outputDir"]      = ["ws_scan_v25_4l_dTilde" + floating_pois_4l + "_OO1_4l_cHWBtil1_reco"]
    buildList(inputDict6, scan1D)
    
    inputDict7 = {}
    inputDict7["poiList"] = [
        "dTilde:200:-2.0:2.0",
    ]
    inputDict7["config"]         = "wscanner/H4lCP/allAxesConfig_dTilde" + floating_pois_4l + ".txt"
    inputDict7["scanType"]       = ["statOnly"]
    inputDict7["dimension"]      = "1D"
    inputDict7["doData"]         = [False]
    # OO1_4l 1D: 
    # Use 3d reco workspace
    inputDict7["fileName"]       = [ws_3d_4l_OO1_4l_dTilde1_name]
    inputDict7["outputDir"]      = ["ws_scan_v25_4l_dTilde" + floating_pois_4l + "_OO1_4l_dTilde1_reco"]
    buildList(inputDict7, scan1D)



    

#====================
# Prod 2D
#====================

#====================
# prod tCzz vs tCza
#====================

if doProdtCzzvstCza:

    # tCzz vs tCza
    inputDict1 = {}
    inputDict1["poiList"] = [
        # "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
        "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
    ]
    inputDict1["config"]         = "wscanner/H4lCP/allAxesConfig_tCzz_tCza.txt"
    inputDict1["scanType"]       = ["statOnly"]
    inputDict1["dimension"]      = "2D"
    inputDict1["doData"]         = [False]
    # OO1_jj 2D:
    inputDict1["fileName"]       = [ws_2d_jj_OO1_jj_tCzz1_name]
    inputDict1["outputDir"]      = ["ws_scan_v25_VBF_tCzz_tCza" + floating_pois_jj + "_OO1_jj_tCzz1_reco"]
    buildList(inputDict1, scan2D)

    inputDict2 = {}
    inputDict2["poiList"] = [
        # "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
        "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
    ]
    inputDict2["config"]         = "wscanner/H4lCP/allAxesConfig_tCzz_tCza.txt"
    inputDict2["scanType"]       = ["statOnly"]
    inputDict2["dimension"]      = "2D"
    inputDict2["doData"]         = [False]
    # OO1_jj 2D:
    inputDict2["fileName"]       = [ws_2d_jj_OO1_jj_tCza1_name]
    inputDict2["outputDir"]      = ["ws_scan_v25_VBF_tCzz_tCza" + floating_pois_jj + "_OO1_jj_tCza1_reco"]
    buildList(inputDict2, scan2D)

    inputDict3 = {}
    inputDict3["poiList"] = [
        # "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
        "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
    ]
    inputDict3["config"]         = "wscanner/H4lCP/allAxesConfig_tCzz_tCza.txt"
    inputDict3["scanType"]       = ["statOnly"]
    inputDict3["dimension"]      = "2D"
    inputDict3["doData"]         = [False]
    # OO1_jj 2D:
    inputDict3["fileName"]       = [ws_2d_jj_OO1_jj_tCaa1_name]
    inputDict3["outputDir"]      = ["ws_scan_v25_VBF_tCzz_tCza" + floating_pois_jj + "_OO1_jj_tCaa1_reco"]
    buildList(inputDict3, scan2D)

    inputDict4 = {}
    inputDict4["poiList"] = [
        # "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
        "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
    ]
    inputDict4["config"]         = "wscanner/H4lCP/allAxesConfig_tCzz_tCza.txt"
    inputDict4["scanType"]       = ["statOnly"]
    inputDict4["dimension"]      = "2D"
    inputDict4["doData"]         = [False]
    # OO1_jj 2D:
    inputDict4["fileName"]       = [ws_2d_jj_OO1_jj_cHWtil1_name]
    inputDict4["outputDir"]      = ["ws_scan_v25_VBF_tCzz_tCza" + floating_pois_jj + "_OO1_jj_cHWtil1_reco"]
    buildList(inputDict4, scan2D)
    
    inputDict5 = {}
    inputDict5["poiList"] = [
        # "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
        "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
    ]
    inputDict5["config"]         = "wscanner/H4lCP/allAxesConfig_tCzz_tCza.txt"
    inputDict5["scanType"]       = ["statOnly"]
    inputDict5["dimension"]      = "2D"
    inputDict5["doData"]         = [False]
    # OO1_jj 2D:
    inputDict5["fileName"]       = [ws_2d_jj_OO1_jj_cHBtil1_name]
    inputDict5["outputDir"]      = ["ws_scan_v25_VBF_tCzz_tCza" + floating_pois_jj + "_OO1_jj_cHBtil1_reco"]
    buildList(inputDict5, scan2D)
    
    inputDict6 = {}
    inputDict6["poiList"] = [
        # "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
        "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
    ]
    inputDict6["config"]         = "wscanner/H4lCP/allAxesConfig_tCzz_tCza.txt"
    inputDict6["scanType"]       = ["statOnly"]
    inputDict6["dimension"]      = "2D"
    inputDict6["doData"]         = [False]
    # OO1_jj 2D:
    inputDict6["fileName"]       = [ws_2d_jj_OO1_jj_cHWBtil1_name]
    inputDict6["outputDir"]      = ["ws_scan_v25_VBF_tCzz_tCza" + floating_pois_jj + "_OO1_jj_cHWBtil1_reco"]
    buildList(inputDict6, scan2D)
    
    # inputDict7 = {}
    # inputDict7["poiList"] = [
    #     # "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
    #     "tCzz:50:-6.0:6.0,tCza:50:-12.0:12.0",
    # ]
    # inputDict7["config"]         = "wscanner/H4lCP/allAxesConfig_tCzz_tCza.txt"
    # inputDict7["scanType"]       = ["statOnly"]
    # inputDict7["dimension"]      = "2D"
    # inputDict7["doData"]         = [False]
    # # OO1_jj 2D:
    # inputDict7["fileName"]       = [ws_2d_jj_OO1_jj_tCzz1_VS_OO1_jj_tCza1_name]
    # inputDict7["outputDir"]      = ["ws_scan_v25_VBF_tCzz_tCza" + floating_pois_jj + "_OO1_jj_tCzz1_VS_OO1_jj_tCza1_reco"]
    # buildList(inputDict7, scan2D)
    
    
#====================
# Prod 1D
#====================


# Prod 1D scan with 1,2,3D WSs


if doProd1DtCzz:
    inputDict1 = {}
    inputDict1["poiList"] = [
        "tCzz:200:-6.0:6.0",
    ]
    inputDict1["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCzz_fixed_tCza" + floating_pois_jj + ".txt"
    inputDict1["scanType"]       = ["statOnly"]
    inputDict1["dimension"]      = "1D"
    inputDict1["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict1["fileName"]       = [ws_2d_jj_OO1_jj_tCzz1_name]
    inputDict1["outputDir"]      = ["ws_scan_v25_VBF_tCzz" + floating_pois_jj + "_OO1_jj_tCzz1_reco"]
    buildList(inputDict1, scan1D)

    inputDict2 = {}
    inputDict2["poiList"] = [
        "tCzz:200:-6.0:6.0",
    ]
    inputDict2["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCzz_fixed_tCza" + floating_pois_jj + ".txt"
    inputDict2["scanType"]       = ["statOnly"]
    inputDict2["dimension"]      = "1D"
    inputDict2["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict2["fileName"]       = [ws_2d_jj_OO1_jj_tCza1_name]
    inputDict2["outputDir"]      = ["ws_scan_v25_VBF_tCzz" + floating_pois_jj + "_OO1_jj_tCza1_reco"]
    buildList(inputDict2, scan1D)

    inputDict3 = {}
    inputDict3["poiList"] = [
        "tCzz:200:-6.0:6.0",
    ]
    inputDict3["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCzz_fixed_tCza" + floating_pois_jj + ".txt"
    inputDict3["scanType"]       = ["statOnly"]
    inputDict3["dimension"]      = "1D"
    inputDict3["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict3["fileName"]       = [ws_2d_jj_OO1_jj_tCaa1_name]
    inputDict3["outputDir"]      = ["ws_scan_v25_VBF_tCzz" + floating_pois_jj + "_OO1_jj_tCaa1_reco"]
    buildList(inputDict3, scan1D)

    inputDict4 = {}
    inputDict4["poiList"] = [
        "tCzz:200:-6.0:6.0",
    ]
    inputDict4["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCzz_fixed_tCza" + floating_pois_jj + ".txt"
    inputDict4["scanType"]       = ["statOnly"]
    inputDict4["dimension"]      = "1D"
    inputDict4["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict4["fileName"]       = [ws_2d_jj_OO1_jj_cHWtil1_name]
    inputDict4["outputDir"]      = ["ws_scan_v25_VBF_tCzz" + floating_pois_jj + "_OO1_jj_cHWtil1_reco"]
    buildList(inputDict4, scan1D)

    inputDict5 = {}
    inputDict5["poiList"] = [
        "tCzz:200:-6.0:6.0",
    ]
    inputDict5["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCzz_fixed_tCza" + floating_pois_jj + ".txt"
    inputDict5["scanType"]       = ["statOnly"]
    inputDict5["dimension"]      = "1D"
    inputDict5["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict5["fileName"]       = [ws_2d_jj_OO1_jj_cHBtil1_name]
    inputDict5["outputDir"]      = ["ws_scan_v25_VBF_tCzz" + floating_pois_jj + "_OO1_jj_cHBtil1_reco"]
    buildList(inputDict5, scan1D)

    inputDict6 = {}
    inputDict6["poiList"] = [
        "tCzz:200:-6.0:6.0",
    ]
    inputDict6["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCzz_fixed_tCza" + floating_pois_jj + ".txt"
    inputDict6["scanType"]       = ["statOnly"]
    inputDict6["dimension"]      = "1D"
    inputDict6["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict6["fileName"]       = [ws_2d_jj_OO1_jj_cHWBtil1_name]
    inputDict6["outputDir"]      = ["ws_scan_v25_VBF_tCzz" + floating_pois_jj + "_OO1_jj_cHWBtil1_reco"]
    buildList(inputDict6, scan1D)

    inputDict7 = {}
    inputDict7["poiList"] = [
        "tCzz:200:-6.0:6.0",
    ]
    inputDict7["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCzz_fixed_tCza" + floating_pois_jj + ".txt"
    inputDict7["scanType"]       = ["statOnly"]
    inputDict7["dimension"]      = "1D"
    inputDict7["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict7["fileName"]       = [ws_2d_jj_dijet_deltaphi_name]
    inputDict7["outputDir"]      = ["ws_scan_v25_VBF_tCzz" + floating_pois_jj + "_dijet_deltaphi_reco"]
    buildList(inputDict7, scan1D)

    
if doProd1DtCza:
    inputDict1 = {}
    inputDict1["poiList"] = [
        "tCza:200:-6.0:6.0",
    ]
    inputDict1["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCza_fixed_tCzz" + floating_pois_jj + ".txt"
    inputDict1["scanType"]       = ["statOnly"]
    inputDict1["dimension"]      = "1D"
    inputDict1["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict1["fileName"]       = [ws_2d_jj_OO1_jj_tCzz1_name]
    inputDict1["outputDir"]      = ["ws_scan_v25_VBF_tCza" + floating_pois_jj + "_OO1_jj_tCzz1_reco"]
    buildList(inputDict1, scan1D)

    inputDict2 = {}
    inputDict2["poiList"] = [
        "tCza:200:-6.0:6.0",
    ]
    inputDict2["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCza_fixed_tCzz" + floating_pois_jj + ".txt"
    inputDict2["scanType"]       = ["statOnly"]
    inputDict2["dimension"]      = "1D"
    inputDict2["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict2["fileName"]       = [ws_2d_jj_OO1_jj_tCza1_name]
    inputDict2["outputDir"]      = ["ws_scan_v25_VBF_tCza" + floating_pois_jj + "_OO1_jj_tCza1_reco"]
    buildList(inputDict2, scan1D)

    inputDict3 = {}
    inputDict3["poiList"] = [
        "tCza:200:-6.0:6.0",
    ]
    inputDict3["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCza_fixed_tCzz" + floating_pois_jj + ".txt"
    inputDict3["scanType"]       = ["statOnly"]
    inputDict3["dimension"]      = "1D"
    inputDict3["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict3["fileName"]       = [ws_2d_jj_OO1_jj_tCaa1_name]
    inputDict3["outputDir"]      = ["ws_scan_v25_VBF_tCza" + floating_pois_jj + "_OO1_jj_tCaa1_reco"]
    buildList(inputDict3, scan1D)

    inputDict4 = {}
    inputDict4["poiList"] = [
        "tCza:200:-6.0:6.0",
    ]
    inputDict4["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCza_fixed_tCzz" + floating_pois_jj + ".txt"
    inputDict4["scanType"]       = ["statOnly"]
    inputDict4["dimension"]      = "1D"
    inputDict4["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict4["fileName"]       = [ws_2d_jj_OO1_jj_cHWtil1_name]
    inputDict4["outputDir"]      = ["ws_scan_v25_VBF_tCza" + floating_pois_jj + "_OO1_jj_cHWtil1_reco"]
    buildList(inputDict4, scan1D)

    inputDict5 = {}
    inputDict5["poiList"] = [
        "tCza:200:-6.0:6.0",
    ]
    inputDict5["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCza_fixed_tCzz" + floating_pois_jj + ".txt"
    inputDict5["scanType"]       = ["statOnly"]
    inputDict5["dimension"]      = "1D"
    inputDict5["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict5["fileName"]       = [ws_2d_jj_OO1_jj_cHBtil1_name]
    inputDict5["outputDir"]      = ["ws_scan_v25_VBF_tCza" + floating_pois_jj + "_OO1_jj_cHBtil1_reco"]
    buildList(inputDict5, scan1D)

    inputDict6 = {}
    inputDict6["poiList"] = [
        "tCza:200:-6.0:6.0",
    ]
    inputDict6["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCza_fixed_tCzz" + floating_pois_jj + ".txt"
    inputDict6["scanType"]       = ["statOnly"]
    inputDict6["dimension"]      = "1D"
    inputDict6["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict6["fileName"]       = [ws_2d_jj_OO1_jj_cHWBtil1_name]
    inputDict6["outputDir"]      = ["ws_scan_v25_VBF_tCza" + floating_pois_jj + "_OO1_jj_cHWBtil1_reco"]
    buildList(inputDict6, scan1D)

    inputDict7 = {}
    inputDict7["poiList"] = [
        "tCza:200:-6.0:6.0",
    ]
    inputDict7["config"]         = "wscanner/H4lCP/allAxesConfig_float_tCza_fixed_tCzz" + floating_pois_jj + ".txt"
    inputDict7["scanType"]       = ["statOnly"]
    inputDict7["dimension"]      = "1D"
    inputDict7["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict7["fileName"]       = [ws_2d_jj_dijet_deltaphi_name]
    inputDict7["outputDir"]      = ["ws_scan_v25_VBF_tCza" + floating_pois_jj + "_dijet_deltaphi_reco"]
    buildList(inputDict7, scan1D)

    
if doProd1DcHWtil:
    inputDict1 = {}
    inputDict1["poiList"] = [
        "cHWtil:200:-8.0:8.0",
    ]
    inputDict1["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_jj + ".txt"
    inputDict1["scanType"]       = ["statOnly"]
    inputDict1["dimension"]      = "1D"
    inputDict1["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict1["fileName"]       = [ws_1d_jj_OO1_jj_tCzz1_name]
    inputDict1["outputDir"]      = ["ws_scan_v25_VBF_cHWtil" + floating_pois_jj + "_OO1_jj_tCzz1_reco"]
    buildList(inputDict1, scan1D)

    inputDict2 = {}
    inputDict2["poiList"] = [
        "cHWtil:200:-8.0:8.0",
    ]
    inputDict2["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_jj + ".txt"
    inputDict2["scanType"]       = ["statOnly"]
    inputDict2["dimension"]      = "1D"
    inputDict2["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict2["fileName"]       = [ws_1d_jj_OO1_jj_tCza1_name]
    inputDict2["outputDir"]      = ["ws_scan_v25_VBF_cHWtil" + floating_pois_jj + "_OO1_jj_tCza1_reco"]
    buildList(inputDict2, scan1D)

    inputDict3 = {}
    inputDict3["poiList"] = [
        "cHWtil:200:-8.0:8.0",
    ]
    inputDict3["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_jj + ".txt"
    inputDict3["scanType"]       = ["statOnly"]
    inputDict3["dimension"]      = "1D"
    inputDict3["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict3["fileName"]       = [ws_1d_jj_OO1_jj_tCaa1_name]
    inputDict3["outputDir"]      = ["ws_scan_v25_VBF_cHWtil" + floating_pois_jj + "_OO1_jj_tCaa1_reco"]
    buildList(inputDict3, scan1D)

    inputDict4 = {}
    inputDict4["poiList"] = [
        "cHWtil:200:-8.0:8.0",
    ]
    inputDict4["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_jj + ".txt"
    inputDict4["scanType"]       = ["statOnly"]
    inputDict4["dimension"]      = "1D"
    inputDict4["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict4["fileName"]       = [ws_1d_jj_OO1_jj_cHWtil1_name]
    inputDict4["outputDir"]      = ["ws_scan_v25_VBF_cHWtil" + floating_pois_jj + "_OO1_jj_cHWtil1_reco"]
    buildList(inputDict4, scan1D)

    inputDict5 = {}
    inputDict5["poiList"] = [
        "cHWtil:200:-8.0:8.0",
    ]
    inputDict5["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_jj + ".txt"
    inputDict5["scanType"]       = ["statOnly"]
    inputDict5["dimension"]      = "1D"
    inputDict5["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict5["fileName"]       = [ws_1d_jj_OO1_jj_cHBtil1_name]
    inputDict5["outputDir"]      = ["ws_scan_v25_VBF_cHWtil" + floating_pois_jj + "_OO1_jj_cHBtil1_reco"]
    buildList(inputDict5, scan1D)

    inputDict6 = {}
    inputDict6["poiList"] = [
        "cHWtil:200:-8.0:8.0",
    ]
    inputDict6["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_jj + ".txt"
    inputDict6["scanType"]       = ["statOnly"]
    inputDict6["dimension"]      = "1D"
    inputDict6["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict6["fileName"]       = [ws_1d_jj_OO1_jj_cHWBtil1_name]
    inputDict6["outputDir"]      = ["ws_scan_v25_VBF_cHWtil" + floating_pois_jj + "_OO1_jj_cHWBtil1_reco"]
    buildList(inputDict6, scan1D)

    inputDict7 = {}
    inputDict7["poiList"] = [
        "cHWtil:200:-6.0:6.0",
    ]
    inputDict7["config"]         = "wscanner/H4lCP/allAxesConfig_cHWtil" + floating_pois_jj + ".txt"
    inputDict7["scanType"]       = ["statOnly"]
    inputDict7["dimension"]      = "1D"
    inputDict7["doData"]         = [False]
    # OO1_jj 1D: 
    # Use 2d reco workspace
    inputDict7["fileName"]       = [ws_1d_jj_dijet_deltaphi_name]
    inputDict7["outputDir"]      = ["ws_scan_v25_VBF_cHWtil" + floating_pois_jj + "_dijet_deltaphi_reco"]
    buildList(inputDict7, scan1D)

    


# cHWtil=cHBtil
inputDict["poiList"] = [
    "cHWtilcHBtil:200:-6.0:6.0",
    # "cHBtil:200:-6.0:6.0",
    ]
inputDict["config"]         = "wscanner/H4lCP/allAxesConfig.txt"
inputDict["scanType"]       = ["statOnly"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
# OO1_4l 1D: 
# Use 2d workspace
inputDict["fileName"]       = ["workspaces/WS_combined_CP_v25_ggF_cHWtilcHBtil_axis_opt_v1.root"]
inputDict["outputDir"]      = ["ws_scan_v25_ggF_cHWtilcHBtil_axis_opt_v1"]
# buildList(inputDict, scan1D)


#Prod 1D

inputDict["poiList"] = [
"tCzz:31:-2.5:2.5"
]
inputDict["config"]         = "../source/data/CP/tCzzConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_CP_v25_tCzz.root"]
inputDict["outputDir"]      = ["WS_combined_CP_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)



inputDict["poiList"] = [
"tCza:31:-4:4"
]
inputDict["config"]         = "../source/data/CP/tCzaConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_CP_v25_tCza.root"]
inputDict["outputDir"]      = ["WS_combined_CP_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)



inputDict["poiList"] = [
"cHWtil:31:-4:4"
]
inputDict["config"]         = "../source/data/CP/cHWtilConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_CP_v25_cHWtil.root"]
inputDict["outputDir"]      = ["WS_combined_CP_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)

# Decay

inputDict["poiList"] = [
"tCaa:41:-2:2"
]
inputDict["config"]         = "../source/data/CP/tCaaConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_ggF_CP_OO4l_v25_tCaa.root"]
inputDict["outputDir"]      = ["WS_combined_ggF_CP_OO4l_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)


inputDict["poiList"] = [
"tCza:41:-1:1"
]
inputDict["config"]         = "../source/data/CP/tCzaConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_ggF_CP_OO4l_v25_tCza.root"]
inputDict["outputDir"]      = ["WS_combined_ggF_CP_OO4l_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)

inputDict["poiList"] = [
"cHWtil:61:-2:2"
]
inputDict["config"]         = "../source/data/CP/cHWtilConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_ggF_CP_OO4l_v25_cHWtil.root"]
inputDict["outputDir"]      = ["WS_combined_ggF_CP_OO4l_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)


inputDict["poiList"] = [
"cHWBtil:41:-2:2"
]
inputDict["config"]         = "../source/data/CP/cHWBtilConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_ggF_CP_OO4l_v25_cHWBtil.root"]
inputDict["outputDir"]      = ["WS_combined_ggF_CP_OO4l_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)

inputDict["poiList"] = [
"cHBtil:41:-1:1"
]
inputDict["config"]         = "../source/data/CP/cHBtilConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_ggF_CP_OO4l_v25_cHBtil.root"]
inputDict["outputDir"]      = ["WS_combined_ggF_CP_OO4l_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)

# Decay Theo Sys

inputDict["poiList"] = [
"tCaa:41:-2:2"
]
inputDict["config"]         = "../source/data/CP/tCaaConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_sysTheo_ggF_CP_OO4l_v25_tCaa.root"]
inputDict["outputDir"]      = ["WS_combined_sysTheo_ggF_CP_OO4l_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)


inputDict["poiList"] = [
"tCza:41:-1:1"
]
inputDict["config"]         = "../source/data/CP/tCzaConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_sysTheo_ggF_CP_OO4l_v25_tCza.root"]
inputDict["outputDir"]      = ["WS_combined_sysTheo_ggF_CP_OO4l_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)

inputDict["poiList"] = [
"cHWtil:41:-2:2"
]
inputDict["config"]         = "../source/data/CP/cHWtilConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_sysTheo_ggF_CP_OO4l_v25_cHWtil.root"]
inputDict["outputDir"]      = ["WS_combined_sysTheo_ggF_CP_OO4l_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)


inputDict["poiList"] = [
"cHWBtil:41:-2:2"
]
inputDict["config"]         = "../source/data/CP/cHWBtilConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_sysTheo_ggF_CP_OO4l_v25_cHWBtil.root"]
inputDict["outputDir"]      = ["WS_combined_sysTheo_ggF_CP_OO4l_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)

inputDict["poiList"] = [
"cHBtil:41:-1:1"
]
inputDict["config"]         = "../source/data/CP/cHBtilConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_sysTheo_ggF_CP_OO4l_v25_cHBtil.root"]
inputDict["outputDir"]      = ["WS_combined_sysTheo_ggF_CP_OO4l_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)




#Production and Decay

inputDict["poiList"] = [
"cHWtil:61:-1.5:1.5"
]
inputDict["config"]         = "../source/data/CP/cHWtilConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_ggF_VBFVH_CP_OO4l_v25_cHWtil.root"]
inputDict["outputDir"]      = ["WS_combined_ggF_VBFVH_CP_OO4l_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)

# Theosys
inputDict["poiList"] = [
"tCzz:41:-2.:2."
]
inputDict["config"]         = "../source/data/CP/tCzzConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_VBFVH_CP_OOjjH_theosys_v25_tCzz.root"]
inputDict["outputDir"]      = ["WS_combined_VBFVH_CP_OOjjH_theosys_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)

inputDict["poiList"] = [
"tCza:31:-4:4"
]
inputDict["config"]         = "../source/data/CP/tCzaConfig.txt"
inputDict["scanType"]       = ["statOnly","allSys"]
inputDict["dimension"]      = "1D"
inputDict["doData"]         = [False]
inputDict["fileName"]       = ["workspaces/WS_combined_VBFVH_CP_OOjjH_theosys_v25_tCza.root"]
inputDict["outputDir"]      = ["WS_combined_VBFVH_CP_OOjjH_theosys_v25"]
inputDict["nSplitJob"]      = 20
# buildList(inputDict, scan1D)

##### Floating #######                                                                                                                      
inputDict["poiList"]            = ["tCzz:200:-2.5:2.5","mu_qqZZ:100:0.5:1.5","mu_ggF:100:0.5:1.5"]
inputDict["config"]             = "wscanner/H4lCP/H4lCPScanConfigFloat.txt"
inputDict["scanType"]           = ["statOnly"]
inputDict["dimension"]          = "1D"
inputDict["doData"]             = [False]
inputDict["extraVarSave"]       = ["mu_qqZZ","mu_ggF","CSM"]
#inputDict["fileName"]           = ["workspaces/RecoSRCR2_CatVBFNN/tCzz_CSM__ggF_VBFVH_qqZZ_asimov0__VBFlike_binNN_SRCR1__OO1_jj_tCzz1/combined.root"]
inputDict["fileName"]           = ["workspaces/RecoSRCR2_CatVBFNN/tCzz_CSM__ggF_VBFVH_qqZZ_asimov0__VBFlike_binNN_SRCR1__OO1_jj_tCzz1/combined.root"]
inputDict["outputDir"]          = ["RecoSRCR2_CatVBFNN_tCzz_Float"]
inputDict["nSplitJob"]      = 1
# buildList(inputDict, scan1D) 
