// Local includes
#include "WSMaker/CategoryContainer.h"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include <filesystem>
#include <fstream>

using namespace std;

void CategoryContainer::processHist()
{
    fillYieldInfo();

    cleanHist();

    rebinHist();

    clearOverUnderFlow();
    createFlatNonEmptyHist();

}

// Rebin the outside histograms
void CategoryContainer::rebinHist()
{
    for(const auto& procHists:m_histList)
    {
        for(const auto& hist: procHists.second)
        {
            TClass* cl = gROOT->GetClass(hist.second->ClassName());
            if (cl->InheritsFrom("TH2"))
            {
                TH2* cHist = (TH2*) hist.second;
                if(m_rebinX > 1) cHist->RebinX(m_rebinX);  
                if(m_rebinY > 1) cHist->RebinY(m_rebinY);  
            }
            else if (cl->InheritsFrom("TH1"))
            {
                if(m_rebinX > 1) hist.second->RebinX(m_rebinX);  
                if(m_rebinY > 1)
                {
                    std::cout<<hist.second->GetName()<<" TH1 is being configured to be rebinned on Y axis. Ignore this for now"<<std::endl;
                } 
            }
            else
            {
                std::cout<<hist.second->GetName()<<" type cannot be figured out for rebinning. Ignoring"<<std::endl;
            }
        }
    }
}

// Get the yields from the histograms
void CategoryContainer::fillYieldInfo()
{
    for(const auto& procHists:m_histList)
    {
        for(const auto& hist: procHists.second)
        {
            m_yieldList[procHists.first][hist.first] = hist.second->Integral();
            // std::cout<<m_catName<<" "<<procHists.first<<" "<<hist.first<<" "<<m_yieldList[procHists.first][hist.first]<<std::endl;
        }
    }
}

// remove processes with zero yield
void CategoryContainer::cleanHist()
{
    std::set<std::string> processToRemove;
    auto processList = getProcessList();
    for(const auto& proc: processList)
    {
        if(m_yieldList["Nominal"][proc] == 0)
        { 
            processToRemove.insert(proc);
            cout<<"\033[1;31mStupid Removal:\033[0m Removing process due to zero yeild: "<<m_catName<<" "<<proc<<endl;
        }
    }

    // remove from all maps
    for(auto& procHists: m_histList)
    {
        for(auto& proc: processToRemove)
        {
            procHists.second.erase(proc);
            m_yieldList[procHists.first].erase(proc);
        }
    }
}


// Clear OverUnderFlow bins so they are not counted for the hist flattening 
void CategoryContainer::clearOverUnderFlow()
{
    for(const auto& procHists:m_histList)
    {
        for(const auto& hist: procHists.second)
        {
            hist.second->ClearUnderflowAndOverflow();
        }
    }
}


void CategoryContainer::createFlatNonEmptyHist()
{
    // First get the number of nonEmpty bins
    for(const auto& procHists:m_histList)
    {
        for(const auto& hist: procHists.second)
        {
            // Add all the hist into one combined on
            if(!m_sumHist)
            {
                m_sumHist = (TH1*) hist.second->Clone(("combHist" + m_catName).c_str());
                m_sumHist->Reset();
            }
            m_sumHist->Add(hist.second);
        }
    }

    // Get the number of non-zero bins
    int nNonZeroBins = 0;
    for(int i = 0; i < m_sumHist->GetNcells(); i++)
    {
        if(m_sumHist->GetBinContent(i+1) != 0) nNonZeroBins++;
    }

    std::cout<<m_catName<<" found nonzero bins: "<<nNonZeroBins<<std::endl;

    // Create flat versions of the hist
    // First get the number of nonEmpty bins
    for(const auto& procHists:m_histList)
    {
        for(const auto& hist: procHists.second)
        {
            TString histName = TString(hist.second->GetTitle()) + "_flat";
            TH1D* flatHist = new TH1D(histName, histName, nNonZeroBins, 0, nNonZeroBins);

            int index = 0;
            for(int i = 0; i < m_sumHist->GetNcells(); i++)
            {
                if(m_sumHist->GetBinContent(i+1) != 0)
                {
                    flatHist->SetBinContent(index, hist.second->GetBinContent(i+1));
                    flatHist->SetBinError(index, hist.second->GetBinError(i+1));
                    index++;
                }
            }
            // std::cout<<m_catName<<" yield: "<<m_yieldList[procHists.first][hist.first]<<" "<<hist.second->GetTitle()<<" hist: "<<hist.second->Integral()<<" flat: "<<flatHist->Integral()<<" "<<flatHist->GetEntries()<<std::endl;
            
            flatHist->Scale(1.0/flatHist->Integral());
            m_flatHistList[procHists.first][hist.first] = flatHist;
        }
    }
}

void CategoryContainer::writeHistToRoot()
{
    TString path = m_outputDir + "/hist";
    std::filesystem::create_directories(path.Data());

    TFile* outFile = new TFile(getRootOutputFileName().c_str(), "recreate");

    for(const auto& procHists:m_flatHistList)
    {
        outFile->mkdir(procHists.first.c_str());
        outFile->cd(procHists.first.c_str());
        
        for(const auto& hist: procHists.second)
        {
            hist.second->Write();
        }
        outFile->cd();
    }

    m_sumHist->Write();

    outFile->Close();

}


void CategoryContainer::writeXML()
{
    TString path = m_outputDir + "/XML/";
    std::filesystem::create_directories(path.Data());
    std::filesystem::copy_file("../source/WSMaker/data/HistFactorySchema.dtd", (path +  "/HistFactorySchema.dtd").Data(), std::filesystem::copy_options::skip_existing);

    std::ofstream oFile(getXMLFileName().c_str());

    oFile<<"<!DOCTYPE Channel SYSTEM 'HistFactorySchema.dtd'> \n"<<endl;
    oFile<<"<Channel Name=\"R_"<<m_catName<<"\" HistoPath=\"\"> \n"<<endl;

    // If we want to do stat err
    oFile<<"<StatErrorConfig RelErrorThreshold=\"0.1\" ConstraintType=\"Poisson\" /> \n"<<endl;

    auto processList = getProcessList();

    for(const auto& proc: processList)
    {
        // For each process
        oFile<<"<Sample Name=\""<<proc<<"\" InputFile=\""<<getRootOutputFileName()<<"\" HistoName=\""<<m_flatHistList.at("Nominal").at(proc)->GetName()<<"\" HistoPath=\"/Nominal/\" NormalizeByTheory=\"False\" >"<<endl;

        TString normName = "N_" + proc + "_" + m_catName;
        oFile<<"<NormFactor Name=\""<<normName<<"\" Val=\""<<m_yieldList.at("Nominal").at(proc)<<"\" Low=\"-40\" High=\"40\" />"<<endl;
        m_constVarList.push_back(normName.Data());
        oFile<<"<NormFactor Name=\"Lumi_cat\" Val=\""<<m_lumi<<"\" Low=\"0\" High=\"1000000000\" />"<<endl;
        m_constVarList.push_back("Lumi_cat");

        if(std::find(m_constProcList.begin(), m_constProcList.end(), proc) == m_constProcList.end())
        {
            TString poiName = "mu_" + proc;
            oFile<<"<NormFactor Name=\""<<poiName<<"\" Val=\""<<1<<"\" Low=\"0\" High=\"20\" />"<<endl;
            m_poiList.push_back(poiName.Data());

        }

        oFile<<"</Sample> \n"<<endl;
    }



    oFile<<"</Channel> \n"<<endl;


    oFile.close();

}

std::set<std::string> CategoryContainer::getProcessList()
{
    std::set<std::string> processList;
    for(const auto& procHists:m_histList["Nominal"])
    {
        processList.insert(procHists.first);
    }
    return processList;
}