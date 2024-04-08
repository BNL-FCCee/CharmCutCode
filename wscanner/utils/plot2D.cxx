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
#include "fstream"

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
#include <TPolyLine.h>
#include "commonCode.h"
#include "AtlasLabels.C"
#include "AtlasStyle.C"
#include <nlohmann/json.hpp>
#include "Math/ProbFunc.h"


using namespace std;

// Global vars
map<TString,TString> opts;
map<TString,TString> metaData;


// // Functions
bool cmdline(int argc, char** argv, map<TString,TString>& opts);
TString getBaseName(TString pathName);

void setPrettyStuff();

void fill2DHist(TH2D* inHist, TTree* treeInfo, std::map<TString, TString> fileInfo, TString varName);
TMarker* getBestFitPoint(TTree* treeInfo, std::map<TString, TString> fileInfo);
double getBestFitNLL(TTree* treeInfo, std::map<TString, TString> fileInfo);
vector<TPolyLine*> getSMEllipse(std::map<TString, TString> fileInfo, TCanvas* c1);

int main(int argc, char** argv)
{
    setPrettyStuff();

    if (!cmdline(argc,argv,opts)) return 0;

    // Split the results
    vector<TString> fileNameList = tokenizeStr(opts["fileName"],",");
    std::vector<std::map<TString, TString>> metadataVect = getMetadata(fileNameList);

    // Plot Config
    TString jsonPlotConfig = "${wscanner_data}/PlotConfig/" + metadataVect[0].at("plotConfig");
    jsonPlotConfig = expandEnvironmentVariables(jsonPlotConfig.Data());
    std::ifstream f(jsonPlotConfig.Data());
    nlohmann::json plotConfig = nlohmann::json::parse(f);

    for(size_t i = 0; i < fileNameList.size(); i++)
    {
        cout<<"---------------------------"<<endl;
        cout<<fileNameList[i]<<endl;
        for(const auto& cMap: metadataVect[i]) cout<<cMap.first<<" "<<cMap.second<<endl;
    }

    bool hasData = false;
    bool hasAsimov = false;
    for(size_t i = 0; i < fileNameList.size(); i++)
    {
        if(metadataVect[i]["dataType"] == "data") hasData = true;
        if(metadataVect[i]["dataType"] == "asimov") hasAsimov = true;
    }


    // For output
    TString fileNameApp = "";
    if(hasData && !hasAsimov) fileNameApp = "Obs_";
    if(!hasData && hasAsimov) fileNameApp = "Exp_";


    double minXF = std::numeric_limits<double>::infinity();
    double maxXF = -std::numeric_limits<double>::infinity();
    double minYF = std::numeric_limits<double>::infinity();
    double maxYF = -std::numeric_limits<double>::infinity();

    map<TString, TTree*> treeList;
    map<TString, TH2D*>  histList;
    map<TString, TH2D*>  cont95percList;
    map<TString, TH2D*>  cont68percList;
    map<TString, TMarker*>  bestFitMarkerList;
    map<TString, double>  bestFitNLLList;


    // Figure out the ranges for the X and Y
    for (size_t i = 0; i < fileNameList.size(); i++)
    {
        TString cFileName = fileNameList[i];
        cout<<"File "<<cFileName<<endl;

        std::map<TString, TString> metadataMap = metadataVect.at(i);

        // make the base name right now
        TString baseName = getBaseName(cFileName);
        TString directoryName = opts["plotDir"] + "/" + baseName;
        system("mkdir -vp " + directoryName);

        // Open the file
        TChain* chain = new TChain("results");
        chain->Add(cFileName);

        treeList[cFileName] = chain;

        double cXMax = chain->GetMaximum(metadataMap["poi_1"]);
        double cXMin = chain->GetMinimum(metadataMap["poi_1"]);

        double cYMax = chain->GetMaximum(metadataMap["poi_2"]);
        double cYMin = chain->GetMinimum(metadataMap["poi_2"]);

        // Increase the cYMax by a constant amount to have a nice white space above it
        int nSteps = (cYMax - cYMin)/metadataMap["poi_2_delta"].Atof();
        nSteps *= 1.3;
        cYMax = cYMin + nSteps * metadataMap["poi_2_delta"].Atof();

        if(cXMax >= maxXF) maxXF = cXMax;
        if(cXMin <= minXF) minXF = cXMin;

        if(cYMax >= maxYF) maxYF = cYMax;
        if(cYMin <= minYF) minYF = cYMin;
    }

    cout<<"Ranges: minXF "<<minXF<<" maxXF: "<<maxXF<<" minYF: "<<minYF<<" maxYF: "<<maxXF<<endl;


    double nDOF = 2;
    // If this is provided through an input argument overwrite the 
    double externalNDOF = opts["nDOF"].Atof();
    if(externalNDOF != -1) nDOF = externalNDOF;


    cout<<"NDOF: "<<nDOF<<" 68%: "<<TMath::ChisquareQuantile(.68, nDOF)<<" 95%: "<<TMath::ChisquareQuantile(.95, nDOF)<<endl;

    double contour68[]{TMath::ChisquareQuantile(.68, nDOF)};
    double contour95[]{TMath::ChisquareQuantile(.95, nDOF)};
    // double contour3Sigma[]{TMath::ChisquareQuantile(0.997300203936740, nDOF)};
    // double contour4Sigma[]{TMath::ChisquareQuantile(0.999936657516334, nDOF)};
    // double contour5Sigma[]{TMath::ChisquareQuantile(0.32, nDOF)};

    // Actually read in the main histogram
    // And create the contours
    for (size_t i = 0; i < fileNameList.size(); i++)
    {
        TString cFileName = fileNameList[i];

        std::map<TString, TString> metadataMap = metadataVect.at(i);

        int nXSteps = (maxXF - minXF)/metadataMap["poi_1_delta"].Atof();
        int nYSteps = (maxYF - minYF)/metadataMap["poi_2_delta"].Atof();

        TString histName = cFileName;
        histName.ReplaceAll(".root","").ReplaceAll("/",""); 


        TH2D* hist = new TH2D(histName, histName, nXSteps, minXF, maxXF, nYSteps, minYF, maxYF);
        fill2DHist(hist, treeList[cFileName], metadataMap, "nll");
        histList[cFileName] = hist;

        histName = cFileName;
        histName.ReplaceAll(".root",""); 

        TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 600, 600);
        hist->SetContour(1000);
        hist->SetMaximum(20);
        hist->Draw("colz");
        c1->SetRightMargin(0.2);
        c1->SaveAs(histName+".pdf");

        delete c1;


        cout<<"histName "<<histName<<" Max: "<<hist->GetMaximum()<<" min: "<<hist->GetMinimum()<<endl;


        TH2D* hist_68 = (TH2D*)hist->Clone(histName + "68");        
        hist_68->SetContour(1,contour68);
        cont68percList[cFileName] = hist_68;

        TH2D* hist_95 = (TH2D*)hist->Clone(histName + "95");        
        hist_95->SetContour(1,contour95);
        cont95percList[cFileName] = hist_95;


        // TH2D* hist_3Sigma = (TH2D*)hist->Clone(histName + "3Sigma");        
        // hist_3Sigma->SetContour(1,contour3Sigma);
        // cont95percList[cFileName + "3sigma"] = hist_3Sigma;

        // TH2D* hist_4Sigma = (TH2D*)hist->Clone(histName + "4Sigma");        
        // hist_4Sigma->SetContour(1,contour4Sigma);
        // cont95percList[cFileName + "4sigma"] = hist_4Sigma;

        // TH2D* hist_5Sigma = (TH2D*)hist->Clone(histName + "5Sigma");        
        // hist_5Sigma->SetContour(1,contour5Sigma);
        // cont95percList[cFileName + "5sigma"] = hist_5Sigma;

        bestFitMarkerList[cFileName] = getBestFitPoint(treeList[cFileName], metadataMap);
        bestFitNLLList[cFileName]    = getBestFitNLL(treeList[cFileName], metadataMap);


        // Save the hist file for adaptive sampling
        TString directoryName = opts["plotDir"] +"/" + getBaseName(fileNameList[0]) + "/InputsForAdaptiveSampling/";
        system("mkdir -vp " + directoryName);

        TString saveName = directoryName + "/"  + metadataVect[0]["poi_1"] + "_" + metadataVect[0]["poi_2"] + ".root";

        TFile* histFile = TFile::Open(saveName, "recreate");
        TH2D* histExclusionCont = (TH2D*) hist->Clone("ExclusionCont");        

        for(int ixBin = 0; ixBin <= histExclusionCont->GetXaxis()->GetNbins(); ixBin++)
        {
            for(int iyBin = 0; iyBin <= histExclusionCont->GetYaxis()->GetNbins(); iyBin++)
            {
                double valToFill = histExclusionCont->GetBinContent(ixBin + 1, iyBin+1);
                valToFill = ROOT::Math::chisquared_cdf(valToFill, nDOF);
                histExclusionCont->SetBinContent(ixBin + 1, iyBin+1, valToFill);
            }
        }
        histExclusionCont->Write();
        histFile->Close();



    }

    // format the plots
    int numDraw = 1756;
    for (size_t i = 0; i < fileNameList.size(); i++)
    {
        TString cFileName = fileNameList[i];

        std::map<TString, TString> metaDataMap = metadataVect.at(i);

        cont95percList[cFileName]->SetLineColor(numDraw+1);
        cont95percList[cFileName]->SetLineWidth(2);

        cont68percList[cFileName]->SetLineColor(numDraw+1);
        cont68percList[cFileName]->SetLineWidth(2);


        // if(labelsMap["dataType"] == "data") 
        // {
        //     cont95percList[cFileName]->SetLineColor(kBlack); 
        //     cont68percList[cFileName]->SetLineColor(kBlack);
        //     bestFitMarkerList[cFileName]->SetMarkerColor(kBlack);
        // }
        // else if(labelsMap["dataType"] == "data") 
        // {
        //     cont95percList[cFileName]->SetLineColor(kBlack); 
        //     cont68percList[cFileName]->SetLineColor(kBlack);
        //     bestFitMarkerList[cFileName]->SetMarkerColor(kBlack);
        // }       
        // else if(labelsMap["scanType"].Contains("statOnly"))
        // {
        //     cont95percList[cFileName]->SetLineColor(kBlack); 
        //     cont68percList[cFileName]->SetLineColor(kBlack);
        // }
        // else 
        // {
        //     cont95percList[cFileName]->SetLineColor(kAzure+7); 
        //     cont68percList[cFileName]->SetLineColor(kAzure+7);

        //     // cont95percList[cFileName + "3sigma"]->SetLineColor(kMagenta);
        //     // cont95percList[cFileName + "4sigma"]->SetLineColor(kPink + 3);
        //     // cont95percList[cFileName + "5sigma"]->SetLineColor(kOrange);

        //     bestFitMarkerList[cFileName]->SetMarkerColor(kAzure+7);
        // }

        cont68percList[cFileName]->SetLineStyle(2);

        numDraw++;
    }


    // Start the plotting
    TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 600, 600);

    TH2D* frame = new TH2D("frame", "frame", 10, minXF, maxXF, 10, minYF, maxYF);
    frame->GetXaxis()->SetTitleSize(0.8*frame->GetXaxis()->GetTitleSize());
    frame->GetXaxis()->SetTitleOffset(1.25*frame->GetXaxis()->GetTitleOffset());
    frame->GetXaxis()->SetLabelOffset(2.5*frame->GetXaxis()->GetLabelOffset());

    frame->GetYaxis()->SetTitleSize(0.8*frame->GetYaxis()->GetTitleSize());
    frame->GetYaxis()->SetTitleOffset(1.25*frame->GetYaxis()->GetTitleOffset());
    frame->GetYaxis()->SetLabelOffset(2.5*frame->GetYaxis()->GetLabelOffset());

    TString poiName_1 = plotConfig["poiNameMap"][metadataVect[0]["poi_1"].Data()];
    frame->GetXaxis()->SetTitle(poiName_1);
    TString poiName_2 = plotConfig["poiNameMap"][metadataVect[0]["poi_2"].Data()];
    frame->GetYaxis()->SetTitle(poiName_2);


    TH2D* toyHist = NULL;

    if(opts["toyFileName"].Length() > 0)
    {    
        cout<<"Doing toys "<<endl;
        TFile* toyFile = TFile::Open(opts["toyFileName"]);
        TTree* toyTree = (TTree*) toyFile->Get("results");
        toyHist = new TH2D("toyHist", "toyHist", 25, minXF, maxXF, 25, minYF, maxYF);

        toyTree->Draw(metadataVect[0]["poi_2"]+ ":" + metadataVect[0]["poi_1"] + ">>toyHist", "(isUnconditional == 0)");
    
        toyHist->SetMarkerSize(0.4);
    }

    // 
    frame->Draw();

    if(toyHist) toyHist->Draw("p");
    for(const auto& hist: cont68percList) hist.second->Draw("cont3 same");
    for(const auto& hist: cont95percList) hist.second->Draw("cont3 same");
    if((plotConfig["plotStyle"] == "PaperNominal") || (plotConfig["plotStyle"] == "Nominal"))
    { 
        for(const auto& hist: bestFitMarkerList) hist.second->Draw("same");
    }


    auto SMList = getSMEllipse(metadataVect.at(0), c1);
    for(const auto& p: SMList) p->Draw("fsame");

    TMarker* smPoint = NULL;
    if(SMList.size() == 0)
    {
        if((plotConfig["analType"] == "EFT") || (plotConfig["analType"] =="CP")) smPoint = new TMarker(0, 0, 29);
        else smPoint = new TMarker(1, 1, 29);
        smPoint->SetMarkerColor(kAzure + 7);
        smPoint->Draw("same");
    }





    // Top level stuff
    double leftDist = 0.19;
    std::string expName = plotConfig["experimentString"];
    std::string plotType = plotConfig["plotTypeString"];
    ExpLabel(expName.c_str(), leftDist, 0.875, plotType.c_str(), 1);

    TString channelStr = plotConfig["channelString"];
    TLatex* tMain = new TLatex (leftDist, 0.81, channelStr);
    tMain->SetNDC();
    tMain->SetTextSize(0.0425);
    tMain->SetTextFont(42);
    tMain->Draw();

    TString lumiString = plotConfig["lumiString"];
    TLatex* lumInfo = new TLatex (leftDist, 0.76, lumiString);
    lumInfo->SetNDC();
    lumInfo->SetTextSize(0.0325);
    lumInfo->SetTextFont(42);
    lumInfo->Draw();

    if(plotConfig["analType"] == "XS")
    {
        TString rapidityStr = "|y_{H}| < 2.5";
        TLatex* rapInfo = new TLatex (leftDist, 0.72, rapidityStr);
        rapInfo->SetNDC();
        rapInfo->SetTextSize(0.0275);
        rapInfo->SetTextFont(42);
        rapInfo->Draw();
    }


    TString DOFString = "";
    // DOFString.Form("nDOF: %.2f", nDOF);
    // TLatex* DOFInfo = new TLatex (leftDist, 0.72, DOFString);
    // DOFInfo->SetNDC();
    // DOFInfo->SetTextSize(0.025);
    // DOFInfo->SetTextFont(42);
    // DOFInfo->Draw();


    double dataNLL = 0;
    // double SMNLL = 0;
    for(size_t i = 0; i < fileNameList.size(); i++)
    {
        TString toAppend = "";
        if(metadataVect[i]["dataType"] != "data") continue;
        if(metadataVect[i]["scanType"].Contains("statOnly")) continue;

        dataNLL = bestFitNLLList[fileNameList.at(i)];
    }

    if(dataNLL != 0 && (plotConfig["plotStyle"] != "PaperNominal"))
    {
        DOFString.Form("Best Fit pValue: %.2f", ROOT::Math::chisquared_cdf_c(dataNLL, opts["nPOI"].Atof()));
        auto DOFInfo = new TLatex (leftDist, 0.72, DOFString);
        DOFInfo->SetNDC();
        DOFInfo->SetTextSize(0.025);
        DOFInfo->SetTextFont(42);
        DOFInfo->Draw();
    }


    TLegend* leg =  new TLegend (0.6, 0.76, 0.90, 0.925);
    leg->SetFillColor(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.0275);
     // Legend
    for(size_t i = 0; i < fileNameList.size(); i++)
    {
        TString toAppend = "";
        if(fileNameList.size() == 1)                         toAppend = "";
        if(metadataVect[i]["dataType"] == "data")            toAppend = "Obs ";
        else                                                 toAppend = "Exp ";
        
        if(metadataVect[i]["scanType"].Contains("statOnly")) toAppend += "Stat Only- ";

        if((plotConfig["plotStyle"] == "PaperNominal") || (plotConfig["plotStyle"] == "Nominal"))
        { 
            leg->AddEntry(bestFitMarkerList[fileNameList[i]], toAppend + "Best Fit", "p");
        }
        
        leg->AddEntry(cont68percList[fileNameList[i]], toAppend + "68\% CL.", "l");
        leg->AddEntry(cont95percList[fileNameList[i]], toAppend + "95\% CL.", "l");
        // leg->AddEntry(cont95percList[fileNameList[i] + "3sigma"], toAppend + "3Sigma Obs.", "l");
        // leg->AddEntry(cont95percList[fileNameList[i] + "4sigma"], toAppend + "4Sigma Obs.", "l");
        // leg->AddEntry(cont95percList[fileNameList[i] + "5sigma"], toAppend + "5Sigma Obs.", "l");

    }
    
    if(SMList.size() == 0) leg->AddEntry(smPoint, "SM", "p");
    else  leg->AddEntry(SMList[0], "SM", "f");
    if(toyHist) leg->AddEntry(toyHist, "Toys", "p");
    leg->Draw();

    TString saveName = opts["plotDir"] + "/" + getBaseName(fileNameList[0]) + "/" + fileNameApp + metadataVect[0]["poi_1"] + "_" + metadataVect[0]["poi_2"] + "." + plotConfig["plotExtension"];

    if(opts["forceFitType"].EqualTo("true")) saveName = opts["plotDir"] + "/" + getBaseName(fileNameList[0]) + "/" + fileNameApp + metadataVect[0]["poi_1"] + "_" + metadataVect[0]["poi_2"] + "_forced." + plotConfig["plotExtension"];

    c1->SaveAs(saveName);

   return 0;
}

void fill2DHist(TH2D* inHist, TTree* treeInfo, std::map<TString, TString> fileInfo, TString varName)
{
    double poi1, poi2, var, isConditional, fitType;
    treeInfo->SetBranchAddress(fileInfo["poi_1"],   &poi1);
    treeInfo->SetBranchAddress(fileInfo["poi_2"],   &poi2);
    treeInfo->SetBranchAddress(varName,             &var);
    treeInfo->SetBranchAddress("isUnconditional",   &isConditional);
    treeInfo->SetBranchAddress("fitType",   &fitType);

    // To make sure it doesn't hit the bin edge
    double poi1_offset = fileInfo["poi_1_delta"].Atof()/100;
    double poi2_offset = fileInfo["poi_2_delta"].Atof()/100;



    double sumVarConditional = 0;
    int    countVarConditional = 0; 
    double averVarConditional = 0;

    // Calculate the average of the unconditional NLL
    if(varName.EqualTo("nll"))
    {
        bool hasUnparmUnconditional = (treeInfo->GetEntries("fitType == 2")>0 ? 1:0);
        cout << "The result of the unconditional flag is: " << hasUnparmUnconditional << endl;
        if(opts["forceFitType"].EqualTo("true"))
        {
            hasUnparmUnconditional = false;
            cout << "forcing type type to use 1: " << hasUnparmUnconditional << endl;
        }


        for(int i = 0 ; i < treeInfo->GetEntries(); i++)
        {
            treeInfo->GetEntry(i);
            if(hasUnparmUnconditional)
            {
                if(fitType > 1.5 && fitType < 2.5)
                {
                    countVarConditional++;
                    sumVarConditional  += var;
                }
            }
            else
            {
                if(fitType > 0.5 && fitType < 1.5)
                {
                    countVarConditional++;
                    sumVarConditional  += var;
                }
            }
        }
        averVarConditional = sumVarConditional/countVarConditional;
    }
    // TRandom *randNum       = new TRandom();

    double minNLL = std::numeric_limits<double>::infinity();
    // Actually fill the information
    for(int i = 0 ; i < treeInfo->GetEntries(); i++)
    {
        treeInfo->GetEntry(i);
        //cout<<"fill poi1: "<<poi1<<" poi2: "<<poi2<<" isConditional: "<<isConditional<<" fitType: "<<fitType<<" var: "<<var<<endl;
        if(isConditional < 0.5)
        {
            // inHist->Fill(poi1 + poi1_offset, poi2 + poi2_offset, var - averVarConditional);
            //

            int binX = inHist->GetXaxis()->FindBin(poi1 + poi1_offset) ;
            int binY = inHist->GetYaxis()->FindBin(poi2 + poi2_offset) ;

            inHist->SetBinContent(binX, binY, var - averVarConditional);
            


            if((var - averVarConditional) < minNLL) minNLL = var - averVarConditional;
            // float rand = randNum->Rndm(i);
            // // cout << "RandNum: " << rand << endl;
            // double nnL68 = TMath::ChisquareQuantile(.68, 20); 
            // float eps = 0.001;

            // if((var - averVarConditional) < nnL68 + eps && (var - averVarConditional) > nnL68 - eps && rand < 0.5) 
            //     cout << poi1 << "\t" << poi2 << "\t" << var - averVarConditional << endl;


        }
    }

    cout << "Min NLL = " << minNLL << endl;

    // histogram by fill in blanks
    // fill the Y bins
    int lastBinFilled = inHist->GetYaxis()->FindBin(treeInfo->GetMaximum(fileInfo["poi_2"]) + poi2_offset) ;
    for(int i = lastBinFilled; i <= inHist->GetYaxis()->GetNbins(); i++)
    {
        for(int j = 0; j <= inHist->GetXaxis()->GetNbins(); j++)
        {
            double valToFill = 0;
            if(varName.EqualTo("nll"))  valToFill = inHist->GetBinContent(j+1, i);
            inHist->SetBinContent(j+1, i+1, valToFill);
        }
    }

    // fill the X bins
    lastBinFilled = inHist->GetXaxis()->FindBin(treeInfo->GetMaximum(fileInfo["poi_1"]) + poi1_offset) ;
    for(int i = lastBinFilled; i <= inHist->GetXaxis()->GetNbins(); i++)
    {
        for(int j = 0; j <= inHist->GetYaxis()->GetNbins(); j++)
        {
            double valToFill = 0;
            if(varName.EqualTo("nll"))  valToFill = inHist->GetBinContent(i, j+1);
            inHist->SetBinContent(i+1, j+1, valToFill);
        }
    }
}

TMarker* getBestFitPoint(TTree* treeInfo, std::map<TString, TString> fileInfo)
{
    double poi1 = -999;
    double poi2 = -999;
    double isConditional = -999;
    double fitType = -999;
    double nll = -999;
    //cout<<"poi 1 name: "<<fileInfo.at("poi_1")<<" poi 2 name: "<<fileInfo.at("poi_2")<<endl;
    treeInfo->SetBranchAddress("nll",   &nll);
    treeInfo->SetBranchAddress(fileInfo.at("poi_1"),   &poi1);
    treeInfo->SetBranchAddress(fileInfo.at("poi_2"),   &poi2);
    treeInfo->SetBranchAddress("isUnconditional",   &isConditional);
    treeInfo->SetBranchAddress("fitType",   &fitType);

    double bestFitX = 0;
    double bestFitY = 0;

    for(int i = 0 ; i < treeInfo->GetEntries(); i++)
    {
        treeInfo->GetEntry(i);
        if(!(fitType > 0.5 && fitType < 1.5)) continue;

        cout<<"best fit poi1: "<<poi1<<" poi2: "<<poi2<<" isConditional: "<<isConditional<<" fitType: "<<fitType<<endl;
        if(isConditional > 0.5)
        {
            bestFitX = poi1;
            bestFitY = poi2;
            break;
        }
    }
    cout << "Best fit X: " << bestFitX << ", best fit Y: " << bestFitY << endl;
    TMarker* best_fit = new TMarker(bestFitX,bestFitY, 34);
    return best_fit;
}

double getBestFitNLL(TTree* treeInfo, std::map<TString, TString> fileInfo)
{
    double poi1, poi2, isConditional, fitType, nll;
    treeInfo->SetBranchAddress(fileInfo["poi_1"],   &poi1);
    treeInfo->SetBranchAddress(fileInfo["poi_2"],   &poi2);
    treeInfo->SetBranchAddress("isUnconditional",   &isConditional);
    treeInfo->SetBranchAddress("fitType",   &fitType);
    treeInfo->SetBranchAddress("nll",       &nll);

    // double bestFitX = 0;
    // double bestFitY = 0;
    double bestFitNLL = 0;

    for(int i = 0 ; i < treeInfo->GetEntries(); i++)
    {
        treeInfo->GetEntry(i);
        if(!(fitType > 0.5 && fitType < 1.5)) continue;
        if(isConditional > 0.5)
        {
            // bestFitX = poi1;
            // bestFitY = poi2;
            bestFitNLL = nll;
            break;
        }
    }

    double sumVarConditional = 0;
    int    countVarConditional = 0; 
    double averVarConditional = 0;

    bool hasUnparmUnconditional = (treeInfo->GetEntries("fitType == 2")>0 ? 1:0);

    for(int i = 0 ; i < treeInfo->GetEntries(); i++)
    {
        treeInfo->GetEntry(i);
        if(hasUnparmUnconditional)
        {
            if(fitType > 1.5 && fitType < 2.5)
            {
                countVarConditional++;
                sumVarConditional  += nll;
            }
        }
        else
        {
            if(fitType > 0.5 && fitType < 1.5)
            {
                countVarConditional++;
                sumVarConditional  += nll;
            }
        }
    }
    averVarConditional = sumVarConditional/countVarConditional;



    return bestFitNLL - averVarConditional;
}


bool cmdline(int argc, char** argv, map<TString,TString>& opts)
{
    opts.clear();
   
    // defaults
    opts["fileName"]        = "asimovScan.root";
    opts["toyFileName"]     = "";
    opts["baseName"]        = "Empty";
    opts["plotNP"]          = "false";
    opts["plotDir"]         = "Plots";
    opts["nDOF"]            = "-1";
    opts["nPOI"]            = "-1";
    opts["forceFitType"]    = "false";

    for (int i=1;i<argc;i++) {
       
        string opt=argv[i];
       
        if (opt=="--help") {
            cout<<"--fileName       : path to ttree containing the information"<<endl;
            // cout<<"--label          : Label for the legend for the files"<<endl;
            // cout<<"--outFile        : Name for the output file"<<endl;
            // cout<<"--doXS           : do XS poi"<<endl;
            // cout<<"--doNs           : do Ns poi"<<endl;
            // cout<<"--doUpperLim     : true or false"<<endl;
            // cout<<"--plotNP     : true or false"<<endl;
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


vector<TPolyLine*> getSMEllipse(std::map<TString, TString> fileInfo, TCanvas* c1)
{

    std::map<TString, float> uncert;
    uncert["mu_ggF"]                = 0.08/1.17;
    uncert["mu_VBF"]                = 0.002/0.0920;
    uncert["mu_VH"]                 = (0.0027 + 0.0049)/(2*0.0524);
    uncert["mu_gg2H_0J_PTH_0_10"]   = 0.025/0.176;
    uncert["mu_gg2H_0J_PTH_GT10"]   = 0.04/0.55;


    // return if it finds nothing
    if(uncert.find(fileInfo["poi_1"]) == uncert.end()) return {};
    if(uncert.find(fileInfo["poi_2"]) == uncert.end()) return {};


    double xUncert = uncert.at(fileInfo["poi_1"]);
    double yUncert = uncert.at(fileInfo["poi_2"]);


    vector<TPolyLine*> list;

    // double start = 0;
    // double end = 360;
    // int step = 2;

    // double size = (end - start)/step;

    // for(int i = 0; i < step-1; i++)
    // {
    //     TEllipse* esm_lo = new TEllipse(1, 1, xUncert, yUncert, start + i * size, start + (i+1) * size);
    //     // 1,1 is the central value, then followed two radius (x, y), the the angle you want to draw. The example here draws two half ellipse with different x radius.
    //     esm_lo->SetFillColor(kAzure + 7);
    //     esm_lo->SetLineColor(kAzure + 7);
    //     esm_lo->SetFillStyle(1000);
    //     list.push_back(esm_lo);
    // }


    const Int_t np = 50;
    static Double_t x[np+3], y[np+3];
 
    Double_t phi1 = 0;
    Double_t phi2 = 360;

    double r1 = xUncert;
    double r2 = yUncert;
    double theta = 0;

    double x1 = 1;
    double y1 = 1;

    //set number of points approximatively proportional to the ellipse circumference
    // Double_t circ = 3.14*(r1+r2)*(phi2-phi1)/360;
    Int_t n = np;
    Double_t angle,dx,dy;
    Double_t dphi = (phi2-phi1)*3.14/(180*n);
    Double_t ct   = TMath::Cos(3.14*theta/180);
    Double_t st   = TMath::Sin(3.14*theta/180);
    for (Int_t i=0;i<=n;i++) {
       angle = phi1*3.14/180 + Double_t(i)*dphi;
       dx    = r1*TMath::Cos(angle);
       dy    = r2*TMath::Sin(angle);
       x[i]  = c1->XtoPad(x1 + dx*ct - dy*st);
       y[i]  = c1->YtoPad(y1 + dx*st + dy*ct);
       // cout<<x[i]<<" "<<y[i]<<endl;
    }

    TPolyLine *pline = new TPolyLine(np,x,y);
    pline->SetFillColor(kAzure + 7);
    pline->SetLineColor(kAzure + 7);
    pline->SetLineWidth(0);
    list.push_back(pline);

    return list;

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







