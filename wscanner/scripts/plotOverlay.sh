#!/bin/bash

# if no input arguments are provided, plot overlay
if [[ $# -eq 0 ]] ; 
	then
	folder1="WS_combined_noSys_STXS_S_S0_r_All_NN_v20p1Baseline_model_ATest"
	# folder2="WS_combined_noSys_STXS_S_S0_r_All_NN_v20p1Baseline_SBSplit_model_combined_ATest"
	folder3="WS_combined_noSys_STXS_S_S0_r_All_NN_v20p1Baseline_SBSplit_model_weighted_ATest"
	folder4="WS_combined_noSys_STXS_S_S0_r_All_NN_v20p1Baseline_SBSplit_model_dataweighted_ATest"
	## S0
 	# declare -a poiList=("mu_gg2H" "mu_VBF" "mu_VH" "mu_ttH" "r_ZZ_0jet" "r_ZZ_1jet" "r_ZZ_2jet" "r_ttV")
 	# declare -a poiList=("mu_gg2H" "mu_VBF" "mu_VH" "mu_ttH" "r_ttV")
 	# declare -a poiList=("r_ZZ_0jet" "r_ZZ_1jet" "r_ZZ_2jet")
 	declare -a poiList=("r_ZZ_HM_0jet" "r_ZZ_HM_1jet" "r_ZZ_HM_2jet" "r_ZZ_LM_0jet" "r_ZZ_LM_1jet" "r_ZZ_LM_2jet")

 	## ptH
 	# declare -a poiList=("mu_gg2H_0J" "mu_gg2H_1J_PTH_0_60" "mu_gg2H_1J_PTH_60_120"  "mu_gg2H_1J_PTH_GT120" "mu_gg2H_GE2J" "mu_VBF_qq2qq_PTH_GT200" "mu_VBF_qq2qq_PTH_LE200" "mu_VH_Had" "mu_VH_Lep" "mu_ttH" "r_ZZ_0jet" "r_ZZ_1jet" "r_ZZ_2jet" "r_ttV")
 	## RS1p1
 	# declare -a poiList=("mu_gg2H_0J_PTH_0_10" "mu_gg2H_0J_PTH_GT10" "mu_gg2H_1J_PTH_0_60" "mu_gg2H_1J_PTH_60_120" "mu_gg2H_1J_PTH_120_200" "mu_gg2H_GE2J" "mu_gg2H_PTH_GT200" "mu_VBF_qq2qq_PTH_GT200" "mu_VBF_qq2qq_PTH_LE200" "mu_VH_Had" "mu_VH_Lep" "mu_ttH" "r_ZZ_0jet" "r_ZZ_1jet" "r_ZZ_2jet" "r_ttV")
 	## RS1?
    # declare -a poiList=("mu_gg2H_0J" "mu_gg2H_1J_PTH_0_60" "mu_gg2H_1J_PTH_60_120" "mu_gg2H_1J_PTH_GT120" "mu_gg2H_GE2J" "mu_VBF_qq2qq_PTJET1_GT200" "mu_VBF_qq2qq_PTJET1_LE200" "mu_VH_Had" "mu_VH_Lep" "mu_ttH")
	
	for poi in "${poiList[@]}"
	do
		## stat and Sys Asimov
		# ../build/plot1D --fileName root-files/${folder1}/statOnly/asimov/${poi}.root,root-files/${folder1}/allSys/asimov/${poi}.root,root-files/${folder2}/statOnly/asimov/${poi}.root,root-files/${folder2}/allSys/asimov/${poi}.root --label NN-Stat,NN-Sys,BDT-Stat,BDT-Sys --baseName XS_S0_All_NN_v20p1Baseline
		## stat only
		# ../build/plot1D --fileName root-files/${folder1}/statOnly/asimov/${poi}.root,root-files/${folder3}/statOnly/asimov/${poi}.root,root-files/${folder4}/statOnly/asimov/${poi}.root --label Base,MCWeighted,DataWeighted --baseName ATest
		../build/plot1D --fileName root-files/${folder3}/statOnly/asimov/${poi}.root,root-files/${folder4}/statOnly/asimov/${poi}.root --label MCWeighted,DataWeighted --baseName ATest

	done

else
	for folder in $1/*;
		do
			echo "$(tput setaf 2)Now plotting: $(tput sgr 0) ${folder} ";
		    python ../source/scripts/plotScans.py --inputDir ${folder}
		done
fi

