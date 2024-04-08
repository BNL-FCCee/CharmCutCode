#include "scanBase.h"
#include "scanPulls.h"
using namespace std;


// Base class 
scanPulls::scanPulls(std::map<TString, TString>& opts):scanBase(opts){}
scanPulls::~scanPulls(){}

void scanPulls::parseScanOpt()
{

}

void scanPulls::fillVar(RooFitResult* fitresult, RooNLLVar* nll)
{
	scanBase::fillVar(fitresult, nll);

    // fill upr and lwr fit error to map
    auto POIList = m_mc->GetParametersOfInterest();
    auto SysList = m_mc->GetNuisanceParameters();
    
    TIterator* itr = SysList->createIterator();

    RooRealVar* var;
    while ((var = (RooRealVar*)itr->Next())) {
        TString varName(var->GetName());
        m_outVarsDouble[varName + "_uprErr"] = var->getErrorHi();
        m_outVarsDouble[varName + "_dwnErr"] = var->getErrorLo();
    }   
    delete itr;

    itr = POIList->createIterator();
    while ((var = (RooRealVar*)itr->Next())) {
        TString varName(var->GetName());
        m_outVarsDouble[varName + "_uprErr"] = var->getErrorHi();
        m_outVarsDouble[varName + "_dwnErr"] = var->getErrorLo();
    }   
    delete itr;

}

void scanPulls::addVars()
{
	scanBase::addVars();

    // loop over NP list and add upr and lwr to map
    auto POIList = m_mc->GetParametersOfInterest();
    auto SysList = m_mc->GetNuisanceParameters();
    
    TIterator* itr = SysList->createIterator();

    RooRealVar* var;
    while ((var = (RooRealVar*)itr->Next())) {
        TString varName(var->GetName());
        m_outVarsDouble[varName + "_uprErr"] = -999;
        m_outVarsDouble[varName + "_dwnErr"] = -999;
    }   
    delete itr;

    itr = POIList->createIterator();
    while ((var = (RooRealVar*)itr->Next())) {
        TString varName(var->GetName());
        m_outVarsDouble[varName + "_uprErr"] = -999;
        m_outVarsDouble[varName + "_dwnErr"] = -999;
    }   
    delete itr;


}

void scanPulls::runScan()
{



}






