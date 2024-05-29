// C++ includes
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>

// Root includes
#include <TString.h>


// Class include
#include "PostCutCode/AnalysisBase.h"
#include "PostCutCode/AnalysisZHvvJJ.h"
#include "PostCutCode/AnalysisZHAllHad.h"
#include "PostCutCode/AnalysisSelfCouplingTest.h"
#include "PostCutCode/MetadataContainer.h"

using namespace std;

map<TString,std::string> opts;
bool cmdline(int argc, char** argv, map<TString,std::string>& opts);
int main(int argc, char** argv)
{

    std::vector<std::string> v;

    if (!cmdline(argc,argv,opts)) return 0;

    MDC::GetInstance()->setMetadata("analType", opts["analType"]);
    MDC::GetInstance()->setMetadata("inputFileList", opts["inputFileList"]);
    MDC::GetInstance()->setMetadata("outputFileName", opts["outputFileName"]);
    MDC::GetInstance()->setMetadata("sampleName", opts["sampleName"]);
    MDC::GetInstance()->setMetadata("processName", opts["processName"]);
    MDC::GetInstance()->setMetadata("SOWJSONfile", opts["SOWJSONfile"]);
    MDC::GetInstance()->setMetadata("CustomSOWJSONfile", opts["CustomSOWJSONfile"]);
    MDC::GetInstance()->setMetadata("DetectorVarFile", opts["DetectorVarFile"]);
    MDC::GetInstance()->setMetadata("DetectorVar", opts["DetectorVar"]);
    MDC::GetInstance()->setMetadata("nEvents", std::atoi(opts["nEvents"].c_str()));

    std::shared_ptr<AnalysisBase>  analysis;

    if(opts["analType"] == "ZHvvJJ") 
    {
        analysis =  std::make_shared<AnalysisZHvvJJ>();
    }
    else if(opts["analType"] == "ZHAllHad") 
    {
        analysis =  std::make_shared<AnalysisZHAllHad>();
    }
    else if(opts["analType"] == "SelfCoupling") 
    {
        analysis =  std::make_shared<AnalysisSelfCouplingTest>();
    }
    else
    {
        std::cout<<"Anal Type not recognized"<<std::endl;
        std::cout<<"Input analType: "<<opts["analType"]<<std::endl;
        exit(1);
    }


    analysis->initialize();
    analysis->run();
    analysis->finalize();

    return 0;
}





bool cmdline(int argc, char** argv, map<TString,std::string>& opts)
{
    opts.clear();

    // defaults
    opts["nEvents"]              = "-1";
    
    opts["inputFileList"]       = "/Users/haider/FCC/data/ntuples/zh_vvjj/wzp6_ee_nunuH_HZZ_ecm240/events_0.root";
    opts["outputFileName"]      = "wzp6_ee_nunuH_HZZ_ecm240_0.root";
    opts["sampleName"]          = "wzp6_ee_nunuH_HZZ_ecm240";
    opts["processName"]         = "qqH";
    opts["analType"]            = "vvjj";
    opts["SOWJSONfile"]         = "../source/PostCutCode/data/FCCee_procDict_winter2023_IDEA.json";
    opts["CustomSOWJSONfile"]   = "../source/PostCutCode/data/SoWcustom_ZHall.json";
    opts["DetectorVarFile"] = "../source/PostCutCode/data/bcs_scores_baseline_idealVXDCalo_lighter_heavierVXD_100pc_CLD_worse_better_singlehitReso_65pc.root";
    opts["DetectorVar"] = "baseline";



    for (int i=1;i<argc;i++) {

        string opt=argv[i];

        if (opt=="--help") {
            cout<<"--nEvents           : Number of events to run (-1 means all events)"<<endl;
            cout<<"--inputFileList     : comma seperated list of inputs files to run on"<<endl;
            cout<<"--outputFileName    : name (including path) for where to store the output file"<<endl;
            cout<<"--sampleName        : sample name as defined in the Sum of weight file provided by FCC collaboration"<<endl;
            cout<<"--processName       : simple process name - becomes a keyword for the ws building"<<endl;
            cout<<"--analType          : analysis type to preform on the inputs - supported options: vvjj"<<endl;
            cout<<"--SOWJSONfile       : path to the Sum of wieght json file"<<endl;
            cout<<"--DetectorVarFile   : Detector variation file to be used, to update the B/C tagging score."<<endl;
            cout<<"--DetectorVar       : Detector variation to be considered, when updating the B/C tagging score."<<endl;


            return false;
        }

        if(0!=opt.find("--")) {
            cout<<"ERROR: options start with '--'!"<<endl;
            return false;
        }
        opt.erase(0,2);
        if(opt == "isGrid")             {opts["isGrid"] = "true"; continue;}

        if(opts.find(opt)==opts.end()) {
            cout<<"ERROR: invalid option '"<<opt<<"'!"<<endl;
            return false;
        }
        string nxtopt=argv[i+1];
        if(0==nxtopt.find("--")||i+1>=argc) {
            cout<<"ERROR: option '"<<opt<<"' requires value!"<<endl;
            return false;
        }
        opts[opt]=nxtopt;
        i++;
    }

    return true;
}


