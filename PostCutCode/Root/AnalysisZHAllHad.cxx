// Local includes
#include "PostCutCode/AnalysisZHAllHad.h"
#include "PostCutCode/TreeContainer.h"
#include <iomanip>
#include <TFile.h>
#include <fstream>
#include <set>
#include "TLorentzVector.h"
#include <TRandom.h>

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

int AnalysisZHAllHad::getMaxScoreFlav(const std::vector<int>& vec, const std::map<int, std::vector<float>>& mapFlav){
    float max_score = 0;
    int flav = -1;
    int j1 = vec[0];
    int j2 = vec[1];
    if (m_debug) std::cout<<"getMaxScoreFlav!"<<std::endl;
    for(int i = 0; i < 7; i++){
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

void AnalysisZHAllHad::getMissingPair(const std::vector<int>& foundPair, std::vector<int>& missingPair){
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

void AnalysisZHAllHad::run()
{       
    // gRandom->SetSeed(42); 
    float Z_mass = 91.1876;
    float H_mass = 125.11;
    float W_mass = 80.377;
    std::vector<std::string> flavourJets {"b", "c", "s", "g", "q"};
    std::vector<std::string> flavour        {"B","C","S","U","D","G","TAU"};
    std::vector<std::string> flavourCategory {"B", "C", "S","Q", "G","TAU"};
    std::vector<std::string> fitCategory {/*"LowHss","MidHss","HiHss",*/
                                          "LowbbHbb","LowccHbb","LowssHbb","LowqqHbb","LowggHbb","LowttHbb",
                                          "MidbbHbb","MidccHbb","MidssHbb","MidqqHbb","MidggHbb","MidttHbb",
                                          "HibbHbb", "HiccHbb", "HissHbb", "HiqqHbb", "HiggHbb", "HittHbb",
                                          "LowbbHcc","LowccHcc","LowssHcc","LowqqHcc","LowggHcc","LowttHcc",
                                          "MidbbHcc","MidccHcc","MidssHcc","MidqqHcc","MidggHcc","MidttHcc",
                                          "HibbHcc", "HiccHcc", "HissHcc", "HiqqHcc", "HiggHcc", "HittHcc",
                                          "LowbbHss","LowccHss","LowssHss","LowqqHss","LowggHss","LowttHss",
                                          "MidbbHss","MidccHss","MidssHss","MidqqHss","MidggHss","MidttHss",
                                          "HibbHss", "HiccHss", "HissHss", "HiqqHss", "HiggHss", "HittHss",
                                          "LowbbHqq","LowccHqq","LowssHqq","LowqqHqq","LowggHqq","LowttHqq",
                                          "MidbbHqq","MidccHqq","MidssHqq","MidqqHqq","MidggHqq","MidttHqq",
                                          "HibbHqq", "HiccHqq", "HissHqq", "HiqqHqq", "HiggHqq", "HittHqq",
                                          "LowbbHgg","LowccHgg","LowssHgg","LowqqHgg","LowggHgg","LowttHgg",
                                          "MidbbHgg","MidccHgg","MidssHgg","MidqqHgg","MidggHgg","MidttHgg",
                                          "HibbHgg", "HiccHgg", "HissHgg", "HiqqHgg", "HiggHgg", "HittHgg",
                                          "LowbbHtt","LowccHtt","LowssHtt","LowqqHtt","LowggHtt","LowttHtt",
                                          "MidbbHtt","MidccHtt","MidssHtt","MidqqHtt","MidggHtt","MidttHtt",
                                          "HibbHtt", "HiccHtt", "HissHtt", "HiqqHtt", "HiggHtt", "HittHtt",
                                          "Incl"};
    std::vector<std::string> cutFlowMap {"NoCut","njet=4","leptonCut","KineCut", "d123Cut", "d34Cut","Pairing","jjMassCut","ZHmassCut","YieldsFit" };
    std::vector<std::string> fitCategorySimple  {"LowHbb","LowHcc","LowHss","LowHgg","MidHbb","MidHcc","MidHss","MidHgg","HiHbb","HiHcc", "HiHss","HiHgg"};

    
    // Get the histograms
    // auto histo_DetVars = m_histContainer->histo_DetVarsScoreSmear(flavourJets);

    auto scoreMapHist = m_histContainer->get1DHist("scoreMap_1D", flavourCategory.size(), 0, flavourCategory.size(), flavourCategory);
    auto scoreMapFitCatHist = m_histContainer->get1DHist("scoreMapFitCategory_1D", fitCategorySimple.size(), 0, fitCategorySimple.size(), fitCategorySimple);
    auto CountsMapHist = m_histContainer->get1DHist("CountsMap_1D", flavourCategory.size(), 0, flavourCategory.size(), flavourCategory);
    auto CountsFitCatHist = m_histContainer->get1DHist("CountsFitCategory_1D", fitCategorySimple.size(), 0, fitCategorySimple.size(), fitCategorySimple);
    auto obsHist = m_histContainer->getObsHistinFitCategory(fitCategory, 250, 0., 250.,250, 0., 250);
//     auto obsHist = m_histContainer->getObsHistinFitCategory(fitCategory, 90,90.,180.,70,50.,120.);
//     auto obsHist = m_histContainer->getObsHistinFitCategory(fitCategory, 250, 0., 250.);
    auto countingHist = m_histContainer->getCountingHist();
    auto cutFlowHist =  m_histContainer->get1DHist("CutFlow", cutFlowMap.size(), 0, 13,cutFlowMap);

    // auto Bscore =  m_histContainer->get1DHist("Higgs_Bscore", 100, 0, 2);
    // auto Cscore =  m_histContainer->get1DHist("Higgs_Cscore", 100, 0, 2);
    // auto Sscore =  m_histContainer->get1DHist("Higgs_Sscore", 100, 0, 2);
    // auto Qscore =  m_histContainer->get1DHist("Higgs_Qscore", 100, 0, 2);
    // auto Gscore =  m_histContainer->get1DHist("Higgs_Gscore", 100, 0, 2);
    
    // auto Z_Bscore =  m_histContainer->get1DHist("Z_Bscore", 100, 0, 2);
    // auto Z_Cscore =  m_histContainer->get1DHist("Z_Cscore", 100, 0, 2);
    // auto Z_Sscore =  m_histContainer->get1DHist("Z_Sscore", 100, 0, 2);
    // auto Z_Qscore =  m_histContainer->get1DHist("Z_Qscore", 100, 0, 2);
    // auto Z_Gscore =  m_histContainer->get1DHist("Z_Gscore", 100, 0, 2);
//     auto TAUscore =  m_histContainer->get1DHist("TAUscore", 100, 0, 2);
        // auto Uscore =  m_histContainer->get1DHist("Uscore", 100, 0, 2);


    // auto mZ =  m_histContainer->get1DHist("mZ",250, 0., 250.);
    // auto mH =  m_histContainer->get1DHist("mH", 250, 0., 250.);
    // auto mH_corr = m_histContainer->get1DHist("mH_corr", 250, 0., 250.);

    // auto ZZ_cut =  m_histContainer->get1DHist("ZZ_cut",250, 0., 250.);
    // auto WW_cut =  m_histContainer->get1DHist("WW_cut", 250, 0., 250.);

    // auto h_vis_M =  m_histContainer->get1DHist("vis_M",250, 0., 250.);
    // auto h_vis_E =  m_histContainer->get1DHist("vis_e", 250, 0., 250.);
    // auto h_vis_theta =  m_histContainer->get1DHist("vis_theta", 80, 0., 4.);

    // auto h_muons_p =  m_histContainer->get1DHist("muons_p", 125, 0., 250.);
    // auto h_electrons_p =  m_histContainer->get1DHist("electrons_p", 125, 0., 250.);
    // auto h_event_nmu =  m_histContainer->get1DHist("event_nmu", 10, 0., 10.);
    // auto h_event_nel =  m_histContainer->get1DHist("event_nel", 10, 0., 10.);

    // auto h_d_12 =  m_histContainer->get1DHist("d_12", 1000,14000, 60000.);
    // auto h_d_23 =  m_histContainer->get1DHist("d_23", 1000,200, 20000.);
    // auto h_d_34 =  m_histContainer->get1DHist("d_34", 1000,50, 6500.);

        // Extra hist for easy fill up
    auto Incl_obsHist = obsHist["Incl"];
    // H->bb
    auto Low_bbZ_Hbb_obsHist = obsHist["LowbbHbb"];
    auto Low_ccZ_Hbb_obsHist = obsHist["LowccHbb"];
    auto Low_ssZ_Hbb_obsHist = obsHist["LowssHbb"];
    auto Low_qqZ_Hbb_obsHist = obsHist["LowqqHbb"];
    auto Low_ggZ_Hbb_obsHist = obsHist["LowggHbb"];
    auto Low_ttZ_Hbb_obsHist = obsHist["LowttHbb"];

    auto Mid_bbZ_Hbb_obsHist = obsHist["MidbbHbb"];
    auto Mid_ccZ_Hbb_obsHist = obsHist["MidccHbb"];
    auto Mid_ssZ_Hbb_obsHist = obsHist["MidssHbb"];
    auto Mid_qqZ_Hbb_obsHist = obsHist["MidqqHbb"];
    auto Mid_ggZ_Hbb_obsHist = obsHist["MidggHbb"];
    auto Mid_ttZ_Hbb_obsHist = obsHist["MidttHbb"];

    auto Hi_bbZ_Hbb_obsHist = obsHist["HibbHbb"];
    auto Hi_ccZ_Hbb_obsHist = obsHist["HiccHbb"];
    auto Hi_ssZ_Hbb_obsHist = obsHist["HissHbb"];
    auto Hi_qqZ_Hbb_obsHist = obsHist["HiqqHbb"];
    auto Hi_ggZ_Hbb_obsHist = obsHist["HiggHbb"];
    auto Hi_ttZ_Hbb_obsHist = obsHist["HittHbb"];

    //  H->cc
    auto Low_bbZ_Hcc_obsHist = obsHist["LowbbHcc"];
    auto Low_ccZ_Hcc_obsHist = obsHist["LowccHcc"];
    auto Low_ssZ_Hcc_obsHist = obsHist["LowssHcc"];
    auto Low_qqZ_Hcc_obsHist = obsHist["LowqqHcc"];
    auto Low_ggZ_Hcc_obsHist = obsHist["LowggHcc"];
    auto Low_ttZ_Hcc_obsHist = obsHist["LowttHcc"];

    auto Mid_bbZ_Hcc_obsHist = obsHist["MidbbHcc"];
    auto Mid_ccZ_Hcc_obsHist = obsHist["MidccHcc"];
    auto Mid_ssZ_Hcc_obsHist = obsHist["MidssHcc"];
    auto Mid_qqZ_Hcc_obsHist = obsHist["MidqqHcc"];
    auto Mid_ggZ_Hcc_obsHist = obsHist["MidggHcc"];
    auto Mid_ttZ_Hcc_obsHist = obsHist["MidttHcc"];

    auto Hi_bbZ_Hcc_obsHist = obsHist["HibbHcc"];
    auto Hi_ccZ_Hcc_obsHist = obsHist["HiccHcc"];
    auto Hi_ssZ_Hcc_obsHist = obsHist["HissHcc"];
    auto Hi_qqZ_Hcc_obsHist = obsHist["HiqqHcc"];
    auto Hi_ggZ_Hcc_obsHist = obsHist["HiggHcc"];
    auto Hi_ttZ_Hcc_obsHist = obsHist["HittHcc"];

    // H->ss
    auto Low_bbZ_Hss_obsHist = obsHist["LowbbHss"];
    auto Low_ccZ_Hss_obsHist = obsHist["LowccHss"];
    auto Low_ssZ_Hss_obsHist = obsHist["LowssHss"];
    auto Low_qqZ_Hss_obsHist = obsHist["LowqqHss"];
    auto Low_ggZ_Hss_obsHist = obsHist["LowggHss"];
    auto Low_ttZ_Hss_obsHist = obsHist["LowttHss"];

    auto Mid_bbZ_Hss_obsHist = obsHist["MidbbHss"];
    auto Mid_ccZ_Hss_obsHist = obsHist["MidccHss"];
    auto Mid_ssZ_Hss_obsHist = obsHist["MidssHss"];
    auto Mid_qqZ_Hss_obsHist = obsHist["MidqqHss"];
    auto Mid_ggZ_Hss_obsHist = obsHist["MidggHss"];
    auto Mid_ttZ_Hss_obsHist = obsHist["MidttHss"];

    auto Hi_bbZ_Hss_obsHist = obsHist["HibbHss"];
    auto Hi_ccZ_Hss_obsHist = obsHist["HiccHss"];
    auto Hi_ssZ_Hss_obsHist = obsHist["HissHss"];
    auto Hi_qqZ_Hss_obsHist = obsHist["HiqqHss"];
    auto Hi_ggZ_Hss_obsHist = obsHist["HiggHss"];
    auto Hi_ttZ_Hss_obsHist = obsHist["HittHss"];

    // H->qq
    auto Low_bbZ_Hqq_obsHist = obsHist["LowbbHqq"];
    auto Low_ccZ_Hqq_obsHist = obsHist["LowccHqq"];
    auto Low_ssZ_Hqq_obsHist = obsHist["LowssHqq"];
    auto Low_qqZ_Hqq_obsHist = obsHist["LowqqHqq"];
    auto Low_ggZ_Hqq_obsHist = obsHist["LowggHqq"];
    auto Low_ttZ_Hqq_obsHist = obsHist["LowttHqq"];

    auto Mid_bbZ_Hqq_obsHist = obsHist["MidbbHqq"];
    auto Mid_ccZ_Hqq_obsHist = obsHist["MidccHqq"];
    auto Mid_ssZ_Hqq_obsHist = obsHist["MidssHqq"];
    auto Mid_qqZ_Hqq_obsHist = obsHist["MidqqHqq"];
    auto Mid_ggZ_Hqq_obsHist = obsHist["MidggHqq"];
    auto Mid_ttZ_Hqq_obsHist = obsHist["MidttHqq"];

    auto Hi_bbZ_Hqq_obsHist = obsHist["HibbHqq"];
    auto Hi_ccZ_Hqq_obsHist = obsHist["HiccHqq"];
    auto Hi_ssZ_Hqq_obsHist = obsHist["HissHqq"];
    auto Hi_qqZ_Hqq_obsHist = obsHist["HiqqHqq"];
    auto Hi_ggZ_Hqq_obsHist = obsHist["HiggHqq"];
    auto Hi_ttZ_Hqq_obsHist = obsHist["HittHqq"];

    // // H->gg
    auto Low_bbZ_Hgg_obsHist = obsHist["LowbbHgg"];
    auto Low_ccZ_Hgg_obsHist = obsHist["LowccHgg"];
    auto Low_ssZ_Hgg_obsHist = obsHist["LowssHgg"];
    auto Low_qqZ_Hgg_obsHist = obsHist["LowqqHgg"];
    auto Low_ggZ_Hgg_obsHist = obsHist["LowggHgg"];
    auto Low_ttZ_Hgg_obsHist = obsHist["LowttHgg"];

    auto Mid_bbZ_Hgg_obsHist = obsHist["MidbbHgg"];
    auto Mid_ccZ_Hgg_obsHist = obsHist["MidccHgg"];
    auto Mid_ssZ_Hgg_obsHist = obsHist["MidssHgg"];
    auto Mid_qqZ_Hgg_obsHist = obsHist["MidqqHgg"];
    auto Mid_ggZ_Hgg_obsHist = obsHist["MidggHgg"];
    auto Mid_ttZ_Hgg_obsHist = obsHist["MidttHgg"];

    auto Hi_bbZ_Hgg_obsHist = obsHist["HibbHgg"];
    auto Hi_ccZ_Hgg_obsHist = obsHist["HiccHgg"];
    auto Hi_ssZ_Hgg_obsHist = obsHist["HissHgg"];
    auto Hi_qqZ_Hgg_obsHist = obsHist["HiqqHgg"];
    auto Hi_ggZ_Hgg_obsHist = obsHist["HiggHgg"];
    auto Hi_ttZ_Hgg_obsHist = obsHist["HittHgg"];
    
    // H->tautau
    auto Low_bbZ_Htt_obsHist = obsHist["LowbbHtt"];
    auto Low_ccZ_Htt_obsHist = obsHist["LowccHtt"];
    auto Low_ssZ_Htt_obsHist = obsHist["LowssHtt"];
    auto Low_qqZ_Htt_obsHist = obsHist["LowqqHtt"];
    auto Low_ggZ_Htt_obsHist = obsHist["LowggHtt"];
    auto Low_ttZ_Htt_obsHist = obsHist["LowttHtt"];

    auto Mid_bbZ_Htt_obsHist = obsHist["MidbbHtt"];
    auto Mid_ccZ_Htt_obsHist = obsHist["MidccHtt"];
    auto Mid_ssZ_Htt_obsHist = obsHist["MidssHtt"];
    auto Mid_qqZ_Htt_obsHist = obsHist["MidqqHtt"];
    auto Mid_ggZ_Htt_obsHist = obsHist["MidggHtt"];
    auto Mid_ttZ_Htt_obsHist = obsHist["MidttHtt"];

    auto Hi_bbZ_Htt_obsHist = obsHist["HibbHtt"];
    auto Hi_ccZ_Htt_obsHist = obsHist["HiccHtt"];
    auto Hi_ssZ_Htt_obsHist = obsHist["HissHtt"];
    auto Hi_qqZ_Htt_obsHist = obsHist["HiqqHtt"];
    auto Hi_ggZ_Htt_obsHist = obsHist["HiggHtt"];
    auto Hi_ttZ_Htt_obsHist = obsHist["HittHtt"];


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
    my_tree->Branch("b_mH_jj",&mH_jj);
    my_tree->Branch("b_mZ_jj",&mZ_jj);
    my_tree->Branch("b_Hscore",&Hscore);
    my_tree->Branch("b_Zscore",&Zscore);
    my_tree->Branch("b_Hflav",&Hflav);
    my_tree->Branch("b_Zflav",&Zflav);
    my_tree->Branch("b_ChiH",&ChiH);
    my_tree->Branch("b_ChiZ",&ChiZ);
    my_tree->Branch("b_flag",&flag);
    my_tree->Branch("weight", &weight);
  
    // assuming you have vectors as input (should also save option to run w/o vectors)
  
    // flavor scores
       // Connect branches to trees
    varMemberVector<double> jet_px{tree, { "jet0_px_corr","jet1_px_corr", "jet2_px_corr", "jet3_px_corr"}, -999};
    varMemberVector<double> jet_py{tree, { "jet0_py_corr","jet1_py_corr", "jet2_py_corr", "jet3_py_corr"}, -999};
    varMemberVector<double> jet_pz{tree, { "jet0_pz_corr","jet1_pz_corr", "jet2_pz_corr", "jet3_pz_corr"}, -999};
    varMemberVector<double> jet_e{tree, { "jet0_e_corr","jet1_e_corr", "jet2_e_corr", "jet3_e_corr"}, -999};

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
    varMember<float> muons_p {tree, "muons_p"};
    varMember<float> electrons_p {tree, "electrons_p"};
    varMember<double> vis_E {tree, "vis_E"};
    varMember<double> vis_M {tree, "vis_M"};
    varMember<double> vis_theta {tree, "vis_theta"};
    varMember<float> d_12 {tree, "d_12"};
    varMember<float> d_23 {tree, "d_23"};
    varMember<float> d_34 {tree, "d_34"};
    varMember<double> flag_corr {tree, "flag_corr"};

    // varMember<double> M_j0_j1_corr {tree, "M_j0_j1_corr"};
    // varMember<double> M_j0_j2_corr {tree, "M_j0_j2_corr"};
    // varMember<double> M_j0_j3_corr {tree, "M_j0_j3_corr"};
    // varMember<double> M_j1_j2_corr {tree, "M_j1_j2_corr"};
    // varMember<double> M_j1_j3_corr {tree, "M_j1_j3_corr"};
    // varMember<double> M_j2_j3_corr {tree, "M_j2_j3_corr"};

    

    // varMember<double> costhetainv {tree, "costhetainv"};
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
    int BlikeEvents = 0;
    int ClikeEvents = 0;
    int SlikeEvents = 0;
    int GlikeEvents = 0;
    int QlikeEvents = 0;

    int NPassed = -1 ;
    double normWeight = getNormWeight();
//     int TAUlikeEvents = 0;

    std::array<int, 3> BlikeEvents_cat {0, 0, 0};
    std::array<int, 3> ClikeEvents_cat {0, 0, 0};
    std::array<int, 3> SlikeEvents_cat {0, 0, 0};
    std::array<int, 3> GlikeEvents_cat {0, 0, 0};
    std::array<int, 3> QlikeEvents_cat {0, 0, 0};
//     std::array<int, 3> TAUlikeEvents_cat {0, 0, 0};

  
    // Loop over the trees here
    for(int i = 0; i < nEntries; i++)
    {
        treeCont->getEntry(i);
        // Just to store how many events were run over
        countingHist->Fill(1);
        NEventsInt++;
        double mjj_H = -1;
        double mjj_Z = -1;
        double flavSc_H = -1;
        double flavSc_Z = -1;
        int flav_H = -1;
        int flav_Z = -1;

        if(i % 1000 == 0) std::cout<<"Done i: "<<i<<" out of "<<nEntries<<std::endl;
        // Add some basic selection        
        if(event_njet() != 4) continue; //Require EXACTLY 4 jet! This SHOULD be the case!
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
        if (d_12()<=15000. || d_12()>=58000.) continue;//missing!
        if ((d_23()<=400.) || (d_23()>=18000.))continue;
        NdCutd123++;
        if ((d_34()<=100.) || (d_34()>=6000.))continue;
        NdCutd34++; 
        std::vector<TLorentzVector> LVjets;    
        for (size_t lv = 0; lv < 4; ++lv) {
            // TLorentzVector LVjet(jet_px.getVal(lv), jet_py.getVal(lv), jet_pz.getVal(lv), jet_e.getVal(lv));
            TLorentzVector LVjet;
            LVjet.SetPxPyPzE(jet_px.getVal(lv), jet_py.getVal(lv), jet_pz.getVal(lv), jet_e.getVal(lv));
            LVjets.push_back(LVjet);
        }

        // - look for max score of jet 
        std::vector<float> j0_flav {jet0_scoreB(), jet0_scoreC(), jet0_scoreS(), jet0_scoreU(),jet0_scoreD(), jet0_scoreG(), jet0_scoreTAU()};
        std::vector<float> j1_flav {jet1_scoreB(), jet1_scoreC(), jet1_scoreS(), jet1_scoreU(),jet1_scoreD(), jet1_scoreG(), jet1_scoreTAU()};
        std::vector<float> j2_flav {jet2_scoreB(), jet2_scoreC(), jet2_scoreS(), jet2_scoreU(),jet2_scoreD(), jet2_scoreG(), jet2_scoreTAU()};
        std::vector<float> j3_flav {jet3_scoreB(), jet3_scoreC(), jet3_scoreS(), jet3_scoreU(),jet3_scoreD(), jet3_scoreG(), jet3_scoreTAU()};

        std::map<int,std::vector<float>> jetFlavScores;
        jetFlavScores[0] = j0_flav;
        jetFlavScores[1] = j1_flav;
        jetFlavScores[2] = j2_flav;
        jetFlavScores[3] = j3_flav;

        std::vector<float> ::iterator j0_maxScore;
        std::vector<float> ::iterator j1_maxScore;
        std::vector<float> ::iterator j2_maxScore; 
        std::vector<float> ::iterator j3_maxScore; 

        j0_maxScore = std::max_element(j0_flav.begin(), j0_flav.end());
        j1_maxScore = std::max_element(j1_flav.begin(), j1_flav.end());
        j2_maxScore = std::max_element(j2_flav.begin(), j2_flav.end()); 
        j3_maxScore = std::max_element(j3_flav.begin(), j3_flav.end());
       //Can simplify with j<>_maxScore
        int j0_maxScoreIdx = std::distance(j0_flav.begin(),std::max_element(j0_flav.begin(), j0_flav.end()));
        int j1_maxScoreIdx = std::distance(j1_flav.begin(),std::max_element(j1_flav.begin(), j1_flav.end()));
        int j2_maxScoreIdx = std::distance(j2_flav.begin(),std::max_element(j2_flav.begin(), j2_flav.end()));
        int j3_maxScoreIdx = std::distance(j3_flav.begin(),std::max_element(j3_flav.begin(), j3_flav.end()));

        std::array<int, 4> maxScoreIdx {j0_maxScoreIdx,j1_maxScoreIdx,j2_maxScoreIdx,j3_maxScoreIdx};
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
                std::cout << std::endl;}
            //   Find the pairs
        if (m_debug) {
            std::cout<<"j0_flav: "<<" Max score: "<< *j0_maxScore<< " maxScoreIdx: "<< maxScoreIdx[0]<<std::endl;
            std::cout<<"j1_flav: "<<" Max score: "<< *j1_maxScore<< " maxScoreIdx: "<< maxScoreIdx[1]<<std::endl;
            std::cout<<"j2_flav: "<<" Max score: "<< *j2_maxScore<< " maxScoreIdx: "<< maxScoreIdx[2]<<std::endl;
            std::cout<<"j3_flav: "<<" Max score: "<< *j3_maxScore<< " maxScoreIdx: "<< maxScoreIdx[3]<<std::endl;
        }
        
        //Check how many jets have the same flavour highest score
        //Save the jet with same score 
        std::map<int,std::vector<int>> jetFlavMaxScore;
        for (std::size_t i = 0; i < maxScoreIdx.size(); ++i){
            jetFlavMaxScore[maxScoreIdx[i]].push_back(i);
        }
        // initalize jet pairs 
        int nPairs = 0;
        std::vector<int> pair_1 = {-1, -1};
        std::vector<int> pair_2 = {-1, -1};
        int pairFalv_1  = -1;
        int pairFalv_2  = -1;
        for (const auto& pair : jetFlavMaxScore) {
            if (nPairs == 2) continue;
            if (pair.second.size() == 4){
                if (m_debug) std::cout<<"All jets have the same flav "<< pair.first<<std::endl;
                pairFalv_1 = pair.first;
                pairFalv_2 = pair.first;
                nPairs++;
            }
            else if (pair.second.size() == 3){
                if (m_debug) std::cout<<"3 jets have same flavour "<<std::endl;
                pairFalv_1 = pair.first;
                float max_score = 0.;
                for (const int& j1 : pair.second) {
                    for (const int& j2 : pair.second) {
                        if (j1>j2){
                            if (m_debug){
                                std::cout<<"max_score: "<< max_score<<std::endl;
                                std::cout<<"new_score: "<< jetFlavScores[j1][pairFalv_1] + jetFlavScores[j2][pairFalv_1]<<std::endl;
                                std::cout<<"j1: "<< j1<<" j2: "<<j2<<std::endl;
                            }
                            if (j1>j2 && (jetFlavScores[j1][pairFalv_1] + jetFlavScores[j2][pairFalv_1] > max_score) ){
                                max_score = jetFlavScores[j1][pairFalv_1] + jetFlavScores[j2][pairFalv_1];
                                pair_1[0] = j1;
                                pair_1[1] = j2;
                                if (m_debug) std::cout<<"updated max_score: "<< max_score<<std::endl;
                            }
                        }   
                    }
                }
                nPairs++;
                getMissingPair(pair_1,pair_2);
                pairFalv_2 = getMaxScoreFlav(pair_2,jetFlavScores);
                nPairs++;
                if (m_debug){
                    std::cout<<"Pair 1, pairFalv_1: "<< pairFalv_1 << ", jet: "<<std::endl;
                    for (const int& value : pair_1) {
                                std::cout << value << " ";}

                    std::cout << std::endl;
                    std::cout<<"Pair 2, pairFalv_2: "<< pairFalv_2 << ", jet: "<<std::endl;
                    for (const int& value : pair_2) {
                                std::cout << value << " ";}
                    std::cout << std::endl;
                }
            }
            else if (pair.second.size() == 2){
                if (m_debug) std::cout<<"2 jets have same flavour "<<std::endl;
                nPairs++;
                if (pairFalv_1 == -1){
                    if (m_debug) std::cout<<"First pair identified "<<std::endl;
                    pairFalv_1 = pair.first;
                    pair_1[0] = pair.second[0];
                    pair_1[1] = pair.second[1];
                    if (m_debug) std::cout<<"pairFalv_1: "<<pairFalv_1<<std::endl;
                }
                else if (pairFalv_2 == -1){
                    if (m_debug) std::cout<<"Second pair identified "<<std::endl;
                    pairFalv_2 = pair.first;
                    pair_2[0] = pair.second[0];
                    pair_2[1] = pair.second[1];
                    if (m_debug) std::cout<<"pairFalv_2: "<<pairFalv_2<<std::endl;
                }
                else {
                    std::cout<<"More than 2 pairs! Not possible!"<<std::endl;
                    break;}
            }
        }
        if (nPairs==0) {
            std::cout<<"---------------------- No pairs, toss event ---------------------- "<<std::endl;
            continue;
        }
        if (nPairs==1) {
            if (pairFalv_2 == -1){
                if (m_debug) std::cout<<"Need to retrive one pair"<<std::endl;
                getMissingPair(pair_1,pair_2);
                pairFalv_2 = getMaxScoreFlav(pair_2,jetFlavScores);
                nPairs++;
                if (m_debug){
                    std::cout<<"Pair 1, pairFalv_1: "<< pairFalv_1 << ", jet: "<<std::endl;
                    for (const int& value : pair_1) {
                                std::cout << value << " ";}

                    std::cout << std::endl;
                    std::cout<<"Pair 2, pairFalv_2: "<< pairFalv_2 << ", jet: "<<std::endl;
                    for (const int& value : pair_2) {
                                std::cout << value << " ";}
                    std::cout << std::endl;
                }
            }
        }
        //Find which pair is Z and H 
        if (pairFalv_1 == pairFalv_2){
            if (m_debug) std::cout<<"All same flav!"<<std::endl;
            if (m_debug) std::cout<<"flav_H!"<< pairFalv_1<<std::endl;
            if (m_debug) std::cout<<"flav_Z!"<< pairFalv_2<<std::endl;
            flav_H = pairFalv_2;
            flav_Z = pairFalv_1;
            double tmp_pair1_m = -1;
            double tmp_pair2_m = -1;
            double tmp_chi_comb = 10e10;
            for (const int n : {0, 1, 2}){
                for (const int m : {1, 2, 3}){
                    if (m>n){
                        pair_1[0]=n;
                        pair_1[1]=m;
                        getMissingPair(pair_1,pair_2);
                        tmp_pair1_m = (LVjets[pair_1[0]] +  LVjets[pair_1[1]]).M(); 
                        tmp_pair2_m = (LVjets[pair_2[0]] +  LVjets[pair_2[1]]).M(); 
                        if (m_debug) std::cout<<"tmp_chi_comb! "<< pow((tmp_pair1_m-Z_mass), 2) +  pow((tmp_pair2_m-H_mass), 2)<<std::endl;
                        if (m_debug) std::cout<<"tmp_pair1_m! "<< tmp_pair1_m<<std::endl;
                        if (m_debug) std::cout<<"tmp_pair2_m! "<< tmp_pair2_m<<std::endl;
                        if (tmp_chi_comb > (pow((tmp_pair1_m-Z_mass), 2) +  pow((tmp_pair2_m-H_mass), 2))){
                            tmp_chi_comb = pow((tmp_pair1_m-Z_mass), 2) +  pow((tmp_pair2_m-H_mass), 2);
                            mjj_H = tmp_pair2_m;
                            mjj_Z = tmp_pair1_m;
                            flavSc_H = jetFlavScores[pair_2[0]][flav_H] + jetFlavScores[pair_2[1]][flav_H];
                            flavSc_Z = jetFlavScores[pair_1[0]][flav_Z] + jetFlavScores[pair_1[1]][flav_Z];
                        }
                    }
                }
            }
        }
        else if (nPairs==2 && (pairFalv_1 != pairFalv_2)) {
            if (m_debug) std::cout<<"2x paris different flavour!"<<std::endl;
            double pair1_m = (LVjets[pair_1[0]] +  LVjets[pair_1[1]]).M(); 
            double pair2_m = (LVjets[pair_2[0]] +  LVjets[pair_2[1]]).M(); 
            if (m_debug) std::cout<<"Mass pair 1: "<<pair1_m<<"chi_Z: "<< pow((pair1_m-Z_mass), 2)<<std::endl;
            if (m_debug) std::cout<<"Mass pair 2: "<<pair2_m<<"chi_Z: "<< pow((pair2_m-Z_mass), 2)<<std::endl;
            if (pow((pair1_m-Z_mass), 2) <  pow((pair2_m-Z_mass), 2)){
                mjj_H = pair2_m;
                flav_H = pairFalv_2;
                mjj_Z = pair1_m;
                flav_Z = pairFalv_1;
                flavSc_H = jetFlavScores[pair_2[0]][flav_H] + jetFlavScores[pair_2[1]][flav_H];
                flavSc_Z = jetFlavScores[pair_1[0]][flav_Z] + jetFlavScores[pair_1[1]][flav_Z];
            }
            else{
                mjj_H = pair1_m;
                mjj_Z = pair2_m;
                flav_H = pairFalv_1;
                flav_Z = pairFalv_2;
                flavSc_Z = jetFlavScores[pair_2[0]][flav_Z] + jetFlavScores[pair_2[1]][flav_Z];
                flavSc_H = jetFlavScores[pair_1[0]][flav_H] + jetFlavScores[pair_1[1]][flav_H];

            }
        }
        else if (pairFalv_1 == -1 || pairFalv_2 == -1) {
            std::cout<<"Something is really worong, pairFalv_1 == -1 || pairFalv_2 == -1!"<<std::endl;
            break;
        }
        else {
            std::cout<<"Something is really worong!"<<std::endl;
            break;
        }   
        if (m_debug){
            std::cout<<"Higgs m: "<< mjj_H<< " flav: "<< flav_H<<" flavSc_H: " <<flavSc_H<<std::endl;
            std::cout<<"Z m: "<< mjj_Z<< " flav: "<< flav_Z<<" flavSc_Z: " <<flavSc_Z<<std::endl;
            // std::cout<<"M_j0_j1_corr: "<< M_j0_j1_corr()<<std::endl;
            // std::cout<<"M_j0_j2_corr: "<< M_j0_j2_corr()<<std::endl;
            // std::cout<<"M_j1_j2_corr: "<< M_j1_j2_corr()<<std::endl;
            // std::cout<<"M_j1_j3_corr: "<< M_j1_j3_corr()<<std::endl;
            // std::cout<<"M_j2_j3_corr: "<< M_j2_j3_corr()<<std::endl;
        }
        NafterPairing++;
         //A bit of selection 
        float WW_cuts = sqrt(pow( mjj_Z-W_mass ,2) + pow( mjj_H-W_mass ,2));
        float ZZ_cuts = sqrt(pow( mjj_Z-Z_mass ,2) + pow( mjj_H-Z_mass ,2));
        if (m_debug) {
            std::cout << "WW_cuts: " << WW_cuts <<std::endl;
            std::cout << "ZZ_cuts: " << ZZ_cuts <<std::endl;}
        if(WW_cuts<=10) continue;
        if(ZZ_cuts<=10) continue;
        NjjMassCut++;
        if (50. >= mjj_Z)continue; 
        if (mjj_Z >= H_mass) continue; 
        //Fix
        if (mjj_H<=Z_mass) continue;
        // For now remove the Htautau as a cat, only bkg....
        // if (H_flav == 6) continue;
        //After parining and cuts 
        //The flag e correction falg...
        if (m_debug) std::cout << "Passed pairing and kine selection "  <<std::endl;
        NafterSel++;
        mH_jj = mjj_H;
        mZ_jj = mjj_Z;
        Hscore = flavSc_H;
        Zscore = flavSc_Z;
        Hflav = flav_H;
        Zflav = flav_Z;
        ChiH = pow((mjj_H-H_mass), 2);
        ChiZ = pow((mjj_Z-Z_mass), 2);
        flag = flag_corr();
        weight = normWeight;
        my_tree->Fill();
        if (flag_corr()>=10.) continue;
        NafterFlagSel++;
        // if (m_debug) std::cout << "Passed flag_corr cut "  <<std::endl; || flav_H == 6 
        NPassed ++;
        //if (flav_H == 6 || flav_H == 3 || flav_H == 4  ) continue;
        // if (flav_Z == 5 || flav_Z == 6  || flav_H == 6 || flav_H == 3 || flav_H == 4  ) continue;
        Incl_obsHist->Fill(mjj_H,mjj_Z);
        if (flav_H == 0){
            if (m_debug) std::cout << "Hbb"  <<std::endl;
            if (flav_Z == 0){
                if (flavSc_H > 1.8)
                {
                    Hi_bbZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_bbZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_bbZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 1){
                if (flavSc_H > 1.8){
                    Hi_ccZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ccZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_ccZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 2){
                if (flavSc_H > 1.8){
                    Hi_ssZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ssZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_ssZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                }

            }
            else if (flav_Z == 3 || flav_Z == 4){
                if (flavSc_H > 1.8){
                    Hi_qqZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);

                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_qqZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                    
                } 
                else if (flavSc_H <= 1.1){
                    Low_qqZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 5){
                if (flavSc_H > 1.8){
                    Hi_ggZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);

                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ggZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                    
                } 
                else if (flavSc_H <= 1.1){
                    Low_ggZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 6){
                if (flavSc_H > 1.8){
                    Hi_ttZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);

                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ttZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                    
                } 
                else if (flavSc_H <= 1.1){
                    Low_ttZ_Hbb_obsHist->Fill(mjj_H,mjj_Z);
                }
            }

        }
        else if ( flav_H == 1)
        {
            //Fill in Hcc cats
            if (m_debug) std::cout << "Hcc"  <<std::endl;
            if (flav_Z == 0){
                if (flavSc_H > 1.8){
                    Hi_bbZ_Hcc_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_bbZ_Hcc_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (flavSc_H <= 1.1){
                    Low_bbZ_Hcc_obsHist->Fill(mjj_H,mjj_Z); 
                }
            }
            else if (flav_Z == 1){
                if (flavSc_H > 1.8){
                    Hi_ccZ_Hcc_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ccZ_Hcc_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (flavSc_H <= 1.1){
                    Low_ccZ_Hcc_obsHist->Fill(mjj_H,mjj_Z); 
                }

            }
            else if (flav_Z == 2){
                if (flavSc_H > 1.8){
                    Hi_ssZ_Hcc_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ssZ_Hcc_obsHist->Fill(mjj_H,mjj_Z);   
                } 
                else if (flavSc_H <= 1.1){
                    Low_ssZ_Hcc_obsHist->Fill(mjj_H,mjj_Z);   
                }
            }
            else if (flav_Z == 3 || flav_Z == 4){
                if (flavSc_H > 1.8){
                    Hi_qqZ_Hcc_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_qqZ_Hcc_obsHist->Fill(mjj_H,mjj_Z);          
                } 
                else if (flavSc_H <= 1.1){
                    Low_qqZ_Hcc_obsHist->Fill(mjj_H,mjj_Z);           
                }
            }
            else if (flav_Z == 5){
                if (flavSc_H > 1.8){
                    Hi_qqZ_Hcc_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_qqZ_Hcc_obsHist->Fill(mjj_H,mjj_Z);   
                } 
                else if (flavSc_H <= 1.1){
                    Low_qqZ_Hcc_obsHist->Fill(mjj_H,mjj_Z);   
                }
            }
            else if (flav_Z == 6){
                if (flavSc_H > 1.8){
                    Hi_ttZ_Hcc_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ttZ_Hcc_obsHist->Fill(mjj_H,mjj_Z);   
                } 
                else if (flavSc_H <= 1.1){
                    Low_ttZ_Hcc_obsHist->Fill(mjj_H,mjj_Z);   
                }
            }


        }
        else if (flav_H == 2){
            if (m_debug) std::cout << "Hss"  <<std::endl;
            //Fill in Hss cats
            if (flav_Z == 0){
                if (flavSc_H > 1.4){
                    Hi_bbZ_Hss_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (0.8<flavSc_H && flavSc_H <= 1.4){
                    Mid_bbZ_Hss_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (flavSc_H <= 0.8){
                    Low_bbZ_Hss_obsHist->Fill(mjj_H,mjj_Z); 
                }
            }
            else if (flav_Z == 1){
                if (flavSc_H > 1.4){
                    Hi_ccZ_Hss_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (0.8<flavSc_H && flavSc_H <= 1.4){
                    Mid_ccZ_Hss_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (flavSc_H <= 0.8){
                    Low_ccZ_Hss_obsHist->Fill(mjj_H,mjj_Z); 
                }
            }
            else if (flav_Z == 2){
                if (flavSc_H > 1.4){
                    Hi_ssZ_Hss_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (0.8<flavSc_H && flavSc_H <= 1.4){
                    Mid_ssZ_Hss_obsHist->Fill(mjj_H,mjj_Z);   
                } 
                else if (flavSc_H <= 0.8){
                    Low_ssZ_Hss_obsHist->Fill(mjj_H,mjj_Z);   
                }
            }
            else if (flav_Z == 3 || flav_Z == 4){
                if (flavSc_H > 1.4){
                    Hi_qqZ_Hss_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (0.8<flavSc_H && flavSc_H <= 1.4){
                    Mid_qqZ_Hss_obsHist->Fill(mjj_H,mjj_Z);          
                } 
                else if (flavSc_H <= 0.8){
                    Low_qqZ_Hss_obsHist->Fill(mjj_H,mjj_Z);           
                }
            }
            else if (flav_Z == 5){
                if (flavSc_H > 1.4){
                    Hi_ggZ_Hss_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (0.8<flavSc_H && flavSc_H <= 1.4){
                    Mid_ggZ_Hss_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (flavSc_H <= 0.8){
                    Low_ggZ_Hss_obsHist->Fill(mjj_H,mjj_Z); 
                }
            }
            else if (flav_Z == 6){
                if (flavSc_H > 1.4){
                    Hi_ttZ_Hss_obsHist->Fill(mjj_H,mjj_Z); 
                } 
                else if (0.8<flavSc_H && flavSc_H <= 1.4){
                    Mid_ttZ_Hss_obsHist->Fill(mjj_H,mjj_Z);   
                } 
                else if (flavSc_H <= 0.8){
                    Low_ttZ_Hss_obsHist->Fill(mjj_H,mjj_Z);   
                }
            }
            
        }
        else if ( flav_H == 3 || flav_H == 4){
            if (m_debug) std::cout << "Hqq"  <<std::endl;
            //Fill in Hgg cats
            if (flav_Z == 0){
                if (flavSc_H > 1.8){
                    Hi_bbZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_bbZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_bbZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 1){
                if (flavSc_H > 1.8){
                    Hi_ccZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ccZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);  
                } 
                else if (flavSc_H <= 1.1){
                    Low_ccZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 2){
                if (flavSc_H > 1.8){
                    Hi_ssZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ssZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_ssZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 3 || flav_Z == 4){
                if (flavSc_H > 1.8){
                    Hi_qqZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_qqZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_qqZ_Hqq_obsHist->Fill(mjj_H,mjj_Z); 
                }
            }
            else if (flav_Z == 5){
                if (flavSc_H > 1.8){
                    Hi_ggZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ggZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_ggZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 6){
                if (flavSc_H > 1.8){
                    Hi_ttZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ttZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_ttZ_Hqq_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
        }

        else if ( flav_H == 5){
            if (m_debug) std::cout << "Hgg"  <<std::endl;
            //Fill in Hgg cats
            if (flav_Z == 0){
                if (flavSc_H > 1.8){
                    Hi_bbZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_bbZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_bbZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 1){
                if (flavSc_H > 1.8){
                    Hi_ccZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ccZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);  
                } 
                else if (flavSc_H <= 1.1){
                    Low_ccZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 2){
                if (flavSc_H > 1.8){
                    Hi_ssZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ssZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_ssZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 3 || flav_Z == 4){
                if (flavSc_H > 1.8){
                    Hi_qqZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_qqZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_qqZ_Hgg_obsHist->Fill(mjj_H,mjj_Z); 
                }
            }
            else if (flav_Z == 5){
                if (flavSc_H > 1.8){
                    Hi_ggZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ggZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_ggZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 6){
                if (flavSc_H > 1.8){
                    Hi_ttZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ttZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_ttZ_Hgg_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
        }
        else if ( flav_H == 6){
            if (m_debug) std::cout << "Htautau"  <<std::endl;
            //Fill in Hgg cats
            if (flav_Z == 0){
                if (flavSc_H > 1.8){
                    Hi_bbZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_bbZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_bbZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 1){
                if (flavSc_H > 1.8){
                    Hi_ccZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ccZ_Htt_obsHist->Fill(mjj_H,mjj_Z);  
                } 
                else if (flavSc_H <= 1.1){
                    Low_ccZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 2){
                if (flavSc_H > 1.8){
                    Hi_ssZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ssZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_ssZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 3 || flav_Z == 4){
                if (flavSc_H > 1.8){
                    Hi_qqZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_qqZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_qqZ_Htt_obsHist->Fill(mjj_H,mjj_Z); 
                }
            }
            else if (flav_Z == 5){
                if (flavSc_H > 1.8){
                    Hi_ggZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ggZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_ggZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
            else if (flav_Z == 6){
                if (flavSc_H > 1.8){
                    Hi_ttZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if  (1.1<flavSc_H && flavSc_H <= 1.8){
                    Mid_ttZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                } 
                else if (flavSc_H <= 1.1){
                    Low_ttZ_Htt_obsHist->Fill(mjj_H,mjj_Z);
                }
            }
        }

        else{
            std::cout << "Event should be but is not included in the fit!"  <<std::endl;
            break;
        }
        Nfit++;
        
        if (m_debug) std::cout<<"---------------------- END OF EVENT PROC ---------------------- "<<std::endl;
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
    cutFlowHist->SetBinContent(10,NafterFlagSel);
    cutFlowHist->SetBinContent(11,Nfit);

    std::cout<<"NEventsInt: "<<NEventsInt<<", pre sel: "<< NdCutd34 << " all sel: "<<NafterFlagSel <<" Nfit: "<<Nfit<<std::endl;


    for(int i = 0; i < 3; i++){
        scoreMapFitCatHist->SetBinContent(1+i*5, BlikeEvents_cat[i]*100./NafterSel);
        scoreMapFitCatHist->SetBinContent(2+i*5, ClikeEvents_cat[i]*100./NafterSel);
        scoreMapFitCatHist->SetBinContent(3+i*5, SlikeEvents_cat[i]*100./NafterSel);
        scoreMapFitCatHist->SetBinContent(4+i*5, GlikeEvents_cat[i]*100./NafterSel);
        scoreMapFitCatHist->SetBinContent(5+i*5, QlikeEvents_cat[i]*100./NafterSel);
        // scoreMapFitCatHist->SetBinContent(6+i*6, TAUlikeEvents_cat[i]*100./NafterSel);

        CountsFitCatHist->SetBinContent(1+i*4, BlikeEvents_cat[i]);
        CountsFitCatHist->SetBinContent(2+i*4, ClikeEvents_cat[i]);
        CountsFitCatHist->SetBinContent(3+i*4, SlikeEvents_cat[i]);
        CountsFitCatHist->SetBinContent(4+i*4, GlikeEvents_cat[i]);
//         CountsFitCatHist->SetBinContent(5+i*5, QlikeEvents_cat[i]);
        // CountsFitCatHist->SetBinContent(6+i*6, TAUlikeEvents_cat[i]);
    }
    std::cout<<"Done with event!"<<std::endl;
    //Make histograms for the cats 
}
