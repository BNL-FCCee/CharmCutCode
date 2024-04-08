#include "scanBase.h"
#include "computeCompatibility.h"

using namespace std;

computeCompatibility::computeCompatibility(std::map<TString, TString>& opts):scanBase(opts){}
computeCompatibility::~computeCompatibility(){}


void computeCompatibility::runScan()
{   

    // Setup the scan
    TString indexBase = "CompatibilityInfo_" + m_opts["scanType"] + "_";

    auto POIList = m_mc->GetParametersOfInterest();
    setParameters(indexBase, POIList);


    LOG(logINFO)<<"Doing Compatibility: "<<m_opts["scanType"];

    POIList->Print("v");
    

    // do the fit and save the results
    RooFitResult* fitresult = NULL;
    RooNLLVar* nll = NULL;
    int status = -999;

    fitPoint(fitresult, status, nll);

    m_outVarsDouble["nll"]              = 2.*nll->getVal();
    m_outVarsDouble["status"]           = status;
    m_outVarsDouble["isUnconditional"]  = 0;

    // Store variables
    fillVar(fitresult, nll);

    m_outFile->cd();

    m_outTree->Fill();

    delete fitresult;
    delete nll;
}


void computeCompatibility::scanTypeSetup()
{
    auto sysList = m_mc->GetNuisanceParameters();
    TString indexBase = "SysInfo_allSys_";

    setParameters(indexBase, sysList);

    if(m_opts["debug"].EqualTo("true"))
    {
        if(sysList) sysList->Print("v");
    }
}





