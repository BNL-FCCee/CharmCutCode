#include <stdio.h>
#include <stddef.h>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>

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
#include <TF1.h>
#include <TLine.h>
#include <TGraph.h>
#include <TColor.h>
#include <TLegend.h>
#include <stdio.h>
#include "commonCode.h"
#include "AtlasLabels.C"
#include "AtlasStyle.C"
#include "InterpolatedGraph.h"

using namespace std;

// Global vars
map<TString,TString> opts;
map<TString,TString> metaData;


// // Functions
bool cmdline(int argc, char** argv, map<TString,TString>& opts);
TString getBaseName(TString pathName);
void setPrettyStuff();
vector<TString> getVarName(TTree* tree);

int main(int argc, char** argv)
{
    setPrettyStuff();

    if (!cmdline(argc,argv,opts)) return 0;

    TString baseName = getBaseName(opts["fileName"]);

    TString directoryName = opts["plotDir"] + "/" + baseName ; 
    
    system("mkdir -vp " + directoryName);

    // Open the file
    TChain* basechain = new TChain("results");
    basechain->Add(opts["fileName"]);

    // get list of variable names
    std::vector<TString> varNames = getVarName(basechain);


    TString cutList = "(isUnconditional == 0) && (status == 0) && (deltaNLL < 500) && (fitType >= 0)";
    TTree* chain = basechain->CopyTree(cutList);
    for(unsigned int i = 0; i < varNames.size(); i++)
    {
        double lwrLim = chain->GetMinimum(varNames.at(i));
        double uprLim = chain->GetMaximum(varNames.at(i));
        int nBins = 500;

        // double epsilonVar = (uprLim - lwrLim)/nBins;
        double epsilonVar = 0;


        TH1D *toyHist = new TH1D("toyHist", "toyHist", nBins + 2, lwrLim - epsilonVar, uprLim + epsilonVar);

        chain->Draw(varNames.at(i) + ">> toyHist", cutList);


        TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 600, 600);
        toyHist->GetXaxis()->SetTitle(varNames.at(i));
        toyHist->SetMaximum(toyHist->GetMaximum()*1.3);
        toyHist->SetLineWidth(2);
        toyHist->SetLineColor(kBlack);
        toyHist->Draw("hist");

        // Fancy title stuff
        double leftDist = 0.19;

        ATLASLabel(leftDist, 0.875, "Internal", 1);
        TString channelStr = "H #rightarrow ZZ* #rightarrow 4l";
        TLatex* tMain = new TLatex (leftDist, 0.81, channelStr);
        tMain->SetNDC();
        tMain->SetTextSize(0.0425);
        tMain->SetTextFont(42);
        tMain->Draw();

        TString lumiString = "13 TeV, 139 fb^{-1}";
        TLatex* lumInfo = new TLatex (leftDist, 0.76, lumiString);
        lumInfo->SetNDC();
        lumInfo->SetTextSize(0.0325);
        lumInfo->SetTextFont(42);
        lumInfo->Draw();

        double rightDist = 0.7;
        double topDist = 0.86;
        TString numToys = Form("Toys: %.2f", toyHist->GetEntries());
        TLatex* numToysInfo = new TLatex (rightDist, topDist, numToys);
        numToysInfo->SetNDC();
        numToysInfo->SetTextSize(0.0325);
        numToysInfo->SetTextFont(42);
        numToysInfo->Draw();

        TString mean = Form("Mean: %.3f", toyHist->GetMean() );
        TLatex* meanInfo = new TLatex (rightDist, topDist - 0.04, mean);
        meanInfo->SetNDC();
        meanInfo->SetTextSize(0.0325);
        meanInfo->SetTextFont(42);
        meanInfo->Draw();

        TString sigma = Form("Width: %.3f", toyHist->GetStdDev() );
        TLatex* sigmaInfo = new TLatex (rightDist, topDist - 0.08, sigma);
        sigmaInfo->SetNDC();
        sigmaInfo->SetTextSize(0.0325);
        sigmaInfo->SetTextFont(42);
        sigmaInfo->Draw();



        if(varNames.at(i).EqualTo("deltaNLL"))
        {
            // Fit the distribution with chi square
            TF1* f1 = new TF1("f1","[1]*ROOT::Math::chisquared_pdf(x,[0], 0)", lwrLim, uprLim);
            f1->SetParameter(0,0.66);
            f1->SetParameter(1,toyHist->Integral());

            toyHist->Fit(f1, "EM");
            f1->SetLineColor(kAzure+7);
            f1->Draw("same");

            TString NDOF = Form("NDOF: %.2f", f1->GetParameter(0) );
            TLatex* NDOFInfo = new TLatex (rightDist, topDist - 0.12, NDOF);
            NDOFInfo->SetNDC();
            NDOFInfo->SetTextSize(0.0325);
            NDOFInfo->SetTextFont(42);
            NDOFInfo->Draw();

        }


        TString saveName = opts["plotDir"] + "/" + baseName + "/" + varNames.at(i) + ".eps";
        c1->SaveAs(saveName);

        delete toyHist;
        delete c1;

    }

    delete chain;

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
        return folderName[1] + "/" + folderName[2];
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

    new TColor(ci, 0,    0.4470,    0.7410); //56
    ci++;
    
    new TColor(ci, 254/255., 139/255., 113/255.); //55
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



vector<TString> getVarName(TTree* tree)
{

    vector<TString> varList;
    TObjArray* listLf = tree->GetListOfLeaves();

    for (Int_t i = 0; i < listLf->GetEntries(); i++)
    {
        TLeaf* lf = (TLeaf*) listLf->At(i);

        TString typeName   = lf->GetTypeName();
        TString titleName  = lf->GetTitle();
        TString varName    = lf->GetName();



        if(typeName.Contains("Int_t") && !typeName.Contains("UInt_t"))
        {
            varList.push_back(varName);
        }
        else if(typeName.Contains("UInt_t"))
        {
            varList.push_back(varName);
        }
        else if(typeName.Contains("Float_t"))
        {
            varList.push_back(varName);
        }
        else if(typeName.Contains("Double_t"))
        {
            varList.push_back(varName);
        }
        else if(typeName.Contains("ULong64_t"))
        {
            varList.push_back(varName);
        }
        else
        {
            cout<<"I have been feed a type that I don't know how to deal with"<<endl;
            cout<<"Type: "<<typeName<<endl;
            exit(1);
        }
    }

    return varList;

}





