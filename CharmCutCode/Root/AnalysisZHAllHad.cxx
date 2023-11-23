// Local includes
#include "CharmCutCode/AnalysisZHAllHad.h"
#include "CharmCutCode/TreeContainer.h"
#include <iomanip>
#include <TFile.h>
#include <fstream>


using namespace std;

// Base class 
AnalysisZHAllHad::AnalysisZHAllHad():
    AnalysisBase()
{}
AnalysisZHAllHad::~AnalysisZHAllHad()
{}


void AnalysisZHAllHad::run()
{

    std::vector<std::string> flavourCategory {"B", "C", "S", "G"};
    std::vector<std::string> fitCategory {"BH", "BM", "BL", "CH", "CM", "CL", "SH", "SM", "SL", "GH", "GM", "GL"};

    
    // Get the histograms
    auto scoreMapHist = m_histContainer->get1DHist("scoreMap_1D", flavourCategory.size(), 0, flavourCategory.size(), flavourCategory);
    auto scoreMapFitCatHist = m_histContainer->get1DHist("scoreMapFitCategory_1D", fitCategory.size(), 0, fitCategory.size(), fitCategory);
    auto obsHist = m_histContainer->getObsHistinFitCategory(fitCategory, 250, 0, 250, 250, 0, 250);
    auto countingHist = m_histContainer->getCountingHist();


    // Get the trees
    auto treeCont = std::make_shared<TreeContainer>();
    std::cout<<"sampleName: "<<MDC::GetInstance()->getSampleName()<<" events: "<<treeCont->getEntries()<<std::endl;

    // Get max events to run on
    int nEntries = treeCont->getEventsToRun();

    // Connect branches to trees
    auto tree = treeCont->getTree();
    varMember<float> jet0_scoreB {tree, "jet0_scoreB"};
    varMember<float> jet1_scoreB {tree, "jet1_scoreB"};
    varMember<float> jet2_scoreB {tree, "jet2_scoreB"};
    varMember<float> jet3_scoreB {tree, "jet3_scoreB"};
    // varMember<double> M_jj {tree, "M_jj"};
    // varMember<double> Mrec_jj {tree, "Mrec_jj"};
    // varMember<float> muons_p {tree, "muons_p"};
    // varMember<float> electrons_p {tree, "electrons_p"};
    // varMember<double> costhetainv {tree, "costhetainv"};


    // Loop over the trees here
    for(int i = 0; i < nEntries; i++)
    {
        treeCont->getEntry(i);
        // Just to store how many events were run over
        countingHist->Fill(1);

        if(i % 10000 == 0) std::cout<<"Done i: "<<i<<" out of "<<nEntries<<std::endl;
    }

}


