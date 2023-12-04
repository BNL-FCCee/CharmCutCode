// Local includes
#include "CharmCutCode/AnalysisZHAllHad.h"
#include "CharmCutCode/TreeContainer.h"
#include <iomanip>
#include <TFile.h>
#include <fstream>

// DEFINE sel_visMtheta, sel_d123, sel_d34! WHICH SEL TO PICK! CAREFUL!
using namespace std;

// Base class 
AnalysisZHAllHad::AnalysisZHAllHad():
    AnalysisBase()
{}
AnalysisZHAllHad::~AnalysisZHAllHad()
{}


void AnalysisZHAllHad::run()
{

    std::vector<std::string> flavourCategory {"B", "C", "S", "G","TAU","Q"};
    std::vector<std::string> fitCategory {"BH", "BM", "BL", "CH", "CM", "CL", "SH", "SM", "SL", "GH", "GM", "GL"};

    
    // Get the histograms
    auto scoreMapHist = m_histContainer->get1DHist("scoreMap_1D", flavourCategory.size(), 0, flavourCategory.size(), flavourCategory);
    auto scoreMapFitCatHist = m_histContainer->get1DHist("scoreMapFitCategory_1D", fitCategory.size(), 0, fitCategory.size(), fitCategory);
    auto obsHist = m_histContainer->getObsHistinFitCategory(fitCategory, 250, 0, 250, 250, 0, 250);
    auto countingHist = m_histContainer->getCountingHist();

    auto score2DHist_j12GC = m_histContainer->get2DHist("j12GC",60,0,3,60,0,3);
    auto score2DHist_j13GC = m_histContainer->get2DHist("j13GC",60,0,3,60,0,3);
    auto score2DHist_j14GC = m_histContainer->get2DHist("j14GC",60,0,3,60,0,3);

    auto score2DHist_j12CC = m_histContainer->get2DHist("j12CC",60,0,3,60,0,3);
    auto score2DHist_j13CC = m_histContainer->get2DHist("j13CC",60,0,3,60,0,3);
    auto score2DHist_j14CC = m_histContainer->get2DHist("j14CC",60,0,3,60,0,3);

    auto score2DHist_j12GG = m_histContainer->get2DHist("j12GG",60,0,3,60,0,3);
    auto score2DHist_j13GG = m_histContainer->get2DHist("j13GG",60,0,3,60,0,3);
    auto score2DHist_j14GG = m_histContainer->get2DHist("j14GG",60,0,3,60,0,3);

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

    varMember<float> jet0_scoreC {tree, "jet0_scoreC"};
    varMember<float> jet1_scoreC {tree, "jet1_scoreC"};
    varMember<float> jet2_scoreC {tree, "jet2_scoreC"};
    varMember<float> jet3_scoreC {tree, "jet3_scoreC"};

    varMember<float> jet0_scoreG {tree, "jet0_scoreG"};
    varMember<float> jet1_scoreG {tree, "jet1_scoreG"};
    varMember<float> jet2_scoreG {tree, "jet2_scoreG"};
    varMember<float> jet3_scoreG {tree, "jet3_scoreG"};

    varMember<int> event_njet {tree, "event_njet"};
    // varMember<long> event_nmu {tree, "event_nmu"};
    // varMember<long> event_nel {tree, "event_nel"};
    varMember<double> vis_E {tree, "vis_E"};
    varMember<double> vis_M {tree, "vis_M"};
    varMember<double> vis_theta {tree, "vis_theta"};
    varMember<float> d_12 {tree, "d_12"};
    varMember<float> d_23 {tree, "d_23"};
    varMember<float> d_34 {tree, "d_34"};

    // varMember<double> M_jj {tree, "M_jj"};
    // varMember<double> Mrec_jj {tree, "Mrec_jj"};

    varMember<float> muons_p {tree, "muons_p"};
    varMember<float> electrons_p {tree, "electrons_p"};
    // varMember<double> costhetainv {tree, "costhetainv"};

    int NafterCut = 0;

    // Loop over the trees here
    for(int i = 0; i < nEntries; i++)
    {
        treeCont->getEntry(i);
        // Just to store how many events were run over
        countingHist->Fill(1);

        if(i % 10000 == 0) std::cout<<"Done i: "<<i<<" out of "<<nEntries<<std::endl;
        // Add some basic selection
        
        if(event_njet() != 4) continue; //Require EXACTLY 4 jet! This SHOULD be the case!
        // reject events with leptons with > 20 GeV leptons
        if(muons_p() > 20) continue;
        if(electrons_p() > 20) continue;
        //less >=2 electrons/muons req.
        // std::cout<<"Number of muons "<<" "<<event_nmu()<<" and the number of electrons: "<<event_nel()<<std::endl;

        // if(event_nmu() > 2) continue;
        // if(event_nel() > 2) continue;
        //Add this  to the options, as in which selection to choose
        if (vis_M()<150) continue;
        if (vis_E()<150) continue;
        if (vis_theta()<1.15 || vis_theta()>3.0 ) continue;
    
        if (d_12()>58000) continue;
        if ((d_23()<400) || (d_23()>18000))continue;
        if ((d_34()<100) || (d_34()>6000))continue;
        // if(sel_visMtheta){
        //     if (vis_M()<150) continue;
        //     if (vis_E()<150) continue;
        //     if (vis_theta()<1.15 || vis_theta()>3.0 ) continue;
        //    // if (vis_theta()>3.0) continue;

        // }
        // if(sel_d123){
        //     if (d_12()>58000) continue;
        //     if ((d_23()<400) || (d_23()>18000))continue;
        // }
        // if(sel_d34){
        //     if ((d d_34<100) || (d_34>6000))continue;
        // }
        // increment counter of events that pass the cuts!
        NafterCut++;

        score2DHist_j12GC->Fill(jet0_scoreG(),jet1_scoreC());
        score2DHist_j13GC->Fill(jet0_scoreG(),jet2_scoreC());
        score2DHist_j14GC->Fill(jet0_scoreG(),jet3_scoreC());

        score2DHist_j12CC->Fill(jet0_scoreC(),jet1_scoreC());
        score2DHist_j13CC->Fill(jet0_scoreC(),jet2_scoreC());
        score2DHist_j14CC->Fill(jet0_scoreC(),jet3_scoreC());

        score2DHist_j12GG->Fill(jet0_scoreG(),jet1_scoreG());
        score2DHist_j13GG->Fill(jet0_scoreG(),jet2_scoreG());
        score2DHist_j14GG->Fill(jet0_scoreG(),jet3_scoreG());
        //Maybe just make 2D hisotgrams of the scores

    }
    //Make histograms for the cats 

}


