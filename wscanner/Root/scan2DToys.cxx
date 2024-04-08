#include "scanBase.h"
#include "scan2DToys.h"

#include "RooStats/ToyMCImportanceSampler.h"
#include "RooStats/SamplingDistribution.h"
#include "RooStats/AsymptoticCalculator.h"

using namespace RooStats;

using namespace std;


// Base class 
scan2DToys::scan2DToys(std::map<TString, TString>& opts):scanBase(opts){}
scan2DToys::~scan2DToys(){}


void scan2DToys::parseScanOpt()
{
    std::vector<TString> splitPOIs = tokenizeStr(m_opts["poiScan"], ",");

    // Sanity check
    if(splitPOIs.size() != 3) showPOIError();


    std::vector<TString> POIvec1 = tokenizeStr(splitPOIs.at(0), ":");
    std::vector<TString> POIvec2 = tokenizeStr(splitPOIs.at(1), ":");
    std::vector<TString> POIvec3 = tokenizeStr(splitPOIs.at(2), ":");

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

    m_nToys     = POIvec3[1].Atoi();

    LOG(logINFO) << "poiName_1: " <<m_poiName_1;
    LOG(logINFO) << "steps_1: "   <<m_steps_1;
    LOG(logINFO) << "lwrLim_1: "  <<m_lwrLim_1;
    LOG(logINFO) << "uprLim_1: "  <<m_uprLim_1;

    LOG(logINFO) << "poiName_2: " <<m_poiName_2;
    LOG(logINFO) << "steps_2: "   <<m_steps_2;
    LOG(logINFO) << "lwrLim_2: "  <<m_lwrLim_2;
    LOG(logINFO) << "uprLim_2: "  <<m_uprLim_2;

    LOG(logINFO) << "nToys: "     <<m_nToys;

    m_poi_1 = m_ws->var(m_poiName_1);
    m_poi_2 = m_ws->var(m_poiName_2);

}
void scan2DToys::showPOIError()
{
    LOG(logERROR)<<"ERROR - poiScan var not in the correct format";
    LOG(logERROR)<<"ERROR - poiScan is "<<m_opts["poiScan"];
    LOG(logERROR)<<"ERROR - example is "<< "POI1:steps:lwrLim:uprLim,POI2:steps:lwrLim:uprLim,nToys:XXX";
    exit(1);
}


void scan2DToys::fillVar(RooFitResult* fitresult, RooNLLVar* nll)
{
    scanBase::fillVar(fitresult, nll);
    m_outVarsDouble["poi_1"] = m_ws->var(m_poiName_1)->getVal();
    m_outVarsDouble["poi_2"] = m_ws->var(m_poiName_2)->getVal();
}

void scan2DToys::addVars()
{
    scanBase::addVars();
    m_outVarsDouble["poi_1"]     = -999;
    m_outVarsDouble["poi_2"]     = -999;
    m_outVarsDouble["toyNumber"] = -999;
    m_outVarsDouble["nToys"]     = -999;
    m_outVarsDouble["deltaNLL"]  = -999;
    

    m_outVarsDouble["poi_1_generated"]     = -999;
    m_outVarsDouble["poi_2_generated"]     = -999;

}


void scan2DToys::runScan()
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
    for(int i = 0; i < m_steps_1; i++)
    {
        for(int j = 0; j < m_steps_2; j++)
        { 

            // Create the toy generation snapshot
            m_ws->loadSnapshot("preUnConditional");
            // set the POI
            m_poi_1->setVal(m_lwrLim_1 + i * m_delta_1);
            m_poi_1->setConstant(true);

            m_poi_2->setVal(m_lwrLim_2 + j * m_delta_2);
            m_poi_2->setConstant(true);
            m_ws->saveSnapshot("ToyGenSetup", m_ws->allVars());


            if(m_opts["debug"].EqualTo("true"))
            {
                LOG(logDEBUG)<<"---------------- Start of Toy Gen Setup ------------------";
                m_ws->allVars().Print("v");
                LOG(logDEBUG)<<"---------------- End of Toy Gen Setup ------------------";
            }



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
            LOG(logINFO)<<"Doing first dataset fit";
            fitPoint(fitresultUnCond, statusUnCond, nllUnCond, NULL, m_mcUnc);
            double valUnCond = 2*nllUnCond->getVal();


            // Set the point
            m_ws->loadSnapshot("ToyCondSetup");
            cout<<"Setting poi_1 to : "<<m_lwrLim_1 + i * m_delta_1<<endl;
            cout<<"Setting poi_2 to : "<<m_lwrLim_2 + j * m_delta_2<<endl;

            m_poi_1->setVal(m_lwrLim_1 + i * m_delta_1);
            m_poi_1->setConstant(true);

            m_poi_2->setVal(m_lwrLim_2 + j * m_delta_2);
            m_poi_2->setConstant(true);



            // do the conditional fit
            RooFitResult* fitresultCond = NULL;
            RooNLLVar* nllCond = NULL;
            int statusCond = -999;

            LOG(logINFO)<<"Doing second dataset fit";
            fitPoint(fitresultCond, statusCond, nllCond);

            // Save the information
            m_outVarsDouble["nll"]              = 2.*nllCond->getVal();
            m_outVarsDouble["deltaNLL"]         = 2.*nllCond->getVal() - valUnCond;
            m_outVarsDouble["status"]           = statusCond;
            m_outVarsDouble["isUnconditional"]  = 1;
            m_outVarsDouble["fitType"]          = 3;
            m_outVarsDouble["toyNumber"]        = -999;
            m_outVarsDouble["poi_1_generated"]  = m_lwrLim_1 + i * m_delta_1;
            m_outVarsDouble["poi_2_generated"]  = m_lwrLim_2 + j * m_delta_2;

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
                LOG(logINFO)<<"Doing first toy fit";
                fitPoint(fitresultUnCondToy, statusUnCondToy, nllUnCondToy, NULL, m_mcUnc);
                double valUnCond = 2*nllUnCondToy->getVal();


                // Load the toy snapshot
                m_ws->loadSnapshot("ToySetup");


                // do the conditional fit
                RooFitResult* fitresultCondToy = NULL;
                RooNLLVar* nllCondToy = NULL;
                int statusCondToy = -999;

                LOG(logINFO)<<"Doing the second toy fit";
                fitPoint(fitresultCondToy, statusCondToy, nllCondToy);

                // Save the information
                m_outVarsDouble["nll"]              = 2.*nllCondToy->getVal();
                m_outVarsDouble["deltaNLL"]         = 2.*nllCondToy->getVal() - valUnCond;
                m_outVarsDouble["status"]           = statusCond;
                m_outVarsDouble["isUnconditional"]  = 0;
                m_outVarsDouble["fitType"]          = 0;
                m_outVarsDouble["toyNumber"]        = cToy;
                m_outVarsDouble["poi_1_generated"]  = m_lwrLim_1 + i * m_delta_1;
                m_outVarsDouble["poi_2_generated"]  = m_lwrLim_2 + j * m_delta_2;
                
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

}






