// Local includes
#include "CharmCutCode/AnalysisSelfCouplingTest.h"
#include "CharmCutCode/TreeContainer.h"
#include <iomanip>
#include <TFile.h>
#include <fstream>


using namespace std;

// Base class 
AnalysisSelfCouplingTest::AnalysisSelfCouplingTest():
    AnalysisBase()
{}
AnalysisSelfCouplingTest::~AnalysisSelfCouplingTest()
{}


void AnalysisSelfCouplingTest::run()
{

    std::vector<std::string> flavourCategory {"B", "C", "S", "G"};
    std::vector<std::string> fitCategory {"Inclusive"};

    
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
    varMember<ROOT::VecOps::RVec<float>> jet_phi {tree, "jet_phi"};
    varMember<ROOT::VecOps::RVec<float>> jet_mass {tree, "jet_mass"};



    // Loop over the trees here
    for(int i = 0; i < nEntries; i++)
    {
        treeCont->getEntry(i);
        // Just to store how many events were run over
        countingHist->Fill(1);

        if(i % 10000 == 0) std::cout<<"Done i: "<<i<<" out of "<<nEntries<<std::endl;

        for(int i = 0; i < jet_mass.size(); i++)
        {
            std::cout<<"Jet: "<<i<<" phi: "<<jet_phi.at(i)<<" mass: "<<jet_mass.at(i)<<std::endl;
        }
    }

}


