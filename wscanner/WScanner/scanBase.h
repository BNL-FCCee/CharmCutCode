#ifndef _SCANBASE_H
#define _SCANBASE_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <map>

// ROOT include
#include <TFile.h>
#include <TH2.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TROOT.h>
#include "TMatrixDSym.h"

// ROOFit include
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooArgSet.h"
#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/AsymptoticCalculator.h"
#include "RooMinimizer.h"
#include "RooNLLVar.h"
#include "TStopwatch.h"
#include "RooRandom.h"
#include "RooFitResult.h"
#include "RooNumIntConfig.h"
#include "RooRealSumPdf.h"
#include "RooAddPdf.h"
#ifdef hasRooFitExtension 
    #include "RooFitExtensions/RooStarMomentMorph.h"
#endif

// special includes
#include "log.h"
#include "commonCode.h"


using namespace RooStats;
using namespace RooFit ;

// Base class 
class scanBase 
{
    public:
        scanBase(std::map<TString, TString>& opts);
        virtual ~scanBase();

        // open the file and setup POI and NP ranges
        void run();

        // void runScan() The pure virtual function we will override

        // save to an output tree
        void finalize();

    protected:
        // set output verbosity for rooFit
        void setOutputLevel();

        // settings for minimizer
        void setupRoofit();

        // open file, mc, dataset and set those variables
        void openWorkspace();

        // If specified, this function will load the snapshots
        void loadSnapshot();

        // set POIs
        void setPOIs();

        // prefit guess
        void modifyPrefitGuess();

        // set systematics
        void setSystematics();

        // Setting all vars constant in a map
        void setConstant(const RooArgSet* m_Param);


        // parse the step size etc.
        virtual void parseScanOpt() = 0;

        // does unconditional fit if one is not loaded in
        virtual void doUnconditional();

        // does an unconditional fit using a specified MC if it is loaded in
        virtual void doUnconditionalUnparam();

        // override the systematic settings for a particular scan
        virtual void scanTypeSetup();

        // actually does the setting of central val, limits, floating
        void setParameters(TString indexBase, const RooArgSet* m_allVars);

        // set binned workspace
        void setBinned();

        // modify other vars
        void modifyVars();

        // modify other vars
        void modifyForcedVars();

        // calls getnll, calls minimize
        void fitPoint(RooFitResult* &fitresult, int &status, RooNLLVar* &nll, RooArgSet* minosSet = NULL, bool onlyMinos = false);
        void fitPoint(RooFitResult* &fitresult, int &status, RooNLLVar* &nll, RooArgSet* minosSet,RooStats:: ModelConfig* mc, bool onlyMinos = false);

        // building the NLL
        RooNLLVar* getNLL();
        RooNLLVar* getNLL(RooStats::ModelConfig* mc);

        void fixRooStarMomentMorphCache();

        // setup variables for saving
        void prepTree();

        // writes the keys for the variable maps
        void initializeMap(const RooArgSet* m_Param);

        // makes the .root file name
        TString getOutputName();

        // Helper to fill variables
        virtual void fillVar(RooFitResult* fitresult = NULL, RooNLLVar* nll = NULL);
        virtual void addVars();

        virtual void runScan() = 0;

        RooFitResult* minimize(RooAbsReal* fcn, int& statusOut, RooArgSet* minosSet = NULL, bool onlyMinos = false);

        // helper functions
        TString getKey(TString inputStr);
        TString getVal(TString inputStr);

        std::map<TString, TString> m_opts;

        TFile* m_file;
        RooWorkspace* m_ws;
        RooStats::ModelConfig* m_mc;
        RooStats::ModelConfig* m_mcUnc;
        RooDataSet*  m_dataSet;
        // const RooArgSet* m_POI;
        // const RooArgSet* m_POIUnc;
        // const RooArgSet* m_Sys;
        // const RooArgSet* m_globs;
        // const RooArgSet* m_condObs;
        bool m_uncModelConfigFlag;


        TFile* m_outFile;
        TTree* m_outTree;
        TTree* m_metaDataTree;
        std::map<TString, double> m_outVarsDouble;
        std::map<TString, TString> m_outVarsMetaData;
        std::map<TString, TString> m_outVarsTString;


        TH2* m_correlationHist;
        TH2* m_covarianceHist;
        RooFitResult* m_uncondFitResult;




};


#endif
