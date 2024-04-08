#include "scanBase.h"
#include "scanMinosError.h"

using namespace std;

scanMinosError::scanMinosError(std::map<TString, TString>& opts):scanBase(opts)
{
    TString lowercase = m_opts["scanType"];
    lowercase.ToLower();

}
scanMinosError::~scanMinosError(){}


void scanMinosError::fillVar(RooFitResult* fitresult, RooNLLVar* nll)
{
	scanBase::fillVar(fitresult, nll);
	//m_outVarsDouble["poi"] = m_ws->var(m_poiName)->getVal();
}

void scanMinosError::parseScanOpt()
{
    m_paramsToFit = tokenizeStr(m_opts["poiScan"], ",");

    for(const auto& paramToFit:m_paramsToFit) LOG(logINFO) << "Var to be run: " << paramToFit;
}

void scanMinosError::addVars()
{
	scanBase::addVars();
    m_outVarsTString["VarName"]          = "";

    m_outVarsDouble["VarVal"]            = -999;
    m_outVarsDouble["VarErrorHi"]        = -999;
    m_outVarsDouble["VarErrorLow"]       = -999;


}
void scanMinosError::runScan()
{   
    // Snapshot
    // m_ws->saveSnapshot("initialFit", m_ws->allVars());

    RooArgSet minosSet;

    for(const auto& paramToFit:m_paramsToFit)
    {
        RooRealVar* var = m_ws->var(paramToFit);
        if(var) minosSet.add(*var);
    }

    // do the fit and save the results
    RooFitResult* fitresult = NULL;
    RooNLLVar* nll = NULL;
    int status = -999;

    if(!m_opts["skipMinos"].EqualTo("true")) fitPoint(fitresult, status, nll, &minosSet, false);

    for(const auto& paramToFit:m_paramsToFit)
    {
        if(!m_opts["skipMinos"].EqualTo("true")) m_outVarsDouble["nll"]              = 2.*nll->getVal();
        if(!m_opts["skipMinos"].EqualTo("true")) m_outVarsDouble["status"]           = status;
        m_outVarsDouble["isUnconditional"]  = 0;

        m_outVarsDouble["VarVal"]            = m_ws->var(paramToFit)->getVal();
        m_outVarsDouble["VarErrorHi"]        = m_ws->var(paramToFit)->getErrorHi();
        m_outVarsDouble["VarErrorLow"]       = m_ws->var(paramToFit)->getErrorLo();

        m_outVarsTString["VarName"]          = paramToFit;


        // Store variables
        fillVar(fitresult, nll);
        m_outFile->cd();
        m_outTree->Fill();
    }

    delete fitresult;
    delete nll;

}







