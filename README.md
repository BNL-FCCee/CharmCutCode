
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

**Do remember to compile again if you edit any of the c++ code :)**

## CharmCutCode

The starting point of this code is ```runAnalysis.cxx```. This code is designed to parse the input options, create the correct analysis running, and call the required function on that. Do note that this is relatively hard-written process, in the sense that categories and their filling are left to be implemented in c++. This is done to ensure the greatest flexibility, since we don't know what is required to implemeted a FCCee analysis. A couple things to note:
- AnalysisXXX.cxx: This code must implement the simple event loop and apply selection and fill the relevant hist. Only this code needs to know what is the exact definition of the category
- TreeContainer: Simple but a dynamic reader to read only the variable requested. Make stuff faster
- HistContainer: A simple class to create and maintain a list of hist. Some of the hist name, like the ones for the observables are keyworded. The behaviour is expected to be modified based on analysis type
- MetadataContainer: This class is designed to propagate metadata as needed. Two funtions to note are the ```getFlavourCategory``` and ```getFitCategories```. These functions are expected to modify their behaviour based on ```analType``` option/

### Usage: Local running for testing
This is typically going to used for local testing
```
runAnalysis --inputFileList file1,file2 --outputFileName test.root --sampleName wzp6_ee_nunuH_HZZ_ecm240 --processName HZZ --analType vvjj --SOWJSONfile ../source/CharmCutCode/data/FCCee_procDict_winter2023_IDEA.json
```

### Usage: Batch running for processing all samples
```
python3 ../source/CharmCutCode/submission/submit.py --inputFolder <folder/with/inputs> --outputFolder <folderToSaveOutput> --doZHvvjj --<runLocal/runInteractive/submitCondor/...> #pick one option and remove the <brackets>
```

This will submit the jobs to be run in the choosen way. It will run multiple files in one job, as specified by the 'mergeFile' option. 

After the jobs are done, a manually merging of the files is needed
```
cd <folderToSaveOutput>
hadd -f combined.root */*root
```

## WSMaker
This code is designed to take input from the above code, and make it into workspace. It relies on the input json file, for which an example is provided in the repository. This code will rebin, clean, unroll 2D hist into a 1D hist, save the outputs and write the XML needed for HistFactory.

**It has the structure to do systematics, but that is very much not supported right now. This is a feature to come**

### Usage
```
// This will create all the inputs for HistFactory
makeWS --jsonConfig ../source/WSMaker/data/ZHvvJJConfig.json --inputFile <folderToSaveOutput>/combined.root --outputDir <wsfolder>

// Run HistFactory
hist2workspace <wsfolder>/XML/driver.xml  
```

At this end of this, there will be a combined workspace that can be used for fitting

## WScanner (stat analysis)
The workspace above can be used in any statistics related software. I will add this in the future. Need to see how to make my stat framework into a standalone easy to use system. Potentially could provide a simple script for testing