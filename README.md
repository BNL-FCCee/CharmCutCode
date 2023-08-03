
# CharmCutCode

This repository contains the code to perform a simple analyses for FCCee. It contains the 3 parts

-  **CharmCutCode**: A simple ttree that apply selection cuts and organizes the histograms that can be used for the next step. This is a relatively hard-coded setup. While the analysis categories and observable hists are defined in a Common area, the selection cuts which fill each category is coded by the user in the .cxx file. This code base also provides a relatively flexible setup to submit batch jobs on a variety of different clusters
-  **WSMaker**: This code uses the inputs from above to make files that then feed into HistFactory to make the workspace. This code is intended to be dynamic and controlled through JSON configuration files
-  **Wscanner**: This code is to come, that will do the statistical analysis on the input ws from above

  

## Usage
There are not crazy dependencies, only a semi recent version of root is required.
```
mkdir source run build
cd source
git clone --recursive https://gitlab.cern.ch/sabidi/CharmCutCode.git ./
```

For first time usage:
```
cd ../build
cmake ../source
make -j
cd ../run
source ../build/setup.sh
```

For normal usage after the first compile:
```
cd run
source ../build/setup.sh
```

** Do remember to compile again if you edit any of the c++ code :) **