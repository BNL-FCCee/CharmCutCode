#!/bin/bash
setupATLAS
lsetup  "root 6.08.06-HiggsComb-x86_64-slc6-gcc49-opt"
alias sbuild='cd ../source;make;cd ../run'
alias scleanbuild='cd ../source && make clean && make && cd ../run'