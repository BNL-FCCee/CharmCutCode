#include <stdio.h>
#include <stddef.h>
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <math.h> 
#include <set>
// ROOT include
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TLine.h>
#include <TGraph.h>
#include <TColor.h>
#include <TLegend.h>
#include <TGaxis.h>
#include "commonCode.h"
#include "AtlasLabels.C"
#include "AtlasStyle.C"
#include "TGraphAsymmErrors.h"
#include "fstream"
#include <nlohmann/json.hpp>

using namespace std;

struct NPInformation 
{
  TString NPName;
  double baseLine;
  double NPFitVal;
  double NPErrLo;
  double NPErrHi;
  double Plus_1Sigma_PostFit;
  double Minus_1Sigma_PostFit;
  double Plus_1Sigma_PreFit;
  double Minus_1Sigma_PreFit;
};


bool cmdline(int argc, char** argv, map<TString,TString>& opts);
void setPrettyStuff();
void getVariableTree(TTree *tree);
void setupReadTree(TTree *tree);
std::set<TString> getNPList(TTree *tree);
std::vector<NPInformation> readRootFile(TString impactedPOI);
void  plotRankedNP(vector<NPInformation> &paramsToRank, TString poiName);
TString getPrettyName(TString name);
vector<NPInformation> rankNP(vector<NPInformation> &paramsToRank);
TString getBaseName(TString pathName);
void fillMetaMap();
bool isMatched(std::string input, std::string regex);

map<TString,TString> opts;
std::map<TString, TString> metaData;
std::map<TString, double>       doubleVarMap;
std::map<TString, TString*>     TStringVarMap;
int numToRank = 15;


int main(int argc, char** argv)
{
    setPrettyStuff();

    if (!cmdline(argc,argv,opts)) return 0;

    fillMetaMap();
    
    std::vector<TString> checkImpactOn = tokenizeStr(opts["impactPOIs"],",");

    for(size_t i = 0; i < checkImpactOn.size(); i++)
    {
        vector<NPInformation> paramsToRank;
        paramsToRank = readRootFile(checkImpactOn.at(i));

        plotRankedNP(paramsToRank, checkImpactOn.at(i));

        for(auto& str:TStringVarMap) delete str.second;
    }

    return 0;
}

void fillMetaMap()
{
    TFile *fileName = TFile::Open(opts["fileName"], "r");
    TTree *tree = (TTree*)fileName->Get("metaData");

    TObjArray* listLf = tree->GetListOfLeaves();
    std::map<std::string, TString*> m_stringMetaDataVar;

    for (int i = 0; i < listLf->GetEntries(); i++)
    {
        TLeaf* lf = (TLeaf*) listLf->At(i);

        TString typeName   = lf->GetTypeName();
        TString titleName  = lf->GetTitle();
        if(typeName.Contains("TString"))
        {
            m_stringMetaDataVar[lf->GetName()] = 0;
        }
    }   

    for (auto it = m_stringMetaDataVar.begin(); it != m_stringMetaDataVar.end(); ++it)
    {
        TString varName = it->first;
        tree->SetBranchAddress(varName, &it->second);
    }

    tree->GetEntry(0);

    // Copy over the map
    for (auto it = m_stringMetaDataVar.begin(); it != m_stringMetaDataVar.end(); ++it)
    {
        TString varName = it->first;
        metaData[it->first] = *it->second;
    }

}

vector<NPInformation> rankNP(vector<NPInformation> &paramsToRank)
{
    // Sorting
    for(size_t i = 0; i < paramsToRank.size()-1; i++)
    {
        for(size_t j = 0; j < paramsToRank.size()-1; j++)
        {
            bool doSawp = (fabs(paramsToRank[j].Plus_1Sigma_PostFit)+fabs(paramsToRank[j].Minus_1Sigma_PostFit)) > (fabs(paramsToRank[j+1].Plus_1Sigma_PostFit)+fabs(paramsToRank[j+1].Minus_1Sigma_PostFit));
                
            if(doSawp)
            {
                auto saveParam      = paramsToRank[j];
                paramsToRank[j]     = paramsToRank[j+1];
                paramsToRank[j+1]   = saveParam;
            }
        }       
    }

    std::reverse(paramsToRank.begin(),paramsToRank.end());

    vector<NPInformation> highestImpact;

    if(numToRank > int(paramsToRank.size())) numToRank = paramsToRank.size();

    // return only the top N highest ranked NP
    for(int i = 0; i < numToRank; i++)
    {
        highestImpact.push_back(paramsToRank[i]);
    }
    std::reverse(paramsToRank.begin(),paramsToRank.end());
    std::reverse(highestImpact.begin(),highestImpact.end());

    numToRank = highestImpact.size();

    return highestImpact;

}

void  plotRankedNP(vector<NPInformation> &paramsToRankIn, TString poiName)
{

    // Plot Config
    TString jsonPlotConfig = "${wscanner_data}/PlotConfig/" + metaData.at("plotConfig");
    jsonPlotConfig = expandEnvironmentVariables(jsonPlotConfig.Data());
    std::ifstream f(jsonPlotConfig.Data());
    nlohmann::json plotConfig = nlohmann::json::parse(f);

    // Scale the impacts so they plot nicely on the axis with the poi axis
    double maxImpact = -std::numeric_limits<double>::infinity();

    std::vector<NPInformation> NPtoPlot;
    NPtoPlot = rankNP(paramsToRankIn);


    for(const auto& NP:NPtoPlot)
    {
        if(fabs(NP.Minus_1Sigma_PostFit) > maxImpact) maxImpact = fabs(NP.Minus_1Sigma_PostFit); 
        if(fabs(NP.Plus_1Sigma_PostFit)  > maxImpact) maxImpact = fabs(NP.Plus_1Sigma_PostFit); 
        if((fabs(NP.Minus_1Sigma_PreFit) > maxImpact) && !opts["removePrefit"].EqualTo("true")) maxImpact = fabs(NP.Minus_1Sigma_PreFit); 
        if((fabs(NP.Plus_1Sigma_PreFit)  > maxImpact) && !opts["removePrefit"].EqualTo("true")) maxImpact = fabs(NP.Plus_1Sigma_PreFit); 
    }

    // Scale the max impact
    if(!opts["doAbsImpact"].EqualTo("true")) maxImpact = fabs(maxImpact/NPtoPlot[0].baseLine);

    float NPextraSpace = 1.8;
    float scaleValue = 2.0/(NPextraSpace * maxImpact);

    for(auto& NP:NPtoPlot)
    {
        NP.Minus_1Sigma_PostFit *=  scaleValue;
        NP.Plus_1Sigma_PostFit  *=  scaleValue;
        NP.Minus_1Sigma_PreFit  *=  scaleValue;
        NP.Plus_1Sigma_PreFit   *=  scaleValue;
    }

    int color_prefit = TColor::GetColor("#FF8C6E");
    int color_postfit = kAzure + 7;

    std::vector<float> xPOIVal;
    std::vector<float> xPOIErrHi;
    std::vector<float> xPOIErrLo;

    std::vector<float> xPostFitMinusVal;
    std::vector<float> xPostFitPlusVal;
    std::vector<float> xPreFitMinusVal;
    std::vector<float> xPreFitPlusVal;
    std::vector<float> yVal;
    std::vector<float> yValHalves;
    std::vector<float> yValQuater;
    std::vector<float> xValZeros;

    int index = 1;
    for(const auto& NP:NPtoPlot)
    {
        xPOIVal.push_back(NP.NPFitVal);
        xPOIErrHi.push_back(fabs(NP.NPErrHi));
        xPOIErrLo.push_back(fabs(NP.NPErrLo));

        if(opts["doAbsImpact"].EqualTo("true"))
        {
            xPostFitMinusVal.push_back(-(NP.Minus_1Sigma_PostFit));
            xPostFitPlusVal.push_back((NP.Plus_1Sigma_PostFit));
            xPreFitMinusVal.push_back(-(NP.Minus_1Sigma_PreFit));
            xPreFitPlusVal.push_back((NP.Plus_1Sigma_PreFit));
        }
        else
        {
            xPostFitMinusVal.push_back(-(NP.Minus_1Sigma_PostFit)/NP.baseLine);
            xPostFitPlusVal.push_back((NP.Plus_1Sigma_PostFit)/NP.baseLine);
            xPreFitMinusVal.push_back(-(NP.Minus_1Sigma_PreFit)/NP.baseLine);
            xPreFitPlusVal.push_back((NP.Plus_1Sigma_PreFit)/NP.baseLine);
        }

        yVal.push_back(index-0.5);
        yValHalves.push_back(0.5-0.05);
        yValQuater.push_back(0.25);
        xValZeros.push_back(0);
        index++;
    }   

    TGraphAsymmErrors* centralValGraph = new TGraphAsymmErrors(yVal.size(), &xPOIVal[0], &yVal[0], &xPOIErrLo[0], &xPOIErrHi[0], NULL, NULL);
    centralValGraph->SetLineColor(kBlack);
    centralValGraph->SetMarkerColor(kBlack);
    centralValGraph->SetMarkerStyle(20);
    centralValGraph->SetLineStyle(1);
    centralValGraph->SetLineWidth(2);
    centralValGraph->SetMarkerSize(2);
    centralValGraph->GetXaxis()->SetTitleOffset(1.2);


    TGraphAsymmErrors* postFitPlusGraph = new TGraphAsymmErrors(yVal.size(), &xValZeros[0], &yVal[0], NULL, &xPostFitPlusVal[0], &yValHalves[0], &yValHalves[0]);
    postFitPlusGraph->SetLineColor(color_postfit);
    postFitPlusGraph->SetFillColor(color_postfit);
    postFitPlusGraph->SetFillStyle(1001);
    postFitPlusGraph->SetLineWidth(2);
    postFitPlusGraph->SetMarkerSize(0);
    TGraphAsymmErrors* postFitPlusGraphBorder = new TGraphAsymmErrors(yVal.size(), &xValZeros[0], &yVal[0], NULL, &xPostFitPlusVal[0], &yValHalves[0], &yValHalves[0]);
    postFitPlusGraphBorder->SetLineColor(color_postfit);
    postFitPlusGraphBorder->SetFillStyle(0);
    postFitPlusGraphBorder->SetLineWidth(2);
    postFitPlusGraphBorder->SetMarkerSize(0);


    TGraphAsymmErrors* postFitMinusGraph = new TGraphAsymmErrors(yVal.size(), &xValZeros[0], &yVal[0], &xPostFitMinusVal[0], NULL, &yValHalves[0], &yValHalves[0]);
    postFitMinusGraph->SetLineColor(color_postfit);
    postFitMinusGraph->SetFillColor(color_postfit);
    postFitMinusGraph->SetFillStyle(0);
    postFitMinusGraph->SetLineWidth(2);
    postFitMinusGraph->SetMarkerSize(0);

    TGraphAsymmErrors* preFitPlusGraph = new TGraphAsymmErrors(yVal.size(), &xValZeros[0], &yVal[0], NULL, &xPreFitPlusVal[0], &yValQuater[0], &yValQuater[0]);
    preFitPlusGraph->SetLineColor(color_prefit);
    preFitPlusGraph->SetFillColor(color_prefit);
    preFitPlusGraph->SetFillStyle(0);
    preFitPlusGraph->SetLineWidth(2);
    preFitPlusGraph->SetMarkerSize(0);

    TGraphAsymmErrors* preFitMinusGraph = new TGraphAsymmErrors(yVal.size(), &xValZeros[0], &yVal[0], &xPreFitMinusVal[0], NULL, &yValQuater[0], &yValQuater[0]);
    preFitMinusGraph->SetLineColor(color_prefit);
    preFitMinusGraph->SetFillColor(color_prefit);
    preFitMinusGraph->SetFillStyle(1001);
    preFitMinusGraph->SetLineWidth(2);
    preFitMinusGraph->SetMarkerSize(0);
    TGraphAsymmErrors* preFitMinusGraphBorder = new TGraphAsymmErrors(yVal.size(), &xValZeros[0], &yVal[0], &xPreFitMinusVal[0], NULL, &yValQuater[0], &yValQuater[0]);
    preFitMinusGraphBorder->SetLineColor(color_prefit);
    preFitMinusGraphBorder->SetFillStyle(0);
    preFitMinusGraphBorder->SetLineWidth(2);
    preFitMinusGraphBorder->SetMarkerSize(0);

    float lowerLim = 0.45;
    float upperLim = 0.92;
    if(!opts["removePrefit"].EqualTo("true")){ lowerLim = 0.45; upperLim = 0.685;}
    TLegend* legPull = new TLegend(lowerLim, 0.825, upperLim,0.855);
    // legPull->SetNColumns(2);
    // legPull->SetTextAlign(13);
    legPull->AddEntry(centralValGraph, "Pull", "lp");
    legPull->SetFillColor(0);
    legPull->SetBorderSize(0);
    legPull->SetTextFont(42);
    legPull->SetTextSize(0.0175);

    TLegend* leg = new TLegend(0.45,0.77, 0.92,0.825);
    if(!opts["removePrefit"].EqualTo("true")) leg->SetNColumns(2);
    leg->AddEntry(postFitPlusGraph, "+1#sigma Postfit Effect on #hat#sigma", "f");
    if(!opts["removePrefit"].EqualTo("true")) leg->AddEntry(preFitPlusGraph, "+1#sigma Prefit Effect on #hat#sigma", "f");
    leg->AddEntry(postFitMinusGraph, "-1#sigma Postfit Effect on #hat#sigma", "f");
    if(!opts["removePrefit"].EqualTo("true")) leg->AddEntry(preFitMinusGraph, "-1#sigma Prefit Effect on #hat#sigma", "f");
    leg->SetFillColor(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.0175);

    int offset = 2;
    if(numToRank > 15) offset = offset + ceil(numToRank/15);

    TH2D *frameHistogram = new TH2D("frame", "frame",  1, -2, 2, numToRank+offset+1, 0,numToRank+offset+1);
    frameHistogram->GetYaxis()->SetLabelSize(0.04);
    frameHistogram->GetXaxis()->SetLabelSize(0);
    frameHistogram->GetXaxis()->SetTickSize(0);
    frameHistogram->GetYaxis()->SetTickSize(0);

    for(int bin = offset+1; bin < frameHistogram->GetYaxis()->GetNbins(); bin++)
    {
        TString niceName = getPrettyName(NPtoPlot.at(bin-1-offset).NPName);
        frameHistogram->GetYaxis()->SetBinLabel(bin-offset, niceName);
    }

    TCanvas* c1 = new TCanvas("c1","c1",1024,1200);

    TPad *pad1 = new TPad("pad1", "pad1", 0.0  , 0.0  , 1.0 , 1.0  , 0);
    TPad *pad2 = new TPad("pad2", "pad2", 0.63, 0.1, 0.94, 0.22, 0);

    pad1->SetLeftMargin(0.40);//Graph size
    pad1->SetRightMargin(0.05);
    pad1->SetBottomMargin(0.125);
    pad1->SetTopMargin(0.12 );
    pad2->SetLeftMargin(0.325);
    pad2->SetRightMargin(0.01);
    pad1->Draw();
    pad1->cd();


    TGaxis *axis_theta = new TGaxis(frameHistogram->GetXaxis()->GetXmin(), frameHistogram->GetYaxis()->GetXmin(),frameHistogram->GetXaxis()->GetXmax(), frameHistogram->GetYaxis()->GetXmin(), frameHistogram->GetXaxis()->GetXmin(), frameHistogram->GetXaxis()->GetXmax(), 510);
    axis_theta->ImportAxisAttributes(frameHistogram->GetXaxis());
    axis_theta->SetName("axis_theta");
    axis_theta->SetTitle("(#hat{#theta} - #theta_{0})/#Delta#theta");
    axis_theta->SetTitleOffset(1.1);
    axis_theta->SetLineColor(kBlack);
    axis_theta->SetLabelColor(kBlack);
    axis_theta->SetTitleColor(kBlack);
    axis_theta->SetLabelSize(0.045);
    if(numToRank > 15)axis_theta->SetLabelSize(0.035);
    axis_theta->SetTitleSize(0.045);
    axis_theta->SetNdivisions(505);


    TGaxis *axis_poi = new TGaxis(frameHistogram->GetXaxis()->GetXmin(), frameHistogram->GetYaxis()->GetXmax(),frameHistogram->GetXaxis()->GetXmax(), frameHistogram->GetYaxis()->GetXmax(), frameHistogram->GetXaxis()->GetXmin()/scaleValue, frameHistogram->GetXaxis()->GetXmax()/scaleValue, 510, "-");
    axis_poi->ImportAxisAttributes(frameHistogram->GetXaxis());
    axis_poi->SetName("axis_poi");

    TString poiNameClean = plotConfig["poiNameMap"][poiName.Data()];
    TString fancyPOINameTop = "#Delta" + poiNameClean;
    TString fancyPOIName = fancyPOINameTop + "/#hat{" + poiNameClean + "}";
    if(opts["doAbsImpact"].EqualTo("true")) fancyPOIName = fancyPOINameTop;

    axis_poi->SetTitle(fancyPOIName);
    axis_poi->SetTitleOffset(1.1 * axis_poi->GetTitleOffset());
    axis_poi->SetLineColor(color_postfit);
    axis_poi->SetLabelColor(color_postfit);
    axis_poi->SetTitleColor(color_postfit);
    axis_poi->SetLabelSize(0.045);
    axis_poi->SetTitleSize(0.035);
    axis_poi->SetNdivisions(505);


    TGaxis *axis_ticks_right = new TGaxis(frameHistogram->GetXaxis()->GetXmin(), frameHistogram->GetYaxis()->GetXmin(),frameHistogram->GetXaxis()->GetXmin(), frameHistogram->GetYaxis()->GetXmax()-offset-1, frameHistogram->GetYaxis()->GetXmin(), frameHistogram->GetYaxis()->GetXmax()-offset-1);
    axis_ticks_right->ImportAxisAttributes(frameHistogram->GetYaxis());
    axis_ticks_right->SetLabelSize(0);
    axis_ticks_right->SetTitleSize(0.045);


    TGaxis *axis_ticks_left = new TGaxis(frameHistogram->GetXaxis()->GetXmax(), frameHistogram->GetYaxis()->GetXmin(),frameHistogram->GetXaxis()->GetXmax(), frameHistogram->GetYaxis()->GetXmax()-offset-1, frameHistogram->GetYaxis()->GetXmin(), frameHistogram->GetYaxis()->GetXmax()-offset-1);
    axis_ticks_left->ImportAxisAttributes(frameHistogram->GetYaxis());
    axis_ticks_left->SetLabelSize(0);
    axis_ticks_left->SetOption("+");
    axis_ticks_left->SetTitleSize(0.045);
    

    frameHistogram->Draw();
    axis_theta->Draw();
    axis_poi->Draw();
    axis_ticks_right->Draw();
    axis_ticks_left->Draw();
    
    postFitMinusGraph->Draw("p2same");    
    postFitPlusGraph->Draw("p2same");
    postFitPlusGraphBorder->Draw("p2same"); 

    if(!opts["removePrefit"].EqualTo("true"))
    {
        preFitMinusGraph->Draw("p2same");
        preFitPlusGraph->Draw("p2same");
        preFitMinusGraphBorder->Draw("p2same");
    }

    // some line definitions
    TLine l;
    l.SetLineWidth(2);
    l.SetLineColor(kGray + 2);
    l.SetLineStyle(2);


    l.DrawLine( 0. , 0.,  0. , frameHistogram->GetYaxis()->GetXmax()-offset-1);
    l.DrawLine( 1. , 0.,  1. , frameHistogram->GetYaxis()->GetXmax()-offset-1);
    l.DrawLine(-1. , 0., -1. , frameHistogram->GetYaxis()->GetXmax()-offset-1);

    centralValGraph->Draw("p same");
    leg->Draw();
    legPull->Draw();
    frameHistogram->Draw("same");

    float labelPosX = 0.06;
    float channelPosY = 0.9;
    float labelPosY = channelPosY-0.04;

    pad1->cd();
    std::string expName = plotConfig["experimentString"];
    std::string plotType = plotConfig["plotTypeString"];
    ExpLabel(expName.c_str(), labelPosX, labelPosY, plotType.c_str(), 1, 0.9);

    TString channelStr = plotConfig["channelString"];
    TLatex* tChannelStr = new TLatex (labelPosX, labelPosY-0.035, channelStr);
    tChannelStr->SetNDC();
    tChannelStr->SetTextSize(0.0375);
    tChannelStr->SetTextFont(42);
    tChannelStr->Draw();

    TString lumiString = plotConfig["lumiString"];
    TLatex energyLumi;
    energyLumi.SetNDC();
    energyLumi.SetTextSize(0.03);
    energyLumi.DrawLatex(labelPosX,labelPosY-0.07, lumiString);

    if(opts["addNums"].EqualTo("true"))
    {
        TLatex lwrPostFitText;
        lwrPostFitText.SetTextSize(0.02); 
        lwrPostFitText.SetTextColor(color_postfit);
        TLatex uprPostFitText;
        uprPostFitText.SetTextSize(0.02); 
        uprPostFitText.SetTextColor(color_postfit);
        for(size_t i = 0; i < xPostFitMinusVal.size(); i++)
        {
            TString lowerVal;
            TString upperVal;
            // Remove the scale
            TString formatStr = "%.3f";
            if(xPostFitMinusVal.at(i) > 0) formatStr = " " + formatStr;
            lowerVal.Form(formatStr, -xPostFitMinusVal.at(i)/scaleValue);
            upperVal.Form(formatStr, xPostFitPlusVal.at(i)/scaleValue);
            lwrPostFitText.DrawLatex(frameHistogram->GetXaxis()->GetXmin() + 0.2, frameHistogram->GetYaxis()->GetXmin() + 0.35 + i, lowerVal);
            uprPostFitText.DrawLatex(frameHistogram->GetXaxis()->GetXmax() - 0.55, frameHistogram->GetYaxis()->GetXmin() + 0.35 + i, upperVal);
        }
        lwrPostFitText.DrawLatex(frameHistogram->GetXaxis()->GetXmin() + 0.20, frameHistogram->GetYaxis()->GetXmin() + 0.14 + xPostFitMinusVal.size(), "-1#sigma Impact");
        uprPostFitText.DrawLatex(frameHistogram->GetXaxis()->GetXmax() - 0.75, frameHistogram->GetYaxis()->GetXmin() + 0.14 + xPostFitMinusVal.size(), "+1#sigma Impact");

    }

    TString baseName = getBaseName(opts["fileName"]);
    TString directoryName = opts["plotDir"] + "/"+baseName;
    system("mkdir -vp " + directoryName);
    TString dataStr = metaData["doData"];
    dataStr.ToLower();
    if(dataStr.Contains("true")) dataStr = "data";
    else dataStr = "asimov";


    TString saveName = directoryName + "/" + "impact_" + dataStr + "_" + poiName + "." + plotConfig["plotExtension"];
    c1->SaveAs(saveName);
}

TString getPrettyName(TString name)
{

    TString prettyName = name;
    prettyName.ReplaceAll("_", " ");
    prettyName.ReplaceAll("ATLAS", "");
    prettyName.ReplaceAll("alpha", "");

    return prettyName;
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
}


bool cmdline(int argc, char** argv, map<TString,TString>& opts)
{
    opts.clear();
   
    // defaults
    opts["fileName"]    = "asimovScan.root";
    opts["baseName"]    = "Empty";
    opts["impactPOIs"]  = "mu_gg2H,mu_VH,mu_VBF,mu_ttH";
    opts["removePrefit"]= "false";
    opts["doAbsImpact"] = "false";
    opts["addNums"]     = "true";
    opts["plotDir"]     = "Plots";
    opts["removeNP"]    = "";
    opts["setPrefitToZero"] = "";


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
        if(opt == "removePrefit")   {opts["removePrefit"] = "true"; continue;}
        if(opt == "doAbsImpact")    {opts["doAbsImpact"]  = "true"; continue;}
        if(opt == "addNums")        {opts["addNums"] = "true"; continue;}

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
    return true;
}


std::vector<NPInformation> readRootFile(TString impactedPOI)
{

    std::vector<TString> removeNPList = tokenizeStr(opts["removeNP"],",");


    TFile *fileName = TFile::Open(opts["fileName"], "r");
    TTree *tree = (TTree*)fileName->Get("results");

    getVariableTree(tree);

    setupReadTree(tree);

    std::set<TString> NPNameList = getNPList(tree);

    std::vector<NPInformation> paramsToRank;
    int j =0;

    std::map<int, TString> map_NPnameTree = {};
    for(int i = 0; i < tree->GetEntries(); i++)
    {
      tree->GetEntry(i);
      map_NPnameTree[i]=TStringVarMap["NPName"][0];
    }


    for(const auto& NPname:NPNameList)
    {
        // See if we want to skip any 
        bool skip = false;
        for(const auto& removeNP: removeNPList)
        {
            if(removeNP.Length() == 0) continue;
            
            if (isMatched(NPname.Data(), removeNP.Data()))
            {
                skip = true;
                break;
            }
        }
        if(skip) continue;

        paramsToRank.push_back(NPInformation());
        paramsToRank[j].NPName = NPname;

        std::vector<int> NPIndexTree;
        for (auto &NP_i : map_NPnameTree)
        {
          if( NP_i.second == NPname){
            NPIndexTree.push_back(NP_i.first);
          }
        }

        for(auto i:NPIndexTree)
        {
            tree->GetEntry(i);  

            if(TStringVarMap["NPName"][0].EqualTo(paramsToRank[j].NPName))
            {
                if(TStringVarMap["fitName"][0].EqualTo("BaseLine"))
                {
                    paramsToRank[j].baseLine = doubleVarMap[impactedPOI];
                    paramsToRank[j].NPFitVal = doubleVarMap[NPname];
                    paramsToRank[j].NPErrLo = doubleVarMap["NPErrorLow"];
                    paramsToRank[j].NPErrHi = doubleVarMap["NPErrorHi"];
                }
                else if(TStringVarMap["fitName"][0].EqualTo("Plus_1Sigma_PostFit")) paramsToRank[j].Plus_1Sigma_PostFit = doubleVarMap[impactedPOI];
                else if(TStringVarMap["fitName"][0].EqualTo("Minus_1Sigma_PostFit"))paramsToRank[j].Minus_1Sigma_PostFit = doubleVarMap[impactedPOI];
                else if(TStringVarMap["fitName"][0].EqualTo("Plus_1Sigma_PreFit")) paramsToRank[j].Plus_1Sigma_PreFit = doubleVarMap[impactedPOI];
                else if(TStringVarMap["fitName"][0].EqualTo("Minus_1Sigma_PreFit"))paramsToRank[j].Minus_1Sigma_PreFit = doubleVarMap[impactedPOI];
            }
        }

        j++;

    }

    std::vector<TString> prefitNPList = tokenizeStr(opts["setPrefitToZero"],",");

    // Subtract off baseline
    for(auto &NP:paramsToRank)
    {
        NP.Plus_1Sigma_PostFit  = NP.Plus_1Sigma_PostFit - NP.baseLine;
        NP.Minus_1Sigma_PostFit = NP.Minus_1Sigma_PostFit - NP.baseLine;
        NP.Plus_1Sigma_PreFit   = NP.Plus_1Sigma_PreFit - NP.baseLine;
        NP.Minus_1Sigma_PreFit  = NP.Minus_1Sigma_PreFit - NP.baseLine;

        // See if we want to skip any 
        bool setToZero = false;
        for(const auto& npName: prefitNPList)
        {
            if(npName.Length() == 0) continue;
            
            if (isMatched(NP.NPName.Data(), npName.Data()))
            {
                setToZero = true;
                break;
            }
        }
        if(setToZero)
        {
            NP.Plus_1Sigma_PreFit  = 0;
            NP.Minus_1Sigma_PreFit = 0;

        }

    }

    fileName->Close();
    delete fileName;

    return paramsToRank;


}

// return a unique key list of the NP
std::set<TString> getNPList(TTree *tree)
{
    std::set<TString> uniqueNPNames;
    for(int i = 0; i < tree->GetEntries(); i++)
    {
        tree->GetEntry(i);

        TString currentNP = TStringVarMap["NPName"][0];
        if(doubleVarMap["isUnconditional"] == 0) uniqueNPNames.insert(currentNP);

    }

    return uniqueNPNames;


}

void getVariableTree(TTree *tree)
{
    TObjArray* listLf = tree->GetListOfLeaves();

    
    for (Int_t i = 0; i < listLf->GetEntries(); i++)
    {
        TLeaf* lf = (TLeaf*) listLf->At(i);

        TString typeName   = lf->GetTypeName();
        std::string brName = lf->GetTitle();
        TString titleName  = lf->GetTitle();


        if(typeName.Contains("Double_t"))
        {
            doubleVarMap[lf->GetName()] = -999;
        }

        else if(typeName.Contains("TString"))
        {
            TStringVarMap[lf->GetName()] = new TString();
        }
    }
    
}
void setupReadTree(TTree *tree)
{

    for (auto it = doubleVarMap.begin(); it != doubleVarMap.end(); ++it)
    {
        TString varName = it->first;
        tree->SetBranchAddress(varName, &it->second);
    }
    for (auto it = TStringVarMap.begin(); it != TStringVarMap.end(); ++it)
    {
        TString varName = it->first;
        tree->SetBranchAddress(varName, &it->second);
    }

}
void setPrettyStuff()
{
    SetAtlasStyle();
}

bool isMatched(std::string input, std::string regex)
{
    std::regex e (regex);
    return std::regex_match (input,e);
}
