#include "scanBase.h"
#include "scan1DToys.h"

#include "RooStats/ToyMCImportanceSampler.h"
#include "RooStats/SamplingDistribution.h"
#include "RooStats/AsymptoticCalculator.h"

using namespace RooStats;

using namespace std;


// Base class 
scan1DToys::scan1DToys(std::map<TString, TString>& opts):scanBase(opts){}
scan1DToys::~scan1DToys(){}


void scan1DToys::parseScanOpt()
{
    std::vector<TString> joinedParsedScanVec = tokenizeStr(m_opts["poiScan"], ",");
    std::vector<TString> parsedScanVec       = tokenizeStr(joinedParsedScanVec[0], ":");
    std::vector<TString> toyVec              = tokenizeStr(joinedParsedScanVec[1], ":");

    // Sanity check
    if(parsedScanVec.size() != 4 || toyVec.size() != 2)
    {
        LOG(logERROR) << "poiScan var not in the correct format";
        LOG(logERROR) << "poiScan is " << m_opts["poiScan"];
        exit(1);
    }

    // Parse the information
    m_nToys     = toyVec[1].Atoi();

    m_poiName   = parsedScanVec[0];
    m_steps     = parsedScanVec[1].Atoi();
    m_lwrLim    = parsedScanVec[2].Atof();
    m_uprLim    = parsedScanVec[3].Atof();
    m_delta     = (m_uprLim - m_lwrLim)/m_steps;
    
    LOG(logINFO) << "poiName: " << m_poiName;
    LOG(logINFO) << "steps: "   << m_steps;
    LOG(logINFO) << "lwrLim: "  << m_lwrLim;
    LOG(logINFO) << "uprLim: "  << m_uprLim;

    m_poi   = m_ws->var(m_poiName);
}

void scan1DToys::fillVar(RooFitResult* fitresult, RooNLLVar* nll)
{
    scanBase::fillVar(fitresult, nll);
    m_outVarsDouble["poi"] = m_ws->var(m_poiName)->getVal();
}

void scan1DToys::addVars()
{
    scanBase::addVars();
    m_outVarsDouble["poi"] = -999;
    m_outVarsDouble["toyNumber"] = -999; 
    m_outVarsDouble["toyNumber"] = -999;
    m_outVarsDouble["nToys"]     = -999;
    m_outVarsDouble["deltaNLL"]  = -999;
    

    m_outVarsDouble["poi_generated"]     = -999;

}



void scan1DToys::runScan()
{ 

    // Set it once
    m_outVarsDouble["nToys"] = m_nToys;

    TStopwatch stopwatch;
    vector <double> time;
    cout << "***************" << endl;
    
    RooArgSet* obsList= (RooArgSet*) m_mc->GetObservables();

    m_ws->saveSnapshot("ToyCondSetup", m_ws->allVars());

    // For caching, since we overwrite the toy dataset
    auto nomDataSet = m_dataSet;

    // Scan
    for(int i = 0; i < m_steps; i++)
    {         

        // Create the toy generation snapshot
        m_ws->loadSnapshot("preUnConditional");
        // set the POI
        m_poi->setVal(m_lwrLim + i * m_delta);
        m_poi->setConstant(true);
        m_ws->saveSnapshot("ToyGenSetup", m_ws->allVars());

        
        // Reset the state
        m_dataSet = nomDataSet;

        // do the unconditional snapshot
        m_ws->loadSnapshot(m_opts["unconditionalSnap"]);
        // Reset the POI as just in case
        setPOIs();
        scanTypeSetup();

        // do the conditional fit
        RooFitResult* fitresultUnCond = NULL;
        RooNLLVar* nllUnCond = NULL;
        int statusUnCond = -999;

        // Fit the point
        fitPoint(fitresultUnCond, statusUnCond, nllUnCond, NULL, m_mcUnc);
        double valUnCond = 2*nllUnCond->getVal();


        // Set the point
        m_ws->loadSnapshot("ToyCondSetup");
        cout<<"Setting poi to : "<<m_lwrLim + i * m_delta<<endl;

        m_poi->setVal(m_lwrLim + i * m_delta);
        m_poi->setConstant(true);



        // do the conditional fit
        RooFitResult* fitresultCond = NULL;
        RooNLLVar* nllCond = NULL;
        int statusCond = -999;

        fitPoint(fitresultCond, statusCond, nllCond);

        // Save the information
        m_outVarsDouble["nll"]              = 2.*nllCond->getVal();
        m_outVarsDouble["deltaNLL"]         = 2.*nllCond->getVal() - valUnCond;
        m_outVarsDouble["status"]           = statusCond;
        m_outVarsDouble["isUnconditional"]  = 1;
        m_outVarsDouble["fitType"]          = 3;
        m_outVarsDouble["toyNumber"]        = -999;
        m_outVarsDouble["poi_generated"]    = m_lwrLim + i * m_delta;

        // Store variables
        fillVar(fitresultCond, nllCond);

        m_outFile->cd();
        m_outTree->Fill();

        delete fitresultCond;
        delete nllCond;
        delete fitresultUnCond;
        delete nllUnCond;


        m_ws->saveSnapshot("ToySetup", m_ws->allVars());

        auto* toyGen = new ToyMCSampler();
        toyGen->SetPdf( *m_mc->GetPdf() );
        toyGen->SetObservables( *m_mc->GetObservables() );
        toyGen->SetNuisanceParameters(*m_mc->GetNuisanceParameters());

        // generate a random seed
        int cSeed = 0; 
        // get seeds
        cSeed = m_opts.at("seed").Atoi();
        RooRandom::randomGenerator()->SetSeed(cSeed);

        for(int cToy = 0; cToy < m_nToys; cToy++)
        {

            if(cToy % 10 == 0)
            {
                double vm_usage     = 0;
                double resident_set = 0;
                process_mem_usage(vm_usage, resident_set);
                LOG(logINFO) <<"Processing event " << cToy << " mem vm/rss " << vm_usage/1000. << "/" << resident_set/1000; 
            }

            m_ws->loadSnapshot("ToyGenSetup");
            m_dataSet = (RooDataSet*) toyGen->GenerateToyData(*obsList);

            // do the unconditional snapshot
            m_ws->loadSnapshot(m_opts["unconditionalSnap"]);
            // Reset the POI as just in case
            setPOIs();
            scanTypeSetup();

            // do the conditional fit
            RooFitResult* fitresultUnCondToy = NULL;
            RooNLLVar* nllUnCondToy = NULL;
            int statusUnCondToy = -999;


            // Fit the point
            //cout<<"------ unconditional ------"<<endl;
            fitPoint(fitresultUnCondToy, statusUnCondToy, nllUnCondToy, NULL, m_mcUnc);
            double valUnCond = 2*nllUnCondToy->getVal();


            // Load the toy snapshot
            m_ws->loadSnapshot("ToySetup");


            // do the conditional fit
            RooFitResult* fitresultCondToy = NULL;
            RooNLLVar* nllCondToy = NULL;
            int statusCondToy = -999;

            //cout<<"------ conditional ------"<<endl;
            fitPoint(fitresultCondToy, statusCondToy, nllCondToy);

            // Save the information
            m_outVarsDouble["nll"]              = 2.*nllCondToy->getVal();
            m_outVarsDouble["deltaNLL"]         = 2.*nllCondToy->getVal() - valUnCond;
            m_outVarsDouble["status"]           = statusCond;
            m_outVarsDouble["isUnconditional"]  = 0;
            m_outVarsDouble["fitType"]          = 0;
            m_outVarsDouble["toyNumber"]        = cToy;
            m_outVarsDouble["poi_generated"]    = m_lwrLim + i * m_delta;
            
            // Store variables
            fillVar(fitresultCondToy, nllCondToy);


            m_outFile->cd();
            m_outTree->Fill();

            delete fitresultCondToy;
            delete nllCondToy;
            delete fitresultUnCondToy;
            delete nllUnCondToy;

            delete m_dataSet;


        }
        

        delete toyGen;

        stopwatch.Stop();

        time.push_back(stopwatch.RealTime());

        //std::cout.flush();
        stopwatch.Reset();

        
    }

}

/*
void scan1DToys::runScan()
{
    TStopwatch stopwatch;
    vector <double> time;
    cout << "***************" << endl;

    RooArgSet* obsList= (RooArgSet*) m_mc->GetObservables();
    // Scan
    for(int i = 0; i < m_steps; i++)
    {
        cout<<"Setting poi to : "<<m_lwrLim + i * m_delta<<endl;

        m_poi->setVal(m_lwrLim + i * m_delta);
        m_poi->setConstant(true);

        m_ws->saveSnapshot("ToySetup", m_ws->allVars());

        auto* toyGen = new ToyMCSampler();
        toyGen->SetPdf( *m_mc->GetPdf() );
        toyGen->SetObservables( *m_mc->GetObservables() );
        toyGen->SetNuisanceParameters(*m_mc->GetNuisanceParameters());

        // generate a random seed
        int cSeed = 0; 
        // get seeds
        cSeed = m_opts.at("seed").Atoi();
        RooRandom::randomGenerator()->SetSeed(cSeed);

        for(int cToy = 0; cToy < m_nToys; cToy++)
        {
            m_ws->loadSnapshot("ToySetup");
            m_dataSet = (RooDataSet*) toyGen->GenerateToyData(*obsList);

            m_poi->setConstant(false);

            RooFitResult* fitresult = NULL;
            RooNLLVar* nll = NULL;
            int status = -999;

            fitPoint(fitresult, status, nll);

            m_outVarsDouble["nll"]              = 2.*nll->getVal();
            m_outVarsDouble["status"]           = status;
            m_outVarsDouble["isUnconditional"]  = 0;
            m_outVarsDouble["toyNumber"]        = cToy;

            // Store variables
            fillVar(fitresult, nll);

            m_outFile->cd();

            m_outTree->Fill();
            delete fitresult;
            delete nll;

        }

        delete toyGen;

        stopwatch.Stop();

        time.push_back(stopwatch.RealTime());

        //std::cout.flush();
        stopwatch.Reset();
    }
}
*/





