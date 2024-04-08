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
#include <limits>
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
#include <nlohmann/json.hpp>
#include "fstream"

using namespace std;

// Global vars
map<TString,TString> opts;
map<TString,TString> metaData;


// // Functions
bool cmdline(int argc, char** argv, map<TString,TString>& opts);
TString getBaseName(TString pathName);

void setPrettyStuff();

void fill2DHist(TH2D* inHist, TTree* treeInfo, std::map<TString, TString> fileInfo, TString varName,TString cFileName);
TMarker* getBestFitPoint(TTree* treeInfo, std::map<TString, TString> fileInfo);


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

    double minXF = std::numeric_limits<double>::infinity();
    double maxXF = -std::numeric_limits<double>::infinity();
    double minYF = std::numeric_limits<double>::infinity();
    double maxYF = -std::numeric_limits<double>::infinity();

    map<TString, TTree*> treeList;
    map<TString, TH2D*>  histList;
    map<TString, TH2D*>  cont95percList;
    map<TString, TH2D*>  cont68percList;
    map<TString, TMarker*>  bestFitMarkerList;


    // Figure out the ranges for the X and Y
    for (size_t i = 0; i < fileNameList.size(); i++)
    {
        TString cFileName = fileNameList[i];
        cout<<"File "<<cFileName<<endl;

        std::map<TString, TString> metadataMap = metadataVect.at(i);

        // make the base name right now
        TString baseName = getBaseName(cFileName);
        TString directoryName = opts["plotDir"] +"/"+baseName;
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


    double contour68[]{1-0.63};
    double contour95[]{1-0.95};
    // double contour3Sigma[]{1-0.997300203936740};
    // double contour4Sigma[]{1-0.999936657516334};
    // double contour5Sigma[]{1-0.999999426696856};

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
        fill2DHist(hist, treeList[cFileName], metadataMap, "nll", cFileName);

        // for(int xBin = 0; xBin < hist->GetNbinsX(); xBin++)
        // {
        //     for(int yBin = 0; yBin < hist->GetNbinsX(); yBin++)
        //     {
        //         cout << "Bin x: " << xBin << " Bin Y: " << yBin << " Entry: " << hist->GetBinContent(xBin+1,yBin+1)<< endl;
        //     }
        // }
        
        histList[cFileName] = hist;

        TString fileNameApp = "raw_";

        histName = opts["plotDir"] + "/" + getBaseName(fileNameList[0]) + "/" + fileNameApp + metadataVect[0]["poi_1"] + "_" + metadataVect[0]["poi_2"] + "." + plotConfig["plotExtension"];

        TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 600, 600);
        hist->SetContour(1000);
        // hist->SetMaximum(5);
        hist->Draw("colz");
        // hist->Draw("sameText");
        c1->SetRightMargin(0.2);
        c1->SaveAs(histName);

        delete c1;



        TH2D* hist_68= (TH2D*)hist->Clone(histName + "68");        
        hist_68->SetContour(1, contour68);
        cont68percList[cFileName] = hist_68;

        TH2D* hist_95= (TH2D*)hist->Clone(histName + "95");        
        hist_95->SetContour(1,contour95);
        cont95percList[cFileName] = hist_95;

        // bestFitMarkerList[cFileName] = getBestFitPoint(treeList[cFileName], metadataMap);

    }
    // format the plots
    int numDraw = 1756;
    for (size_t i = 0; i < fileNameList.size(); i++)
    {
        TString cFileName = fileNameList[i];

        std::map<TString, TString> metadataMap = metadataVect.at(i);

        // cont95percList[cFileName]->SetLineColor(numDraw+1);
        // cont95percList[cFileName]->SetLineWidth(2);

        // cont68percList[cFileName]->SetLineColor(numDraw+1);
        // cont68percList[cFileName]->SetLineWidth(2);

        if(metadataMap["dataType"] == "data") 
        {
            cont95percList[cFileName]->SetLineColor(kBlack); 
            cont68percList[cFileName]->SetLineColor(kBlack);
            // bestFitMarkerList[cFileName]->SetMarkerColor(kBlack);
        }
        else if(metadataMap["scanType"].Contains("statOnly"))
        {
            cont95percList[cFileName]->SetLineColor(kPink+3); 
            cont68percList[cFileName]->SetLineColor(kPink+3);
        }
        else 
        {
            cont95percList[cFileName]->SetLineColor(kAzure+7); 
            cont68percList[cFileName]->SetLineColor(kAzure+7);

            // cont95percList[cFileName + "3sigma"]->SetLineColor(kMagenta);
            // cont95percList[cFileName + "4sigma"]->SetLineColor(kPink + 3);
            // cont95percList[cFileName + "5sigma"]->SetLineColor(kOrange);

            // bestFitMarkerList[cFileName]->SetMarkerColor(kAzure+7);
        }

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
    if(fileNameList[0].Contains("flavourViolation_vectorContact"))
    {
        frame->GetXaxis()->SetTitle("(#varepsilon_{Z#mu_{L}} = #varepsilon_{Z#mu_{R}})");
        frame->GetYaxis()->SetTitle("(#varepsilon_{Ze_{L}} = #varepsilon_{Ze_{R}})");

    }
    else if (fileNameList[0].Contains("flavourViolation_axialContact"))
    {
        frame->GetXaxis()->SetTitle("(#varepsilon_{Z#mu_{L}} = -#varepsilon_{Z#mu_{R}})");
        frame->GetYaxis()->SetTitle("(#varepsilon_{Ze_{L}} = -#varepsilon_{Ze_{R}})");
    }
    else
    {
        TString poiName_1 = plotConfig["poiNameMap"][metadataVect[0]["poi_1"].Data()];
        frame->GetXaxis()->SetTitle(poiName_1);
        TString poiName_2 = plotConfig["poiNameMap"][metadataVect[0]["poi_2"].Data()];
        frame->GetYaxis()->SetTitle(poiName_2);
    }   

    TH2D* toyHist = NULL;

    
    frame->Draw();

    if(toyHist) toyHist->Draw("p");
    for(const auto& hist: cont68percList) hist.second->Draw("cont3 same");
    for(const auto& hist: cont95percList) hist.second->Draw("cont3 same");
    for(const auto& hist: bestFitMarkerList) hist.second->Draw("same");

    TMarker* smPoint = new TMarker(1,1,29);
    smPoint->SetMarkerColor(kAzure + 7);
    smPoint->Draw("same");

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

    TLegend *leg =  new TLegend (0.6, 0.76, 0.90, 0.925);
    leg->SetFillColor(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.0275);

     // Legend
    for(size_t i = 0; i < fileNameList.size(); i++)
    {
        TString toAppend = "";
             if(fileNameList.size() == 1)            toAppend = "";
        else if(metadataVect[i]["dataType"] == "data") toAppend = "Obs ";
        else                                         toAppend = "Exp ";

        if(metadataVect[i]["scanType"].Contains("statOnly")) toAppend = "Stat Only- ";


        // leg->AddEntry(bestFitMarkerList[fileNameList[i]], toAppend + "Best Fit", "p");
        leg->AddEntry(cont68percList[fileNameList[i]], toAppend + "68\% CL.", "l");
        leg->AddEntry(cont95percList[fileNameList[i]], toAppend + "95\% CL.", "l");
    }
    
    leg->AddEntry(smPoint, "SM", "p");
    if(toyHist) leg->AddEntry(toyHist, "Toys", "p");
    leg->Draw();

    bool hasData = false;
    bool hasAsimov = false;
    for(size_t i = 0; i < fileNameList.size(); i++)
    {
        if(metadataVect[i]["dataType"] == "data") hasData = true;
        if(metadataVect[i]["dataType"] == "asimov") hasAsimov = true;
    }

    TString fileNameApp = "";
    if(hasData && !hasAsimov) fileNameApp = "Obs_";
    if(!hasData && hasAsimov) fileNameApp = "Exp_";

    TString saveName = opts["plotDir"] + "/" + getBaseName(fileNameList[0]) + "/" + fileNameApp + metadataVect[0]["poi_1"] + "_" + metadataVect[0]["poi_2"] + ".eps";

    c1->SaveAs(saveName);
    
    return 0;
}

void fill2DHist(TH2D* inHist, TTree* treeInfo, std::map<TString, TString> fileInfo, TString varName, TString cFileName)
{
    double poi1, poi2, nll, isConditional, toyNumber, fitType, deltaNLL, nToys;
    double poi1_generated, poi2_generated;
    treeInfo->SetBranchAddress(fileInfo["poi_1"],   &poi1);
    treeInfo->SetBranchAddress(fileInfo["poi_2"],   &poi2);
    treeInfo->SetBranchAddress("toyNumber",         &toyNumber);
    treeInfo->SetBranchAddress("nll",               &nll);
    treeInfo->SetBranchAddress("deltaNLL",          &deltaNLL);
    treeInfo->SetBranchAddress("isUnconditional",   &isConditional);
    treeInfo->SetBranchAddress("fitType",           &fitType);
    treeInfo->SetBranchAddress("nToys",             &nToys);

    treeInfo->SetBranchAddress("poi_1_generated",   &poi1_generated);
    treeInfo->SetBranchAddress("poi_2_generated",   &poi2_generated);
    // Hist limits
    double histLimitsMax = -std::numeric_limits<double>::infinity();
    double histLimitsMin = std::numeric_limits<double>::infinity();




    // To make sure it doesn't hit the bin edge
    double poi1_offset = fileInfo["poi_1_delta"].Atof()/100;
    double poi2_offset = fileInfo["poi_2_delta"].Atof()/100;


    // Calculate the average of the unconditional NLL
    if(varName.EqualTo("nll"))
    {
        // quick loop of unconditional fits to get the average minimum
        for(int i = 0 ; i < treeInfo->GetEntries(); i++)
        {
            treeInfo->GetEntry(i);
            if(i % 100000 == 0) cout<<i<<" "<<treeInfo->GetEntries()<<endl;

            if(fitType == 1 || fitType == 2 || fitType == 3) continue;

            if(deltaNLL > histLimitsMax) histLimitsMax =  deltaNLL;
            if(deltaNLL < histLimitsMin) histLimitsMin =  deltaNLL;

            if(deltaNLL > histLimitsMax) histLimitsMax =  deltaNLL;
            if(deltaNLL < histLimitsMin) histLimitsMin =  deltaNLL;

        }  

        if(histLimitsMin < 0) histLimitsMin = 0;

        // cout << "minNLL:  " << averVarConditional << endl;
        cout << "histLimitsMin:  " << histLimitsMin << endl;
        cout << "histLimitsMax:  " << histLimitsMax << endl;

        TString baseName = getBaseName(cFileName);        
        TString subDirectoryName = opts["plotDir"] +"/"+baseName + "/toyPlots/";
        system("mkdir -vp " + subDirectoryName);


        TString poi_1_val;
        TString poi_2_val;
        TString histName;


        // For storing stuff
        std::map<TString, std::vector<double>> delNLLHistMap;
        std::map<TString, double> datasetNLL;
        std::map<TString, double> xVal;
        std::map<TString, double> yVal;
        std::map<TString, double> nomNLL;

        Long64_t nToysInTree = treeInfo->GetEntries(); 
        for(int i = 0 ; i < nToysInTree; i++)
        {
            treeInfo->GetEntry(i);

            if(fitType == 1 || fitType == 2) continue;

            poi_1_val = Form("%.4f", poi1_generated);
            poi_2_val = Form("%.4f", poi2_generated);
            histName = poi_1_val + "_" + poi_2_val; 


            xVal[histName] = poi1_generated;
            yVal[histName] = poi2_generated;

            auto empl = delNLLHistMap.emplace(std::make_pair(histName, std::vector<double>{})); 
            if (empl.second) empl.first->second.reserve(nToysInTree); 

            // This is a fit to the actual dataset
            if(fitType == 3)
            {
                datasetNLL[histName] = deltaNLL;
                continue;
            }

            // Fit to toy
            if(fitType == 0)
            {
                if (!(deltaNLL < 0)) empl.first->second.push_back(deltaNLL);
                continue;
            }
        }

        // Loop over and fill the actual hist
        for(auto& hist:delNLLHistMap)
        {
            /// sort the toys by NLL
            std::sort (hist.second.begin(),hist.second.end()); 
            double perExc = plotToy(hist.second, datasetNLL.at(hist.first), subDirectoryName, hist.first);
            double poi1ValGraph = xVal.at(hist.first);
            double poi2ValGraph = yVal.at(hist.first);

            std::cout << "nToys perExc: " << perExc << " poi1: "<< poi1ValGraph << " poi2: " << poi2ValGraph << endl;
            int binX = inHist->GetXaxis()->FindBin(poi1ValGraph + poi1_offset) ;
            int binY = inHist->GetYaxis()->FindBin(poi2ValGraph + poi2_offset) ;
            inHist->SetBinContent(binX, binY, perExc);
        }




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
}



TMarker* getBestFitPoint(TTree* treeInfo, std::map<TString, TString> fileInfo)
{
    cout<<"getBestFitPoint"<<endl;
    cout<<"treeInfo"<<treeInfo<<endl;
    treeInfo->Print();
    cout<<"fileInfo[poi_1]"<<fileInfo["poi_1"]<<endl;
    cout<<"fileInfo[poi_2]"<<fileInfo["poi_2"]<<endl;

    double poi1, poi2, isConditional;
    treeInfo->SetBranchAddress(fileInfo["poi_1"],   &poi1);
    treeInfo->SetBranchAddress(fileInfo["poi_2"],   &poi2);
    treeInfo->SetBranchAddress("isUnconditional",   &isConditional);

    double bestFitX = 0;
    double bestFitY = 0;

    for(int i = 0 ; i < treeInfo->GetEntries(); i++)
    {
        treeInfo->GetEntry(i);
        cout<<"entry i "<<i<<endl;

    cout<<"treeInfo"<<treeInfo<<endl;
    cout<<"[poi_1]"<<poi1<<endl;
    cout<<"[poi_2]"<<poi2<<endl;
    cout<<"[isConditional]"<<isConditional<<endl;

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


bool cmdline(int argc, char** argv, map<TString,TString>& opts)
{
    opts.clear();
   
    // defaults
    opts["fileName"]    = "asimovScan.root";
    opts["toyFileName"] = "";
    opts["baseName"]    = "Empty";
    opts["plotNP"]      = "false";
    opts["plotDir"]     = "Plots";

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







