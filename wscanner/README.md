This wscanner framework can be used on an arbitary workspace for:
* 1D/2D nll scans
* ranking of NPs
* Compatibility
* Correlation matrix
* Save and unconditional fit as a snapshot
* Read in snapshots
* Generate and fit toys
* Fit/scan datasets stored workspace (useful for bootstrap MC)
* 1D/2D scans by looking at percent of toys excluded for each point

Email bianca.ciungu@cern.ch or syed.haider.abidi@cern.ch for clarification on instructions.

**Table of contents:**
1. [Setting Up](#setup)
2. [Type of Scans](#types)
3. [Scanning Workspaces](#scanning)
3.1 [Submitting Scans](#submit)
3.2 [Config Files](#config)
4. [Plotting](#plotting)
5. [Package Structure and Philosophy](#structure)
----

## 1. Setting up <a name="setup"></a>
Please note that we are in the process of updating how this package is used and compiled to bring it inline with the new HComb software. Unless you are constrained to use the old method, please use the top level package at [Link](https://gitlab.cern.ch/HZZ/HZZSoftware/wscannerframework). Instructions are provided here for convenience

### First time compile
```
mkdir wscannerframework; cd wscannerframework; mkdir run build
git clone --recursive https://:@gitlab.cern.ch:8443/HZZ/HZZSoftware/wscannerframework.git source
cd source/wscanner/
git checkout master # or any other branch you want
cd ../../
source source/setup.sh
cd build
cmake ../source
make
source wscanner/setup.sh
```

### Every time use
```
cd wscannerframework
source source/setup.sh
source build/wscanner/setup.sh

```


## 2. Types of scans <a name= "types"></a>
The package has a few types of scans and tests that it can do. These are all derived classes of scanBase and are called by the input option refered to as dimention
* **1D NLL Scan** (dimension: 1D, class: scan1D): This is a simple negative log likelhood scan of a parameter of interest specified by the user
* **2D NLL Scan** (dimension: 2D, class: scan2D): This is a simple negative log likelhood scan of two parameters of interest specified by the user
* **Ranking** (dimension: ranking, class: rankNP): This gives you the prefit and postfit impact of a list of nussance parameters or POIs on all POIs in your workspace
* **Correlation Hist** (dimension: correlationHist, class: scan1D) This does an unconditional fit and saves all fit values
* **1D Exclusion Scan** (dimension 1DToys, class: scan1DToys): This gives you the same type of scan as scan1D with the difference that at each POI step in your scan, toys are generated and the percent excluded is saved
* **2D Exclusion Scan** (dimension 2DToys, class: scan2DToys): This gives you the same type of scan as scan2D with the difference that at each POI1,POI2 step in your scan, toys are generated and the percent excluded is saved
* **Run Toys** (dimension:runToys, class:runToys): This class can either generate toys from the workspace, or read in pre-made toys from another file. You can stop this at the generation stage or generate/readin toys and fit them
* **Scan Dataset** (dimension:scanDatasets, class scanDatasets): This reads in toy datasets and fit them saving you the fit for each point. You can even perform a 1D or 2D scan of each dataset you want to fit
* **Compatibility** (dimension:compatibility, class:computeCompatibility): This computes the compatibility for you. You can set the values of what you want to compare it to in the config.

All derived classes are short and easy to read for more information. More classes (such as scan 3D) and functionality can easily be added to this flexible framework. The output of all these scan types share a common format and have dedicated plotting scripts.



## 3. Scanning Workspaces <a name="scanning"></a>
Scans of workspaces can be done locally, on a cluster, or on the grid. All submissions are controlled by submitScans.py (found in source/scripts). The job itself is configured in jobList*.py (also in source/scripts). There are example jobs shown for all mentioned types in the existing jobLists (jobListCouplings, jobListPOs, etc.) There are a few key features that are common:
* inputDict["poiList"] : Here you would put your "poiName:steps:lwrRange:uprRange" in a comma seperated list for a 1D scan or  "poiName1:steps1:lwrRange1:uprRange1,poiName2:step2:lwrRange2:uprRange2" for a 2D scan of poiName1 vs poiName2. You can of course include more 1D/2D scans in the last by seperating with a comma. For ranking you can put a comma seperated list of the NPs you want to rank. The code will also understand regex expressions, for example "JET*" will only submit ranking on JET NPs. You can also leave it as a wildcard and all will be ranked.  For the other type of scans in jobList (compatibility, correlationHist, unconditional) please leave it as a wildcard. If you are using 1DToys or 2DToys the syntax is "poiName:steps:lwrRange:uprRange,nToys:XXX" or "poiName1:steps1:lwrRange1:uprRange1,poiName2:step2:lwrRange2:uprRange2,nToys:XXX" where XXX is the the number of toys you will generate at each scan point to get the exclusion limit. If you are doing runToys, you can do nToys:XXX to generate XXX toys from the workspace, or nToys:workspaceName.root:XXX to load in premade toys into the workspace (if you write nToys:inWorkspace:XXX it will look inside the same root file for the toys). Lastly, if you wish to scanDatasets you will follow the same pattern as for runToys, for example, "scanDatasets:inWorkspace:2000".
* inputDic["config"] : Here put the path to the config file relative to the run folder where we recommend you launch all scans from.
* inputDic["scanType"] : You can put a comma seperated list such as ["statOnly", "allSys", "modTheory"] of all the scans you wish to do. Make sure any special scans are setup in the config under the same name as you give here.
* inputDic["dimension"] : This is an instruction on which class the code should make. The choices are described in [Types](#types).
* inputDic["doData"] : Is a list of booleans. If you put [True, False] this will scan on asimov and on data. Make sure the names of your dataset are correct in the config you are using.
* inputDic["fileName"] : This is a list of paths to your workspaces. We again recommend you keep everything in the run directory and launch from here.
* inputDic["outputDir"] : Is a list of the names of the output directory the code will make and put nTuples into. It will have its own directory substructure with folders corresponding to your requests. This will be put into a folder in the run directory called "root-files" that will automatically get made.

The important thing to take note of is the config file that is used for the job. That is up to the user to setup according to their workspace, see [Section 3.2](#config).
``We always recommend you launch any scan from the run directory.``
It is easiest to make a jobList*.py file for your analysis. You can see several examples checked in for H4l Differential, Couplings, POs, etc. Make sure you import your new one into submitScan.py
### 3.1 Submitting Scans <a name="submit"></a>
Look inside source/scripts/submitScan.py and check what scans in the scansToRun list are commented in. Those are the ones that will be submitted. The list is shown below, where we are submitting the 1D nll scan from our jobList that we have setup previously. Make sure you are using the correct list (e.g. jobListExample, jobListDiff, etc) and that it is imported.
```
scansToRun = [
   jobListExample.scan1D, 
   # jobListExample.scan2D,
   # jobListExample.ranking,
   # jobListExample.compatibility,
   # jobListExample.correlationHist,
   # jobListExample.unCondWS,
]  
```
1. Running Locally
In the **run folder** we use the command:
```
python ../source/scripts/submitScan.py --runLocal
```
You can also add (--maxProcs <maxNum> to limit or increase the number of parallel processes run on your computer).
2. Running on Condor/Slurm
 In the **run folder** we use the following command for submitting on condor (ex. lxplus):
```
python ../source/scripts/submitScan.py --submitCondor
```
Or this command for submitting on slurm (many home institutes):
```
python ../source/scripts/submitScan.py --submitSlurm
```
You have the additional option of --queue <queueName> where you can use different queue of condor depending on how long your job will take.
3. Runing on the Grid
 In the **run folder** we use the command:
```
python ../source/scripts/submitScan.py --sumbitGrid --uniqueID <yourID>
```
Afterwards
```
mkdir gridDownloads
rucio download user.name*yourID* --dir gridDownloads
python ../source/scripts/unTarDatasets.py --inputDir gridDownloads
```

4. In general
You have the options:
```
--splitJob <numOfJobSplits>
--retry
```
Retry will only only retry failed jobs when you run locally or on condor. When debuging, we suggest doing --dryRun and sourcing an individual jobList. You can add --debug to the command in the jobList file to get more information.
### 3.2 Config Files <a name="config"></a>
This file is where you can define your workspace. The instructions will be executed line-by-line so the lines further down the text file overwrite the previous ones.
All files should follow a general pattern. Here is an example of a config ready to scan an mu workspace for the H4l analysis. Under POI regex is used to setup the POIs themselves. You can see that all POIs with name mu* are first set to float, but afterwards on the next line the total mu is fixed as constant in the fit. This line takes overwrites what the previous one did. This allows you the freedom to float sidebands, fix BSM POIs, set lwr limits, etc.

This is followed by [Sys - all] and the same type of regexing expressions. If there are special NP you for some reason want to hold constant, you could do it on the next line. You can see the pattern that evolves [sys - statOnly] fixes all NPs, [sys - modTheory] floats everything but the theory systematics. You can write any scan you want here and the call in jobList inputDic["scanType"] = ["statOnly", "allSys", "modTheory"]. Note that these names are not case sensitive.


Lastly there is a part called [Global]. This is important as it holds the names for your datasets, modelConfig, etc. You can see this is where you would give names of your snapshot, change the fit strategy, etc.

```
[POI]
name:mu*, float:true, centralVal:1, lwrLim:-10, uprLim:10
name:mu, float:false, centralVal:1, lwrLim:-10, uprLim:10
name:r*, float:true, centralVal:1, lwrLim:-10, uprLim:10
name:mu_ttH, lwrLim:0, uprLim:10

[Sys - allSys] 
name:*, float:true, centralVal:0

[Sys - statOnly] 
name:*, float:False, centralVal:0

[Sys - modTheory] 
name:*, float:true, centralVal:0
name:*PDF*, float:false
name:*pdf*, float:false
name:*QCD*, float:false
name:*ggF*Comp*, float:false
name:*ggF_heavyFlavour*, float:false
name:*ttV_genComp*, float:false
name:*r_bbH_ggH*, float:false

[Compatibility - singlePOI] 
name:mu_*, float:False, centralVal:0

[Global]
workspaceName: combined
modelConfigName: ModelConfig
datasetName: obsData
asimovName: asimovData
fitStrategy: 1
applyOffset: false
applyRooStartMomentFix: false
loadSnapshot: false
snapshotName: noSnapshot
unconditionalSnap: noSnapshot
optimizeConst: 2
epsVal:0.01

# Information on the workspace. To help with the plotting
workspaceType: mu
labelName: H #rightarrow ZZ* #rightarrow 4l
```
There are more advanced options that can be added to the config. You can add modifyVar that will set other parameters not stored in your POI, or NP list
```
[ModifyVar]
name:Muqq*, centralVal:1, float:True
```
You can also have a seperate setup for toys
```
[Toys - POISetup]
name:kappa*, centralVal:1
[Toys - NPSetup]
name:*, centralVal:0
[Toys - OtherSetup]
name:Muqq*, centralVal:1
[Toys - FitSetup]
name:*, float:False
[Toys - scanSetup]
Dimesion:2
poiInfo:kappaB:15:-4.2:8.5,kappaC:15:-12:12
```
That lets you setup your toy scan seperately from your unconditional fit.

You can also add
```
modelConfigName: ModelConfig_reparam
uncModelConfigName: ModelConfig
```
to your [Global] portion in your config. This will perform your unconditional fit from your unparamterized ModelConfig, and do your scan from your reparameterzied one. This is useful for things like interpretations.
For scan datasets you can have:
```
[scanDatasets - scanSetup]
Dimension: 1
poiName: mH
MinosFit: True
```
Telling it to do a MinosFit for each dataset, or you can do a full 1D/2D scan as shown above for toys.


## 4. Plotting <a name="plotting"></a>
There is one python script that controls plotting (and also combines files from different jobs). For 1D/2D/ranking/1DToys/2DToys/scanDataset the plotting command is simple:
```
python ../source/scripts/plotScans.py --inputDir root-files/scanFolder
```
Which will make a directory in plots with the scans. For correlation Hist
```
python ../source/scripts/plotScans.py --inputDir root-files/scanFolder --impactPOIs mu1,mu2,etc
```
we let the user provide a comma seperated list in a specific order of their chosing so they can decide how the axis appear. They must have the same name as they did in the workspace. Lastly, you can plot toys with the simple command, or if you have the corresponding 1D/2D scan, you can overlay them by doing:
```
python ../source/scripts/plotScans.py --inputDir root-files/scanFolder --toyFile root-files/path-to-merged.root
```

## 5. Package Structure and Philosophy <a name="structure"></a>
### Scanning
The philosophy for this code is to have one base class that controls the main functionality of the scanning code. The name of this class is scanBase with the function run() being responsible for the execution of setting up the job, running the scan, and saving the output. There are derived classes called Scan1D, scan2D and rankNP,etc that only overwrite a few of the functions in scanBase, specifically the runScan function that controls the setting of the POI and running the minimization. You can easily write another class if you are interesting in something particular, such as a 3D scan, in much the same way. 

The code is steered by the utils/runScan.cxx. This instantiates the correct class based on the input dimension, then run() is called with executes the scan.

### Submission
There is one file that controls all submission types. The code is setup well to be called in the *run directory* where it will make a root-files directory which will contain a folder with the name of the outputDir in the jobList as you defined it. Inside that folder there will be directories depending on the scans you have requested. 

This scripts/submitScans.py works by taking the python dictionary given from jobList and generating a commandList. These commands are command to any type of running style you request. For local running, they are put into .sh files in jobList then source in parallel. For condor running, their location in jobList is recorded in the condorexe so they can be source. For the grid the commands in jobList are tared up, unpacked on the grid, and run sequentially.

### Plotting
Here again the main philosophy is to have one script that is responsible for all plotting regardless scan type. The script scripts/plotScans.py is designed to take in the folder structure made by the submission script and plot it. You pass it the inputDir (root-file/folderName) and it will automatically make a folder plots/folderName inside the run directory with the plots you wanted. 

This script works a bit differently where it inside calls plot1D.cxx, plot2D.cxx, plotRanking.cxx, etc. These calls to different scripts are handled by the python code automatically since it opens the metaData information in the folder you provide it and figures out which script to call. 

You can of course instead run each of these plotting scripts by themselves by looking at the input options for each scripts. Do --help on any of these exectuables to see the options.



