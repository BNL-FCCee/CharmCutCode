// Local includes
#include "PostCutCode/AnalysisBase.h"
#include "PostCutCode/TreeContainer.h"
#include <iomanip>
#include <TFile.h>
#include <nlohmann/json.hpp>
#include <fstream>


using namespace std;


void AnalysisBase::writeHistogram()
{
    // using json = nlohmann::json;
    std::ifstream f(MDC::GetInstance()->getSOWJSONFile());
    nlohmann::json data = nlohmann::json::parse(f);

    // override the sum of weights, if it is inside the extra files that we built by hand
    std::ifstream customF(MDC::GetInstance()->getCustomSOWJSONFile());
    nlohmann::json customData = nlohmann::json::parse(customF);

    auto sName = MDC::GetInstance()->getSampleName();

    double normWeight = (double)data[sName]["crossSection"]/(double)data[sName]["sumOfWeights"];
    
    // If the information is in the custom file, scale it
    if(customData.contains(sName))
    {
        normWeight = (double)data[sName]["crossSection"]/(double)customData[sName]["sumOfWeights"];
    }
    
    // Close the outputFile
    auto outFile = TFile::Open(MDC::GetInstance()->getOutputFileName().c_str(), "recreate");
    
    // get all the hist
    auto histList = m_histContainer->getHistList();

    outFile->mkdir("Nominal");
    outFile->cd("Nominal");
    
    for(const auto& h: histList)
    {
        // if the keyword ObsHist is in the name, normalize it
        TString histName = h->GetTitle();
        if(!histName.Contains("counting"))
        {
            h->Scale(normWeight);
        }
        h->Write();
    } 
    
    outFile->Close();
}