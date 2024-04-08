#include <stdio.h>
#include <stddef.h>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
#include <limits>
#include <math.h> 
#include <algorithm>
// ROOT include
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLeaf.h>
#include <TObject.h>
#include <TObjArray.h>
#include <TChain.h>
#include <TMath.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TLine.h>
#include <TGraph.h>
#include <TColor.h>
#include <TLegend.h>
#include <stdio.h>
#include "commonCode.h"
#include "AtlasLabels.C"
#include "AtlasStyle.C"
#include "TGraphAsymmErrors.h"
#include "RooWorkspace.h"
#include "fstream"
#include <nlohmann/json.hpp>


using namespace std;

// Global vars
map<TString,TString> opts;
map<TString,TString> metaData;

enum MatrixType { covMat, corrMat };    

// // Functions
bool cmdline(int argc, char** argv, map<TString,TString>& opts);
TString getBaseName(TString pathName);

void setPrettyStuff();
void plotMatrix(TH2D* inHist, std::map<TString, TString> metadataMap, MatrixType matrixType, TString fileName);
vector<TString> getPOIList(std::map<TString, TString> metadataMap);
vector<TString> getClearPOIList(std::map<TString, TString> metadataMap);

RooWorkspace* m_ws;
void fillWS(TString fileName);
int main(int argc, char** argv)
{
    setPrettyStuff();

    if (!cmdline(argc,argv,opts)) return 0;
    m_ws = NULL;

    // Split the results
    vector<TString> fileNameList = tokenizeStr(opts["fileName"],",");
    std::vector<std::map<TString, TString>> metadataVect = getMetadata(fileNameList);

    if(fileNameList.size() > 1)
    {
        cout<<"More than 1 file provided to plotCorrHist... there is no support for this"<<endl;
        exit(1);
    }


    for(size_t i = 0; i < fileNameList.size(); i++)
    {
        cout<<"---------------------------"<<endl;
        cout<<fileNameList[i]<<endl;
        for(const auto& cMap: metadataVect[i]) cout<<cMap.first<<" "<<cMap.second<<endl;
        if(i==0) fillWS(fileNameList[i]);
    }

    // Extract the correlation and covariance matrix
    TFile* inFile = TFile::Open(fileNameList[0]);
    TH2D* covMatrix     = (TH2D*) inFile->Get("covarianceHist");
    TH2D* corrMatric    = (TH2D*) inFile->Get("correlation_matrix");

    plotMatrix(covMatrix, metadataVect[0], covMat, fileNameList[0]);
    plotMatrix(corrMatric, metadataVect[0], corrMat, fileNameList[0]);
}

void plotMatrix(TH2D* inHist, std::map<TString, TString> metadataMap, MatrixType matrixType, TString fileName)
{
    // Plot Config
    TString jsonPlotConfig = "${wscanner_data}/PlotConfig/" + metadataMap.at("plotConfig");
    jsonPlotConfig = expandEnvironmentVariables(jsonPlotConfig.Data());
    std::ifstream f(jsonPlotConfig.Data());
    nlohmann::json plotConfig = nlohmann::json::parse(f);

    // First get a subset of the inHist with just the POI
    vector<TString> poiList = getPOIList(metadataMap);
    vector<TString> poiClearList = getClearPOIList(metadataMap);
    vector<int> xIndex, yIndex;

    // Get the index for the size
    for(const auto& poiName: poiList)
    {
        bool isFound = false;
        for(int i = 0; i < inHist->GetXaxis()->GetNbins();i++)
        {
            TString binLabel = inHist->GetXaxis()->GetBinLabel(i+1);    
            if(binLabel.EqualTo(poiName)) {xIndex.push_back(i+1); isFound = true;}

        }
        for(int i = 0; i < inHist->GetYaxis()->GetNbins();i++)
        {
            TString binLabel = inHist->GetYaxis()->GetBinLabel(i+1);    
            if(binLabel.EqualTo(poiName)) {yIndex.push_back(i+1); isFound = true;}
            // cout << binLabel << endl;
        }
        if(!isFound) {cout<<"Can't find: "<<poiName<<endl;}
    }


    if(xIndex.size() != poiList.size() || yIndex.size() != poiList.size())
    {
        cout<<"POI index size doesn't match the label size"<<endl;
        exit(1);
    }

    // Set the bin content
    TH2D* cHist = new TH2D("cHist", "cHist", xIndex.size(), 0, xIndex.size(), yIndex.size(), 0, yIndex.size());
    for(size_t i = 0 ; i < poiList.size(); i++)
    {
        for(size_t j = 0 ; j < poiList.size()-i; j++)
        {
            if(poiClearList.size() > 0 && (i != yIndex.size() - j - 1))
            {
                if(find(poiClearList.begin(), poiClearList.end(), poiList[yIndex.size() - j - 1]) != poiClearList.end())
                {
                    continue;
                }
            }

            cHist->SetBinContent(i+1, j+1, inHist->GetBinContent(xIndex[i], yIndex[yIndex.size() - j - 1]));
        }        
    }
    // Set the bin labels
    for(size_t i = 0 ; i < poiList.size(); i++)
    {
        TString xLabel = plotConfig["poiNameMap"][inHist->GetXaxis()->GetBinLabel(xIndex[i])];
        cHist->GetXaxis()->SetBinLabel(i+1, xLabel);
        TString yLabel = plotConfig["poiNameMap"][inHist->GetYaxis()->GetBinLabel(yIndex[yIndex.size() - i - 1])];
        cHist->GetYaxis()->SetBinLabel(i+1, yLabel);     
    }

    // Define the color
    const int Number = 5;
    Double_t Green[Number]  = {0.31, 0.67, 1.00, 0.69, 0.29};
    Double_t Blue[Number]   = {0.60, 0.85, 1.00, 0.72, 0.14};
    Double_t Red[Number]    = {0.13, 0.45, 1.00, 0.96, 0.74};
    Double_t Length[Number] = {0.00, 0.10, 0.50, 0.90, 1.00};//16

    if(matrixType == covMat)
    {
        double max = cHist->GetMaximum();
        double min = cHist->GetMinimum();
        Length[1] =  fabs(min)/(max-min)*0.5;
        Length[2] = fabs(min)/(max-min);
        Length[3] = fabs(min)/(max-min)*1.50;
    }
    else
    {
        cHist->SetMaximum(1);
        cHist->SetMinimum(-1);
    }

    Int_t nb = 1000;//20;
    TColor::CreateGradientColorTable(Number, Length, Red, Green, Blue, nb);

    // Format the hist
    cHist->LabelsOption("v");
    if(poiList.size() > 8) cHist->SetMarkerSize(0.75);
    else cHist->SetMarkerSize(1.5);
    cHist->GetXaxis()->SetLabelSize(0.04);
    cHist->GetYaxis()->SetLabelSize(0.04);
    gStyle->SetPaintTextFormat("2.2f");  
    cHist->SetContour(1000);


    TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 600, 600);
    c1->SetRightMargin(0.15);
    if(poiList.size() > 8)
    {
        c1->SetLeftMargin(0.225);
        c1->SetBottomMargin(0.225);
    }
    else
    {

    }

    cHist->Draw("colz");
    cHist->Draw("sameText");

    double leftDist = 0.44;
    double offset = -0.02;
    TString dataType = metadataMap["dataType"];
    dataType.ToLower();

    std::string expName = plotConfig["experimentString"];
    std::string plotType = plotConfig["plotTypeString"];
    if(dataType.Contains("asimov"))
    {
        ExpLabel(expName.c_str(), leftDist, 0.875, "Simulation", 1);
        offset = 0.02;
        TLatex p; 
        p.SetNDC();
        p.SetTextFont(42);
        p.SetTextColor(1);
        p.DrawLatex(leftDist,0.835, plotType.c_str());
    }
    else ExpLabel(expName.c_str(), leftDist, 0.875, plotType.c_str(), 1);

    TString channelStr = plotConfig["channelString"];
    TLatex* tMain = new TLatex (leftDist, 0.815 - offset,channelStr);
    tMain->SetNDC();
    tMain->SetTextSize(0.0425);
    tMain->SetTextFont(42);
    tMain->Draw();

    TString lumiString = plotConfig["lumiString"];
    TLatex* lumInfo = new TLatex (leftDist, 0.78 - offset, lumiString);
    lumInfo->SetNDC();
    lumInfo->SetTextSize(0.0325);
    lumInfo->SetTextFont(42);
    lumInfo->Draw();

    // Draw the hatches
    for(const auto& poiSkip:poiClearList)
    {
        vector<Float_t> binCenter  ;
        vector<Float_t> binContent ;
        vector<Float_t> upErr      ;
        vector<Float_t> downErr    ;
        vector<Float_t> xupErr     ;
        vector<Float_t> xdownErr   ;

        int yIndex = -1;
        for(size_t i = 0; i < poiList.size(); i++)
        {
            if(poiList[i].EqualTo(poiSkip)) yIndex = poiList.size() - i -1;
        }

        for(size_t i = 0; i < poiList.size(); i++)
        {
            if(i >= poiList.size()  - 1 - yIndex) continue;
            binCenter.push_back(i+0.5);
            binContent.push_back(yIndex + 0.5);
            upErr.push_back(0.5);
            downErr.push_back(0.5);
            xupErr.push_back(0.5);
            xdownErr.push_back(0.5);
        }     

        TGraphAsymmErrors* hatch = new TGraphAsymmErrors(binCenter.size(),&binCenter[0],&binContent[0],&xupErr[0],&xdownErr[0],&upErr[0],&downErr[0]);
        hatch->SetMarkerStyle(0);
        hatch->SetLineColor(0);
        hatch->SetFillColor(1);
        hatch->SetFillStyle(3454);
        hatch->Draw("2same");   
    }


    TString saveName = opts["plotDir"] + "/" + getBaseName(fileName) + "/";
    system("mkdir -vp " + saveName);

    if(!dataType.Contains("asimov")) saveName += "Obs_";
    else                             saveName += "Exp_";

    if(metadataMap["scanType"].Contains("statonly")) saveName += "noSys_";
    else                                            saveName += "Sys_";

    if(matrixType == corrMat) saveName += "corrMatrix." + plotConfig["plotExtension"]; 
    if(matrixType == covMat) saveName += "covMatrix." + plotConfig["plotExtension"];
    c1->SaveAs(saveName);

    delete cHist;
    delete c1;
}

vector<TString> getPOIList(std::map<TString, TString> /*metadataMap*/)
{
    vector<TString> POIList;

    // Some default orders
    vector<TString> SXTS_incl   = {"mu", "r_ZZ_0jet", "r_ZZ_1jet", "r_ZZ_2jet", "r_ttV"};
    vector<TString> SXTS_S0     = {"mu_ggF", "mu_VBF", "mu_VH", "mu_ttH", "r_ZZ_0jet", "r_ZZ_1jet", "r_ZZ_2jet", "r_ttV"};
    vector<TString> SXTS_S0qqH  = {"mu_gg2H", "mu_qqH2qq", "mu_qq2Hll", "mu_ttH", "r_ZZ_0jet", "r_ZZ_1jet", "r_ZZ_2jet", "r_ttV"};
    vector<TString> SXTS_RS1p1  = {"mu_ggF_0J_PTH_0_10", "mu_ggF_0J_PTH_GT10", "mu_ggF_1J_PTH_0_60", "mu_ggF_1J_PTH_60_120", "mu_ggF_1J_PTH_120_200", "mu_ggF_GE2J", "mu_ggF_PTH_GT200", "mu_VBF_qq2qq_PTH_LE200", "mu_VBF_qq2qq_PTH_GT200", "mu_VH_Had", "mu_VH_Lep", "mu_ttH", "r_ZZ_0jet", "r_ZZ_1jet", "r_ZZ_2jet", "r_ttV"};
    vector<TString> SXTS_RS1p1qqH  = {"mu_gg2H_0J_PTH_0_10", "mu_gg2H_0J_PTH_GT10", "mu_gg2H_1J_PTH_0_60", "mu_gg2H_1J_PTH_60_120", "mu_gg2H_1J_PTH_120_200", "mu_gg2H_GE2J", "mu_gg2H_PTH_GT200", "mu_qqH2qq_VHHad", "mu_qqH2qq_rest", "mu_qq2Hqq_mJJ_GT350_PTH_GT200", "mu_qq2Hll", "mu_ttH", "r_ZZ_0jet", "r_ZZ_1jet", "r_ZZ_2jet", "r_ttV"};
    vector<TString> diffList    = {"sigma_bin0_llee", "sigma_bin1_llee", "sigma_bin2_llee", "sigma_bin3_llee","sigma_bin4_llee", "sigma_bin0_llmumu", "sigma_bin1_llmumu", "sigma_bin2_llmumu", "sigma_bin3_llmumu","sigma_bin4_llmumu","sigma_bin0_incl", "sigma_bin1_incl", "sigma_bin2_incl", "sigma_bin3_incl","sigma_bin4_incl", "sigma_bin5_incl", "sigma_bin6_incl", "sigma_bin7_incl", "sigma_bin8_incl", "sigma_bin9_incl", "sigma_bin10_incl", "sigma_bin11_incl", "sigma_bin12_incl", "sigma_bin13_incl", "sigma_bin23_incl", "sigma_bin123_incl", "sigma_bin0123_incl", "sigma", "sigma_2l2l", "sigma_4l", "sigma_4mu", "sigma_4e", "sigma_2mu2e", "sigma_2e2mu", "sigma_sum", "MuqqZZ0", "MuqqZZ1", "MuqqZZ2", "MuqqZZ3", "MuqqZZ4", "MuqqZZ5"};
    vector<TString> PO_contact  = {"eL","eR"}; 
    vector<TString> massChanDecor  = {"mH_4mu","mH_4e","mH_2mu2e","mH_2e2mu"}; 
    vector<TString> lightYukawa = {"kappaB","kappaC"}; 
    vector<TString> ProdBR      = { "r_ggF_yy","r_VBF_yy","r_VH_yy","r_ttH_yy","r_ggF_ZZ","r_VBF_ZZ","r_VH_ZZ","r_ggF_WW","r_VBF_WW","r_ggF_tautau","r_VBF_tautau","r_ttH_tautau","r_VBF_bb","r_VH_bb","r_ttH_bb","r_ttH_VV"}; 
    vector<TString> TotalXS     = { "r_ggF","r_VBF","r_WH", "r_ZH", "r_ttH"}; 
    vector<TString> RatioXS     = { "r_ggF","r_VBF","r_WH", "r_ZH", "r_ttH"}; 

    if(opts["impactPOIs"].Contains("mu_gg2H_0J_PTH_0_10"))
    {
        // Yay, we are looking at Stage 1p1
        POIList = SXTS_RS1p1qqH;
    }
    else if(opts["impactPOIs"].Contains("mu_gg2H"))
    {
        // Yay, we are looking at Stage 0
        POIList = SXTS_S0qqH;        
    }
    else if(opts["impactPOIs"].Contains("mu_ggF_0J_PTH_0_10"))
    {
        // Yay, we are looking at Stage 1p1
        POIList = SXTS_RS1p1;
    }
    else if(opts["impactPOIs"].Contains("mu_ggF"))
    {
        // Yay, we are looking at Stage 0
        POIList = SXTS_S0;        
    }
    else if(opts["impactPOIs"].Contains("mu") && opts["impactPOIs"].Contains("r_ttV"))
    {
        // Yay, we are looking at incl mu
        POIList = SXTS_incl;        
    }
    else if(opts["impactPOIs"].Contains("sigma_bin"))
    {
        // Yay, we are looking at Diff
        POIList = diffList;        
    }
    else if(opts["impactPOIs"].Contains("sigma") && !opts["impactPOIs"].Contains("DCB"))
    {
        // Yay, we are looking at Diff
        POIList = diffList;        
    }
    else if(opts["impactPOIs"].Contains("eL"))
    {
        POIList = PO_contact;        
    }
    else if(opts["impactPOIs"].Contains("kappaB"))
    {
        POIList = lightYukawa;        
    }
    else if(opts["impactPOIs"].Contains("mH_4mu"))
    {
        POIList = massChanDecor;        
    }
    else if(opts["impactPOIs"].Contains("r_ggF_yy"))
    {
        POIList = ProdBR;        
    }
    else if(opts["impactPOIs"].Contains("r_ggF"))
    {
        POIList = TotalXS;        
    }

    // Now clean up
    if(POIList.size() > 0)
    {
        // clearip the list
        auto tempList = POIList;
        POIList.clear();
        for(const auto& poiName: tempList)
        {
            if(poiName.EqualTo("sigma"))
            {
                if(opts["impactPOIs"].EqualTo(poiName)) {POIList.push_back(poiName);}
                continue;
            }

            if(opts["impactPOIs"].Contains(poiName) && m_ws->var(poiName)) POIList.push_back(poiName);
            if(m_ws && m_ws->var(poiName) && poiName.Contains("MuqqZZ")) POIList.push_back(poiName);
        }
    }
    else
    {
        // Try to decripher the list from here
        POIList = tokenizeStr(opts["impactPOIs"],",");
    }

    return POIList;
}

vector<TString> getClearPOIList(std::map<TString, TString> /*metadataMap*/)
{
    vector<TString> POIList;
    // TString dataType = metadataMap["dataType"];
    // dataType.ToLower();
    // if(!dataType.Contains("asimov")) POIList = {"mu_ttH"};
    return POIList;
}

bool cmdline(int argc, char** argv, map<TString,TString>& opts)
{
    opts.clear();
   
    // defaults
    opts["fileName"]    = "asimovScan.root";
    opts["baseName"]    = "Empty";
    opts["plotDir"]     = "Plots";
    opts["impactPOIs"]  = "";

    for (int i=1;i<argc;i++) {
       
        string opt=argv[i];
       
        if (opt=="--help") {
            cout<<"--fileName       : path to ttree containing the information"<<endl;
            return false;
        }
       
        if(0!=opt.find("--")) {
            cout<<"ERROR: options start with '--'!"<<endl;
            cout<<"ERROR: options is: "<<opt<<endl;
            return false;
        }
        opt.erase(0,2);
        if(opts.find(opt)==opts.end()) {
            cout<<"ERROR: invalid option '"<<opt<<"'!"<<endl;
            return false;
        }
        string nxtopt=argv[i+1];
        if(0==nxtopt.find("--")||i+1>=argc) {
            cout<<"ERROR: option '"<<opt<<"' requires value!"<<endl;
            return false;
        }
       
        opts[opt]=nxtopt;
        i++;
    }
    //if(opts["fileName"].Contains("ttH")) opts["doUpperLim"]  = "true";
    return true;
}

TString getBaseName(TString pathName)
{
    TString baseName = "";
    if(opts["baseName"].EqualTo("Empty"))
    { 
        std::vector<TString> folderName = tokenizeStr(pathName, "/");
        return folderName[1];
    }
    else
    {
        return opts["baseName"];
    }
    // cout << "size"<< folderName.size() << endl;
    // for(const auto& part: folderName) cout << "Prinint out paths "<< part << endl;

    
}


void setPrettyStuff()
{
    SetAtlasStyle();
    Int_t ci = 1756; // color index
    vector<TColor*> TColors;
    new TColor(ci, 0,    0.4470,    0.7410); //56
    ci++;
    new TColor(ci, 0.8500,    0.3250,    0.0980); //57
    ci++;          
    new TColor(ci, 0.4940,    0.1840,    0.5560); //58
    ci++;
    new TColor(ci, 0.9290,    0.6940,    0.1250); //59
    ci++; 
    new TColor(ci, 0.4660,    0.6740,    0.1880); //60
    ci++;
    new TColor(ci, 0.3010,    0.7450,    0.9330); //61
    ci++;
    //new TColor(ci, 0.6350,    0.0780,    0.1840); //62
    //ci++;
    //new TColor(ci, 142.0/255 , 0.0/255 , 62.0/255);
    //ci++;
    //new TColor(ci, 96.0/255 , 78.0/255 , 0.0/255);
    //ci++;
    //new TColor(ci, 92.0/255 , 174.0/255 , 0.0/255);
    //ci++;
    new TColor(ci, 1.0/255 , 237.0/255 , 171.0/255);
    ci++;
    new TColor(ci, 50.0/255 , 134.0/255 , 255.0/255);
    ci++;
    new TColor(ci, 112.0/255 , 0.0/255 , 88.0/255);
    ci++;
    new TColor(ci, 28.0/255 , 0.0/255 , 19.0/255);
    ci++;
    new TColor(ci, 255.0/255 , 102.0/255 , 165.0/255);
    ci++;
}


void fillWS(TString fileName)
{
    if(doesExist(fileName.Data()))
    {
        auto metadata = getMetadata(vector<TString>{fileName}).at(0);
        
        if(doesExist(metadata["fileName"].Data()))
        {
            TFile* wsFile = TFile::Open(metadata["fileName"]);
            m_ws = (RooWorkspace*) wsFile->Get(metadata["workspaceName"]);

            cout<<"Found the ws"<<endl;
            return;
        }
    }
    
}




