// Local includes
#include "CharmCutCode/AnalysisZHvvJJ.h"
#include "CharmCutCode/TreeContainer.h"
#include <iomanip>
#include <TFile.h>
#include <nlohmann/json.hpp>
#include <fstream>


using namespace std;

// Base class 
AnalysisZHvvJJ::AnalysisZHvvJJ()
{
    m_histContainer = std::make_shared<HistContainer>();;
}
AnalysisZHvvJJ::~AnalysisZHvvJJ()
{
}




void AnalysisZHvvJJ::run()
{
    // Get the histograms
    auto scoreMapHist = m_histContainer->getFlavourCategoryHist();
    auto scoreMapFitCatHist = m_histContainer->getFitCategoryHist();
    auto obsHist = m_histContainer->getObsHistinFitCategory();

    // Extra hist for easy fill up
    auto BH_obsHist = obsHist["BH"];
    auto BM_obsHist = obsHist["BM"];
    auto BL_obsHist = obsHist["BL"];
    auto CH_obsHist = obsHist["CH"];
    auto CM_obsHist = obsHist["CM"];
    auto CL_obsHist = obsHist["CL"];
    auto SH_obsHist = obsHist["SH"];
    auto SM_obsHist = obsHist["SM"];
    auto SL_obsHist = obsHist["SL"];
    auto GH_obsHist = obsHist["GH"];
    auto GM_obsHist = obsHist["GM"];
    auto GL_obsHist = obsHist["GL"];

    // Get the trees
    auto treeCont = std::make_shared<TreeContainer>();

    std::cout<<"sampleName: "<<MDC::GetInstance()->getSampleName()<<" events: "<<treeCont->getEntries()<<std::endl;

    // Get max events to run on
    int nEntries = treeCont->getEntries();
    if(MDC::GetInstance()->getNEvents() > 0) nEntries = MDC::GetInstance()->getNEvents();
    auto tree = treeCont->getTree();


    // Connect branches to trees
    varMember<float> B {tree, "B"};
    varMember<float> C {tree, "C"};
    varMember<float> S {tree, "S"};
    varMember<float> G {tree, "G"};
    varMember<double> M_jj {tree, "M_jj"};
    varMember<double> Mrec_jj {tree, "Mrec_jj"};
    varMember<float> muons_p {tree, "muons_p"};
    varMember<float> electrons_p {tree, "electrons_p"};
    varMember<double> costhetainv {tree, "costhetainv"};

    int NafterCut = 0;
    int BlikeEvents = 0;
    int ClikeEvents = 0;
    int SlikeEvents = 0;
    int GlikeEvents = 0;
    std::array<int, 3> BlikeEvents_cat {0, 0, 0};
    std::array<int, 3> ClikeEvents_cat {0, 0, 0};
    std::array<int, 3> SlikeEvents_cat {0, 0, 0};
    std::array<int, 3> GlikeEvents_cat {0, 0, 0};
    // Loop over the trees here
    for(int i = 0; i < nEntries; i++)
    {
        treeCont->getEntry(i);

        if(i % 10000 == 0) std::cout<<"Done i: "<<i<<" out of "<<nEntries<<std::endl;

        // std::cout<<"Event i "<<i<<" "<<B()<<" Mrec_jj: "<<Mrec_jj()<<" M_jj:"<<M_jj()<<std::endl;

        // reject events with leptons with > 20 GeV leptons
        if(muons_p() > 20) continue;
        if(electrons_p() > 20) continue;
        // To figure out?
        if(std::abs(costhetainv()) > 0.85) continue;
        
        // increment counter
        NafterCut++;

        // Find the max score
        auto max_value = std::max({B(), C(), S(), G()});

        // if B is the highest score
        if (max_value == B()) 
        {    
            BlikeEvents++;
            if (B() < 1.1)
            {
                BL_obsHist->Fill(M_jj(), Mrec_jj());
                BlikeEvents_cat[0]++;
            }
            else if ((B() >= 1.1) && (B() <= 1.9))
            {
                BM_obsHist->Fill(M_jj(), Mrec_jj());
                BlikeEvents_cat[1]++;
            }
            else
            {
                BH_obsHist->Fill(M_jj(), Mrec_jj());
                BlikeEvents_cat[2]++;
            }
        }
        else if (max_value == C())
        {
            ClikeEvents++;
            if (C() < 1.0)
            {
                CL_obsHist->Fill(M_jj(), Mrec_jj());
                ClikeEvents_cat[0]++;
            }
            else if ((C() >= 1.0) && (C() <= 1.8))
            {
                CM_obsHist->Fill(M_jj(), Mrec_jj());
                ClikeEvents_cat[1]++;
            }
            else
            {
                CH_obsHist->Fill(M_jj(), Mrec_jj());
                ClikeEvents_cat[2]++;
            }
        }
        else if (max_value == S())
        {  

            SlikeEvents++;
            if (S() < 1.1)
            {
                SL_obsHist->Fill(M_jj(), Mrec_jj());
                SlikeEvents_cat[0]++;
            }
            else if ((S() >= 1.1) && (S() <= 1.7))
            {
                SM_obsHist->Fill(M_jj(), Mrec_jj());
                SlikeEvents_cat[1]++;
            }
            else
            {
                SH_obsHist->Fill(M_jj(), Mrec_jj());
                SlikeEvents_cat[2]++;
            }
        }
        else if (max_value == G())
        {    
            GlikeEvents++;
            if (G() < 1.2)
            {
                GL_obsHist->Fill(M_jj(), Mrec_jj());
                GlikeEvents_cat[0]++;
            }
            else if ((G() >= 1.2) && (G() <= 1.5))
            {
                GM_obsHist->Fill(M_jj(), Mrec_jj());
                SlikeEvents_cat[1]++;
            }
            else
            {
                GH_obsHist->Fill(M_jj(), Mrec_jj());
                GlikeEvents_cat[2]++;
            }   
        }
        else
        {
            std::cout<<B()<<" "<<C()<<" "<<S()<<" "<<G()<<" couldn't find best score"<<std::endl;
        }
    }


    scoreMapHist->SetBinContent(1, BlikeEvents*100./NafterCut);
    scoreMapHist->SetBinContent(2, ClikeEvents*100./NafterCut);
    scoreMapHist->SetBinContent(3, SlikeEvents*100./NafterCut);
    scoreMapHist->SetBinContent(4, GlikeEvents*100./NafterCut);



    for(int i = 0; i < 3; i++)
    {
        scoreMapFitCatHist->SetBinContent(1+i*3, BlikeEvents_cat[i]*100./NafterCut);
        scoreMapFitCatHist->SetBinContent(2+i*3, ClikeEvents_cat[i]*100./NafterCut);
        scoreMapFitCatHist->SetBinContent(3+i*3, SlikeEvents_cat[i]*100./NafterCut);
        scoreMapFitCatHist->SetBinContent(4+i*3, GlikeEvents_cat[i]*100./NafterCut);
    }


  
}


void AnalysisZHvvJJ::finalize()
{
    // using json = nlohmann::json;
    std::ifstream f(MDC::GetInstance()->getSOWJSONFile());
    nlohmann::json data = nlohmann::json::parse(f);
    double normWeight = (double)data[MDC::GetInstance()->getSampleName()]["crossSection"]/(double)data[MDC::GetInstance()->getSampleName()]["sumOfWeights"];
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
        if(histName.Contains("ObsHist"))
        {
            h->Scale(normWeight);
        }
        h->Write();
    } 
    
    outFile->Close();
}