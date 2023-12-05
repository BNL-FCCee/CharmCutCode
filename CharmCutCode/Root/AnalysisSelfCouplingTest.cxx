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

    std::vector<std::string> fitCategory {"Inclusive"};

    
    // Get the histograms
    auto obsHist = m_histContainer->getObsHistinFitCategory(fitCategory, 100, 100, 150);
    auto countingHist = m_histContainer->getCountingHist();

    auto Inclusive_obsHist = obsHist["Inclusive"];

    // Get the trees
    auto treeCont = std::make_shared<TreeContainer>();
    std::cout<<"sampleName: "<<MDC::GetInstance()->getSampleName()<<" events: "<<treeCont->getEntries()<<std::endl;

    // Get max events to run on
    int nEntries = treeCont->getEventsToRun();

    // Connect branches to trees
    auto tree = treeCont->getTree();
    varMember<ROOT::VecOps::RVec<float>> all_invariant_masses {tree, "all_invariant_masses"};
    varMember<ROOT::VecOps::RVec<float>> recoil_masses {tree, "recoil_masses"};
    varMember<ROOT::VecOps::RVec<float>> recojetpair_isC {tree, "recojetpair_isC"};


    double maxPairCscore = -9;
    double thisPairCscore = -9;
    int highestPairCscore_index = -9;
    double thisPairInvMass = -9; 
    bool foundZccCandidate = false;
    double ZccCandMass;
    double HCandMass;

    // Loop over the trees here
    for(int i = 0; i < nEntries; i++)
    {
        foundZccCandidate = false;
        thisPairCscore = -9;
        maxPairCscore = -9;
        highestPairCscore_index = -9;
        treeCont->getEntry(i);
        
        // Just to store how many events were run over
        countingHist->Fill(1);

        if(i % 10000 == 0) std::cout<<"On event "<<i<<" / "<<nEntries << std::endl;

        for(int j = 0; j < all_invariant_masses.size(); j++)
        {
            
            thisPairInvMass = all_invariant_masses.at(j);
            thisPairCscore = recojetpair_isC.at(j);

            // require invariant mass of that pair to be between 75-110 GeV
            // require sumCscore > 1.6
            if(thisPairInvMass < 75 || thisPairInvMass > 110) continue; 
            if(thisPairCscore < 1.6) continue; 

            // for jet pairs in event passing criteria, choose the one with the highest sumCscore
            if(thisPairCscore > maxPairCscore){
                foundZccCandidate = true;
                maxPairCscore = thisPairCscore;
                highestPairCscore_index = j;
            }

        }

        if(foundZccCandidate){
            ZccCandMass = all_invariant_masses.at(highestPairCscore_index);
            HCandMass = recoil_masses.at(highestPairCscore_index);
            Inclusive_obsHist->Fill(HCandMass);
        }
        
    }

}


