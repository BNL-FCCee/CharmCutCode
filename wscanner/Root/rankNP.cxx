#include "scanBase.h"
#include "rankNP.h"

using namespace std;

rankNP::rankNP(std::map<TString, TString>& opts):scanBase(opts)
{
    TString lowercase = m_opts["scanType"];
    lowercase.ToLower();

    if(!lowercase.Contains("allsys"))
    {
        LOG(logERROR)<<"Trying to do ranking with scanType: "<<m_opts["scanType"];
        LOG(logERROR)<<"This combination is not supported";
        exit(1);
    }
}
rankNP::~rankNP(){}


void rankNP::fillVar(RooFitResult* fitresult, RooNLLVar* nll)
{
	scanBase::fillVar(fitresult, nll);
	//m_outVarsDouble["poi"] = m_ws->var(m_poiName)->getVal();
}

void rankNP::parseScanOpt()
{
    m_paramsToRank = tokenizeStr(m_opts["poiScan"], ",");

    for(const auto& paramToRank:m_paramsToRank) LOG(logINFO) << "NP to be ranked: " << paramToRank;
}

void rankNP::addVars()
{
	scanBase::addVars();
    // m_outVarsDouble["prefit"] 
	m_outVarsTString["fitName"]         = "";
    m_outVarsTString["NPName"]          = "";

    m_outVarsDouble["NPErrorHi"]        = -999;
    m_outVarsDouble["NPErrorLow"]       = -999;


}

void rankNP::scanPointHelper(TString fitName, TString NPName, RooArgSet* minosSet)
{

    LOG(logDEBUG)<<"--------------------- doing "<<fitName<<"---------------------------";

    // do the fit and save the results
    RooFitResult* fitresult = NULL;
    RooNLLVar* nll = NULL;
    int status = -999;

    fitPoint(fitresult, status, nll, minosSet);

    m_outVarsDouble["nll"]              = 2.*nll->getVal();
    m_outVarsDouble["status"]           = status;
    m_outVarsDouble["isUnconditional"]  = 0;
    m_outVarsDouble["fitType"]          = 0;

    m_outVarsDouble["NPErrorHi"]        = m_ws->var(NPName)->getErrorHi();
    m_outVarsDouble["NPErrorLow"]       = m_ws->var(NPName)->getErrorLo();

    m_outVarsTString["fitName"]         = fitName;
    m_outVarsTString["NPName"]          = NPName;


    // Store variables
    fillVar(fitresult, nll);

    m_outFile->cd();

    m_outTree->Fill();

    delete fitresult;
    delete nll;
}

void rankNP::runScan()
{   
    // Snapshot
    m_ws->saveSnapshot("initialFit", m_ws->allVars());

    for(const auto& paramToRank:m_paramsToRank)
    {

        RooRealVar* nuip = m_ws->var(paramToRank);
        m_ws->loadSnapshot("initialFit");

        RooArgSet minosSet;
        minosSet.add(*nuip);

        // Do the baseLine fit and get the error on the NP
        m_ws->loadSnapshot("initialFit");
        scanPointHelper("BaseLine", paramToRank, &minosSet);        

        // Get the central and error for the 
        double npHat    = nuip->getVal();
        double npLwrLim = nuip->getErrorLo();
        double npUprLim = nuip->getErrorHi();

        LOG(logINFO)<<"NP: "<<paramToRank<<" val: "<<npHat<<" +"<<npUprLim<<" "<<npLwrLim;

        // Post: +1 sigma fit
        // Load the snapshot again
        m_ws->loadSnapshot("initialFit");
        nuip->setVal(npHat + fabs(npUprLim));
        nuip->setConstant(true);
        scanPointHelper("Plus_1Sigma_PostFit", paramToRank);

        // Post: -1 sigma fit
        // Load the snapshot again
        m_ws->loadSnapshot("initialFit");
        nuip->setVal(npHat - fabs(npLwrLim));
        nuip->setConstant(true);
        scanPointHelper("Minus_1Sigma_PostFit", paramToRank);


        // Skip preFit impact to save time if needed
        if(m_opts["skipPrefitImpact"].EqualTo("true")) continue;

        // Pre: +1 sigma fit
        // Load the snapshot again
        m_ws->loadSnapshot("initialFit");
        nuip->setVal(npHat + 1);
        nuip->setConstant(true);
        scanPointHelper("Plus_1Sigma_PreFit", paramToRank);

        // Pre: -1 sigma fit
        // Load the snapshot again
        m_ws->loadSnapshot("initialFit");
        nuip->setVal(npHat - 1);
        nuip->setConstant(true);
        scanPointHelper("Minus_1Sigma_PreFit", paramToRank);

    }

}







