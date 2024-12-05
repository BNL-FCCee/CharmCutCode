// Local includes
#include "PostCutCode/AnalysisZHAllHad6J.h"
#include "PostCutCode/TreeContainer.h"
#include <iomanip>
#include <TFile.h>
#include <fstream>
#include <set>
#include "TLorentzVector.h"
#include <TRandom.h>
#include <nlohmann/json.hpp>

// DEFINE sel_visMtheta, sel_d123, sel_d34! WHICH SEL TO PICK! CAREFUL!
using namespace std;

struct ZCandidate {
    std::pair<int, int> values;
    std::vector<TLorentzVector> Jet1;
    std::vector<TLorentzVector> Jet2;
    TLorentzVector Z_combined;
    double Z_mass;
    double Z_abs_diff;
};

struct HCandidate {
    struct ZCandidate ZCandidate1;
    struct ZCandidate ZCandidate2;
    TLorentzVector H_combined;
    double H_mass;
    double H_abs_diff;
};

AnalysisZHAllHad6J::AnalysisZHAllHad6J():
    AnalysisBase(),
    m_debug(false)    
{}
AnalysisZHAllHad6J::~AnalysisZHAllHad6J()
{}




void AnalysisZHAllHad6J::printZcandidate(struct ZCandidate zCandidate){
    std::cout << "Z Candidate Values: " << zCandidate.values.first << ", " << zCandidate.values.second << std::endl;
    std::cout << "Jet1: ";
    for (const auto& jet : zCandidate.Jet1) {
        std::cout << "(" << jet.Px() << ", " << jet.Py() << ", " << jet.Pz() << ", " << jet.E() << ") ";
    }
    std::cout << std::endl;
    std::cout << "Jet2: ";
    for (const auto& jet : zCandidate.Jet2) {
        std::cout << "(" << jet.Px() << ", " << jet.Py() << ", " << jet.Pz() << ", " << jet.E() << ") ";
    }
    std::cout << std::endl;
    std::cout << "Z Combined: ";
    std::cout << "(" << zCandidate.Z_combined.Px() << ", " << zCandidate.Z_combined.Py() << ", " << zCandidate.Z_combined.Pz() << ", " << zCandidate.Z_combined.E() << ") ";
    
    std::cout << std::endl;
    std::cout << "Z Mass: " << zCandidate.Z_mass << std::endl;
    std::cout << "Z Absolute Difference: " << zCandidate.Z_abs_diff << std::endl;
}


HCandidate GetHCandidatesfromZs(struct ZCandidate Z1_HZZ, struct ZCandidate Z2_HZZ){
    HCandidate hCandidate_for_Zs;
    hCandidate_for_Zs.ZCandidate1 = Z1_HZZ;
    hCandidate_for_Zs.ZCandidate2 = Z2_HZZ;
    hCandidate_for_Zs.H_combined = Z1_HZZ.Z_combined + Z2_HZZ.Z_combined;
    hCandidate_for_Zs.H_mass = hCandidate_for_Zs.H_combined.M();
    hCandidate_for_Zs.H_abs_diff = abs(hCandidate_for_Zs.H_mass - 125.11);
    return hCandidate_for_Zs;
}


int AnalysisZHAllHad6J::getMaxScoreFlav(const std::vector<int>& vec, const std::map<int, std::vector<float>>& mapFlav){
    float max_score = 0;
    int flav = -1;
    int j1 = vec[0];
    int j2 = vec[1];
    if (m_debug) std::cout<<"getMaxScoreFlav!"<<std::endl;
    for(int i = 0; i < 5; i++){
        if (m_debug){
            std::cout<<"max_score: "<< max_score<<", new_score: "<< mapFlav.at(j1)[i] + mapFlav.at(j2)[i]<<std::endl;
        }
        if (max_score < (mapFlav.at(j1)[i] + mapFlav.at(j2)[i])) {
            max_score = mapFlav.at(j1)[i] + mapFlav.at(j2)[i];
            flav = i;
            if (m_debug){
                std::cout<<"updated_score: "<< max_score<<", updated_flav: "<< flav<<std::endl;
            }
        }
    }
    return flav;}

void AnalysisZHAllHad6J::getMissingPair(const std::vector<int>& foundPair, std::vector<int>& missingPair){
    // std::vector<int> missingPair;
    int i=0;
    if (m_debug) std::cout<<"getMissingPair!"<<std::endl;
    for (const int n : {0, 1, 2, 3}){
        if (std::find(foundPair.begin(), foundPair.end(), n) == std::end(foundPair)){
            if (m_debug) std::cout<<"idx jet: "<< n<<std::endl;
            missingPair.at(i)=n;
            i++;
        }
    }
}

void AnalysisZHAllHad6J::run()
{       
    m_debug=false;
    // using json = nlohmann::json;
    std::ifstream f(MDC::GetInstance()->getSOWJSONFile());
    nlohmann::json data = nlohmann::json::parse(f);

    // override the sum of weights, if it is inside the extra files that we built by hand
    std::ifstream customF(MDC::GetInstance()->getCustomSOWJSONFile());
    nlohmann::json customData = nlohmann::json::parse(customF);

    auto sName = MDC::GetInstance()->getSampleName();

    double norm_weight = (double)data[sName]["crossSection"]/(double)data[sName]["sumOfWeights"];
    
    // If the information is in the custom file, scale it
    if(customData.contains(sName))
    {
        norm_weight = (double)data[sName]["crossSection"]/(double)customData[sName]["sumOfWeights"];
    }


    float Z_mass = 91.1876;
    float H_mass = 125.11;
    float W_mass = 80.377;

    std::vector<std::string> flavourCategory {"B", "C", "S","Q", "G"};
    std::vector<std::string> fitCategory {"LowHss","MidHss","HiHss","LowbbHbb","LowccHbb","LowssHbb","LowqqHbb","LowbbHcc","LowccHcc","LowssHcc","LowqqHcc","LowbbHgg","LowccHgg","LowssHgg","LowqqHgg","MidbbHbb","MidccHbb","MidssHbb","MidqqHbb","MidbbHcc","MidccHcc","MidssHcc","MidqqHcc","MidbbHgg","MidccHgg","MidssHgg","MidqqHgg","HibbHbb","HiccHbb","HissHbb","HiqqHbb","HibbHcc","HiccHcc","HissHcc","HiqqHcc","HibbHgg","HiccHgg","HissHgg","HiqqHgg","Incl"};
    std::vector<std::string> cutFlowMap {"NoCut","NoNaNFlavScore","njet=4","leptonCut","KineCut", "d123Cut", "d34Cut","Pairing","jjMassCut","ZHmassCut","YieldsFit" };
    std::vector<std::string> fitCategorySimple  {"LowHbb","LowHcc","LowHss","LowHgg","MidHbb","MidHcc","MidHss","MidHgg","HiHbb","HiHcc", "HiHss","HiHgg"};

    
    // Get the histograms

    auto countingHist = m_histContainer->getCountingHist();
    auto cutFlowHist =  m_histContainer->get1DHist("CutFlow", cutFlowMap.size(), 0, 13,cutFlowMap);   

    // Get the trees
    auto treeCont = std::make_shared<TreeContainer>();
    std::cout<<"sampleName: "<<MDC::GetInstance()->getSampleName()<<" events: "<<treeCont->getEntries()<<std::endl;

    // Get max events to run on
    int nEntries = treeCont->getEventsToRun();
  

    // Connect branches to trees
    auto tree = treeCont->getTree();

    //Define ttree for analysis
    TTree* t = new TTree("analysis","my analysis tree");
    my_tree = (TTree*) t;
    //define the output branches 
    my_tree->Branch("b_mH_ZZ",&mH_ZZ);
    my_tree->Branch("b_mZ1_jj",&mZ1_jj);
    my_tree->Branch("b_mZ2_jj",&mZ2_jj);
    my_tree->Branch("b_mZstar_jj",&mZ3_jj);
    // my_tree->Branch("b_Hscore",&Hscore);
    // my_tree->Branch("b_Zscore",&Zscore);
    // my_tree->Branch("b_Hflav",&Hflav);
    // my_tree->Branch("b_Zflav",&Zflav);
    // my_tree->Branch("b_ChiH",&ChiH);
    // my_tree->Branch("b_ChiZ",&ChiZ);
    my_tree->Branch("b_w",&mc_weight);

  
    // assuming you have vectors as input (should also save option to run w/o vectors)
    //New Tagger
    varMember<ROOT::VecOps::RVec<float>> recojet_isB {tree, "recojet_isB"};
    varMember<ROOT::VecOps::RVec<float>> recojet_isC {tree, "recojet_isC"};
    varMember<ROOT::VecOps::RVec<float>> recojet_isS {tree, "recojet_isS"};
    varMember<ROOT::VecOps::RVec<float>> recojet_isU {tree, "recojet_isU"};
    varMember<ROOT::VecOps::RVec<float>> recojet_isD {tree, "recojet_isD"};
    varMember<ROOT::VecOps::RVec<float>> recojet_isG {tree, "recojet_isG"};
    varMember<ROOT::VecOps::RVec<float>> recojet_isTAU {tree, "recojet_isTAU"};
    //add truth jet flav
    varMember<ROOT::VecOps::RVec<int>> truth_flav {tree, "jets_truth"};
    
  
    // corrected momentum
    varMember<ROOT::VecOps::RVec<float>> jet_px {tree, "jet_px_corr"};

    
    varMember<ROOT::VecOps::RVec<float>> jet_py {tree, "jet_py_corr"}; 
    varMember<ROOT::VecOps::RVec<float>> jet_pz {tree, "jet_pz_corr"};
    varMember<ROOT::VecOps::RVec<float>> jet_e {tree, "jet_e_corr"}; 

    varMember<int> event_njet {tree, "event_njet"};
    varMember<ulong> event_nmu {tree, "event_nmu"};
    varMember<ulong> event_nel {tree, "event_nel"};
    varMember<float> muons_p {tree, "muons_p"};
    varMember<float> electrons_p {tree, "electrons_p"};
    varMember<double> vis_E {tree, "vis_E"};
    varMember<double> vis_M {tree, "vis_M"};
    varMember<double> vis_theta {tree, "vis_theta"};
    varMember<float> d_12 {tree, "d_12"};
    varMember<float> d_23 {tree, "d_23"};
    varMember<float> d_34 {tree, "d_34"};
    varMember<float> d_45 {tree, "d_45"};
    varMember<float> d_56 {tree, "d_56"};
    // varMember<double> flag_corr {tree, "flag_corr"};

    // varMember<double> costhetainv {tree, "costhetainv"};
    int NokFlav = 0;
    int NjetCut = 0;
    int NleptonCut = 0;
    int NkineCut = 0;
    int NdCutd123 = 0;
    int NdCutd34 = 0;
    int NafterPairing = 0;
    int NjjMassCut = 0;
    int NafterSel = 0;
    int NafterFlagSel = 0;
    int Nfit = 0;
    int NEventsInt = 0;
   
    int NPassed = -1 ;

    //     int debug = 1;
  
    // Loop over the trees here
    for(int i = 0; i < nEntries; i++)
    {
        if(i % 1000 == 0) std::cout<<"Done i: "<<i<<" out of "<<nEntries<<std::endl;

        treeCont->getEntry(i);
        // Just to store how many events were run over
        countingHist->Fill(1);
        NEventsInt++;

        ZCandidate Z1;
        ZCandidate Z2;
        ZCandidate Zstar;
        HCandidate H_ZZ;
        double mjj_H = -1;
        double mjj_Z1 = -1;
        double mjj_Z2 = -1;
        double mjj_Z3 = -1;
        double flavSc_H = -1;
        double flavSc_Z1 = -1;
        double flavSc_Z2 = -1;
        double flavSc_Z3 = -1;
        int flav_H = -1;
        int flav_Z1 = -1;
        int flav_Z2 = -1;
        int flav_Z3 = -1;

        bool flage_toss = false;
        if (recojet_isB.size() == 0){
            flage_toss = true;
        }
        if (flage_toss) continue;


        // Flav scores of each jet, determine to consider D or U score.
        float jet0_scoreQ = recojet_isU.at(0) < recojet_isD.at(0)? recojet_isD.at(0) :recojet_isU.at(0);
        float jet1_scoreQ = recojet_isU.at(1) < recojet_isD.at(1)? recojet_isD.at(1) :recojet_isU.at(1);
        float jet2_scoreQ = recojet_isU.at(2) < recojet_isD.at(2)? recojet_isD.at(2) :recojet_isU.at(2);
        float jet3_scoreQ = recojet_isU.at(3) < recojet_isD.at(3)? recojet_isD.at(3) :recojet_isU.at(3);
        float jet4_scoreQ = recojet_isU.at(4) < recojet_isD.at(4)? recojet_isD.at(4) :recojet_isU.at(4);
        float jet5_scoreQ = recojet_isU.at(5) < recojet_isD.at(5)? recojet_isD.at(5) :recojet_isU.at(5);



        // - look for max score of jet 
        std::vector<float> j0_flav {recojet_isB.at(0), recojet_isC.at(0), recojet_isS.at(0), jet0_scoreQ, recojet_isG.at(0), recojet_isTAU.at(0)};
        std::vector<float> j1_flav {recojet_isB.at(1), recojet_isC.at(1), recojet_isS.at(1), jet1_scoreQ, recojet_isG.at(1), recojet_isTAU.at(1)};
        std::vector<float> j2_flav {recojet_isB.at(2), recojet_isC.at(2), recojet_isS.at(2), jet2_scoreQ, recojet_isG.at(2), recojet_isTAU.at(2)};
        std::vector<float> j3_flav {recojet_isB.at(3), recojet_isC.at(3), recojet_isS.at(3), jet3_scoreQ, recojet_isG.at(3), recojet_isTAU.at(3)};
        std::vector<float> j4_flav {recojet_isB.at(4), recojet_isC.at(4), recojet_isS.at(4), jet4_scoreQ, recojet_isG.at(4), recojet_isTAU.at(4)};
        std::vector<float> j5_flav {recojet_isB.at(5), recojet_isC.at(5), recojet_isS.at(5), jet5_scoreQ, recojet_isG.at(5), recojet_isTAU.at(5)};
        


        for (float j0_f : j0_flav) {
            if (std::isnan(j0_f)) {
                flage_toss = true;
                continue;
            }
        }
        for (float j1_f : j1_flav) {
            if (std::isnan(j1_f)) {
                flage_toss = true;
                continue;
            }
        }
        for (float j2_f : j2_flav) {
            if (std::isnan(j2_f)) {
                flage_toss = true;
                continue;
            }
        }
        for (float j3_f : j3_flav) {
            if (std::isnan(j3_f)) {
                flage_toss = true;
                continue;
            }
        }for (float j4_f : j4_flav) {
            if (std::isnan(j4_f)) {
                flage_toss = true;
                continue;
            }
        }for (float j5_f : j5_flav) {
            if (std::isnan(j5_f)) {
                flage_toss = true;
                continue;
            }
        }
        
        NokFlav++;
        // std::cout<<"Nan toss away!"<<std::endl;

        // Add some basic selection        
        if(event_njet() != 6) continue; //Require EXACTLY 6 jet! This SHOULD be the case!
        NjetCut++;
        //numebr of electrons and muons cut! 2!
        // reject events with leptons with > 20 GeV leptons
        if(event_nmu() > 2.) continue;
        if(event_nel() > 2.) continue;
        if(muons_p() >= 20.) continue;
        if(electrons_p() >= 20.) continue;
        NleptonCut++;

        //Add this  to the options, as in which selection to choose
        if (vis_M() <= 150.) continue;
        if (vis_E() <= 150.) continue;
        if (vis_theta()<= 0.15 || vis_theta()>=3.0 ) continue; //typo! rerun!
        NkineCut++;
        // change the cuts! update! 
        if (d_12()<=1500. || d_12()>=58000.) continue;//missing!
        if ((d_23()<=400.) || (d_23()>=18000.))continue;
        NdCutd123++;
        if ((d_34()<=100.) || (d_34()>=6000.))continue;
        NdCutd34++; 

        

        std::vector<TLorentzVector> LVjets;  
        // std::cout<< "jet_px.size() " << jet_px.size() << std::endl;
        // // Print all elements of jet_px
        // for (size_t i = 0; i < jet_px.size(); i++) {
        //     std::cout << "jet_px[" << i << "] = " << jet_px.at(i) << std::endl;
        // }

        for (size_t lv = 0; lv < jet_px.size(); ++lv) {
            // TLorentzVector LVjet(jet_px.getVal(lv), jet_py.getVal(lv), jet_pz.getVal(lv), jet_e.getVal(lv));
            TLorentzVector LVjet;
            // LVjet.SetPxPyPzE(jet_px.getVal(lv), jet_py.getVal(lv), jet_pz.getVal(lv), jet_e.getVal(lv));
            LVjet.SetPxPyPzE(jet_px.at(lv), jet_py.at(lv), jet_pz.at(lv), jet_e.at(lv));
            LVjets.push_back(LVjet);
        }


        std::map<int,std::vector<float>> jetFlavScores;
        jetFlavScores[0] = j0_flav;
        jetFlavScores[1] = j1_flav;
        jetFlavScores[2] = j2_flav;
        jetFlavScores[3] = j3_flav;
        jetFlavScores[4] = j4_flav;
        jetFlavScores[5] = j5_flav;


        std::vector<float> ::iterator j0_maxScore;
        std::vector<float> ::iterator j1_maxScore;
        std::vector<float> ::iterator j2_maxScore; 
        std::vector<float> ::iterator j3_maxScore; 
        std::vector<float> ::iterator j4_maxScore; 
        std::vector<float> ::iterator j5_maxScore; 

        j0_maxScore = std::max_element(j0_flav.begin(), j0_flav.end());
        j1_maxScore = std::max_element(j1_flav.begin(), j1_flav.end());
        j2_maxScore = std::max_element(j2_flav.begin(), j2_flav.end()); 
        j3_maxScore = std::max_element(j3_flav.begin(), j3_flav.end());
        j4_maxScore = std::max_element(j4_flav.begin(), j4_flav.end());
        j5_maxScore = std::max_element(j5_flav.begin(), j5_flav.end());

       //Can simplify with j<>_maxScore
        int j0_maxScoreIdx = std::distance(j0_flav.begin(),std::max_element(j0_flav.begin(), j0_flav.end()));
        int j1_maxScoreIdx = std::distance(j1_flav.begin(),std::max_element(j1_flav.begin(), j1_flav.end()));
        int j2_maxScoreIdx = std::distance(j2_flav.begin(),std::max_element(j2_flav.begin(), j2_flav.end()));
        int j3_maxScoreIdx = std::distance(j3_flav.begin(),std::max_element(j3_flav.begin(), j3_flav.end()));
        int j4_maxScoreIdx = std::distance(j4_flav.begin(),std::max_element(j4_flav.begin(), j4_flav.end()));
        int j5_maxScoreIdx = std::distance(j5_flav.begin(),std::max_element(j5_flav.begin(), j5_flav.end()));


        std::array<int, 6> maxScoreIdx {j0_maxScoreIdx,j1_maxScoreIdx,j2_maxScoreIdx,j3_maxScoreIdx,j4_maxScoreIdx,j5_maxScoreIdx};
        if (m_debug) {
            std::cout << "maxScoreIdx: ";
                for (float ScoreIdx : maxScoreIdx) {
                    std::cout << ScoreIdx << " ";
                }
                std::cout << std::endl;
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
                std::cout << std::endl;
            std::cout << "Jet score for j4: ";
                for (float j4_f : j4_flav) {
                    std::cout << j4_f << " ";
                }
                std::cout << std::endl;
            std::cout << "Jet score for j5: ";
                for (float j5_f : j5_flav) {
                    std::cout << j5_f << " ";
                }
                std::cout << std::endl;}
            //   Find the pairs
        if (m_debug) {
            std::cout<<"j0_flav: "<<" Max score: "<< *j0_maxScore<< " maxScoreIdx: "<< maxScoreIdx[0]<<std::endl;
            std::cout<<"j1_flav: "<<" Max score: "<< *j1_maxScore<< " maxScoreIdx: "<< maxScoreIdx[1]<<std::endl;
            std::cout<<"j2_flav: "<<" Max score: "<< *j2_maxScore<< " maxScoreIdx: "<< maxScoreIdx[2]<<std::endl;
            std::cout<<"j3_flav: "<<" Max score: "<< *j3_maxScore<< " maxScoreIdx: "<< maxScoreIdx[3]<<std::endl;
            std::cout<<"j4_flav: "<<" Max score: "<< *j4_maxScore<< " maxScoreIdx: "<< maxScoreIdx[4]<<std::endl;
            std::cout<<"j5_flav: "<<" Max score: "<< *j5_maxScore<< " maxScoreIdx: "<< maxScoreIdx[5]<<std::endl;
        }

        //Check how many jets have the same flavour highest score
        //Save the jet with same score 
        std::map<int,std::vector<int>> jetFlavMaxScore;
        // std::cout << " max score index size is : " << maxScoreIdx.size() << std::endl;
        for (std::size_t i = 0; i < maxScoreIdx.size(); ++i){
            jetFlavMaxScore[maxScoreIdx[i]].push_back(i);
        }
        

        // if (m_debug) {
            for (const auto& pair : jetFlavMaxScore) {
                std::cout << "Jet: " << pair.first << " has the following jets with the same score: ";
                for (const int& value : pair.second) {
                    std::cout << value << " ";
                }
                std::cout << std::endl;
            }
        // }
        // // initalize jet pairs 
        int nPairs = 0;

        std::vector<ZCandidate> vecOfZCandidates;
        std::vector<HCandidate> vecOfHCandidates;
        
        std::set<int> uniqueElements(maxScoreIdx.begin(), maxScoreIdx.end());
        // Print unique elements
        std::cout << "Unique elements in maxScoreIdx: ";
        for (const int& elem : uniqueElements) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;


        for (const auto& pair : jetFlavMaxScore){
            std::cout << "pair.second.size() :" << pair.second.size() << std::endl;
            std::cout << "pair.first:" << pair.first << std::endl;
            if (pair.second.size() == 6){
                std::cout << "All jets have the same flav " << pair.first << std::endl;
                for (const int& value : pair.second) {
                    for (const int& value2 : pair.second) {
                        if (value > value2){
                            ZCandidate ZCandidate;
                            ZCandidate.values = {value, value2};
                            ZCandidate.Jet1 = {LVjets[value]};
                            ZCandidate.Jet2 = {LVjets[value2]};
                            ZCandidate.Z_combined = LVjets[value]+LVjets[value2];
                            ZCandidate.Z_mass = (LVjets[value]+LVjets[value2]).M();
                            ZCandidate.Z_abs_diff = std::abs((LVjets[value]+LVjets[value2]).M()-Z_mass);
                            vecOfZCandidates.push_back(ZCandidate);
                        }
                    }
                }
            }
            if (pair.second.size() == 4){
                for (const int& value : pair.second) {
                    for (const int& value2 : pair.second) {
                        if (value > value2){
                            ZCandidate ZCandidate;
                            ZCandidate.values = {value, value2};
                            ZCandidate.Jet1 = {LVjets[value]};
                            ZCandidate.Jet2 = {LVjets[value2]};
                            ZCandidate.Z_combined = LVjets[value]+LVjets[value2];
                            ZCandidate.Z_mass = (LVjets[value]+LVjets[value2]).M();
                            ZCandidate.Z_abs_diff = std::abs((LVjets[value]+LVjets[value2]).M()-Z_mass);
                            vecOfZCandidates.push_back(ZCandidate);
                        }
                    }
                }
            }
            if (pair.second.size() == 2){
                for (const int& value : pair.second) {
                    for (const int& value2 : pair.second) {
                        if (value > value2){
                            ZCandidate ZCandidate;
                            ZCandidate.values = {value, value2};
                            ZCandidate.Jet1 = {LVjets[value]};
                            ZCandidate.Jet2 = {LVjets[value2]};
                            ZCandidate.Z_combined = LVjets[value]+LVjets[value2];
                            ZCandidate.Z_mass = (LVjets[value]+LVjets[value2]).M();
                            ZCandidate.Z_abs_diff = std::abs((LVjets[value]+LVjets[value2]).M()-Z_mass);
                            vecOfZCandidates.push_back(ZCandidate);
                        }
                    }
                }
            }
        }
        
        // std::cout << "vecOfZCandidates.size() :" << vecOfZCandidates.size() << std::endl;
        if (vecOfZCandidates.size()<3) continue;
        // for (const auto& ZCandidate : vecOfZCandidates){
           
        // }
        Z1 = vecOfZCandidates[0];
        Z2 = vecOfZCandidates[1];
        for (const auto& ZCandidate_iterator : vecOfZCandidates){
            
            if (ZCandidate_iterator.values == Z1.values || ZCandidate_iterator.values == Z2.values) {
                continue;
            }
            if (!std::isnan(ZCandidate_iterator.Z_abs_diff) && (ZCandidate_iterator.Z_abs_diff < Z1.Z_abs_diff)){
                Z2 = Z1;
                Z1 = ZCandidate_iterator;
            }
            else if (!std::isnan(ZCandidate_iterator.Z_abs_diff) && (ZCandidate_iterator.Z_abs_diff < Z2.Z_abs_diff)){
                Z2 = ZCandidate_iterator;
            }
        }
        for (const auto& ZCandidate_iterator : vecOfZCandidates){
            if (ZCandidate_iterator.values == Z1.values || ZCandidate_iterator.values == Z2.values) {
                continue;
            }
            else if (!std::isnan(ZCandidate_iterator.Z_abs_diff)){
                Zstar = ZCandidate_iterator;
                vecOfHCandidates.push_back(GetHCandidatesfromZs(Z1, Zstar));
                vecOfHCandidates.push_back(GetHCandidatesfromZs(Z2, Zstar));
            }
        }
        std::cout << "vecOfHCandidates.size() :" << vecOfHCandidates.size() << std::endl;
        if (vecOfHCandidates.size()<1) continue;

        H_ZZ = vecOfHCandidates[0];
        for (const auto& HCandidate : vecOfHCandidates){
            if (!std::isnan(HCandidate.H_abs_diff) && (HCandidate.H_abs_diff < H_ZZ.H_abs_diff)){
                H_ZZ = HCandidate;
            }
        }

        Zstar=H_ZZ.ZCandidate2;
        // printZcandidate(Z1);
        // printZcandidate(Z2);
        // printZcandidate(Zstar);

        if(Z1.Z_combined.M() > 150.) continue;
        if(Z2.Z_combined.M() > 250.) continue;
        if(Zstar.Z_combined.M() > 250.) continue;
        

        std::cout << "H_ZZ mass: " << H_ZZ.H_combined.M() << std::endl;
        mH_ZZ= H_ZZ.H_combined.M();
        mZ1_jj = Z1.Z_combined.M();
        mZ2_jj = Z2.Z_combined.M();
        mZ3_jj = Zstar.Z_combined.M();
        mc_weight = norm_weight;
        my_tree->Fill();


        // for (const auto& pair : jetFlavMaxScore) {
        //     if (nPairs == 3) continue;
        //     if (pair.second.size() == 6){
        //         if (m_debug) std::cout<<"All jets have the same flav "<< pair.first<<std::endl;
        //         pairFalv_1 = pair.first;
        //         pairFalv_2 = pair.first;
        //         pairFalv_3 = pair.first;
        //         nPairs++;
        //     }
        //     else if (pair.second.size() == 5){
        //         if (m_debug) std::cout<<"5 jets have same flavour "<<std::endl;
        //         pairFalv_1 = pair.first;
        //         float max_score = 0.;
        //         for (const int& j1 : pair.second) {
        //             for (const int& j2 : pair.second) {
        //                 if (j1>j2){
        //                     if (m_debug){
        //                         std::cout<<"max_score: "<< max_score<<std::endl;
        //                         std::cout<<"new_score: "<< jetFlavScores[j1][pairFalv_1] + jetFlavScores[j2][pairFalv_1]<<std::endl;
        //                         std::cout<<"j1: "<< j1<<" j2: "<<j2<<std::endl;
        //                     }
        //                     if (j1>j2 && (jetFlavScores[j1][pairFalv_1] + jetFlavScores[j2][pairFalv_1] > max_score) ){
        //                         max_score = jetFlavScores[j1][pairFalv_1] + jetFlavScores[j2][pairFalv_1];
        //                         pair_1[0] = j1;
        //                         pair_1[1] = j2;
        //                         if (m_debug) std::cout<<"updated max_score: "<< max_score<<std::endl;
        //                     }
        //                 }   
        //             }
        //         }
        //         nPairs++;
        //         getMissingPair(pair_1,pair_2);
        //         pairFalv_2 = getMaxScoreFlav(pair_2,jetFlavScores);
        //         nPairs++;
        //         if (m_debug){
        //             std::cout<<"Pair 1, pairFalv_1: "<< pairFalv_1 << ", jet: "<<std::endl;
        //             for (const int& value : pair_1) {
        //                         std::cout << value << " ";}

        //             std::cout << std::endl;
        //             std::cout<<"Pair 2, pairFalv_2: "<< pairFalv_2 << ", jet: "<<std::endl;
        //             for (const int& value : pair_2) {
        //                         std::cout << value << " ";}
        //             std::cout << std::endl;
        //         }
        //     }
        //     else if (pair.second.size() == 2){
        //         if (m_debug) std::cout<<"2 jets have same flavour "<<std::endl;
        //         nPairs++;
        //         if (pairFalv_1 == -1){
        //             if (m_debug) std::cout<<"First pair identified "<<std::endl;
        //             pairFalv_1 = pair.first;
        //             pair_1[0] = pair.second[0];
        //             pair_1[1] = pair.second[1];
        //             if (m_debug) std::cout<<"pairFalv_1: "<<pairFalv_1<<std::endl;
        //         }
        //         else if (pairFalv_2 == -1){
        //             if (m_debug) std::cout<<"Second pair identified "<<std::endl;
        //             pairFalv_2 = pair.first;
        //             pair_2[0] = pair.second[0];
        //             pair_2[1] = pair.second[1];
        //             if (m_debug) std::cout<<"pairFalv_2: "<<pairFalv_2<<std::endl;
        //         }
        //         else {
        //             std::cout<<"More than 2 pairs! Not possible!"<<std::endl;
        //             break;}
        //     }
        // }
        // if (nPairs==0) {
        //     std::cout<<"---------------------- No pairs, toss event ---------------------- "<<std::endl;
        //     continue;
        // }
        // if (nPairs==1) {
        //     if (pairFalv_2 == -1){
        //         if (m_debug) std::cout<<"Need to retrive one pair"<<std::endl;
        //         getMissingPair(pair_1,pair_2);
        //         pairFalv_2 = getMaxScoreFlav(pair_2,jetFlavScores);
        //         nPairs++;
        //         if (m_debug){
        //             std::cout<<"Pair 1, pairFalv_1: "<< pairFalv_1 << ", jet: "<<std::endl;
        //             for (const int& value : pair_1) {
        //                         std::cout << value << " ";}

        //             std::cout << std::endl;
        //             std::cout<<"Pair 2, pairFalv_2: "<< pairFalv_2 << ", jet: "<<std::endl;
        //             for (const int& value : pair_2) {
        //                         std::cout << value << " ";}
        //             std::cout << std::endl;
        //         }
        //     }
        // }
        // //Find which pair is Z and H 
        // if (pairFalv_1 == pairFalv_2){
        //     if (m_debug) std::cout<<"All same flav!"<<std::endl;
        //     if (m_debug) std::cout<<"flav_H!"<< pairFalv_1<<std::endl;
        //     if (m_debug) std::cout<<"flav_Z!"<< pairFalv_2<<std::endl;
        //     flav_H = pairFalv_2;
        //     flav_Z = pairFalv_1;
        //     double tmp_pair1_m = -1;
        //     double tmp_pair2_m = -1;
        //     double tmp_chi_comb = 10e10;
        //     for (const int n : {0, 1, 2}){
        //         for (const int m : {1, 2, 3}){
        //             if (m>n){
        //                 pair_1[0]=n;
        //                 pair_1[1]=m;
        //                 getMissingPair(pair_1,pair_2);
        //                 tmp_pair1_m = (LVjets[pair_1[0]] +  LVjets[pair_1[1]]).M();
        //                 tmp_pair2_m = (LVjets[pair_2[0]] +  LVjets[pair_2[1]]).M(); 
        //                 if (m_debug) std::cout<<"tmp_chi_comb! "<< pow((tmp_pair1_m-Z_mass), 2) +  pow((tmp_pair2_m-H_mass), 2)<<std::endl;
        //                 if (m_debug) std::cout<<"tmp_pair1_m! "<< tmp_pair1_m<<std::endl;
        //                 if (m_debug) std::cout<<"tmp_pair2_m! "<< tmp_pair2_m<<std::endl;
        //                 if (tmp_chi_comb > (pow((tmp_pair1_m-Z_mass), 2) +  pow((tmp_pair2_m-H_mass), 2))){
        //                     tmp_chi_comb = pow((tmp_pair1_m-Z_mass), 2) +  pow((tmp_pair2_m-H_mass), 2);
        //                     mjj_H = tmp_pair2_m;
        //                     mjj_Z = tmp_pair1_m;
        //                     flavSc_H = jetFlavScores[pair_2[0]][flav_H] + jetFlavScores[pair_2[1]][flav_H];
        //                     flavSc_Z = jetFlavScores[pair_1[0]][flav_Z] + jetFlavScores[pair_1[1]][flav_Z];
        //                 }
        //             }
        //         }
        //     }
        // }
        // else if (nPairs==2 && (pairFalv_1 != pairFalv_2)) {
        //     if (m_debug) std::cout<<"2x paris different flavour!"<<std::endl;
        //     double pair1_m = (LVjets[pair_1[0]] +  LVjets[pair_1[1]]).M(); 
        //     double pair2_m = (LVjets[pair_2[0]] +  LVjets[pair_2[1]]).M(); 
        //     if (m_debug) std::cout<<"Mass pair 1: "<<pair1_m<<"chi_Z: "<< pow((pair1_m-Z_mass), 2)<<std::endl;
        //     if (m_debug) std::cout<<"Mass pair 2: "<<pair2_m<<"chi_Z: "<< pow((pair2_m-Z_mass), 2)<<std::endl;
        //     if (pow((pair1_m-Z_mass), 2) <  pow((pair2_m-Z_mass), 2)){
        //         mjj_H = pair2_m;
        //         flav_H = pairFalv_2;
        //         mjj_Z = pair1_m;
        //         flav_Z = pairFalv_1;
        //         flavSc_H = jetFlavScores[pair_2[0]][flav_H] + jetFlavScores[pair_2[1]][flav_H];
        //         flavSc_Z = jetFlavScores[pair_1[0]][flav_Z] + jetFlavScores[pair_1[1]][flav_Z];
        //     }
        //     else{
        //         mjj_H = pair1_m;
        //         mjj_Z = pair2_m;
        //         flav_H = pairFalv_1;
        //         flav_Z = pairFalv_2;
        //         flavSc_Z = jetFlavScores[pair_2[0]][flav_Z] + jetFlavScores[pair_2[1]][flav_Z];
        //         flavSc_H = jetFlavScores[pair_1[0]][flav_H] + jetFlavScores[pair_1[1]][flav_H];

        //     }
        // }
        // else if (pairFalv_1 == -1 || pairFalv_2 == -1) {
        //     std::cout<<"Something is really worong, pairFalv_1 == -1 || pairFalv_2 == -1!"<<std::endl;
        //     break;
        // }
        // else {
        //     std::cout<<"Something is really worong!"<<std::endl;
        //     break;
        // }   
        // if (m_debug){
        //     std::cout<<"Higgs m: "<< mjj_H<< " flav: "<< flav_H<<" flavSc_H: " <<flavSc_H<<std::endl;
        //     std::cout<<"Z m: "<< mjj_Z<< " flav: "<< flav_Z<<" flavSc_Z: " <<flavSc_Z<<std::endl;
        // }
        // NafterPairing++;
        //  //A bit of selection 
        // float WW_cuts = sqrt(pow( mjj_Z-W_mass ,2) + pow( mjj_H-W_mass ,2));
        // float ZZ_cuts = sqrt(pow( mjj_Z-Z_mass ,2) + pow( mjj_H-Z_mass ,2));
        // if (m_debug) {
        //     std::cout << "WW_cuts: " << WW_cuts <<std::endl;
        //     std::cout << "ZZ_cuts: " << ZZ_cuts <<std::endl;}
        // if(WW_cuts<=10) continue;
        // if(ZZ_cuts<=10) continue;
        // NjjMassCut++;
        // if (50. >= mjj_Z)continue; 
        // if (mjj_Z >= H_mass) continue; 
        // //Fix
        // if (mjj_H<=Z_mass) continue;
        // // For now remove the Htautau as a cat, only bkg....
        // // if (H_flav == 6) continue;
        // //After parining and cuts 
        // //The flag e correction falg...
        // if (m_debug) std::cout << "Passed pairing and kine selection "  <<std::endl;
        // NafterSel++;
        // float flag_ecorr = 0.0;
        // //Add flag 
        // for (size_t je = 0; je < 4; ++je) { 
        //     if (jet_e.at(je) > 240.0 || jet_e.at(je) < 0.0) {
        //         flag_ecorr += 1000.0;}
        // }
        // if (flag_ecorr>=1000.) continue;

        // // if (flag_corr()>=10.) continue;
        // NafterFlagSel++;
        // // if (m_debug) std::cout << "Passed flag_corr cut "  <<std::endl; || flav_H == 6 
        // NPassed ++;
        

        // Hscore = flavSc_H;
        // Zscore = flavSc_Z;
        // Hflav = flav_H;
        // Zflav = flav_Z;
        // mc_weight = norm_weight;
        // my_tree->Fill();
        
        
        // if (m_debug) std::cout<<"---------------------- END OF EVENT PROC ---------------------- "<<std::endl;
    }

    //CutFlow

    // cutFlowHist->SetBinContent(1,NEventsInt);
    // cutFlowHist->SetBinContent(2,NokFlav);
    // cutFlowHist->SetBinContent(3,NjetCut);
    // cutFlowHist->SetBinContent(4,NleptonCut);
    // cutFlowHist->SetBinContent(5,NkineCut);
    // cutFlowHist->SetBinContent(6,NdCutd123);
    // cutFlowHist->SetBinContent(7,NdCutd34);
    // cutFlowHist->SetBinContent(8,NafterPairing);
    // cutFlowHist->SetBinContent(9,NjjMassCut);
    // cutFlowHist->SetBinContent(10,NafterSel);
    // cutFlowHist->SetBinContent(11,NafterFlagSel);
    // cutFlowHist->SetBinContent(12,Nfit);
}
