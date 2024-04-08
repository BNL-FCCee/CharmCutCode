#ifndef COMMONCODE_H
#define COMMONCODE_H

#include <stdio.h>
#include <stddef.h>
#include <map>
#include <vector>
#include <string>
#include <regex>

// ROOT include
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TH2.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMatrixDSym.h>
#include <TGraph.h>
#include <TGraph.h>
#include <TObjString.h>
#include "TColor.h"
#include "TLine.h"
#include "TLatex.h"

using namespace std;

std::vector<TString> tokenizeStr(TString str, TString key);

template<class T>
void bookTree(TTree* currTree, map<TString, T>& outVars)
{
    for (auto it = outVars.begin(); it != outVars.end(); ++it)
    {
        TString varName = it->first;       
        if (std::is_same<T, float>::value) currTree->Branch(varName, &it->second, varName+"/F");
        if (std::is_same<T, double>::value) currTree->Branch(varName, &it->second, varName+"/D");
        if (std::is_same<T, int>::value) currTree->Branch(varName, &it->second, varName+"/I");
        if (std::is_same<T, TString>::value) currTree->Branch(varName, &it->second);
    }
}


std::map<TString, TString> readMetadata(TTree* tree);
std::vector<std::map<TString, TString>>  getMetadata(std::vector<TString> fileNameList);
TString getFancyLegname(TString name);

vector<TString> getVariableList(TTree *tree);
// reads the var as a function of the poi
map<double,double> read1DVar(TString varName, TTree* tree);

// // Subtracts the minimum from the variable
void fixOffset(map<double,double>& varInfo);

TGraph* get1DGraph(map<double,double>& varInfo);

void printFitInfo(map<double,double>& NLLVar, double& outMinValX, double& outLeftErr, double& outRightErr,
                  double& outLeft2SigErr, double& outRight2sigErr, bool doBetterError = false);


struct PlottingInfo 
{
	bool hasStatSys;
	bool hasMultipleFolder;
};

PlottingInfo parsePlottingInfo(std::vector<TString> fileNameList);

bool doesExist(const std::string& name) ;

vector<TString> splitString(TString string, TString delimiter);


TString joinList(vector<TString> arr, TString delimiter);


void process_mem_usage(double& vm_usage, double& resident_set);

void drawToy(TH1D & toyHist, double nomNLLVal, double percExcluded, TString subDirectoryName, TString histName);
double plotToy(const std::vector<double>&  toyResults, double nomNLLVal, TString subDirectoryName, TString histName);

// Update the input string.
void autoExpandEnvironmentVariables( std::string & text );

// Leave input alone and return new string.
std::string expandEnvironmentVariables( const std::string & input );

#endif




