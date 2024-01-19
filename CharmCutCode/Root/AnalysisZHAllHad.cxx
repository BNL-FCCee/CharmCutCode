// Local includes
#include "CharmCutCode/AnalysisZHAllHad.h"
#include "CharmCutCode/TreeContainer.h"
#include <iomanip>
#include <TFile.h>
#include <fstream>
#include <set>
#include "TLorentzVector.h"

// DEFINE sel_visMtheta, sel_d123, sel_d34! WHICH SEL TO PICK! CAREFUL!
using namespace std;

// Base class 
// std::vector<unsigned int> AnalysisZHAllHad::missing_pair(std::vector<int> BLA){

    
// }

AnalysisZHAllHad::AnalysisZHAllHad():
    AnalysisBase(),
    m_debug(false)
{}
AnalysisZHAllHad::~AnalysisZHAllHad()
{}


void AnalysisZHAllHad::run()
{       
    float Z_mass = 91.1876;
    float H_mass = 125.11;
    float W_mass = 80.377;

    std::vector<std::string> flavours{"B","C","S","D","U","G","TAU"};
    std::vector<std::string> flavourCategory {"B", "C", "S","Q", "G","TAU"};
    std::vector<std::string> fitCategory {"LowHbb","LowHcc","LowHss","LowHgg","LowHqq","LowHtautau","MidHbb","MidHcc","MidHss","MidHgg","MidHqq","MidHtautau","HiHbb","HiHcc","HiHss","HiHgg","HiHqq","HiHtautau","Incl","Incl_Corr"};
    std::vector<std::string> cutFlowMap {"NoCut","njet=4","leptonCut","KineCut", "d123Cut", "d34Cut","Pairing","jjMassCut","ZHmassCut"};

    
    // Get the histograms
    auto scoreMapHist = m_histContainer->get1DHist("scoreMap_1D", flavourCategory.size(), 0, flavourCategory.size(), flavourCategory);
    auto scoreMapFitCatHist = m_histContainer->get1DHist("scoreMapFitCategory_1D", fitCategory.size(), 0, fitCategory.size(), fitCategory);
    auto CountsMapHist = m_histContainer->get1DHist("CountsMap_1D", flavourCategory.size(), 0, flavourCategory.size(), flavourCategory);
    auto CountsFitCatHist = m_histContainer->get1DHist("CountsFitCategory_1D", fitCategory.size(), 0, fitCategory.size(), fitCategory);
    auto obsHist = m_histContainer->getObsHistinFitCategory(fitCategory, 75, 50., 125.,160, 90., 250.);
    auto countingHist = m_histContainer->getCountingHist();
    auto cutFlowHist =  m_histContainer->get1DHist("CutFlow", cutFlowMap.size(), 0, 12,cutFlowMap);

    auto Bscore =  m_histContainer->get1DHist("Bscore", 100, 0, 2);
    auto Cscore =  m_histContainer->get1DHist("Cscore", 100, 0, 2);
    auto Sscore =  m_histContainer->get1DHist("Sscore", 100, 0, 2);
    auto Qscore =  m_histContainer->get1DHist("Qscore", 100, 0, 2);
    // auto Uscore =  m_histContainer->get1DHist("Uscore", 100, 0, 2);
    auto Gscore =  m_histContainer->get1DHist("Gscore", 100, 0, 2);
    auto TAUscore =  m_histContainer->get1DHist("TAUscore", 100, 0, 2);

    auto mZ =  m_histContainer->get1DHist("mZ",250, 0., 250.);
    auto mH =  m_histContainer->get1DHist("mH", 250, 0., 250.);

    auto ZZ_cut =  m_histContainer->get1DHist("ZZ_cut",250, 0., 250.);
    auto WW_cut =  m_histContainer->get1DHist("WW_cut", 250, 0., 250.);

    auto h_vis_M =  m_histContainer->get1DHist("vis_M",250, 0., 250.);
    auto h_vis_E =  m_histContainer->get1DHist("vis_e", 250, 0., 250.);
    auto h_vis_theta =  m_histContainer->get1DHist("vis_theta", 80, 0., 4.);

    auto h_muons_p =  m_histContainer->get1DHist("muons_p", 125, 0., 250.);
    auto h_electrons_p =  m_histContainer->get1DHist("electrons_p", 125, 0., 250.);
    auto h_event_nmu =  m_histContainer->get1DHist("event_nmu", 10, 0., 10.);
    auto h_event_nel =  m_histContainer->get1DHist("event_nel", 10, 0., 10.);

    auto h_d_12 =  m_histContainer->get1DHist("d_12", 1000,14000, 60000.);
    auto h_d_23 =  m_histContainer->get1DHist("d_23", 1000,200, 20000.);
    auto h_d_34 =  m_histContainer->get1DHist("d_34", 1000,150, 6000.);

        // Extra hist for easy fill up
    auto Incl_obsHist = obsHist["Incl"];
    auto Incl_obsHist_corr = obsHist["Incl_Corr"];
    // auto Hbb_obsHist = obsHist["Hbb"];
    // auto Hcc_obsHist = obsHist["Hcc"];
    // auto Hgg_obsHist = obsHist["Hgg"];
    // auto Hss_obsHist = obsHist["Hss"];
    // auto Hqq_obsHist = obsHist["Hqq"];
    // auto Htautau_obsHist = obsHist["Htautau"];
    auto LowB_obsHist = obsHist["LowHbb"];
    auto MidB_obsHist = obsHist["MidHbb"];
    auto HiB_obsHist = obsHist["HiHbb"];

    auto LowC_obsHist = obsHist["LowHcc"];
    auto MidC_obsHist = obsHist["MidHcc"];
    auto HiC_obsHist = obsHist["HiHcc"];

    auto LowS_obsHist = obsHist["LowHss"];
    auto MidS_obsHist = obsHist["MidHss"];
    auto HiS_obsHist = obsHist["HiHss"];

    auto LowQ_obsHist = obsHist["LowHqq"];
    auto MidQ_obsHist = obsHist["MidHqq"];
    auto HiQ_obsHist = obsHist["HiHqq"];

    auto LowG_obsHist = obsHist["LowHgg"];
    auto MidG_obsHist = obsHist["MidHgg"];
    auto HiG_obsHist = obsHist["HiHgg"];

    // auto LowTAU_obsHist = obsHist["LowHtautau"];
    // auto MidTAU_obsHist = obsHist["MidHtautau"];
    // auto HiTAU_obsHist = obsHist["HiHtautau"];


    // Get the trees
    auto treeCont = std::make_shared<TreeContainer>();
    std::cout<<"sampleName: "<<MDC::GetInstance()->getSampleName()<<" events: "<<treeCont->getEntries()<<std::endl;

    // Get max events to run on
    int nEntries = treeCont->getEventsToRun();

    // Connect branches to trees
    auto tree = treeCont->getTree();
    varMemberVector<float> jet_px_nc{tree, {"jet0_px","jet1_px","jet2_px","jet3_px"}, -999};
    varMemberVector<float> jet_py_nc{tree, {"jet0_py","jet1_py","jet2_py","jet3_py"}, -999};
    varMemberVector<float> jet_pz_nc{tree, {"jet0_pz","jet1_pz","jet2_pz","jet3_pz"}, -999};
    varMemberVector<float> jet_e_nc{tree, {"jet0_e","jet1_e","jet2_e","jet3_e"}, -999};


    varMemberVector<double> jet_px{tree, { "jet0_px_corr","jet1_px_corr", "jet2_px_corr", "jet3_px_corr"}, -999};
    varMemberVector<double> jet_py{tree, { "jet0_py_corr","jet1_py_corr", "jet2_py_corr", "jet3_py_corr"}, -999};
    varMemberVector<double> jet_pz{tree, { "jet0_pz_corr","jet1_pz_corr", "jet2_pz_corr", "jet3_pz_corr"}, -999};
    varMemberVector<double> jet_e{tree, { "jet0_e_corr","jet1_e_corr", "jet2_e_corr", "jet3_e_corr"}, -999};
    
    // varMemberVector<float> jet_px{tree, {"jet0_px","jet1_px","jet2_px","jet3_px"}, -999};
    // varMemberVector<float> jet_py{tree, {"jet0_py","jet1_py","jet2_py","jet3_py"}, -999};
    // varMemberVector<float> jet_pz{tree, {"jet0_pz","jet1_pz","jet2_pz","jet3_pz"}, -999};
    // varMemberVector<float> jet_e{tree, {"jet0_e","jet1_e","jet2_e","jet3_e"}, -999};


    // varMemberVector<double> jet_px_nc{tree, { "jet0_px_corr","jet1_px_corr", "jet2_px_corr", "jet3_px_corr"}, -999};
    // varMemberVector<double> jet_py_nc{tree, { "jet0_py_corr","jet1_py_corr", "jet2_py_corr", "jet3_py_corr"}, -999};
    // varMemberVector<double> jet_pz_nc{tree, { "jet0_pz_corr","jet1_pz_corr", "jet2_pz_corr", "jet3_pz_corr"}, -999};
    // varMemberVector<double> jet_e_nc{tree, { "jet0_e_corr","jet1_e_corr", "jet2_e_corr", "jet3_e_corr"}, -999};

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

    varMember<float> jet0_scoreU {tree, "jet0_scoreU"};
    varMember<float> jet1_scoreU {tree, "jet1_scoreU"};
    varMember<float> jet2_scoreU {tree, "jet2_scoreU"};
    varMember<float> jet3_scoreU {tree, "jet3_scoreU"};

    varMember<float> jet0_scoreTAU {tree, "jet0_scoreTAU"};
    varMember<float> jet1_scoreTAU {tree, "jet1_scoreTAU"};
    varMember<float> jet2_scoreTAU {tree, "jet2_scoreTAU"};
    varMember<float> jet3_scoreTAU {tree, "jet3_scoreTAU"};

    varMember<float> jet0_scoreD {tree, "jet0_scoreD"};
    varMember<float> jet1_scoreD {tree, "jet1_scoreD"};
    varMember<float> jet2_scoreD {tree, "jet2_scoreD"};
    varMember<float> jet3_scoreD{tree, "jet3_scoreD"};

    varMember<float> jet0_scoreS {tree, "jet0_scoreS"};
    varMember<float> jet1_scoreS {tree, "jet1_scoreS"};
    varMember<float> jet2_scoreS {tree, "jet2_scoreS"};
    varMember<float> jet3_scoreS {tree, "jet3_scoreS"};

    varMember<int> event_njet {tree, "event_njet"};
    varMember<ulong> event_nmu {tree, "event_nmu"};
    varMember<ulong> event_nel {tree, "event_nel"};
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

    int H_flav = -1;
    int NjetCut = 0;
    int NleptonCut = 0;
    int NkineCut = 0;
    int NdCutd123 = 0;
    int NdCutd34 = 0;
    int NafterPairing = 0;
    int NjjMassCut = 0;
    int NafterSel = 0;
    int NEventsInt = 0;
    int BlikeEvents = 0;
    int ClikeEvents = 0;
    int SlikeEvents = 0;
    int GlikeEvents = 0;
    int QlikeEvents = 0;
    int TAUlikeEvents = 0;

    std::array<int, 3> BlikeEvents_cat {0, 0, 0};
    std::array<int, 3> ClikeEvents_cat {0, 0, 0};
    std::array<int, 3> SlikeEvents_cat {0, 0, 0};
    std::array<int, 3> GlikeEvents_cat {0, 0, 0};
    std::array<int, 3> QlikeEvents_cat {0, 0, 0};
    std::array<int, 3> TAUlikeEvents_cat {0, 0, 0};

    // Loop over the trees here
    for(int i = 0; i < nEntries; i++)
    {
        std::cout<<"New Event Investigated! Event: " <<i<< std::endl; 
        // Check if 10 iterations have been reached
        // if (i + 1 == 10) {
        //     std::cout << "Exiting loop after 20 iterations." << std::endl;
        //     break;  // Exit the loop
        // }
        treeCont->getEntry(i);
        // Just to store how many events were run over
        countingHist->Fill(1);
        NEventsInt++;

        if(i % 10000 == 0) std::cout<<"Done i: "<<i<<" out of "<<nEntries<<std::endl;
        // Add some basic selection
        
        if(event_njet() != 4) continue; //Require EXACTLY 4 jet! This SHOULD be the case!
        NjetCut++;
        //numebr of electrons and muons cut! 2!
        // reject events with leptons with > 20 GeV leptons
        if(event_nmu() > 2) continue;
        if(event_nel() > 2) continue;
        if(muons_p() >= 20) continue;
        if(electrons_p() >= 20) continue;
        NleptonCut++;
        // std::cout<<"Number of muons "<<" "<<event_nmu()<<" and the number of electrons: "<<event_nel()<<std::endl;

        //Add this  to the options, as in which selection to choose
        if (vis_M()<150) continue;
        if (vis_E()<150) continue;
        if (vis_theta()<0.15 || vis_theta()>3.0 ) continue; //typo! rerun!
        NkineCut++;
        // change the cuts! update! 
        if (d_12()<15000 || d_12()>58000) continue;//missing!
        if ((d_23()<400) || (d_23()>18000))continue;
        NdCutd123++;
        if ((d_34()<100) || (d_34()>6000))continue;
        NdCutd34++;

        // ALL GOOD UP TO HERE!!!!!!
        // Step 1: check efficiency ~ 80%
        // increment counter of events that pass the cuts!
       //make lorentz vectors
       std::cout<<"Passed intial selection!" << std::endl; 
       std::vector<TLorentzVector> LVjets;
       std::vector<TLorentzVector> LVjets_nc;
       for (size_t lv = 0; lv < 4; ++lv) {
            TLorentzVector LVjet(jet_px.getVal(lv), jet_py.getVal(lv), jet_pz.getVal(lv), jet_e.getVal(lv));
            LVjets.push_back(LVjet);
            TLorentzVector LVjet_nc(jet_px_nc.getVal(lv), jet_py_nc.getVal(lv), jet_pz_nc.getVal(lv), jet_e_nc.getVal(lv));
            LVjets_nc.push_back(LVjet_nc);
        }
       // Flav scores of each jet
       std::array<float,7> j0_flav {jet0_scoreB(), jet0_scoreC(), jet0_scoreS(),jet0_scoreD(),jet0_scoreU(),jet0_scoreG(),jet0_scoreTAU()};
       std::array<float,7> j1_flav {jet1_scoreB(), jet1_scoreC(), jet1_scoreS(),jet1_scoreD(),jet1_scoreU(),jet1_scoreG(),jet1_scoreTAU()};
       std::array<float,7> j2_flav {jet2_scoreB(), jet2_scoreC(), jet2_scoreS(),jet2_scoreD(),jet2_scoreU(),jet2_scoreG(),jet2_scoreTAU()};
       std::array<float,7> j3_flav {jet3_scoreB(), jet3_scoreC(), jet3_scoreS(),jet3_scoreD(),jet3_scoreU(),jet3_scoreG(),jet3_scoreTAU()};

        // - look for max score of jet 
       std::array<float,7>::iterator j0_maxScore;
       std::array<float,7>::iterator j1_maxScore;
       std::array<float,7>::iterator j2_maxScore; 
       std::array<float,7>::iterator j3_maxScore; 

       j0_maxScore = std::max_element(j0_flav.begin(), j0_flav.end());
       j1_maxScore = std::max_element(j1_flav.begin(), j1_flav.end());
       j2_maxScore = std::max_element(j2_flav.begin(), j2_flav.end());
       j3_maxScore = std::max_element(j3_flav.begin(), j3_flav.end());
       //Can simplify with j<>_maxScore
       int j0_maxScoreIdx = std::distance(j0_flav.begin(),std::max_element(j0_flav.begin(), j0_flav.end()));
       int j1_maxScoreIdx = std::distance(j1_flav.begin(),std::max_element(j1_flav.begin(), j1_flav.end()));
       int j2_maxScoreIdx = std::distance(j2_flav.begin(),std::max_element(j2_flav.begin(), j2_flav.end()));
       int j3_maxScoreIdx = std::distance(j3_flav.begin(),std::max_element(j3_flav.begin(), j3_flav.end()));

       std::map<int, std::array<float,7>> flavMap;
       flavMap[0]=j0_flav;
       flavMap[1]=j1_flav;
       flavMap[2]=j2_flav;
       flavMap[3]=j3_flav;

       std::array<int, 4> maxScoreIdx {j0_maxScoreIdx,j1_maxScoreIdx,j2_maxScoreIdx,j3_maxScoreIdx};

       if (m_debug) {
            std::cout<<"j0_flav: "<<" Max score: "<< *j0_maxScore<< " maxScoreIdx: "<< maxScoreIdx[0]<<std::endl;
            std::cout<<"j1_flav: "<<" Max score: "<< *j1_maxScore<< " maxScoreIdx: "<< maxScoreIdx[1]<<std::endl;
            std::cout<<"j2_flav: "<<" Max score: "<< *j2_maxScore<< " maxScoreIdx: "<< maxScoreIdx[2]<<std::endl;
            std::cout<<"j3_flav: "<<" Max score: "<< *j3_maxScore<< " maxScoreIdx: "<< maxScoreIdx[3]<<std::endl;
       }
       if (m_debug) {
            std::cout << "Jet score for j0: ";
                for (float j0_f : j0_flav) {
                    std::cout << j0_f << " ";
                }
                std::cout << std::endl;

            std::cout << "Jet score for j1: ";
                for (float j1_f : j1_flav) {
                    std::cout << j1_f << " ";
                }
                std::cout << std::endl;
            std::cout << "Jet score for j2: ";
                for (float j2_f : j2_flav) {
                    std::cout << j2_f << " ";
                }
                std::cout << std::endl;
            std::cout << "Jet score for j3: ";
                for (float j3_f : j3_flav) {
                    std::cout << j3_f << " ";
                }
                std::cout << std::endl;}
       //Find the pairs
       std::set<int> uniqueFlav(maxScoreIdx.begin(), maxScoreIdx.end());
       
       std::map<int, std::vector<int>> flavOccurance;
       for (int value : uniqueFlav) {
            std::cout<<"uniqueFlav: "<<value<<std::endl;
            auto pos = maxScoreIdx.begin();
            while ((pos = std::find(pos, maxScoreIdx.end(), value)) != maxScoreIdx.end()) {
                int idx = std::distance(maxScoreIdx.begin(), pos);
                flavOccurance[value].push_back(idx);
                ++pos;
            }
        } 
       int pair = 0;
        //Pair the jets, make sth very simple 
       std::map<int, std::vector<int>> jet_pair;
       std::map<int, int> jet_pair_flav;
       std::vector<int> z_idx;
       std::vector<int> h_idx;
       for (const auto& entry : flavOccurance) {
            for (size_t index : entry.second) {
                        std::cout << index << " ";
                    }
            std::cout << std::endl;
            if (entry.second.size() > 1) {
                jet_pair[pair]=entry.second;   
                jet_pair_flav[pair]=entry.first;
                pair++;
                if (m_debug) std::cout << "Flav " << flavours[entry.first] << " found at indices: ";
                if (m_debug) {
                    for (size_t index : entry.second) {
                        std::cout << index << " ";
                    }
                    std::cout << std::endl;
            
                } }
                }

        //Check how many pairs you get from the simple method        
        std::vector<int> idx_jet ={0, 1, 2, 3};
        if (m_debug) std::cout << "jet_pair: " <<jet_pair.size()<<std::endl;
        if (jet_pair.size() == 2){
            //So given that there are only 4 jets reconstructed, I am pretty sure that this should work.
            if (m_debug) std::cout << "Oleeee! You have two paird! You can continue with your mission!"<<std::endl;
        }
        else if (jet_pair.size() == 3){
            std::cout << "THIS IS IMPOSSIBLE! I THINK???"<<std::endl;
        }
        else if (flavOccurance.size()==4){
                std::cout << "No same flavour pair found. Ignore for now!" <<std::endl;
                continue;}
        else if (jet_pair.size() == 1){
            if (m_debug) std::cout << "Only 1 jet pair found!" <<std::endl;
            if (m_debug) std::cout << "flavOccurance.size(): " <<flavOccurance.size()<<std::endl;
            if (flavOccurance.size()==3){
                if (m_debug) std::cout << "2 same flav, 3 diff flav -> 3 unique!"<<std::endl;
                //Need to sum up each flav and see what max?
                auto found_pair = jet_pair[0];
                std::sort(found_pair.begin(), found_pair.end());
                std::sort(idx_jet.begin(), idx_jet.end());
                std::vector<int> missing_pair;
                std::set_symmetric_difference(found_pair.begin(), found_pair.end(), idx_jet.begin(), idx_jet.end(),
                    std::back_inserter(missing_pair));
                //Maybe this should be a a function! Outside this mess defined... Because you will need to call it again...
                float max_score = 0;
                int new_fl = -1;
                for (int fl = 0; fl < 7; ++fl) {
                    if (flavMap[missing_pair[0]][fl]+flavMap[missing_pair[1]][fl]>max_score){
                        new_fl=fl;
                        max_score=flavMap[missing_pair[0]][fl]+flavMap[missing_pair[1]][fl];
                        if (m_debug) std::cout << "Updating max score: " << max_score << ", flav is: "<<flavours[fl] <<std::endl;
                        };
                }
                jet_pair[1]=missing_pair;
                jet_pair_flav[1]=new_fl;
                if (m_debug) std::cout << "New pair found! With flavour " << flavours[jet_pair_flav[1]] << " found at indices: ";
                if (m_debug) {
                    for (size_t index : missing_pair) {
                        std::cout << index << " ";
                    }
                    std::cout << std::endl;
                    }
                }
            else if (flavOccurance.size()==2){
                if (m_debug) std::cout << "3 same flav, 1 diff flav -> 2 unique!"<<std::endl;
                std::vector<int> new_pair;
                float max_sc = 0;
                for (size_t j1_in : jet_pair[0]){
                    for (size_t j2_in : jet_pair[0]){
                        if ( (j1_in!=j2_in)  && (j2_in>j1_in) && (flavMap[j1_in][jet_pair_flav[0]]+flavMap[j2_in][jet_pair_flav[0]] > max_sc) ){
                            max_sc = flavMap[j1_in][jet_pair_flav[0]]+flavMap[j2_in][jet_pair_flav[0]];
                            new_pair={};
                            new_pair.push_back(j1_in);
                            new_pair.push_back(j2_in);
                            if (m_debug) std::cout << "Updating max score: " << max_sc << ", with jet: "<<j1_in << " and "<< j2_in <<std::endl;

                        }
                    }
                    // while (j1_in + j2_in <=3)                  
                    // max_score=flavMap[j1_in][jet_pair_flav[0]]+flavMap[j2_in][jet_pair_flav[0]]
                }
                if (m_debug) std::cout << "New pair found! With flavour " << jet_pair_flav[0] << " found at indices: ";
                for (size_t ind_new : new_pair) {
                    std::cout << ind_new << " ";
                }
                std::cout << std::endl;

                jet_pair[0]=new_pair;
                //Need to sum up each flav and see what max?
                auto found_pair = jet_pair[0];
                std::sort(found_pair.begin(), found_pair.end());
                std::sort(idx_jet.begin(), idx_jet.end());
                std::vector<int> missing_p;
                std::set_symmetric_difference(found_pair.begin(), found_pair.end(), idx_jet.begin(), idx_jet.end(),
                    std::back_inserter(missing_p));
                //Maybe this should be a a function! Outside this mess defined... Because you will need to call it again...
                float max_score = 0;
                int new_fl = -1;
                for (int fla = 0; fla < 7; ++fla) {
                    if (flavMap[missing_p[0]][fla]+flavMap[missing_p[1]][fla]>max_score){
                        new_fl=fla;
                        max_score=flavMap[missing_p[0]][fla]+flavMap[missing_p[1]][fla];
                        if (m_debug) std::cout << "Updating max score: " << max_score << ", flav is: "<<flavours[fla] <<std::endl;
                        };
                }
                jet_pair[1]=missing_p;
                jet_pair_flav[1]=new_fl;
                if (m_debug)  std::cout << "New pair found! With flavour " << flavours[jet_pair_flav[1]] << " found at indices: ";
                for (size_t index : missing_p) {
                    std::cout << index << " ";
                }
                std::cout << std::endl;
            }
            if (flavOccurance.size()==1 or (jet_pair_flav[0]==jet_pair_flav[1])){
                if (m_debug) std::cout << "4 same flav jets, you continue and decide the paris based on the H/Z chi^2!"<<std::endl;
                // std::cout << "Flav PAIR 1:" <<jet_pair_flav[0]<<std::endl;
                // std::cout << "Flav PAIR 2:" <<jet_pair_flav[1]<<std::endl;
                float chi_z=0;
                float chi_h=0; 
                std::vector<float> chi_comb; 
                std::vector<vector<int>> z_ijets;
                std::vector<vector<int>> h_ijets;
                std::vector<int>z_pair;
                for (const auto& j : idx_jet) {
                    for (const auto& jj : idx_jet)  {
                        if (j<jj){
                            z_pair = {j,jj};
                            std::sort(z_pair.begin(), z_pair.end());
                            std::sort(idx_jet.begin(), idx_jet.end());
                            std::vector<int> h_pair;
                            std::set_symmetric_difference(z_pair.begin(), z_pair.end(), idx_jet.begin(), idx_jet.end(),
                    std::back_inserter(h_pair));

                            chi_z = std::pow((LVjets[z_pair[0]]+LVjets[z_pair[1]]).M()-Z_mass,2);
                            chi_h = std::pow((LVjets[h_pair[0]]+LVjets[h_pair[1]]).M()-H_mass,2);
                            z_ijets.push_back(z_pair);
                            h_ijets.push_back(h_pair);
                            chi_comb.push_back(chi_z+chi_h);
                            if (m_debug) {
                                std::cout << "Z mass: " << (LVjets[z_pair[0]]+LVjets[z_pair[1]]).M() << std::endl;
                                std::cout << "H mass: " << (LVjets[h_pair[0]]+LVjets[h_pair[1]]).M() << std::endl;
                                std::cout << "Z indx: " << z_pair[0] << z_pair[1] << std::endl;
                                std::cout << "H indx " << h_pair[0]<<h_pair[1] << std::endl;
                                std::cout << "z_chi: " << chi_z<< std::endl;
                                std::cout << "h_chi: " << chi_h<< std::endl;
                                std::cout << "chi_comb: " << chi_z+chi_h << std::endl;}

                        }
                    }
                }
                 // Using std::min_element to find the minimum element
                auto chi_comb_min = std::min_element(chi_comb.begin(), chi_comb.end());
                size_t minIndex = std::distance(chi_comb.begin(), chi_comb_min);
                jet_pair[0]=z_ijets[minIndex];
                jet_pair[1]=h_ijets[minIndex];
                z_idx=jet_pair[0];
                h_idx=jet_pair[1];
                H_flav=jet_pair_flav[1];
                if (m_debug) {
                    std::cout << "chi z+h minIndex: " << minIndex << std::endl;
                    std::cout << "z index: " << jet_pair[0][0] << jet_pair[0][1] <<std::endl;
                    std::cout << "h index: " << jet_pair[1][0] << jet_pair[1][1] <<std::endl;}
                }
                }
        
        // std::cout << "AFTER FAILING!" <<std::endl;  
        // std::cout << "Flav PAIR 1:" <<jet_pair_flav[0]<<std::endl;
        // std::cout << "Flav PAIR 2:" <<jet_pair_flav[1]<<std::endl;
        if (jet_pair_flav[0]!=jet_pair_flav[1]){
            float chi_p1 = std::pow((LVjets[jet_pair[0][0]]+LVjets[jet_pair[0][1]]).M()-Z_mass,2);
            float chi_p2 = std::pow((LVjets[jet_pair[1][0]]+LVjets[jet_pair[1][1]]).M()-Z_mass,2);
        // std::cout << "chi OF JET PAIR 1:" <<chi_p1<<std::endl;
        // std::cout << "chi OF JET PAIR 2:" <<chi_p2<<std::endl;
            if (chi_p1<chi_p2){
                if (m_debug) std::cout << "Pair 0 is Z!" <<std::endl;
                z_idx=jet_pair[0];
                h_idx=jet_pair[1];
                H_flav=jet_pair_flav[1];
            }
            else{
                if (m_debug) std::cout << "Pair 0 is H!" <<std::endl;
                z_idx=jet_pair[1];
                h_idx=jet_pair[0];
                H_flav=jet_pair_flav[0];
            }
        }

        float m_zjj = (LVjets[z_idx[0]]+LVjets[z_idx[1]]).M();
        float m_hjj = (LVjets[h_idx[0]]+LVjets[h_idx[1]]).M();

        float m_zjj_nc = (LVjets_nc[z_idx[0]]+LVjets_nc[z_idx[1]]).M();
        float m_hjj_nc = (LVjets_nc[h_idx[0]]+LVjets_nc[h_idx[1]]).M();


        float H_flav_sc = flavMap[h_idx[0]][H_flav]+flavMap[h_idx[1]][H_flav];

        // std::cout << "MASS OF Z:" <<m_zjj<<std::endl;
        // std::cout << "MASS OF H:" <<m_hjj<<std::endl;

        // std::cout << "NOT CORR - MASS OF Z:" <<m_zjj_nc<<std::endl;
        // std::cout << " NOT CORR - MASS OF H:" <<m_hjj_nc<<std::endl;
        NafterPairing++;
        Incl_obsHist->Fill(m_zjj,m_hjj);
        Incl_obsHist_corr->Fill(m_zjj_nc,m_hjj_nc);
        //A bit of selection 
        float WW_cuts = sqrt(pow( m_zjj-W_mass ,2) + pow( m_hjj-W_mass ,2));
        float ZZ_cuts = sqrt(pow( m_zjj-Z_mass ,2) + pow( m_hjj-Z_mass ,2));
        if(WW_cuts<=10) continue;
        if(ZZ_cuts<=10) continue;

        NjjMassCut++;
        if (50. >= m_zjj)continue; 
        if (m_zjj >= H_mass) continue; 
        //Fix
        if (m_hjj<=Z_mass) continue;
        // For now remove the Htautau as a cat, only bkg....
        // if (flavours[H_flav]=="TAU") continue;
        //After parining and cuts 
        NafterSel++;
        ZZ_cut->Fill(ZZ_cuts);
        WW_cut->Fill(WW_cuts);
        mZ->Fill(m_zjj);
        mH->Fill(m_hjj);
        h_vis_M->Fill(vis_M());
        h_vis_E->Fill(vis_E());
        h_vis_theta->Fill(vis_theta());
        h_muons_p->Fill(muons_p());
        h_electrons_p->Fill(electrons_p());
        h_event_nmu->Fill(event_nmu());
        h_event_nel->Fill(event_nel());
        h_d_12->Fill(d_12());
        h_d_23->Fill(d_23());
        h_d_34->Fill(d_34());
        if (flavours[H_flav]=="B"){
            BlikeEvents++;
            // Hbb_obsHist->Fill(m_zjj,m_hjj);
            // std::cout << "B Flav" << std::endl;
            Bscore->Fill(H_flav_sc);
            if (H_flav_sc < 1.1)
            {
                // std::cout << "B Low Cat" << std::endl;
                LowB_obsHist->Fill(m_zjj,m_hjj);
                BlikeEvents_cat[0]++;
            }
            else if ((H_flav_sc >= 1.1) && (H_flav_sc <= 1.9))
            {
                // std::cout << "B Mid Cat" << std::endl;
                MidB_obsHist->Fill(m_zjj,m_hjj);
                BlikeEvents_cat[1]++;
            }
            else
            {
                // std::cout << "B Hi Cat" << std::endl;
                HiB_obsHist->Fill(m_zjj,m_hjj);
                BlikeEvents_cat[2]++;
            }
        }
        else if (flavours[H_flav]=="C"){
            ClikeEvents++;
            Cscore->Fill(H_flav_sc);
            // Hcc_obsHist->Fill(m_zjj,m_hjj);
            if (H_flav_sc < 1.1)
            {
                LowC_obsHist->Fill(m_zjj,m_hjj);
                ClikeEvents_cat[0]++;
            }
            else if ((H_flav_sc >= 1.1) && (H_flav_sc <= 1.9))
            {
                MidC_obsHist->Fill(m_zjj,m_hjj);
                ClikeEvents_cat[1]++;
            }
            else
            {
                HiC_obsHist->Fill(m_zjj,m_hjj);
                ClikeEvents_cat[2]++;
            }
        }
        else if (flavours[H_flav]=="S"){
            Sscore->Fill(H_flav_sc);
            SlikeEvents++;
            // Hss_obsHist->Fill(m_zjj,m_hjj);
            if (H_flav_sc < 1.1)
            {
                LowS_obsHist->Fill(m_zjj,m_hjj);
                SlikeEvents_cat[0]++;
            }
            else if ((H_flav_sc >= 1.1) && (H_flav_sc <= 1.9))
            {
                MidS_obsHist->Fill(m_zjj,m_hjj);
                SlikeEvents_cat[1]++;
            }
            else
            {
                HiS_obsHist->Fill(m_zjj,m_hjj);
                SlikeEvents_cat[2]++;
            }
        }
        else if (flavours[H_flav]=="G"){
            GlikeEvents++;
            Gscore->Fill(H_flav_sc);
            // Hgg_obsHist->Fill(m_zjj,m_hjj);
            if (H_flav_sc < 1.1)
            {
                LowG_obsHist->Fill(m_zjj,m_hjj);
                GlikeEvents_cat[0]++;
            }
            else if ((H_flav_sc >= 1.1) && (H_flav_sc <= 1.9))
            {
                MidG_obsHist->Fill(m_zjj,m_hjj);
                GlikeEvents_cat[1]++;
            }
            else
            {
                HiG_obsHist->Fill(m_zjj,m_hjj);
                GlikeEvents_cat[2]++;
            }
        }
        else if (flavours[H_flav]=="U" || flavours[H_flav]=="D" ){
            QlikeEvents++;
            Qscore->Fill(H_flav_sc);
            // Hqq_obsHist->Fill(m_zjj,m_hjj);
            if (H_flav_sc < 1.1)
            {
                LowQ_obsHist->Fill(m_zjj,m_hjj);
                QlikeEvents_cat[0]++;
            }
            else if ((H_flav_sc >= 1.1) && (H_flav_sc <= 1.9))
            {
                MidQ_obsHist->Fill(m_zjj,m_hjj);
                QlikeEvents_cat[1]++;
            }
            else
            {
                HiQ_obsHist->Fill(m_zjj,m_hjj);
                QlikeEvents_cat[2]++;
            }
        }
        // else if (flavours[H_flav]=="TAU"){
        //     TAUlikeEvents++;
        //     TAUscore->Fill(H_flav_sc);
        //     // Htautau_obsHist->Fill(m_zjj,m_hjj);
        //     if (H_flav_sc < 1.1)
        //     {
        //         LowTAU_obsHist->Fill(m_zjj,m_hjj);
        //         TAUlikeEvents_cat[0]++;
        //     }
        //     else if ((H_flav_sc >= 1.1) && (H_flav_sc <= 1.9))
        //     {
        //         MidTAU_obsHist->Fill(m_zjj,m_hjj);
        //         TAUlikeEvents_cat[1]++;
        //     }
        //     else
        //     {
        //         HiTAU_obsHist->Fill(m_zjj,m_hjj);
        //         TAUlikeEvents_cat[2]++;
        //     }
        // }
        else if (H_flav == -1){
            std::cout << "Something went wrong! No Higgs found!"<<std::endl;
            break;
            }
        else{
            std::cout<<"Couldn't find best score"<<std::endl;
        }
        // }
        // Step 2: check efficiency ~ 60-70%, for each cat you fit you need to check eff 

    }
    scoreMapHist->SetBinContent(1, BlikeEvents*100./NafterSel);
    scoreMapHist->SetBinContent(2, ClikeEvents*100./NafterSel);
    scoreMapHist->SetBinContent(3, SlikeEvents*100./NafterSel);
    scoreMapHist->SetBinContent(4, QlikeEvents*100./NafterSel);
    scoreMapHist->SetBinContent(5, GlikeEvents*100./NafterSel);
    // scoreMapHist->SetBinContent(6, TAUlikeEvents*100./NafterSel);

    CountsMapHist->SetBinContent(1, BlikeEvents);
    CountsMapHist->SetBinContent(2, ClikeEvents);
    CountsMapHist->SetBinContent(3, SlikeEvents);
    CountsMapHist->SetBinContent(4, QlikeEvents);
    CountsMapHist->SetBinContent(5, GlikeEvents);
    // CountsMapHist->SetBinContent(6, TAUlikeEvents);

    
    //CutFlow

    cutFlowHist->SetBinContent(1,NEventsInt);
    cutFlowHist->SetBinContent(2,NjetCut);
    cutFlowHist->SetBinContent(3,NleptonCut);
    cutFlowHist->SetBinContent(4,NkineCut);
    cutFlowHist->SetBinContent(5,NdCutd123);
    cutFlowHist->SetBinContent(6,NdCutd34);
    cutFlowHist->SetBinContent(7,NafterPairing);
    cutFlowHist->SetBinContent(8,NjjMassCut);
    cutFlowHist->SetBinContent(9,NafterSel);

    for(int i = 0; i < 3; i++)
    {
        scoreMapFitCatHist->SetBinContent(1+i*5, BlikeEvents_cat[i]*100./NafterSel);
        scoreMapFitCatHist->SetBinContent(2+i*5, ClikeEvents_cat[i]*100./NafterSel);
        scoreMapFitCatHist->SetBinContent(3+i*5, SlikeEvents_cat[i]*100./NafterSel);
        scoreMapFitCatHist->SetBinContent(4+i*5, GlikeEvents_cat[i]*100./NafterSel);
        scoreMapFitCatHist->SetBinContent(5+i*5, QlikeEvents_cat[i]*100./NafterSel);
        // scoreMapFitCatHist->SetBinContent(6+i*6, TAUlikeEvents_cat[i]*100./NafterSel);

        CountsFitCatHist->SetBinContent(1+i*5, BlikeEvents_cat[i]);
        CountsFitCatHist->SetBinContent(2+i*5, ClikeEvents_cat[i]);
        CountsFitCatHist->SetBinContent(3+i*5, SlikeEvents_cat[i]);
        CountsFitCatHist->SetBinContent(4+i*5, GlikeEvents_cat[i]);
        CountsFitCatHist->SetBinContent(5+i*5, QlikeEvents_cat[i]);
        // CountsFitCatHist->SetBinContent(6+i*6, TAUlikeEvents_cat[i]);
    }


    //Make histograms for the cats 

}


