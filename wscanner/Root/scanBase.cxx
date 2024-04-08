#include "scanBase.h"

using namespace std;
using namespace RooStats;
using namespace RooFit ;

// Base class 
scanBase::scanBase(std::map<TString, TString>& opts)
{
    m_opts = opts;

    m_correlationHist   = NULL;
    m_covarianceHist    = NULL;
    m_uncondFitResult   = NULL;
}
scanBase::~scanBase()
{
    if(m_ws) delete m_ws;
    if(m_outFile) delete m_outFile;
} 



void scanBase::run()
{
    cout << "Setting up the job..." << endl;
    // set verbosity for rooFit
    setOutputLevel();

    setupRoofit();

    // open file, mc, dataset and set those variables
    openWorkspace();

    // If specified, this function will load the snapshots
    loadSnapshot();

    // parse the scan
    parseScanOpt();

    // setup the variables for the output tree
    prepTree();
   
    // set POIs
    setPOIs();

    // set systematics
    setSystematics();

    // check if it is a binned workspace and setup accordingly
    setBinned();

    // Modify any other vars
    modifyVars();

    // Modify any forced vars
    modifyForcedVars();

    // Save snapshot for before
    m_ws->saveSnapshot("preUnConditional", m_ws->allVars());

    // fits unconditional unparametrized if an additional model config is given in config
    if(m_uncModelConfigFlag)
    { 
        doUnconditionalUnparam();
    }

    // fits if no unconditional fit is loaded
    doUnconditional();

    // set scan type override
    scanTypeSetup();

    // scan the POI
    runScan();

    // write to the output tree
    finalize();

}

void scanBase::openWorkspace()
{

    m_file = TFile::Open(m_opts["fileName"], "read");

    // Read the workspace, the Model config and the dataSet
    m_ws = (RooWorkspace*)m_file->Get(m_opts["workspaceName"]);
    if (!m_ws) 
    {
        LOG(logERROR) <<  "Workspace: " << m_opts["workspaceName"] << " doesn't exist!";
        exit(1);
    }
    else
    {
        cout<<"Opened file: " << m_opts["fileName"]<<endl;
    }

    m_mc = (ModelConfig*)m_ws->obj(m_opts["modelConfigName"]);
    m_uncModelConfigFlag = false;
    if(m_opts.find("uncModelConfigName") != m_opts.end())
    {   
        m_uncModelConfigFlag = true;
        m_mcUnc = (ModelConfig*)m_ws->obj(m_opts["uncModelConfigName"]);
    }
    else
    {
        m_mcUnc = m_mc;
    }

    if (!m_mc || (m_uncModelConfigFlag && !m_mcUnc) )  
    {
        if(!m_mc) {LOG(logERROR) <<  "ModelConfig: " << m_opts["modelConfigName"] << " doesn't exist!";}
        if(m_uncModelConfigFlag && !m_mcUnc) {LOG(logERROR) <<  "uncModelConfigName: " << m_opts["uncModelConfigName"] << " doesn't exist!";}
        exit(1);
    }

    TString rooDataSetName = "";
    if(m_opts["doData"].EqualTo("true"))
    {
        rooDataSetName = m_opts["datasetName"];
    }
    else
    {
        rooDataSetName = m_opts["asimovName"];
    }

    m_dataSet = (RooDataSet*)m_ws->data(rooDataSetName);
    // if the option toyDataSet is in the config or from cmdLine, use it as the dataset
    if(m_opts.find("toyDataset") != m_opts.end())
    {
        std::vector<TString> c_toyDatasetInfo = tokenizeStr(m_opts["toyDataset"],":");

        TFile* c_toyDatasetFile = TFile::Open(c_toyDatasetInfo[0], "read");
        RooWorkspace* c_toyWS = (RooWorkspace*)c_toyDatasetFile->Get(c_toyDatasetInfo[1]);
        m_dataSet = (RooDataSet*)c_toyWS->data(c_toyDatasetInfo[2]);       
    }
    if (!m_dataSet) 
    {
        LOG(logERROR) <<  "Dataset: " << rooDataSetName << " doesn't exist!";
        exit(1);
    }
    cout<<"Got dataset: " << rooDataSetName <<endl;
}



void scanBase::loadSnapshot()
{
    if(!m_opts["loadSnapshot"].EqualTo("true")) return;

    // Don't apply the snapshot if it is not set to be use for data
    if(m_opts["doSnapshotForData"].EqualTo("false") && m_opts["doData"].EqualTo("true")) return;


    // Load the snapshot
    vector<TString> splitInputStr = tokenizeStr(m_opts["snapshotName"],",");

    for(const auto& snapName: splitInputStr)
    {
        LOG(logINFO)<<"Snapshot loading: "<<snapName;
        m_ws->loadSnapshot(snapName);
    }
}

void scanBase::setOutputLevel()
{
    if(m_opts["debug"].EqualTo("true"))
    {
        // DEBUG
        ROOT::Math::MinimizerOptions::SetDefaultPrintLevel(1);
        LOG::ReportingLevel() = LOG::FromString("DEBUG");
    }
    else
    {
        // Very minimum output
        ROOT::Math::MinimizerOptions::SetDefaultPrintLevel(-1);
        RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL); 
        LOG::ReportingLevel() = LOG::FromString("INFO"); 
    }
}


void scanBase::setPOIs()
{
    
    auto POIList = m_mc->GetParametersOfInterest();
    TString indexBase = "POIInfo_";
    TString indexBaseUnc = "POIUnconditionalInfo_";
    setParameters(indexBase, POIList);

    if(m_opts["debug"].EqualTo("true"))
    {
        LOG(logDEBUG)<<"---------------- setPOIs ------------------";
        POIList->Print("v");
    }

    if(m_uncModelConfigFlag)
    {
        auto POIUncList = m_mcUnc->GetParametersOfInterest();

        // setup the unconditional POIs with a different setup the user has specified
        // under [POIUnconditional] 
        if(m_opts.find(indexBaseUnc+"0") != m_opts.end())
        { 
            setParameters(indexBaseUnc, POIUncList);
        }
        // if that doesnt exist, use [POI] setup instead
        else
        {
            setParameters(indexBase, POIUncList);
        }

        if(m_opts["debug"].EqualTo("true"))
        {
            LOG(logDEBUG)<<"---------------- POIUncList ------------------";
            POIUncList->Print("v");
        }

    }
}

void scanBase::setSystematics()
{
    auto SysList = m_mc->GetNuisanceParameters();

    TString indexBase = "SysInfo_allSys_";
    setParameters(indexBase, SysList);

    if(m_opts["debug"].EqualTo("true"))
    {
        LOG(logDEBUG)<<"---------------- setSystematics ------------------";
        if(SysList) SysList->Print("v");
    }

    if(m_uncModelConfigFlag)
    {
        auto SysUncList = m_mcUnc->GetNuisanceParameters();
        setParameters(indexBase, SysUncList);
    }


}
void scanBase::modifyVars()
{
    TString indexBase = "ModifyVarInfo_";

    if(m_opts.find(indexBase+"0") != m_opts.end()) 
    {
        RooArgSet allVars = m_ws->allVars();
        setParameters(indexBase, &allVars);
    }

    if(m_opts["debug"].EqualTo("true"))
    {
        LOG(logDEBUG)<<"---------------- modifyVars ------------------";
        // m_ws->allVars().Print("v");
    }

}

void scanBase::modifyForcedVars()
{
    auto freeVars = tokenizeStr(m_opts["forceFree"], ",");

    for (const auto& varName: freeVars)
    {
        // Set the vars
        RooArgSet* varSubSet = (RooArgSet*) m_ws->allVars().selectByName(varName);

        TIterator* itr = varSubSet->createIterator();
        RooRealVar* var;
        while ((var = (RooRealVar*)itr->Next())) 
        {
            var->setConstant(false);
        }
    }

    auto fixedVars = tokenizeStr(m_opts["forceFix"], ",");

    for (const auto& varName: fixedVars)
    {
        // Set the vars
        RooArgSet* varSubSet = (RooArgSet*) m_ws->allVars().selectByName(varName);

        TIterator* itr = varSubSet->createIterator();
        RooRealVar* var;
        while ((var = (RooRealVar*)itr->Next())) 
        {
            var->setConstant(true);
        }
    }


}


void scanBase::modifyPrefitGuess()
{
    TString indexBase = "PreFitGuessInfo_";

    if(m_opts.find(indexBase+"0") != m_opts.end()) 
    {
        RooArgSet allVars = m_ws->allVars();
        setParameters(indexBase, &allVars);
    }

}



void scanBase::setBinned()
{
    TString setBinnedBool = m_opts["binnedFit"];
    setBinnedBool.ToLower();

    if(!setBinnedBool.Contains("true")) return;
    
    RooFIter iter = m_ws->components().fwdIterator() ;
    RooAbsArg* arg ;
    while((arg = iter.next())) {
        if (arg->IsA() == RooRealSumPdf::Class() || arg->IsA() == RooAddPdf::Class()) {
            arg->setAttribute("BinnedLikelihood");
        }
    }
    
}


// For backwards compatibility
void scanBase::fitPoint(RooFitResult* &fitresult, int &status, RooNLLVar* &nll, RooArgSet* minosSet, bool onlyMinos)
{
    fitPoint(fitresult, status, nll, minosSet, m_mc, onlyMinos);
}



void scanBase::fitPoint(RooFitResult* &fitresult, int &status, RooNLLVar* &nll, RooArgSet* minosSet, RooStats::ModelConfig* mc, bool onlyMinos)
{
    nll = getNLL(mc);
    // nll->Print("v");
    // nll->printArgs(cout);
    fitresult = minimize(nll, status, minosSet, onlyMinos);
    // double NNLVal = 2*nll->getVal();
    // cout<<"NLL val: "<<NNLVal<<endl;
}


// For backwards compatibility
RooNLLVar* scanBase::getNLL()
{
    return getNLL(m_mc);
}

RooNLLVar* scanBase::getNLL(RooStats::ModelConfig* mc)
{

    auto globs   = (RooArgSet*)mc->GetGlobalObservables(); 
    auto condObs = (RooArgSet*)mc->GetConditionalObservables();  
    RooArgSet emptySet;
    if(condObs == NULL) condObs = &emptySet;
    auto SysList = mc->GetNuisanceParameters();

    fixRooStarMomentMorphCache();

    bool doOffSet = false;
    if(m_opts["applyOffset"].Contains("true")) doOffSet = true;

    // int numCPU = 1;
    RooNLLVar* nll = 0;


    if(SysList) nll = (RooNLLVar*)mc->GetPdf()->createNLL( *m_dataSet, CloneData(kFALSE), Constrain(*SysList), Offset(doOffSet), GlobalObservables(*globs), Verbose(kFALSE), NumCPU(1, RooFit::Hybrid), ConditionalObservables(*condObs));
    else  nll = (RooNLLVar*)mc->GetPdf()->createNLL( *m_dataSet, CloneData(kFALSE), Offset(doOffSet), GlobalObservables(*globs), Verbose(kFALSE), NumCPU(1, RooFit::Hybrid), ConditionalObservables(*condObs));

    fixRooStarMomentMorphCache();

    return nll;
}

void scanBase::fixRooStarMomentMorphCache()
{
    if(m_opts["applyRooStartMomentFix"].EqualTo("false")) return;

    #ifdef hasRooFitExtension
        RooFIter iter2 = m_ws->components().fwdIterator();
        RooAbsArg* arg2;
        while ((arg2 = iter2.next())) 
        {
            if (arg2->IsA() == RooStarMomentMorph::Class()) 
            {
                ((RooStarMomentMorph*)arg2)->fixCache();
            }
        }
        return;
    #endif

    if(m_opts["applyRooStartMomentFix"].EqualTo("true"))
    {
        LOG(logERROR)<<"You don't have RooStarMomentMorph either through HComb root or rootfitextension, yet you want the cache fixed";
        exit(1);
    }


}

void scanBase::fillVar(RooFitResult* /*fitresult*/, RooNLLVar* /*nll*/)
{
    // Fill the generic variables
    for(auto &varInfo : m_outVarsDouble) 
    {
        //LOG(logINFO)<<"Trying to fill: "<<varInfo.first;
        if(m_ws->var(varInfo.first))
        {
            varInfo.second = m_ws->var(varInfo.first)->getVal();
        }
        //LOG(logINFO)<<"doneFill";
    }
}


void scanBase::doUnconditionalUnparam()
{
    // Do the unconditional fit
    LOG(logINFO) << "Doing unconditional fit from unparametrized setup";
    
    RooFitResult* fitresult = NULL;
    RooNLLVar* nll = NULL;
    int status = -999;

    fitPoint(fitresult, status, nll, NULL, m_mcUnc);
    LOG(logINFO) << "NLL reparm: "<<2.*nll->getVal();

    m_outVarsDouble["nll"]              = 2.*nll->getVal();
    m_outVarsDouble["status"]           = status;
    m_outVarsDouble["isUnconditional"]  = 1;
    m_outVarsDouble["fitType"]          = 2;

    // Store variables
    fillVar(fitresult, nll);

    m_outFile->cd();
    m_outTree->Fill();
}

void scanBase::doUnconditional()
{
    // Load unconditional snapshot if there is one
    auto snapVar = m_ws->getSnapshot(m_opts["unconditionalSnap"]);
    if(snapVar)
    {
        LOG(logINFO) << "Loading unconditional fit";
        LOG(logINFO) << "Snapshot name: "<<m_opts["unconditionalSnap"];

        m_ws->loadSnapshot(m_opts["unconditionalSnap"]);

        //  // save the variables
        // RooNLLVar* nll = getNLL();

        RooFitResult* fitresult = NULL;
        RooNLLVar* nll = NULL;
        int status = -999;

        fitPoint(fitresult, status, nll);


        m_outVarsDouble["nll"]              = 2.*nll->getVal();
        m_outVarsDouble["status"]           = status;
        m_outVarsDouble["isUnconditional"]  = 1; 
        m_outVarsDouble["fitType"]          = 1; 

        // Store variables
        fillVar(NULL, nll);
        
        m_outFile->cd();
        m_outTree->Fill();
        
        delete nll;
    }
    else
    {
        // Do the unconditional fit
        LOG(logINFO) << "Doing unconditional fit";
        
        RooFitResult* fitresult = NULL;
        RooNLLVar* nll = NULL;
        int status = -999;

        fitPoint(fitresult, status, nll);

        m_outVarsDouble["nll"]              = 2.*nll->getVal();
        m_outVarsDouble["status"]           = status;
        m_outVarsDouble["isUnconditional"]  = 1;
        m_outVarsDouble["fitType"]          = 1; 

        // Store variables
        fillVar(fitresult, nll);

        m_outFile->cd();
        m_outTree->Fill();

        if(m_opts["saveCorrHist"].EqualTo("true"))
        {
            m_correlationHist = fitresult->correlationHist() ;
            TMatrixDSym covMatrix = fitresult->covarianceMatrix() ;

            int n = covMatrix.GetNcols() ;
            m_covarianceHist = new TH2D("covarianceHist","covarianceHist", n, 0, n, n, 0, n) ;

            for (int i = 0 ; i < n ; i++) 
            {
                for (int j = 0 ; j < n; j++) 
                {
                    m_covarianceHist->Fill(i+0.5,n-j-0.5,covMatrix(i,j)) ;
                }
                m_covarianceHist->GetXaxis()->SetBinLabel(i+1, m_correlationHist->GetXaxis()->GetBinLabel(i+1)) ;
                m_covarianceHist->GetYaxis()->SetBinLabel(n-i, m_correlationHist->GetYaxis()->GetBinLabel(n-i)) ;    
            }
        }
        // delete fitresult;
        m_uncondFitResult = fitresult;
        delete nll;

        m_ws->saveSnapshot(m_opts["unconditionalSnap"], m_ws->allVars());
    }
}

void scanBase::setupRoofit()
{
    ROOT::Math::MinimizerOptions::SetDefaultStrategy(m_opts["fitStrategy"].Atoi());
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");

    // If we want a correlation hist, we need the fit strategy at 2
    if(m_opts["saveCorrHist"].EqualTo("true"))
    {
        if(m_opts.find("corrHistStrategy") != m_opts.end())
        {
            ROOT::Math::MinimizerOptions::SetDefaultStrategy(m_opts["corrHistStrategy"].Atof());
        }
        else  ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);        
    }
}

void scanBase::scanTypeSetup()
{
    auto SysList = m_mc->GetNuisanceParameters();

    TString indexBase = "SysInfo_" + m_opts["scanType"] + "_";
    setParameters(indexBase, SysList);

    if(m_opts["debug"].EqualTo("true"))
    {
        LOG(logDEBUG)<<"---------------- set scanTypeSetup Systematics ------------------";
        if(SysList) SysList->Print("v");
    }

    if(m_uncModelConfigFlag)
    {
        auto SysUncList = m_mcUnc->GetNuisanceParameters();
        setParameters(indexBase, SysUncList);

        if(m_opts["debug"].EqualTo("true"))
        {
            LOG(logDEBUG)<<"---------------- set scanTypeSetup unc Systematics ------------------";
            if(SysUncList) SysUncList->Print("v");
        }

    }
    // if(m_opts["holdToUnconditional"].EqualTo("true"))
    // {
    //     if(m_POI) setConstant(m_POI);
    //     if(m_uncModelConfigFlag) setConstant(m_POIUnc);
    // }


}

TString scanBase::getOutputName()
{
    TString outputName = m_opts["outputDir"];
    system("mkdir -vp " + outputName);
    if(m_opts["outFileName"].Length() != 0) 
    {
        outputName += "/" + m_opts["outFileName"];
    }
    else
    {
        exit(1);
    }

    return outputName;
}

void scanBase::initializeMap(const RooArgSet* m_Param)
{
    if(not m_Param) return;
    TIterator* itr = m_Param->createIterator();

    RooRealVar* var;
    while ((var = (RooRealVar*)itr->Next())) {
        TString varName(var->GetName());
        m_outVarsDouble[varName] = -999;
    }   

    delete itr;
}

void scanBase::setConstant(const RooArgSet* m_Param)
{
    if(not m_Param) return;
    TIterator* itr = m_Param->createIterator();

    RooRealVar* var;
    while ((var = (RooRealVar*)itr->Next())) {
        if(var) var->Print();
        if(var) var->setConstant();
    }   

    delete itr;
}


void scanBase::prepTree()
{
    TString outputName = getOutputName();

    m_outFile = TFile::Open(outputName, "recreate");

    // TTree for output information
    m_outTree = new TTree("results", "results");
    m_metaDataTree = new TTree("metaData", "metaData");

    addVars();

    // copy over the input setting map
    m_outVarsMetaData.insert(m_opts.begin(), m_opts.end());
   
    // Book the ouput tree
    bookTree<double>(m_outTree, m_outVarsDouble);
    bookTree<TString>(m_outTree, m_outVarsTString);
    bookTree<TString>(m_metaDataTree, m_outVarsMetaData);
}

void scanBase::addVars()
{
    m_outVarsDouble["nll"]           = -999;
    m_outVarsDouble["status"]        = -999;
    m_outVarsDouble["isUnconditional"] = -999;
    m_outVarsDouble["fitType"]       = -999;


    auto POIList = m_mc->GetParametersOfInterest();
    auto SysList = m_mc->GetNuisanceParameters();
    
    // save the paramater names into the map
    initializeMap(POIList);
    initializeMap(SysList);

    if(m_uncModelConfigFlag)
    {
        auto POIUncList = m_mcUnc->GetParametersOfInterest();
        auto SysUncList = m_mcUnc->GetParametersOfInterest();

        // save the paramater names into the map
        initializeMap(POIUncList);
        initializeMap(SysUncList);
    }



    // The extra vars to save
    RooArgSet* varSubSet = (RooArgSet*) m_ws->allVars().selectByName(m_opts["extraVarSave"].ReplaceAll("(.*)", "*"));
    initializeMap(varSubSet);
}

void scanBase::finalize()
{
    // if we are saving the snapshot workspace, write that out
    if(m_opts["saveUncondWorkspace"].EqualTo("true"))
    {
        TString outPath = "uncondWs/" + m_opts["fileName"];

        // make the directory
        auto filePath = tokenizeStr(outPath, "/");

        TString pathToCreate = "";
        for(size_t i = 0; i < filePath.size(); i++) pathToCreate += filePath[i] + "/";
        system("mkdir -vp " + pathToCreate);

        m_ws->writeToFile(outPath, true);
    }

    m_outFile->cd();
    m_metaDataTree->Fill();
    m_metaDataTree->Write();
    m_outTree->Write();

    // If doing an correlation job or an unconditional fit to save a ws out
    if(m_correlationHist || m_opts["saveUncondWorkspace"].EqualTo("true"))
    {
        m_uncondFitResult->Write();
    }

    if(m_correlationHist) m_correlationHist->Write();
    if(m_covarianceHist)  m_covarianceHist->Write();

    m_outFile->Close();
    delete m_file;
}


// Get the key from a key:val pair
TString scanBase::getKey(TString inputStr)
{
    vector<TString> splitInputStr = tokenizeStr(inputStr,":");
    if(splitInputStr.size() != 2)
    {
        LOG(logERROR)<<"You are giving an input options that doesn't follow key:val pattern";
        LOG(logERROR)<<"option: "<<inputStr;
        exit(1);
    }

    return splitInputStr[0];
}

// Get the value from a key:val pair
TString scanBase::getVal(TString inputStr)
{
    vector<TString> splitInputStr = tokenizeStr(inputStr,":");
    if(splitInputStr.size() != 2)
    {
        LOG(logERROR)<<"You are giving an input options that doesn't follow key:val pattern";
        LOG(logERROR)<<"option: "<<inputStr;
        exit(1);
    }

    return splitInputStr[1];
}

void scanBase::setParameters(TString indexBase, const RooArgSet* allVars)
{
    if(not allVars) return;

    int index = 0;
    bool isInfoAvail = true;
    do
    {
        TString indexName = indexBase + to_string(index);
        if(m_opts.find(indexName) == m_opts.end()) {isInfoAvail = false; continue;}

        // Do the parsing
        auto splitStr = tokenizeStr(m_opts.at(indexName), ",");

        // default values
        TString nameRegex   = "(.*)";
        double centralVal   = -999;
        double lwrLim       = -999;
        double uprLim       = -999;
        bool setConst       = false;
        bool removeLimit    = false;
        bool isSnapshot     = false;

        // Parse the input options
        for(const auto& currOpt: splitStr)
        {
            TString key = getKey(currOpt);
            TString val = getVal(currOpt);

            if(key.EqualTo("name"))
            {
                nameRegex = val;
            }
            if(key.EqualTo("float"))
            {
                val.ToLower();
                if(val.EqualTo("false")) setConst = true;
                else if(val.EqualTo("true")) setConst = false;
                else
                {
                    LOG(logERROR)<<"Cannot recoginze the val pair for the \"float\" option";
                    LOG(logERROR)<<"currOpt: "<<currOpt;
                    exit(1);
                }
            }
            if(key.EqualTo("type"))
            {
                if(val.EqualTo("snapshot")) isSnapshot = true;
                else
                {
                    LOG(logERROR)<<"Cannot recoginze the val pair for the \"type\" option";
                    LOG(logERROR)<<"currOpt: "<<currOpt;
                    exit(1);
                }
            }
            if(key.EqualTo("removeLimit"))
            {
                val.ToLower();
                if(val.EqualTo("false")) removeLimit = false;
                else if(val.EqualTo("true")) removeLimit = true;
                else
                {
                    LOG(logERROR)<<"Cannot recoginze the val pair for the \"removeLimit\" option";
                    LOG(logERROR)<<"currOpt: "<<currOpt;
                    exit(1);
                }
            }

            if(key.EqualTo("centralVal"))
            {
                if(!val.IsFloat())
                {
                    LOG(logERROR)<<"Cannot recoginze the val pair for the \"centralVal\" option";
                    LOG(logERROR)<<"currOpt: "<<currOpt;
                    exit(1);
                }
                centralVal = val.Atof();
            }
            if(key.EqualTo("lwrLim"))
            {
                if(!val.IsFloat())
                {
                    LOG(logERROR)<<"Cannot recoginze the val pair for the \"centralVal\" option";
                    LOG(logERROR)<<"currOpt: "<<currOpt;
                    exit(1);
                }
                lwrLim = val.Atof();
            }
            if(key.EqualTo("uprLim"))
            {
                if(!val.IsFloat())
                {
                    LOG(logERROR)<<"Cannot recoginze the val pair for the \"centralVal\" option";
                    LOG(logERROR)<<"currOpt: "<<currOpt;
                    exit(1);
                }
                uprLim = val.Atof();
            }
        }


        if(nameRegex.Contains("*"))
        {
            // Set the vars
            RooArgSet* varSubSet = (RooArgSet*) allVars->selectByName(nameRegex);

            TIterator* itr = varSubSet->createIterator();
            RooRealVar* var;
            while ((var = (RooRealVar*)itr->Next())) 
            {
                TString varName = var->GetName();
                if(varName.Contains("ATLAS") || varName.Contains("NP"))
                {
                    if(setConst && centralVal != -999)
                    {
                        // LOG(logWARNING)<<"You are setting the var which looks like a NP constant and setting its value to something other than the what is in the WS";
                        // LOG(logWARNING)<<"Var: "<<varName<<" setVal: "<<centralVal;
                        // LOG(logWARNING)<<"Be sure that this is something that you want, otherwise you will see kinks in the NLL around the min point";
                        // LOG(logWARNING)<<"If this is what you want, sorry for the message... Better the user know about this pit fall";
                    }
                }

                var->setConstant(setConst);
                if(lwrLim != -999) var->setMin(lwrLim);
                if(uprLim != -999) var->setMax(uprLim);
                if(centralVal != -999) var->setVal(centralVal);
                if(removeLimit)
                {
                    var->removeMax();
                    var->removeMin();
                }
            }
        }
        else if(isSnapshot)
        {
            LOG(logINFO)<<"Snapshot loading: "<<nameRegex;
            m_ws->loadSnapshot(nameRegex);
        }
        else if(m_ws->var(nameRegex))
        {
            m_ws->var(nameRegex)->setConstant(setConst);
            if(lwrLim != -999) m_ws->var(nameRegex)->setMin(lwrLim);
            if(uprLim != -999) m_ws->var(nameRegex)->setMax(uprLim);
            if(centralVal != -999) m_ws->var(nameRegex)->setVal(centralVal);
            if(removeLimit)
            {
                m_ws->var(nameRegex)->removeMax();
                m_ws->var(nameRegex)->removeMin();
            }
        }

        index++;

    }while(isInfoAvail);

    if(index == 0)
    {
        LOG(logERROR)<<"Tried to set indexBase: "<<indexBase;
        LOG(logERROR)<<"However, no scan matching this index was found in the config";
        LOG(logERROR)<<"Please check what is being done";
        exit(1);
    }
}



RooFitResult* scanBase::minimize(RooAbsReal* fcn, int& statusOut, RooArgSet* minosSet, bool onlyMinos) 
{
    TStopwatch timer;
    timer.Start();

    if(m_opts["applyOffset"].EqualTo("true")) fcn->enableOffsetting(true);

    int printLevel = ROOT::Math::MinimizerOptions::DefaultPrintLevel();
    RooFit::MsgLevel msglevel = RooMsgService::instance().globalKillBelow();
    if (printLevel < 0) RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);

    int strat = ROOT::Math::MinimizerOptions::DefaultStrategy();
    int save_strat = strat;


     // m_ws->allVars().Print("v");
    //cout<<"Default startegy: "<<strat<<endl;

    RooMinimizer minim(*fcn);

    if(m_opts.find("optimizeConst") != m_opts.end())
    {
        LOG(logINFO)<<"Running optimizeConst: "<<m_opts["optimizeConst"].Atoi(); 
        minim.optimizeConst(m_opts["optimizeConst"].Atoi());
    }

    minim.setStrategy(strat);
    minim.setPrintLevel(printLevel);

    //minim.setProfile(1);
    //minim.setMaxFunctionCalls(1e6);
    //minim.setMaxIterations(1e6);
    // This changes the 2*NLL value used for Minuit error determination
    // which will be used for the Minuit error determination,
    // used as starting point for findSigma. Best not to change this!
    // BEtter to use setEps to change the tolerance.
    //minim.setErrorLevel(0.1);

    // Added this
    if(m_opts.find("epsVal") != m_opts.end()) minim.setEps(m_opts["epsVal"].Atof());

    // cout << "Watch out for this" << m_opts["epsVal"].Atof() << endl;
    int status = 0;
    if(!onlyMinos)
    {
        status = minim.minimize(ROOT::Math::MinimizerOptions::DefaultMinimizerType().c_str(), ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo().c_str());

        // up the strategy
        if (status != 0 && status != 1 && strat < 2) 
        {
            strat++;
            cout << "Fit failed with status " << status << ". Retrying with strategy " << strat << endl;
            minim.setStrategy(strat);
            status = minim.minimize(ROOT::Math::MinimizerOptions::DefaultMinimizerType().c_str(), ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo().c_str());
        }
        if (status != 0 && status != 1 && strat == 2) 
        {
            cout << "Fit failed with status " << status << ". Retrying with strategy " << strat << endl;
            minim.setStrategy(strat);
            status = minim.minimize(ROOT::Math::MinimizerOptions::DefaultMinimizerType().c_str(), ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo().c_str());
        }
        if (status != 0 && status != 1) 
        {
            cout << "Fit failed with status " << status << endl;
        }
        else
        {
            //cout<<"fit is ok"<<endl;
        }
    }

    if(m_opts["saveCorrHist"].EqualTo("true"))
    {
        LOG(logINFO)<<"Running Hesse";
        minim.hesse();
    }


    if (printLevel < 0) RooMsgService::instance().setGlobalKillBelow(msglevel);
    ROOT::Math::MinimizerOptions::SetDefaultStrategy(save_strat);

    string name = Form("fitresult_%s",fcn->GetName());
    string title = Form("fitresult_%s",fcn->GetName());

    if (minosSet != NULL) {
        minim.minos(*minosSet);
    }

    RooFitResult* fitresult = minim.save(name.c_str(), title.c_str());


    ROOT::Math::MinimizerOptions::SetDefaultStrategy(save_strat);

    statusOut = status;

    timer.Stop();
    //timer.Print();

    return fitresult;
}







