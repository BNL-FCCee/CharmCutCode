#include <stdio.h>
#include <stddef.h>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <limits>
#include <math.h> 
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
#include <TGaxis.h>
#include "commonCode.h"
#include "AtlasLabels.C"
#include "AtlasStyle.C"
#include "TGraphAsymmErrors.h"
using namespace std;

struct pullInformation 
{
  double cenVal;
  double uprErr;
  double dwnErr;
};


bool cmdline(int argc, char** argv, map<TString,TString>& opts);
void setPrettyStuff();
void getVariableTree(TTree *tree);
void setupReadTree(TTree *tree);
TString getPrettyName(TString name);
TString getBaseName(TString pathName);
void fillMetaMap();
void getUniqueNames();
void fillPullInfo(TTree *tree);
void plot();

map<TString,TString> opts;
std::map<TString, TString> metaData;
std::map<TString, double>       doubleVarMap;
std::map<TString, TString*>     TStringVarMap;
std::vector<TString> uniqueNames;
std::map<TString, pullInformation> pullInfoMap;

int main(int argc, char** argv)
{
    setPrettyStuff();

    if (!cmdline(argc,argv,opts)) return 0;

    fillMetaMap();
    

    TFile *fileName = TFile::Open(opts["fileName"], "r");
    TTree *tree = (TTree*)fileName->Get("results");

    getVariableTree(tree);

    setupReadTree(tree);

    getUniqueNames();

    fillPullInfo(tree);

    plot();

    // for(auto x:pullInfoMap) cout << x.first << "\t" << x.second.cenVal <<"\t" << x.second.uprErr << endl;

    return 0;

}

void plot()
{

    int numOfNPs = 20;
    int counter = 0;
    int globalCounter = 0;
    std::vector<double> xVal;
    std::vector<double> xLo;
    std::vector<double> xHi;
    std::vector<double> yVal;
    std::vector<TString> labelName;

    int offset = 0;

    TH2D* frameHistogram = new TH2D("frame", "frame",  6, -3, 3, numOfNPs+offset, 0,numOfNPs+offset);
    frameHistogram->GetYaxis()->SetLabelSize(0.04);
    frameHistogram->GetXaxis()->SetLabelSize(0);
    frameHistogram->GetXaxis()->SetTickSize(0);
    frameHistogram->GetYaxis()->SetTickSize(0);

    for(const auto& info:pullInfoMap)
    {
        labelName.push_back(getPrettyName(info.first));
        xVal.push_back(info.second.cenVal);
        xLo.push_back(fabs(info.second.uprErr));
        xHi.push_back(fabs(info.second.dwnErr));
        yVal.push_back(counter + 0.5);

        counter++;

        if(counter == numOfNPs)
        {
            TGraphAsymmErrors* centralValGraph = new TGraphAsymmErrors(yVal.size(), &xVal[0], &yVal[0], &xLo[0], &xHi[0], NULL, NULL);
            centralValGraph->SetLineColor(kBlack);
            centralValGraph->SetMarkerColor(kBlack);
            centralValGraph->SetMarkerStyle(20);
            centralValGraph->SetLineStyle(1);
            centralValGraph->SetLineWidth(2);
            centralValGraph->SetMarkerSize(2);
            centralValGraph->GetXaxis()->SetTitleOffset(1.2);  

            for(int bin = offset+1; bin < frameHistogram->GetYaxis()->GetNbins() + 1; bin++)
            {
                TString niceName = getPrettyName(labelName.at(bin - 1) );
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
            axis_theta->SetTitleSize(0.045);
            axis_theta->SetNdivisions(505);

            TGaxis *axis_ticks_right = new TGaxis(frameHistogram->GetXaxis()->GetXmin(), frameHistogram->GetYaxis()->GetXmin(),frameHistogram->GetXaxis()->GetXmin(), frameHistogram->GetYaxis()->GetXmax()-offset-1, 
                frameHistogram->GetYaxis()->GetXmin(), frameHistogram->GetYaxis()->GetXmax()-offset-1);
            axis_ticks_right->ImportAxisAttributes(frameHistogram->GetYaxis());
            axis_ticks_right->SetLabelSize(0);
            axis_ticks_right->SetTitleSize(0.045);


            TGaxis *axis_ticks_left = new TGaxis(frameHistogram->GetXaxis()->GetXmax(), frameHistogram->GetYaxis()->GetXmin(),frameHistogram->GetXaxis()->GetXmax(), frameHistogram->GetYaxis()->GetXmax()-offset-1, 
                frameHistogram->GetYaxis()->GetXmin(), frameHistogram->GetYaxis()->GetXmax()-offset-1);
            axis_ticks_left->ImportAxisAttributes(frameHistogram->GetYaxis());
            axis_ticks_left->SetLabelSize(0);
            axis_ticks_left->SetOption("+");
            axis_ticks_left->SetTitleSize(0.045);
            gPad->RedrawAxis();            

            frameHistogram->Draw();
            
            // axis_poi->Draw();

                

            auto errBandTwoSigma = new TGraphAsymmErrors(1, &vector<double>{0}[0] , &vector<double>{(frameHistogram->GetYaxis()->GetXmax()-1)/2 }[0], &vector<double>{2}[0] ,&vector<double>{2}[0], &vector<double>{(frameHistogram->GetYaxis()->GetXmax()-1)/2.}[0], &vector<double>{(frameHistogram->GetYaxis()->GetXmax()-1)/2. + 1}[0] );
            errBandTwoSigma->SetFillColor(38);
            errBandTwoSigma->SetMarkerSize(0);
            errBandTwoSigma->Draw("2");

            auto errBandOneSigma = new TGraphAsymmErrors(1, &vector<double>{0}[0] , &vector<double>{(frameHistogram->GetYaxis()->GetXmax()-1)/2 }[0], &vector<double>{1}[0] ,&vector<double>{1}[0], &vector<double>{(frameHistogram->GetYaxis()->GetXmax()-1)/2.}[0], &vector<double>{(frameHistogram->GetYaxis()->GetXmax()-1)/2. + 1}[0] );
            errBandOneSigma->SetFillColor(18);
            errBandOneSigma->SetMarkerSize(0);
            errBandOneSigma->Draw("2");

            TLine l;
            l.SetLineWidth(2);
            l.SetLineColor(kGray + 2);
            l.SetLineStyle(2);
            l.DrawLine( 0. , 0.,  0. , frameHistogram->GetYaxis()->GetXmax());

            centralValGraph->Draw("PSame");
            axis_theta->Draw();
            axis_ticks_right->Draw();
            axis_ticks_left->Draw();
            
            float labelPosX = 0.06;
            float labelPosY = 0.94;

            ATLASLabel(labelPosX,labelPosY,"Internal",1, 0.8);

            TString channelStr = "Combination";

            TLatex energyLumi;
            energyLumi.SetNDC();
            energyLumi.SetTextSize(0.027);
            energyLumi.DrawLatex(labelPosX,labelPosY-0.035,"#sqrt{s} = 13 TeV, 36.1, 80, 139 fb^{-1}");


            TString baseName = getBaseName(opts["fileName"]);
            TString directoryName = opts["plotDir"] + "/"+baseName;
            system("mkdir -vp " + directoryName);
            TString dataStr = metaData["doData"];
            dataStr.ToLower();
            if(dataStr.Contains("true")) dataStr = "data";
            else dataStr = "asimov";
            TString saveName = directoryName + "/" + "impact_" + dataStr + "_" + Form("%i", globalCounter) + ".eps";
            c1->SaveAs(saveName);


            xVal.clear();
            xLo.clear();
            xHi.clear();
            yVal.clear();
            labelName.clear();
            counter = 0;
            globalCounter++;
        }

    }


    // TGraphAsymmErrors* centralValGraph = new TGraphAsymmErrors(yVal.size(), &xPOIVal[0], &yVal[0], &xPOIErrLo[0], &xPOIErrHi[0], NULL, NULL);
    // centralValGraph->SetLineColor(kBlack);
    // centralValGraph->SetMarkerColor(kBlack);
    // centralValGraph->SetMarkerStyle(20);
    // centralValGraph->SetLineStyle(1);
    // centralValGraph->SetLineWidth(2);
    // centralValGraph->SetMarkerSize(2);
    // centralValGraph->GetXaxis()->SetTitleOffset(1.2);    



}

void fillPullInfo(TTree *tree)
{
    for(int i = 0; i < tree->GetEntries(); i++)
    {
        tree->GetEntry(i);  

        for(const auto& paramName: uniqueNames)
        {
            pullInfoMap[paramName] = pullInformation();
            pullInfoMap[paramName].cenVal = doubleVarMap[paramName];
            pullInfoMap[paramName].uprErr = doubleVarMap[paramName + "_uprErr"];
            pullInfoMap[paramName].dwnErr = doubleVarMap[paramName + "_dwnErr"];

        }

    }

}


void getUniqueNames()
{

    for(const auto& var:doubleVarMap)
    {
        if( var.first.Contains("_uprErr") || 
            var.first.Contains("_dwnErr") || 
            var.first.EqualTo("fitType") || 
            var.first.EqualTo("isUnconditional") || 
            var.first.EqualTo("nll") || 
            var.first.EqualTo("status")
            ) 
        {
            continue;
        }

        else
        {
            uniqueNames.push_back(var.first);
        }
        
    }
      
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
    opts["plotDir"]     = "Plots";


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
        if(opt == "doPrefit")  {opts["doPrefit"] = "true"; continue;}
        if(opt == "addNums")  {opts["addNums"] = "true"; continue;}

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
