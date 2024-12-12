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

struct JetObject {
    TLorentzVector jet4Vector;
    std::vector<float> jet_flav_scores;
};

struct SixJetObject {
    JetObject jet0;
    JetObject jet1;
    JetObject jet2;
    JetObject jet3;
    JetObject jet4;
    JetObject jet5;
};

struct ZCandidate {
    JetObject Jet1;
    JetObject Jet2;
    double max_jet_flav_score;
    int max_jet_flav_score_idx;
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


std::vector<ZCandidate> GetZCandidates(struct SixJetObject SixJetObjectvar){

    std::vector<ZCandidate> vecOfZCandidates;
    std::vector<JetObject> vecOfJets = {SixJetObjectvar.jet0, SixJetObjectvar.jet1, SixJetObjectvar.jet2, SixJetObjectvar.jet3, SixJetObjectvar.jet4, SixJetObjectvar.jet5};
    for (const auto& jet1 : vecOfJets){
        for (const auto& jet2 : vecOfJets){
            if (jet1.jet4Vector == jet2.jet4Vector){
                continue;
            }
            ZCandidate ZCandidate;
            ZCandidate.Jet1 = jet1;
            ZCandidate.Jet2 = jet2;
            ZCandidate.Z_combined = jet1.jet4Vector + jet2.jet4Vector;
            ZCandidate.Z_mass = ZCandidate.Z_combined.M();
            ZCandidate.Z_abs_diff = std::abs(ZCandidate.Z_mass - 91.1876);
            std::vector<float> max_jet_flav_scores;
            for(int i = 0; i < 6; i++){
                max_jet_flav_scores.push_back(jet1.jet_flav_scores[i]+jet2.jet_flav_scores[i]);
            }
            ZCandidate.max_jet_flav_score = *std::max_element(max_jet_flav_scores.begin(), max_jet_flav_scores.end());
            if (ZCandidate.max_jet_flav_score < 1.0){
                continue;
            }            
            ZCandidate.max_jet_flav_score_idx = std::distance(max_jet_flav_scores.begin(), std::max_element(max_jet_flav_scores.begin(), max_jet_flav_scores.end()));
            vecOfZCandidates.push_back(ZCandidate);
        }
    }
    return vecOfZCandidates;

}


void AnalysisZHAllHad6J::printZcandidate(struct ZCandidate zCandidate){
    // std::cout << "Jet1: ";
    // for (const auto& jet : zCandidate.Jet1) {
    //     std::cout << "(" << jet.Px() << ", " << jet.Py() << ", " << jet.Pz() << ", " << jet.E() << ") ";
    // }
    // std::cout << std::endl;
    // std::cout << "Jet2: ";
    // for (const auto& jet : zCandidate.Jet2) {
    //     std::cout << "(" << jet.Px() << ", " << jet.Py() << ", " << jet.Pz() << ", " << jet.E() << ") ";
    // }
    // std::cout << std::endl;
    // std::cout << "Z Combined: ";
    // std::cout << "(" << zCandidate.Z_combined.Px() << ", " << zCandidate.Z_combined.Py() << ", " << zCandidate.Z_combined.Pz() << ", " << zCandidate.Z_combined.E() << ") ";
    
    // std::cout << std::endl;
    // std::cout << "Z Mass: " << zCandidate.Z_mass << std::endl;
    // std::cout << "Z Absolute Difference: " << zCandidate.Z_abs_diff << std::endl;
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


    // float Z_mass = 91.1876;
    // float H_mass = 125.11;
    // float W_mass = 80.377;

    std::vector<std::string> flavourCategory {"B", "C", "S","Q", "G"};
    std::vector<std::string> fitCategory {"LowHss","MidHss","HiHss","LowbbHbb","LowccHbb","LowssHbb","LowqqHbb","LowbbHcc","LowccHcc","LowssHcc","LowqqHcc","LowbbHgg","LowccHgg","LowssHgg","LowqqHgg","MidbbHbb","MidccHbb","MidssHbb","MidqqHbb","MidbbHcc","MidccHcc","MidssHcc","MidqqHcc","MidbbHgg","MidccHgg","MidssHgg","MidqqHgg","HibbHbb","HiccHbb","HissHbb","HiqqHbb","HibbHcc","HiccHcc","HissHcc","HiqqHcc","HibbHgg","HiccHgg","HissHgg","HiqqHgg","Incl"};
    std::vector<std::string> cutFlowMap {"NoCut","NoNaNFlavScore","njet=6","leptonCut","visMCut","visECut","KinematicsCut","d12Cut", "d23Cut","d34Cut","d45Cut","d56Cut","mZ1Cut","mZ2Cut","mZstarCut","mHCut","Fit" };
    std::vector<std::string> fitCategorySimple  {"LowHbb","LowHcc","LowHss","LowHgg","MidHbb","MidHcc","MidHss","MidHgg","HiHbb","HiHcc", "HiHss","HiHgg"};

    
    // Get the histograms

    auto countingHist = m_histContainer->getCountingHist();
    auto cutFlowHist =  m_histContainer->get1DHist("CutFlow", cutFlowMap.size(), 0, 18,cutFlowMap);   

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
    my_tree->Branch("b_Z1_j1FlavorIdx",&Z1_j1FlavorIdx);
    my_tree->Branch("b_Z1_j2FlavorIdx",&Z1_j2FlavorIdx);
    my_tree->Branch("b_Z1_FlavorScoreCombined",&Z1_FlavorScoreCombined);
    my_tree->Branch("b_Z2_FlavorScoreCombined",&Z2_FlavorScoreCombined);
    my_tree->Branch("b_Z3_FlavorScoreCombined",&Z3_FlavorScoreCombined);
    my_tree->Branch("b_Z2_j1FlavorIdx",&Z2_j1FlavorIdx);
    my_tree->Branch("b_Z2_j2FlavorIdx",&Z2_j2FlavorIdx);
    my_tree->Branch("b_Zstar_j1FlavorIdx",&Zstar_j1FlavorIdx);
    my_tree->Branch("b_Zstar_j2FlavorIdx",&Zstar_j2FlavorIdx);
    my_tree->Branch("b_DeltaR_HZ1Zstar_Z2",&DeltaR_HZZ_Z2);
    my_tree->Branch("b_DeltaPhi_HZZ_Z2",&DeltaPhi_HZZ_Z2);
    my_tree->Branch("b_d12",&v_d12);
    my_tree->Branch("b_d23",&v_d23);
    my_tree->Branch("b_d34",&v_d34);
    my_tree->Branch("b_d45",&v_d45);
    my_tree->Branch("b_d56",&v_d56);
    my_tree->Branch("b_vis_E",&vis_E);
    my_tree->Branch("b_vis_M",&vis_M);

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
    int NVisMCut = 0;
    int NVisECut = 0;
    int NjetCut = 0;
    int NleptonCut = 0;
    int NkineCut = 0;
    int NdCutd12 = 0;
    int NdCutd23 = 0;
    int NdCutd34 = 0;
    int NdCutd45 = 0;
    int NdCutd56 = 0;
    int HMassCut = 0;
    int Z1MassCut = 0;
    int Z2MassCut = 0;
    int ZstarMassCut = 0;
    // int NafterSel = 0;
    // int NafterFlagSel = 0;
    int Nfit = 0;
    int NEventsInt = 0;
   
    // int NPassed = -1 ;

    //     int debug = 1;
  
    // Loop over the trees here
    for(int i = 0; i < nEntries; i++)
    {
        if(i % 1000 == 0) std::cout<<"Done i: "<<i<<" out of "<<nEntries<<std::endl;

        treeCont->getEntry(i);
        // Just to store how many events were run over
        countingHist->Fill(1);
        NEventsInt++;

        bool flage_toss = false;
        if (recojet_isB.size() == 0){
            flage_toss = true;
        }
        if (flage_toss) continue;


        // - look for max score of jet 
        std::vector<float> j0_flav {recojet_isB.at(0), recojet_isC.at(0), recojet_isS.at(0), recojet_isU.at(0),recojet_isD.at(0), recojet_isG.at(0), recojet_isTAU.at(0)};
        std::vector<float> j1_flav {recojet_isB.at(1), recojet_isC.at(1), recojet_isS.at(1), recojet_isU.at(1),recojet_isD.at(1), recojet_isG.at(1), recojet_isTAU.at(1)};
        std::vector<float> j2_flav {recojet_isB.at(2), recojet_isC.at(2), recojet_isS.at(2), recojet_isU.at(2),recojet_isD.at(2), recojet_isG.at(2), recojet_isTAU.at(2)};
        std::vector<float> j3_flav {recojet_isB.at(3), recojet_isC.at(3), recojet_isS.at(3), recojet_isU.at(3),recojet_isD.at(3), recojet_isG.at(3), recojet_isTAU.at(3)};
        std::vector<float> j4_flav {recojet_isB.at(4), recojet_isC.at(4), recojet_isS.at(4), recojet_isU.at(4),recojet_isD.at(4), recojet_isG.at(4), recojet_isTAU.at(4)};
        std::vector<float> j5_flav {recojet_isB.at(5), recojet_isC.at(5), recojet_isS.at(5), recojet_isU.at(5),recojet_isD.at(5), recojet_isG.at(5), recojet_isTAU.at(5)};
        
        v_d12 = d_12();
        v_d23 = d_23();
        v_d34 = d_34();
        v_d45 = d_45();
        v_d56 = d_56();


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
        NVisMCut++;
        if (vis_E() <= 150.) continue;
        NVisECut++;
        if (vis_theta()<= 0.15 || vis_theta()>=3.0 ) continue; //typo! rerun!
        NkineCut++;
        // change the cuts! update!
        if (d_12()<=1500. || d_12()>=58000.) continue;//missing!

        NdCutd12++;
        if ((d_23()<=400.) || (d_23()>=18000.))continue;
        // if (d_23()>3600) continue;
        NdCutd23++;
        // if (d_34()>529) continue;
        if ((d_34()<=100.) || (d_34()>=6000.))continue;
        NdCutd34++;
        // if (d_45()>225) continue;
        NdCutd45++;
        // if (d_56()>100) continue;
        NdCutd56++;

        

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
        JetObject jet0;
        JetObject jet1;
        JetObject jet2;
        JetObject jet3;
        JetObject jet4;
        JetObject jet5;
        jet0.jet4Vector = LVjets[0];
        jet1.jet4Vector = LVjets[1];
        jet2.jet4Vector = LVjets[2];
        jet3.jet4Vector = LVjets[3];
        jet4.jet4Vector = LVjets[4];
        jet5.jet4Vector = LVjets[5];
        jet0.jet_flav_scores = j0_flav;
        jet1.jet_flav_scores = j1_flav;
        jet2.jet_flav_scores = j2_flav;
        jet3.jet_flav_scores = j3_flav;
        jet4.jet_flav_scores = j4_flav;
        jet5.jet_flav_scores = j5_flav;
        
        SixJetObject SixJetObject_instance;
        SixJetObject_instance.jet0 = jet0;
        SixJetObject_instance.jet1 = jet1;
        SixJetObject_instance.jet2 = jet2;
        SixJetObject_instance.jet3 = jet3;
        SixJetObject_instance.jet4 = jet4;
        SixJetObject_instance.jet5 = jet5;


        std::vector<ZCandidate> vecOfZCandidates;
        std::vector<HCandidate> vecOfHCandidates;

        vecOfZCandidates= GetZCandidates(SixJetObject_instance);

        ZCandidate Z1_HZZ;
        ZCandidate Z2_HZZ;
        ZCandidate Zstar_HZZ;
        HCandidate H_ZZ;

        if (vecOfZCandidates.size() < 3) continue;

        Z1_HZZ = vecOfZCandidates[0];
        Z2_HZZ = vecOfZCandidates[1];
        for (const auto& ZCandidate_iterator : vecOfZCandidates){
            if (ZCandidate_iterator.Z_mass == Z1_HZZ.Z_mass || ZCandidate_iterator.Z_mass == Z2_HZZ.Z_mass) {
                continue;
            }
            if (!std::isnan(ZCandidate_iterator.Z_abs_diff) && (ZCandidate_iterator.Z_abs_diff < Z1_HZZ.Z_abs_diff) && (ZCandidate_iterator.Z_mass > Z2_HZZ.Z_mass)){
                Z2_HZZ = Z1_HZZ;
                Z1_HZZ = ZCandidate_iterator;
            }
            else if (!std::isnan(ZCandidate_iterator.Z_abs_diff) && (ZCandidate_iterator.Z_abs_diff < Z2_HZZ.Z_abs_diff) && (ZCandidate_iterator.Z_mass > Z1_HZZ.Z_mass)){
                Z2_HZZ = ZCandidate_iterator;
            }
        }


        for (const auto& ZCandidate_iterator : vecOfZCandidates){
            if (ZCandidate_iterator.Z_mass == Z1_HZZ.Z_mass || ZCandidate_iterator.Z_mass == Z2_HZZ.Z_mass) {
                continue;
            }
            else if (!std::isnan(ZCandidate_iterator.Z_abs_diff)){
                Zstar_HZZ = ZCandidate_iterator;
                vecOfHCandidates.push_back(GetHCandidatesfromZs(Z1_HZZ, Zstar_HZZ));
                vecOfHCandidates.push_back(GetHCandidatesfromZs(Z2_HZZ, Zstar_HZZ));
            }
        }

        if (vecOfHCandidates.size()<1) continue;

        H_ZZ = vecOfHCandidates[0];
        for (const auto& HCandidate_iterator : vecOfHCandidates){
            if (!std::isnan(HCandidate_iterator.H_abs_diff) && (HCandidate_iterator.H_abs_diff < H_ZZ.H_abs_diff)){
                H_ZZ = HCandidate_iterator;
            }
        }

        Zstar_HZZ=H_ZZ.ZCandidate2;
        ZCandidate Z_temp;
        if ( Z2_HZZ.Z_mass == H_ZZ.ZCandidate1.Z_mass) {
            Z_temp=Z1_HZZ;
            Z1_HZZ=Z2_HZZ;
            Z2_HZZ=Z_temp;
        }
        

        if( Z1_HZZ.Z_combined.M() < 70. || Z1_HZZ.Z_combined.M() > 120.) continue;
        Z1MassCut++;
        if(Z2_HZZ.Z_combined.M() < 70. || Z2_HZZ.Z_combined.M() > 100.) continue;
        Z2MassCut++;
        if(Zstar_HZZ.Z_combined.M() > 16.) continue;
        ZstarMassCut++;
        if(H_ZZ.H_combined.M() < 83. || H_ZZ.H_combined.M() > 150.) continue;
        HMassCut++;
        if (Z2_HZZ.max_jet_flav_score_idx < 3 || Z2_HZZ.max_jet_flav_score_idx > 4 ) continue;

        mH_ZZ= H_ZZ.H_combined.M();
        mZ1_jj = Z1_HZZ.Z_combined.M();
        mZ2_jj = Z2_HZZ.Z_combined.M();
        mZ3_jj = Zstar_HZZ.Z_combined.M();
        Z1_j1FlavorIdx = Z1_HZZ.max_jet_flav_score_idx;
        Z1_j2FlavorIdx = Z1_HZZ.max_jet_flav_score_idx;
        Z2_j1FlavorIdx = Z2_HZZ.max_jet_flav_score_idx;
        Z2_j2FlavorIdx = Z2_HZZ.max_jet_flav_score_idx;
        Zstar_j1FlavorIdx = Zstar_HZZ.max_jet_flav_score_idx;
        Zstar_j2FlavorIdx = Zstar_HZZ.max_jet_flav_score_idx;
        Z1_FlavorScoreCombined = Z1_HZZ.max_jet_flav_score;
        Z2_FlavorScoreCombined = Z2_HZZ.max_jet_flav_score;
        Z3_FlavorScoreCombined = Zstar_HZZ.max_jet_flav_score;
        DeltaR_HZZ_Z2 = H_ZZ.H_combined.DeltaR(Z2_HZZ.Z_combined);
        DeltaPhi_HZZ_Z2 = H_ZZ.H_combined.DeltaPhi(Z2_HZZ.Z_combined);
        // if(DeltaR_HZZ_Z2 < 3.1) continue;
        mc_weight = norm_weight;
        Nfit++;
        my_tree->Fill();
        // break;
    }

    //CutFlow

    cutFlowHist->SetBinContent(1,NEventsInt);
    cutFlowHist->SetBinContent(2,NokFlav);
    cutFlowHist->SetBinContent(3,NjetCut);
    cutFlowHist->SetBinContent(4,NleptonCut);
    cutFlowHist->SetBinContent(5,NVisMCut);
    cutFlowHist->SetBinContent(6,NVisECut);
    cutFlowHist->SetBinContent(7,NkineCut);
    cutFlowHist->SetBinContent(8,NdCutd12);
    cutFlowHist->SetBinContent(9,NdCutd23);
    cutFlowHist->SetBinContent(10,NdCutd34);
    cutFlowHist->SetBinContent(11,NdCutd45);
    cutFlowHist->SetBinContent(12,NdCutd56);
    cutFlowHist->SetBinContent(13,Z1MassCut);
    cutFlowHist->SetBinContent(14,Z2MassCut);
    cutFlowHist->SetBinContent(15,ZstarMassCut);
    cutFlowHist->SetBinContent(16,HMassCut);
    cutFlowHist->SetBinContent(17,Nfit);
}
