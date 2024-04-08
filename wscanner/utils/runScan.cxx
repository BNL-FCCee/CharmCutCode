// C++ includes
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>

// ROOT includes
#include <TH1F.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TStopwatch.h>


// package includes
#include "log.h"
#include "commonCode.h"
#include "scanBase.h"
#include "scan1D.h"
#include "scan1DToys.h"
#include "scan2D.h"
#include "scan2DToys.h"
#include "scanMinosError.h"
#include "scanDatasets.h"
#include "rankNP.h"
#include "computeCompatibility.h"
#include "runToys.h"
#include "scanPulls.h"

using namespace std;

bool cmdline(int argc, char** argv, map<TString, TString>& opts, map<TString, TString>& extraOpts);
void readConfig(map<TString, TString>& opts, map<TString, TString>& extraOpts);
// std::vector<TString> tokenizeStr(TString str, TString key);
void printWelcome();

int main(int argc, char *argv[])
{   
    TStopwatch timer;
    timer.Start();

    printWelcome();

    // read in command line arguments
    map<TString, TString> opts;
    map<TString, TString> extraOpts;
    if(!cmdline(argc,argv, opts, extraOpts)) return 0;

    if(opts["debug"])LOG::ReportingLevel() = LOG::FromString("DEBUG");
    else LOG::ReportingLevel() = LOG::FromString("INFO");
    
    // read the config
    readConfig(opts, extraOpts);
    

    LOG(logINFO)  << "The run options for the job are";
    for(const auto& options:opts)
    {
        LOG(logINFO) << options.first << " " << options.second;
    }

    // get scanType (1d, 2d, ranking, computeCompatibility)
    scanBase* scanner = NULL;
    if(opts["dimension"].EqualTo("1D"))
    {
        cout << "Making a scan 1D" << endl;
        scanner = new scan1D(opts);
    }
    else if(opts["dimension"].EqualTo("1DToys"))
    {
        scanner = new scan1DToys(opts);
    }
    else if(opts["dimension"].EqualTo("2D"))
    {
        scanner = new scan2D(opts);
    }
    else if(opts["dimension"].EqualTo("2DToys"))
    {
        scanner = new scan2DToys(opts);
    }
    else if(opts["dimension"].Contains("rank"))
    {
        scanner = new rankNP(opts);
    }
    else if(opts["dimension"].Contains("compatibility"))
    {
        scanner = new computeCompatibility(opts);
    }
    else if(opts["dimension"].Contains("correlationHist"))
    {
        scanner = new scan1D(opts);
    }
    else if(opts["dimension"].Contains("saveUncondws"))
    {
        scanner = new scan1D(opts);
    }
    else if(opts["dimension"].Contains("savePulls"))
    {
        cout << "Making a scanPulls" << endl;
        scanner = new scanPulls(opts);
    }
    else if(opts["dimension"].Contains("genAsimov"))
    {
        scanner = new scan1D(opts);
    }
    else if(opts["dimension"].Contains("runToys"))
    {
        scanner = new runToys(opts);
    }
    else if(opts["dimension"].Contains("minosError"))
    {
        scanner = new scanMinosError(opts);
    }
    else if(opts["dimension"].Contains("scanDatasets"))
    {
        cout << "Making a scanDatasets" << endl;
        scanner = new scanDatasets(opts);
    }
    scanner->run();
    
    delete scanner;

    timer.Stop();
    timer.Print();

    return 0;

}


bool cmdline(int argc, char** argv, map<TString, TString>& opts, map<TString, TString>& extraOpts)
{
    //opts.clear();

    opts["config"]          = "../source/data/defaultConfig.txt";
    opts["scanType"]        = "statOnly";
    opts["dimension"]       = "1D";
    opts["fileName"]        = "";
    opts["poiScan"]         = "sigmatot:50:10:20,secondVar:50:10:20";
    opts["doData"]          = "false";
    opts["outputDir"]       = "scanResults";
    opts["saveCorrHist"]    = "false";
    opts["grid"]            = "false";
    opts["debug"]           = "false";
    opts["seed"]            = "13151515";
    opts["outFileName"]     = "nllScan";
    opts["saveUncondWorkspace"]     = "false";
    opts["extraVarSave"]     = "";
    opts["forceFree"]        = "";
    opts["forceFix"]         = "";


    for(int i = 1; i < argc; ++i)
    {
        string opt=argv[i];

        if(opt=="--help" || opt == "--options" || opt =="--h")
        {
            cout<< "Options menu \n ============================================== \n" << endl;
            cout<<"--config         : <path to config file with inclusions/exlusions of systematics for each type of scan"<< endl;
            cout<<"--scanType       : Ex. stat, sys,  theory, you can define these in the config"<<endl;
            cout<<"--fileName       : path to workspace"<<endl;
            cout<<"--poiScan        : POI to scan - format poiName:Steps:Lower Limit:Upper Limit"<<endl;
            cout<<"--datasetType    : Asimov or data?"<<endl;
            cout<<"--outputDir      : output directory"<<endl;
            cout<<"--saveCorrHist   : save the correlation hist? Default is true"<<endl;
            cout<<"--grid           : setting this to true will override variables that make it allows it to run on the grid"<<endl;
            cout<<"--debug          : to turn on output for debugging"<<endl;
            
            return false;
 
        }

        if(0!=opt.find("--")) {
            cout<<"ERROR: options start with '--'!"<<endl;
            cout<<"ERROR: options is: "<<opt<<endl;
            return false;
        }
        opt.erase(0,2);
        if(opt == "debug")                  {opts["debug"] = "true"; continue;}
        if(opt == "grid")                   {opts["grid"] = "true"; continue;}
        if(opt == "saveCorrHist")           {opts["saveCorrHist"] = "true"; continue;}
        if(opt == "doData")                 {opts["doData"] = "true"; continue;}
        if(opt == "saveUncondWorkspace")    {opts["saveUncondWorkspace"] = "true"; continue;}
        if(opt == "genAsimov")              {opts["genAsimov"] = "true"; continue;}

        
        // If not in the map, add it to the extraVars
        if(opts.find(opt)==opts.end()) 
        {
            extraOpts[opt] = "";
        }

        string nxtopt=argv[i+1];
        if(0==nxtopt.find("--")||i+1>=argc) {
            cout<<"ERROR: option '"<<opt<<"' requires value!"<<endl;
            return false;
        }
        if(opts.find(opt)!=opts.end())  opts[opt]=nxtopt;
        else if (extraOpts.find(opt)!=extraOpts.end())  extraOpts[opt]=nxtopt;
        else
        {
            cout<<"ERROR: invalid option '"<<opt<<"'!"<<endl;
            return false;
        }

        i++;
    }

    if(opts["dimension"].Contains("saveUncondws")) opts["saveUncondWorkspace"] = "true";

    return true;
}


void readConfig(map<TString, TString>& opts, map<TString, TString>& extraOpts)
{
    std::string line;
    TString myLine;
    TString indexKey;
    TString indexKeyBase;
    int index = 0;

    ifstream inFile;
    inFile.open(expandEnvironmentVariables(opts["config"].Data()));

    while(!inFile.eof() && inFile.good())
    {
        getline(inFile, line);
        myLine = line;
        myLine.ReplaceAll(" ", "");
        myLine.ReplaceAll("\t", "");

        if(myLine.IsWhitespace() || myLine.BeginsWith("#")) continue;

        if(myLine.Contains("[") && myLine.Contains("]"))
        {
            
            index = 0;
            myLine.ReplaceAll("[","");
            myLine.ReplaceAll("]", "");
            if(myLine.Contains("-"))
            {
                std::vector<TString> splitVec = tokenizeStr(myLine, "-");
                indexKeyBase = splitVec.at(0) + "Info_";
                vector<TString> remainVect{splitVec.begin() + 1, splitVec.end()};

                if(!splitVec.at(1).EqualTo("all"))
                {
                    vector<TString> remainVect{splitVec.begin() + 1, splitVec.end()};
                    indexKeyBase += joinList(remainVect, ":") + "_";
                }

            }
            else if(!myLine.Contains("-"))
            {
                if(myLine.Contains("Global"))
                {
                    indexKeyBase = "NormalOpt";
                }
                else
                {
                    indexKeyBase = myLine + "Info_";
                }
            }
            continue;
        }

        indexKey = indexKeyBase + to_string(index);
        if(indexKeyBase.EqualTo("NormalOpt"))
        {
            std::vector<TString> splitVec = tokenizeStr(myLine, ":");
            vector<TString> remainVect{splitVec.begin() + 1, splitVec.end()};
            opts[splitVec.at(0)] = joinList(remainVect, ":");
        }
        else
        {
            opts[indexKey] = myLine;
        }   
        index++;

    }

    inFile.close();

    // Overwrite the config with extra flags... but only if it exits in the config. Otherwise exit
    for(const auto& info: extraOpts)
    {
        if(opts.find(info.first) == opts.end())
        {
            LOG(logERROR)<<"Can't find the extra key in the config";
            LOG(logERROR)<<"key: "<<info.first<<" val: "<<info.second;
            exit(1);
        }

        // otherwise overwrite it
        opts.at(info.first) = info.second;
    }
}



void printWelcome()
{
cout << "\033[1;35m======================================== \033[0m" << endl;
cout << "\033[1;35m       .  ..-. .-  .  . .. ..--.-.       \033[0m" << endl;
cout << "\033[1;35m       |/\\|`-.(   /_\\ |\\||\\||- |-'   \033[0m" << endl;
cout << "\033[1;35m       '  '`-' `-'   '' '' ''--'`-       \033[0m" << endl;
cout << "\033[1;35m       Last major update: Feb 2019       \033[0m" << endl;   
cout << "\033[1;35m======================================== \033[0m" << endl;                          
#ifdef WITH_ROOFITEXT
cout << "\033[1;35m Running with RooFitExtensions \033[0m" << endl;                          
#endif
}



