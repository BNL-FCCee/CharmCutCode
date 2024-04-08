#!/bin/bash
setupATLAS
lsetup "root 6.10.06-x86_64-slc6-gcc62-opt"
# lsetup  "root 6.08.06-HiggsComb-x86_64-slc6-gcc49-opt"
lsetup "gcc gcc620_x86_64_slc6"
#source /cvmfs/sft.cern.ch/lcg/releases/LCG_94a/cmaketools/1.6/x86_64-slc6-gcc62-opt/cmaketools-env.sh
lsetup  cmake
alias cbuild='cd ../build;make;cd ../run'
alias ccleanbuild='cd ../build && rm -rf * && cmake ../source && make && cd ../run'



# this is an auto-generated setup script
export PATH=${PATH}:/usatlas/u/ivelisce/FCC/CharmCut/source/wscanner/build/
export wscanner_data=/usatlas/u/ivelisce/FCC/CharmCut/source/wscanner/data/
