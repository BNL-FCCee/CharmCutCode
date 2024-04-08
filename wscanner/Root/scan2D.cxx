#include "scanBase.h"
#include "scan2D.h"
using namespace std;


// Base class 
scan2D::scan2D(std::map<TString, TString>& opts):scanBase(opts){}
scan2D::~scan2D(){}

void scan2D::parseScanOpt()
{
    std::vector<TString> splitPOIs = tokenizeStr(m_opts["poiScan"], ",");

    // Sanity check
    if(splitPOIs.size() != 2) showPOIError();


    std::vector<TString> POIvec1 = tokenizeStr(splitPOIs.at(0), ":");
    std::vector<TString> POIvec2 = tokenizeStr(splitPOIs.at(1), ":");

    // Sanity check
    if(POIvec1.size() != 4 || POIvec2.size() != 4  ) showPOIError();

    m_poiName_1 = POIvec1[0];
    m_steps_1   = POIvec1[1].Atoi();
    m_lwrLim_1  = POIvec1[2].Atof();
    m_uprLim_1  = POIvec1[3].Atof();
    m_delta_1   = (m_uprLim_1 - m_lwrLim_1)/m_steps_1;

    m_poiName_2 = POIvec2[0];
    m_steps_2   = POIvec2[1].Atoi();
    m_lwrLim_2  = POIvec2[2].Atof();
    m_uprLim_2  = POIvec2[3].Atof();
    m_delta_2   = (m_uprLim_2 - m_lwrLim_2)/m_steps_2;

    LOG(logINFO) << "poiName_1: " <<m_poiName_1;
    LOG(logINFO) << "steps_1: "   <<m_steps_1;
    LOG(logINFO) << "lwrLim_1: "  <<m_lwrLim_1;
    LOG(logINFO) << "uprLim_1: "  <<m_uprLim_1;

    LOG(logINFO) << "poiName_2: " <<m_poiName_2;
    LOG(logINFO) << "steps_2: "   <<m_steps_2;
    LOG(logINFO) << "lwrLim_2: "  <<m_lwrLim_2;
    LOG(logINFO) << "uprLim_2: "  <<m_uprLim_2;

    m_poi_1 = m_ws->var(m_poiName_1);
    m_poi_2 = m_ws->var(m_poiName_2);

}
void scan2D::showPOIError()
{
    LOG(logERROR)<<"ERROR - poiScan var not in the correct format";
    LOG(logERROR)<<"ERROR - poiScan is "<<m_opts["poiScan"];
    LOG(logERROR)<<"ERROR - example is "<< "POI1:steps:lwrLim:uprLim,POI2:steps:lwrLim:uprLim";
    exit(1);
}


void scan2D::fillVar(RooFitResult* fitresult, RooNLLVar* nll)
{
    scanBase::fillVar(fitresult, nll);
    cout<<"m_poiName_1: "<<m_poiName_1<<endl;
    cout<<"m_poiName_2: "<<m_poiName_2<<endl;
    m_outVarsDouble["poi_1"] = m_ws->var(m_poiName_1)->getVal();
    m_outVarsDouble["poi_2"] = m_ws->var(m_poiName_2)->getVal();
}

void scan2D::addVars()
{
    scanBase::addVars();
    m_outVarsDouble["poi_1"] = -999;
    m_outVarsDouble["poi_2"] = -999;
    
}

void scan2D::runScan()
{
    // if zero points are asked, just return
    if(m_steps_1 == 0 || m_steps_2 == 0) return; 
    TStopwatch stopwatch;
    vector <double> time;
    cout << "***************" << endl;

    // Scan
    for(int i = 0; i < m_steps_1 + 1; i++)
    {
        for(int j = 0; j < m_steps_2 + 1; j++)
        {
            cout<<"Setting poi_1 to : "<<m_lwrLim_1 + i * m_delta_1<<endl;
            cout<<"Setting poi_2 to : "<<m_lwrLim_2 + j * m_delta_2<<endl;

            modifyPrefitGuess();

            m_poi_1->setVal(m_lwrLim_1 + i * m_delta_1);
            m_poi_1->setConstant(true);

            m_poi_2->setVal(m_lwrLim_2 + j * m_delta_2);
            m_poi_2->setConstant(true);
        
            RooFitResult* fitresult = NULL;
            RooNLLVar*    nll       = NULL;
            int           status    = -999;

            fitPoint(fitresult, status, nll);

            m_outVarsDouble["nll"]              = 2.*nll->getVal();
            m_outVarsDouble["status"]           = status;
            m_outVarsDouble["isUnconditional"]  = 0;
            m_outVarsDouble["fitType"]          = 0;

            // Store variables
            fillVar(fitresult, nll);

            m_outFile->cd();

            m_outTree->Fill();

            delete fitresult;
            delete nll;

            stopwatch.Stop();

            time.push_back(stopwatch.RealTime());

            //std::cout.flush();
            stopwatch.Reset();

        }
    }

}





