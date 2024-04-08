#include "scanBase.h"
#include "scan1D.h"
using namespace std;


// Base class 
scan1D::scan1D(std::map<TString, TString>& opts):scanBase(opts){}
scan1D::~scan1D(){}

void scan1D::parseScanOpt()
{
    std::vector<TString> parsedScanVec = tokenizeStr(m_opts["poiScan"], ":");

    // Sanity check
    if(parsedScanVec.size() != 4)
    {
        LOG(logERROR) << "poiScan var not in the correct format";
        LOG(logERROR) << "poiScan is " << m_opts["poiScan"];
        exit(1);
    }

    // Parse the information
    m_poiName 	= parsedScanVec[0];
    m_steps 	= parsedScanVec[1].Atoi();
    m_lwrLim 	= parsedScanVec[2].Atof();
    m_uprLim 	= parsedScanVec[3].Atof();
    m_delta 	= (m_uprLim - m_lwrLim)/m_steps;
    //double xsVal 	= getXS(poiName, ws, mc, opts);
    
    LOG(logINFO) << "poiName: " << m_poiName;
    LOG(logINFO) << "steps: "   << m_steps;
    LOG(logINFO) << "lwrLim: "  << m_lwrLim;
    LOG(logINFO) << "uprLim: "  << m_uprLim;

    m_poi = m_ws->var(m_poiName);

    if(!m_poi)
    {
        LOG(logERROR)<<"Can't find the POI: "<<m_poiName;
        exit(1);
    }

    //LOG(logINFO)<< "xsVal: "<<xsVal;
}

void scan1D::fillVar(RooFitResult* fitresult, RooNLLVar* nll)
{
	scanBase::fillVar(fitresult, nll);
    cout<<"Poi: "<<m_poiName<<endl;
	m_outVarsDouble["poi"] = m_ws->var(m_poiName)->getVal();
}

void scan1D::addVars()
{
	scanBase::addVars();
	m_outVarsDouble["poi"] = -999;
}

void scan1D::runScan()
{

    // If zero points are asked from the outside world return
    if(m_steps == 0) return;

    TStopwatch stopwatch;
    vector <double> time;
    cout << "***************" << endl;

    // Scan
    for(int i = 0; i < m_steps + 1; i++)
    {
        cout<<"Setting poi to : "<<m_lwrLim + i * m_delta<<endl;


        if(m_opts.find("loadunconditionalSnapBeforeEveryFit") != m_opts.end())
        {        
            if(m_opts["loadunconditionalSnapBeforeEveryFit"].EqualTo("true"))
            {
                m_ws->loadSnapshot(m_opts["unconditionalSnap"]);
                // set scan type override
                scanTypeSetup();
            }
        }
        


        m_poi->setVal(m_lwrLim + i * m_delta);
        m_poi->setConstant(true);
    
	    RooFitResult* fitresult = NULL;
	    RooNLLVar* nll = NULL;
	    int status = -999;

	    fitPoint(fitresult, status, nll);

	    m_outVarsDouble["nll"] 			 	= 2.*nll->getVal();
	    m_outVarsDouble["status"] 		 	= status;
	    m_outVarsDouble["isUnconditional"] 	= 0;
        m_outVarsDouble["fitType"]          = 0;

	    // Store variables
	    fillVar(fitresult, nll);

	    m_outFile->cd();

	    m_outTree->Fill();

	    delete fitresult;
	    delete nll;

        stopwatch.Stop();

        /// progress bar
        std::cout << "\033[0;31m [";
        int pos = i*100/m_steps;
        for (int ii = 0; ii < 100; ++ii) {
            if (ii < pos) std::cout << "=";
            else if (ii == pos) std::cout << ">";
            else std::cout << " ";
        }

        time.push_back(stopwatch.RealTime());

        std::cout << "] " << int((i+1) * 100.0 / m_steps) << " % ETA: " << (std::accumulate(time.begin(), time.end(), 0.0)/time.size())*(m_steps-i-1) << " seconds, step: "<<i<<"\033[0m \r";
        //std::cout.flush();
        stopwatch.Reset();

    }

}






