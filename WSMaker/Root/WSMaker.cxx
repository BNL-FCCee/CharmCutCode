// c++ include
#include "fstream"

// Local includes
#include "WSMaker/WSMaker.h"
// Root include
#include <TH1.h>
#include <TFile.h>
#include <TROOT.h>
#include <TKey.h>
#include <filesystem>
#include <fstream>

using namespace std;


void WSMaker::run()
{
    // Parse the config 
    parseConfig();

    auto histMap = readFile();

    // TODO: loop over sys to go here
    auto histList = histMap.at("Nominal");
    // For each category create a Category container
    // and in each one of them give the nominal histograms for now
    for(const auto& categoryName: m_categoryList)
    {
        auto cat = std::make_shared<CategoryContainer>(categoryName);
        cat->setXRebin(m_jsonConfigData["xRebinFactor"]);
        cat->setYRebin(m_jsonConfigData["yRebinFactor"]);
        cat->setLumi(m_jsonConfigData["lumi"]);

        std::vector<std::string> constProc = m_jsonConfigData["processHeldConstant"];
        cat->setConstantProc(constProc);
        cat->setOutputDir(m_outputDir);
        m_categoryCont[categoryName] = cat;

        // Find the hist for each category
        std::map<ProcessName, TH1*> processHistMap;

        for(const auto& process: m_processList)
        {
            auto name = "ObsHist_Cat_" + categoryName + "_Process_" + process;

            // Find the histogram
            TH1* cHist = NULL;
            for(auto hist: histList)
            {
                if(TString(hist->GetName()).EqualTo(name))
                {
                    cHist = hist;
                    break;
                }
            }
            if(!cHist)
            {
                std::cout<<"\033[1;31mMISSING:\033[0m hist is missing "<<name<<endl;
                exit(1);
            }
            processHistMap[process] = cHist;
        }

        cat->addHist("Nominal", processHistMap);
    }

    // Preprocess the hist
    for(const auto& cat: m_categoryCont) cat.second->processHist();

    // Write the output
    for(const auto& cat: m_categoryCont) cat.second->writeHistToRoot();

    // Write the XML
    for(const auto& cat: m_categoryCont) cat.second->writeXML();

    WriteXML();
}

void WSMaker::WriteXML()
{
    // XML for overall
    TString path = m_outputDir + "/XML";
    std::filesystem::create_directories(path.Data());

    std::ofstream oFile(path + "/driver.xml");

    oFile<<"<!DOCTYPE Combination SYSTEM 'HistFactorySchema.dtd'>"<<endl;

    path = m_outputDir + "/WS";
    std::filesystem::create_directories(path.Data());
    oFile<<"<Combination OutputFilePrefix=\""<<path<<"\"> \n"<<endl;


    for(const auto& cat: m_categoryCont)
    {
        oFile<<"<Input>"<<cat.second->getXMLFileName()<<"</Input>"<<endl;
    }

    oFile<<"\n<Measurement Name=\""<<m_outputDir<<"\" Lumi=\"1\" LumiRelErr=\"0.0001\" ExportOnly=\"True\">"<<endl;

    TString poiListStr = "";
    std::set<std::string> poiList;
    for(const auto& cat: m_categoryCont)
    {
        auto sPOIList =  cat.second->getPOIList();
        for(auto& poiName: sPOIList)
        {
            poiList.insert(poiName);
        }
    }
    for(auto poiName:poiList)
    {
        poiListStr += poiName + " ";
    }
    oFile<<"<POI>"<<poiListStr<<"</POI>"<<endl;
    oFile<<"<ParamSetting Const=\"True\">Lumi</ParamSetting>"<<endl;
    // Const POIs
    std::set<std::string> constList;
    for(const auto& cat: m_categoryCont)
    {
        auto sConstList =  cat.second->getConstantVarList();
        for(auto& constName: sConstList)
        {
            constList.insert(constName);
        }
    }
    for(auto constName:constList)
    {
        oFile<<"<ParamSetting Const=\"True\">"<<constName<<"</ParamSetting>"<<endl;
    }

    oFile<<"</Measurement>"<<endl;
    oFile<<"</Combination>"<<endl;

    oFile.close();
}

void WSMaker::parseConfig()
{   
    using json = nlohmann::json;

    std::ifstream f(m_jsonFile);
    m_jsonConfigData = json::parse(f);

    // TODO: For whatever reason, json is not doing a conversion dynamically?
    std::vector<std::string> test = m_jsonConfigData["category"];
    m_categoryList = std::vector<std::string>{test};
    std::vector<std::string> test1 = m_jsonConfigData["process"];
    m_processList = std::vector<std::string>{test1};
    std::vector<std::string> test2 = m_jsonConfigData["processHeldConstant"];
    m_processHeldConstantList = std::vector<std::string>{test2};
}

std::map<CatName, std::vector<TH1*>> WSMaker::readFile()
{
    std::cout<<"Reading: "<<m_fileName<<std::endl;
    TFile* cFile = TFile::Open(m_fileName.c_str());

    std::map<CatName, std::vector<TH1*>> histMap;

    // Read all the folder from the TFile and read everything inside there into maps
    TIter next(cFile->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)next())) 
    {
        TString keyName(key->GetName());

        TClass *cl = gROOT->GetClass(key->GetClassName());
        if (!cl->InheritsFrom("TDirectoryFile")) continue;

        // if not do sys, don't even read the systematics
        if(!m_doSys)
        {
            if(!TString(key->GetName()).EqualTo("Nominal")) continue;
        }

        // TODO: This is not supported right now
        // bool skipSys = false;
        // for(const auto& sRm: m_sysToRemoveList)
        // {
        //     if(keyName.Contains(sRm) || matchPattern(keyName, sRm))
        //     {
        //         skipSys = true;
        //         break;
        //     }
        // }
        // if(skipSys) continue;

        // // Add the systematic
        // m_sysList.insert(key->GetName());

        // Read hist and values from the folder
        histMap[key->GetName()] = readFromFolder((TDirectoryFile*) cFile->Get(key->GetName()));
    }
    delete cFile;
    return histMap;
}

// Read the input files from the folder
std::vector<TH1*> WSMaker::readFromFolder(TDirectoryFile* obj)
{
    TIter next(obj->GetListOfKeys());
    TKey* key;

    std::vector<TH1*> histList;
    // Reading the hist
    while ((key = (TKey*)next())) 
    {
        TClass* cl = gROOT->GetClass(key->GetClassName());
        if (!cl->InheritsFrom("TH1")) continue;
        if(!TString(key->GetName()).Contains("ObsHist")) continue;

        // Store the hist
        auto hist = (TH1*) obj->Get(key->GetName());
        hist->SetDirectory(0);
        histList.push_back(hist);
    }
    return histList;
}