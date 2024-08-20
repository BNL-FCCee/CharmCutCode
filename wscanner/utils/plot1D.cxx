#include <stdio.h>
#include <stddef.h>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
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
#include "commonCode.h"
#include <nlohmann/json.hpp>
#include "AtlasLabels.C"
#include "AtlasStyle.C"
#include "InterpolatedGraph.h"
#include <nlohmann/json.hpp>

using namespace std;
 
// Global vars
map<TString,TString> opts;
map<TString,TString> metaData;


// // Functions
bool cmdline(int argc, char** argv, map<TString,TString>& opts);
TString getBaseName(TString pathName);
void setPrettyStuff();
map<double, double> readNLL(TString varName, TTree* tree, std::map<TString, TString>& labelInfo, TString plotDir);


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


    // Legend labels
    vector<TString> labelListVect = tokenizeStr(opts["label"],",");
    if(!opts["label"].EqualTo("Empty"))
    {
        if(fileNameList.size() != metadataVect.size()) 
        {
            cout << "You're label list and file list are not the same size, fix this.." << endl;
            return 1;
        }
    }
    map<int, TH1F*> histList;
    map<int, TGraph*> graphList;
    map<int, double> outMinValXList;
    map<int, double> outLeftErrList;
    map<int, double> outRightErrList;
    map<int, double> outLeft2sigErrList;
    map<int, double> outRight2sigErrList;

    double minXF = 100000000;
    double maxXF = -1000000000;
    double minYF = 10000000000000;
    double maxYF = -1000000000000;

    TString baseName = "";
    
    for (size_t i = 0; i < fileNameList.size(); i++)
    {
        cout<<"File "<<fileNameList[i]<<endl;

        std::map<TString, TString> metadataMap = metadataVect.at(i);

        baseName = getBaseName(fileNameList[i]);

        TString directoryName = opts["plotDir"] + "/" + baseName;
        system("mkdir -vp " + directoryName);

        // Open the file
        TChain *chain = new TChain("results");
        chain->Add(fileNameList[i]);

        // Read the NLL into a map
        map<double,double> NLL = readNLL("nll", chain, metadataMap, directoryName);
        // fixOffset(NLL);
        
        TGraph* g_nll = get1DGraph(NLL);
        g_nll->SetLineColor(kBlack);
        g_nll->SetLineWidth(2);
        g_nll->SetMarkerSize(0);

        double minX = TMath::MinElement(g_nll->GetN(),g_nll->GetX());
        double maxX = TMath::MaxElement(g_nll->GetN(),g_nll->GetX());

        double outMinValX = 0;
        double outLeftErr = 0;
        double outRightErr = 0;

        // Print info
        double outLeft2sigErr  = 0;
        double outRight2sigErr = 0;
        printFitInfo(NLL, outMinValX, outLeftErr, outRightErr, outLeft2sigErr, outRight2sigErr);

        // Setup the axis
        TH1F* hist = new TH1F("axis","axis", 10, minX, maxX);
        double minY = (TMath::MinElement(g_nll->GetN(),g_nll->GetY()));
        double maxY = (TMath::MaxElement(g_nll->GetN(),g_nll->GetY()) * 1.5);
        //hist->SetMaximum(TMath::MaxElement(g_nll->GetN(),g_nll->GetY()) * 0.1);
        if(minX < minXF) minXF = minX;  
        if(minY < minYF) minYF = minY;  
        if(maxX > maxXF) maxXF = maxX;  
        if(maxY > maxYF) maxYF = maxY;  
        
        TString xLabel = plotConfig["poiNameMap"][metadataMap["poi"].Data()];

        hist->GetXaxis()->SetTitle(xLabel);
        hist->GetYaxis()->SetTitle("-2#Deltaln(L)");

        graphList[i]           = g_nll;
        histList[i]            = hist;
        outMinValXList[i]      = outMinValX;
        outLeftErrList[i]      = outLeftErr;
        outRightErrList[i]     = outRightErr;
        outLeft2sigErrList[i]  = outLeft2sigErr;
        outRight2sigErrList[i] = outRight2sigErr;
        delete chain;
    }

    if(opts["plotNP"].Contains("true")) minYF = -2;

    TH1F* axisHist = new TH1F("axis","axis", 10, minXF, maxXF);    
    axisHist->SetMaximum(maxYF*1.05);
    axisHist->SetMinimum(minYF);

    if(plotConfig["plotStyle"] == "PaperNominal")  axisHist->SetMaximum(maxYF*0.95);



    axisHist->GetYaxis()->SetTitle("-2ln(#lambda)");
    for(auto hist: histList)
    {
        axisHist->GetXaxis()->SetTitle(hist.second->GetXaxis()->GetTitle());
    }
    axisHist->GetXaxis()->SetTitleSize(0.8*axisHist->GetXaxis()->GetTitleSize());
    axisHist->GetXaxis()->SetTitleOffset(1.25*axisHist->GetXaxis()->GetTitleOffset());
    axisHist->GetXaxis()->SetLabelOffset(2.5*axisHist->GetXaxis()->GetLabelOffset());
    if(fabs(maxXF - minXF) < 0.01)     axisHist->GetXaxis()->SetNdivisions(505);
    if(fabs(maxXF - minXF) < 0.0005)    axisHist->GetXaxis()->SetNdivisions(505);
    if(fabs(maxXF - minXF) < 0.00005)   axisHist->GetXaxis()->SetNdivisions(505);

    // format the plots
    int numDraw = 1754;

    int counter = 0;
    // if you can find the same number of stat and sys then we assume the user gave a stat,sys alternation
    // this is a silly hack for now
    int statCounter = 0;
    int sysCounter = 0;
    for(auto file:fileNameList)
    {
        if( file.Contains("stat"))      statCounter++;
        else if(file.Contains("sys"))   sysCounter++;
    }


    PlottingInfo plottingInfo = parsePlottingInfo(fileNameList);

    for(auto graph: graphList)
    {
        std::map<TString, TString> metadataMap = metadataVect.at(counter);

        graph.second->SetLineWidth(3);
        // set colour
        if(plottingInfo.hasMultipleFolder) 
        {
            graph.second->SetLineColor(numDraw);
            numDraw++;
        }
        else
        {
            if(metadataMap["dataType"].Contains("data")) graph.second->SetLineColor(kBlack);
            else graph.second->SetLineColor(kAzure+7);
        }


        // set the line style
        // set dotted line for stat only, only if there is a sys scan
        if(plottingInfo.hasStatSys) 
        {
            if(metadataMap["scanType"].Contains("statOnly")) graph.second->SetLineStyle(2);
            if(metadataMap["scanType"].Contains("modTheory")) graph.second->SetLineStyle(3);
        }
        
        counter++;
    }



    // NP list
    map<TString, TGraph*> NPgraphList;  

    if(opts["plotNP"].Contains("true"))
    {
        // Open the file
        TChain *chain = new TChain("results");
        chain->Add(fileNameList[0]);

        auto NPList = getVariableList(chain);

        for(const auto& currNP:NPList)
        {
            // Read the NLL into a map
            map<double,double> NLL = read1DVar(currNP, chain);

            TGraph* g_nll = get1DGraph(NLL);
            g_nll->SetLineColor(kBlack);
            g_nll->SetLineWidth(2);
            g_nll->SetMarkerSize(0);

            double minY = TMath::MinElement(g_nll->GetN(),g_nll->GetY());
            double maxY = TMath::MaxElement(g_nll->GetN(),g_nll->GetY());
            
            double cutVal = 0.2;
            if(!(fabs(minY) > cutVal || fabs(maxY) > cutVal)) continue;
            
            NPgraphList[currNP] = g_nll;
        }
        int numColour = 1756;
        for(auto graph: NPgraphList)
        {
            graph.second->SetLineColor(numColour+1);
            graph.second->SetLineWidth(2);
            numColour++;
        }
    }

    TH1D* toyHist = NULL;

    if(opts["toyFileName"].Length() > 0)
    {
        cout<<"Doing toys "<<endl;
        TFile* toyFile = TFile::Open(opts["toyFileName"]);
        TTree* toyTree = (TTree*) toyFile->Get("results");
        toyHist = new TH1D("toyHist", "toyHist", 25, minXF, maxXF);

        toyTree->Draw(metadataVect[0]["poi"] + ">>toyHist", "(isUnconditional == 0)");
        toyHist->Scale(maxYF/(1.5*toyHist->GetMaximum()));
    }

    TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 600, 600);

    TLegend *elLeg =  new TLegend (0.6, 0.76, 0.90, 0.925);
    elLeg->SetFillColor(0);
    elLeg->SetBorderSize(0);
    elLeg->SetTextFont(42);
    elLeg->SetTextSize(0.0275);

    // draw the canvas
    axisHist->Draw();

    if(toyHist) toyHist->Draw("histsame");

    // Fancy title stuff
    double leftDist = 0.19;

    numDraw = 0;
    int numCol = 0;

    counter = 0;
    TString saveName = "";

    for(auto graph: graphList)
    {
        std::map<TString, TString> medataDataMap = metadataVect.at(counter);
        saveName = opts["plotDir"] + "/" + baseName + "/" + medataDataMap.at("poi") + "." + plotConfig["plotExtension"];
        TString fancyLegName = getFancyLegname(medataDataMap.at("label"));

        if(!opts["label"].EqualTo("Empty"))
        {
            fancyLegName = getFancyLegname(labelListVect.at(counter));
        }

        if(plotConfig["plotStyle"] == "PaperNominal")
        {
            fancyLegName = fancyLegName.ReplaceAll("-Sys", "").ReplaceAll("Obs", "Observed");
        }

        double spaceDivisor     = 1;
        double fontScaleDivisor = 1.;
        double widthScale       = 0.35;
        int maxNumDraw          = 2;
        if (plotConfig["analType"] == "CP") {
            if(graphList.size() > 2) {
                spaceDivisor     = 1.5;
                fontScaleDivisor = 1.1;
                maxNumDraw       = 3;
                widthScale       = 0.5;
            }
        }
        else if(graphList.size() > 4) {
            spaceDivisor     = 1.5;
            fontScaleDivisor = 1.1;
        }
        

        if(opts["betterInterp"].Contains("true"))
        {
            InterpolatedGraph f (graph.second, 0.0, 3, 0, false);
            TF1* f1= new TF1 (graph.second->GetName(), f, TMath::MinElement(graph.second->GetN(),graph.second->GetX()), TMath::MaxElement(graph.second->GetN(),graph.second->GetX()), 0);
            f1->SetNpx(100);
            f1->SetLineColor(graph.second->GetLineColor());
            f1->SetLineStyle(graph.second->GetLineStyle());
            f1->SetMarkerStyle(graph.second->GetMarkerStyle());
            f1->SetMarkerSize(graph.second->GetMarkerSize());
            f1->SetMarkerColor(graph.second->GetMarkerColor());
            f1->Draw("same");
        
            // graph.second->SetMarkerSize(kBlack);
            // graph.second->SetMarkerColor(1);
            // graph.second->Draw("psame");
        }
        else
        {
            graph.second->Draw("same");
        }
         


        elLeg->AddEntry(graph.second, fancyLegName , "L");   //


        if(plotConfig["plotStyle"] == "Nominal")
        {
            std::ostringstream outMinValXStr;
            TLatex* fitXInfo =  0;

            int precisionToDisplay = 3;
            if(fabs(maxXF - minXF) < 0.1) precisionToDisplay = 4;
            if(fabs(maxXF - minXF) < 0.01) precisionToDisplay = 5;
            if(fabs(maxXF - minXF) < 0.001) precisionToDisplay = 6;
            if(fabs(maxXF - minXF) < 0.0001) precisionToDisplay = 7;
            if(opts["doUpperLim"].Contains("true"))
            {
                outMinValXStr << std::setprecision(precisionToDisplay) << fixed <<" < "<< outMinValXList[graph.first] + outRightErrList[graph.first];
                fitXInfo = new TLatex (leftDist + widthScale*numCol/spaceDivisor, 0.71 - 0.04*numDraw, TString(fancyLegName+": "+outMinValXStr.str()));
            }
            else
            {
                outMinValXStr << std::setprecision(precisionToDisplay) << fixed << outMinValXList[graph.first];
                if(plotConfig["do2sigErr"])
                {
                    outMinValXStr << "^{+" << outRightErrList[graph.first] << "\ +" << outRight2sigErrList[graph.first] << "}";
                    outMinValXStr << "_{" << outLeftErrList[graph.first]  << "\ " << outLeft2sigErrList[graph.first] << "}";
                }
                else {
                    outMinValXStr << "^{+" << outRightErrList[graph.first] << "}";
                    outMinValXStr << "_{" << outLeftErrList[graph.first] << "}";
                }
                
                fitXInfo = new TLatex (leftDist + widthScale*numCol/spaceDivisor, 0.71 - 0.04*numDraw, TString(fancyLegName+": "+outMinValXStr.str()));
            }
            fitXInfo->SetNDC();
            fitXInfo->SetTextSize(0.025/fontScaleDivisor);
            fitXInfo->SetTextFont(42);
            fitXInfo->Draw();
            numDraw++;
            if(numDraw >= maxNumDraw)
            {
                numDraw = 0;
                numCol ++;
            }
        }
        counter++;

    }
    if(opts["plotNP"].Contains("true"))
    {
        elLeg->SetTextSize(0.0175);        
        for(auto graph: NPgraphList)
        {
            graph.second->Draw("same");
            elLeg->AddEntry(graph.second, graph.first , "L");   
            
        }
    }
    if(toyHist) elLeg->AddEntry(toyHist, "Toys", "L");


    elLeg->Draw();

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

    // if((labelsList.at(0).at("workspaceType").EqualTo("XS") || labelsList.at(0).at("workspaceType").EqualTo("mu")) && (plotConfig["plotStyle"] == "PaperNominal"))
    // {
    //     TString rapidityStr = "|y_{H}| < 2.5";
    //     TLatex* rapInfo = new TLatex (leftDist, 0.72, rapidityStr);
    //     rapInfo->SetNDC();
    //     rapInfo->SetTextSize(0.0275);
    //     rapInfo->SetTextFont(42);
    //     rapInfo->Draw();
    // }



    // Cosmetics
    int lineColour = kGray + 3;
    TLine* oneSigmaLine = new TLine(minXF, 1, maxXF, 1);
    oneSigmaLine->SetLineColor(lineColour);
    oneSigmaLine->SetLineStyle(2);
    oneSigmaLine->SetLineWidth(2);

    TLine* twoSigmaLine = new TLine(minXF, 4, maxXF, 4);
    twoSigmaLine->SetLineColor(lineColour);
    twoSigmaLine->SetLineStyle(2);
    twoSigmaLine->SetLineWidth(2);


    double labelPos = (maxXF - minXF) * 0.925 + minXF;        
    TLatex* oneSigmaText = new TLatex (labelPos, 1.2, "1#sigma");
    oneSigmaText->SetTextSize(0.025);
    oneSigmaText->SetTextFont(42);
    oneSigmaText->SetTextColor(lineColour);
    oneSigmaText->Draw();

    TLatex* twoSigmaText = new TLatex (labelPos, 4.2, "2#sigma");
    twoSigmaText->SetTextSize(0.025);
    twoSigmaText->SetTextFont(42);
    twoSigmaText->SetTextColor(lineColour);
    twoSigmaText->Draw();

    oneSigmaLine->Draw();
    twoSigmaLine->Draw();
    

    c1->SaveAs(saveName);



    return 0;
}


bool cmdline(int argc, char** argv, map<TString,TString>& opts)
{
    opts.clear();
   
    // defaults
    opts["fileName"]        = "asimovScan.root";
    opts["baseName"]        = "Empty";
    opts["label"]           = "Empty";
    opts["plotDir"]         = "Plots";
    opts["plotNP"]          = "false";
    opts["betterInterp"]    = "false";
    opts["toyFileName"]     = "";
    

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
}


void setPrettyStuff()
{
    SetAtlasStyle();
    Int_t ci = 1754; // color index
    vector<TColor*> TColors;
    new TColor(ci, 62/255.,    153/255.,    247/255.); //54
    ci++;
    
    new TColor(ci, 254/255., 139/255., 113/255.); //55
    ci++;



    // new TColor(ci, 0.8500,    0.3250,    0.0980); //57
    // ci++;          
    // new TColor(ci, 0,    0.4470,    0.7410); //56
    // ci++;
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


map<double, double> readNLL(TString varName, TTree* treeInfo, std::map<TString, TString>& labelInfo, TString plotDir)
{
    cout<<"Dimension: "<<labelInfo.at("dimension")<<endl;

    if(!labelInfo.at("dimension").EqualTo("1DToys")) return read1DVar(varName, treeInfo);


    map<double, double> outVar;

    double poi, nll, isConditional, toyNumber, fitType, deltaNLL, nToys, poi_generated;
    treeInfo->SetBranchAddress(labelInfo["poi"],     &poi);
    treeInfo->SetBranchAddress("toyNumber",         &toyNumber);
    treeInfo->SetBranchAddress("nll",               &nll);
    treeInfo->SetBranchAddress("deltaNLL",          &deltaNLL);
    treeInfo->SetBranchAddress("isUnconditional",   &isConditional);
    treeInfo->SetBranchAddress("fitType",           &fitType);
    treeInfo->SetBranchAddress("nToys",             &nToys);

    treeInfo->SetBranchAddress("poi_generated",     &poi_generated);
    // Hist limits
    double histLimitsMax = -std::numeric_limits<double>::infinity();
    double histLimitsMin = std::numeric_limits<double>::infinity();


    // Calculate the average of the unconditional NLL
    if(varName.EqualTo("nll"))
    {
        TString poi_val;
        TString histName;


        // For storing stuff
        std::map<TString, std::vector<double>> delNLLHistMap;
        std::map<TString, double> datasetNLL;
        std::map<TString, double> xVal;
        std::map<TString, double> nomNLL;

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

        TString subDirectoryName = plotDir + "/toyPlots/";
        system("mkdir -vp " + subDirectoryName);

        Long64_t nToysInTree = treeInfo->GetEntries(); 
        for(Long64_t i = 0 ; i < nToysInTree; i++)
        {
            treeInfo->GetEntry(i);

            if(fitType == 1 || fitType == 2) continue;

            poi_val = Form("%.7f", poi_generated);
            histName = poi_val; 


            xVal[histName] = poi_generated;

            /// add a vector if not already there
            auto empl = delNLLHistMap.emplace(std::make_pair(histName, std::vector<double>{}));
            /// reserve sufficient space to avoid re-allocation
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
                /// re-use iterator to save lookup.
                /// Veto negative nll values here. 
                if (!(deltaNLL < 0)) empl.first->second.push_back(deltaNLL);
                continue;
            }

        }

        // Loop over and fill the actual hist
        for(auto& hist:delNLLHistMap)
        {
            // sort the toys in the vec
            std::sort(hist.second.begin(),hist.second.end());
            double perExc = plotToy(hist.second, datasetNLL.at(hist.first), subDirectoryName, hist.first);

            double calibratedNLL = TMath::ChisquareQuantile(1-perExc, 1);
            double poi1ValGraph = xVal.at(hist.first);

            cout << "nToys perExc: " << perExc<<" calibratedNLL: "<<calibratedNLL << " poi1: "<< poi1ValGraph << endl;
            outVar[poi1ValGraph] = calibratedNLL;
        }
    }


    return outVar;



}






