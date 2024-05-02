#include "PostCutCode/HistContainer.h"


HistContainer::HistContainer()
{
}

// For knowing know many events were run over
TH1F* HistContainer::getCountingHist()
{    
    return get1DHist("counting", 10, 0, 10);
}

TH1F* HistContainer::get1DHist(std::string name, int binX, double lwrX, double uprX, std::vector<std::string> optionalBinNames)
{
    name += "_" + MDC::GetInstance()->getProcessName();

    // Check if it exists
    if(getHist(name))
    {
        return (TH1F*) getHist(name);
    }

    // Otherwise create new hist
    auto hist = new TH1F(name.c_str(), name.c_str(), binX, lwrX, uprX);
    registerHist(hist);

    // If provided, rename the bins
    if(optionalBinNames.size() > 0)
    {
        int index = 0;
        for(const auto& v: optionalBinNames)
        {
            hist->GetXaxis()->SetBinLabel(index + 1, v.c_str());
            index++;
        }
    }
    return hist;
}

TH2F* HistContainer::get2DHist(std::string name, int binX, double lwrX, double uprX, int binY, double lwrY, double uprY)
{
    name += "_" + MDC::GetInstance()->getProcessName();
    // Check if it exists
    if(getHist(name))
    {
        return (TH2F*) getHist(name);
    }

    // Otherwise create new hist
    auto hist = new TH2F(name.c_str(), name.c_str(), binX, lwrX, uprX, binY, lwrY, uprY);
    registerHist(hist);
  
    return hist;
}


std::map<std::string, TH2F*> HistContainer::getObsHistinFitCategory(std::vector<std::string> fitCategories, int binX, double lwrX, double uprX, int binY, double lwrY, double uprY)
{
    static std::map<std::string, TH2F*> histList;
    if(histList.size() > 0) return histList;

    for(const auto& c: fitCategories)
    {
        // This is high keywored; It needs to be in ObsHist_Cat_XXX_Process_YYY. The WS building depends on this
        auto name = "ObsHist_Cat_" + c + "_Process_" + MDC::GetInstance()->getProcessName();
        auto hist = new TH2F(name.c_str(), name.c_str(), binX, lwrX, uprX, binY, lwrY, uprY);
        registerHist(hist);
        histList[c] = hist;
    }

    return histList;
}

std::map<std::string, TH1F*> HistContainer::getObsHistinFitCategory(std::vector<std::string> fitCategories, int binX, double lwrX, double uprX)
{
    static std::map<std::string, TH1F*> histList;
    if(histList.size() > 0) return histList;

    for(const auto& c: fitCategories)
    {
        // This is high keywored; It needs to be in ObsHist_Cat_XXX_Process_YYY. The WS building depends on this
        auto name = "ObsHist_Cat_" + c + "_Process_" + MDC::GetInstance()->getProcessName();
        auto hist = new TH1F(name.c_str(), name.c_str(), binX, lwrX, uprX);
        registerHist(hist);
        histList[c] = hist;
    }

    return histList;
}

std::map<std::string, TH3*> HistContainer::histo_DetVarsScoreSmear(std::vector<std::string> jetFlavs){
    //Get Detector varianton 
    auto DetectorVarF = TFile::Open(MDC::GetInstance()->getDetectorVarFile().c_str(), "read");
    if (!DetectorVarF) {
        std::cout << "Error: DetectorVar File not found!" << MDC::GetInstance()->getDetectorVarFile().c_str()<<std::endl;
    }
    static std::map<std::string, TH3*> histos_DetVars;
    if(histos_DetVars.size() > 0) return histos_DetVars;
    
    for(const auto& c: jetFlavs){
        auto name = "histo_" + MDC::GetInstance()->getDetectorVar() + "_H" + c + c;
        std::cout << "Det var histo to get: " << name <<std::endl;
        auto hist = (TH3*) (DetectorVarF->Get(name.c_str()));
        histos_DetVars[c] = hist;
    }
    return histos_DetVars;
}


