#include "runToys.h"

#include "TCollection.h"
#include "TKey.h"
#include "TObject.h"
#include "TObjArray.h"
#include <algorithm>

using namespace std;
using namespace RooStats;

runToys::runToys(std::map<TString, TString>& opts):scanBase(opts)
{
    m_ToyData = NULL;
    m_ToyWS = NULL;



}
runToys::~runToys(){}


 

void runToys::parseScanOpt()
{
    // get numToys or list of toys from the workspace
    auto toyInfoList = tokenizeStr(m_opts["poiScan"], ":");
    if(toyInfoList[1].IsFloat())
    {
        m_Toys = toyInfoList[1].Atoi();
        LOG(logINFO) << "Number of toys to be run: " << m_Toys; 
    }
    else
    {
        // Lets extract all the info from the workspace
        TString toyFileName = toyInfoList[1];
        if(toyFileName.EqualTo("inWorkspace")) toyFileName = m_opts["fileName"];
        m_nToys       = toyInfoList[2].Atoi();
        m_nToystoSkip = toyInfoList[3].Atoi();

        // Get all the toy name
        TFile* cFile = new TFile(toyFileName);

        TIter nextkey(cFile->GetListOfKeys());
        TKey *key;
        while ((key = (TKey*)nextkey())) 
        {
            TObject *Obj = key->ReadObj();
            if(Obj->IsA()->InheritsFrom("RooWorkspace"))
            {
                m_ToyWS = (RooWorkspace*) Obj;
                break;
            }
        }

        if(!m_ToyWS)
        {
            LOG(logERROR)<<"Cannot find the workspace in the file";
            exit(1);
        }


        // Get All the RooDataset
        auto listDatasets = m_ToyWS->allData();
        auto startPOS = std::next(listDatasets.begin(), std::min(m_nToystoSkip, int(listDatasets.size())));
        auto endPOS = std::next(listDatasets.begin(), std::min(m_nToys + m_nToystoSkip, int(listDatasets.size())));
        list<RooAbsData*> dataSetList(startPOS, endPOS);
        LOG(logINFO)<<"Org Dataset list size"<<listDatasets.size();
        LOG(logINFO)<<"Dataset list size"<<dataSetList.size();
        m_toyDatasets = dataSetList;
        m_Toys = m_nToys; 
    }
}



void runToys::modifyToyVars()
{
    TString indexBase = "ToysInfo_OtherSetup_";

    if(m_opts.find(indexBase+"0") != m_opts.end()) 
    {
        RooArgSet allVars = m_ws->allVars();
        setParameters(indexBase, &allVars);
    }
    else
    {
        LOG(logERROR)<<"Can't find "<<indexBase;
        exit(1);
    }
}
void runToys::modifyPOIVars()
{
    TString indexBase = "ToysInfo_POISetup_";

    auto POIList = m_mc->GetParametersOfInterest();
    if(m_opts.find(indexBase+"0") != m_opts.end()) 
    {
        setParameters(indexBase, POIList);
    }
    else
    {
        LOG(logERROR)<<"Can't find "<<indexBase;
        exit(1);
    }

}

void runToys::modifyNPVars()
{
    TString indexBase = "ToysInfo_NPSetup_";

    auto sysList = m_mc->GetNuisanceParameters();
    if(m_opts.find(indexBase+"0") != m_opts.end()) 
    {
        setParameters(indexBase, sysList);
    }
    else
    {
        LOG(logERROR)<<"Can't find "<<indexBase;
        exit(1);
    }
}

void runToys::toyFitSetup()
{
    TString indexBase = "ToysInfo_FitSetup_";

    if(m_opts.find(indexBase+"0") != m_opts.end()) 
    {
        RooArgSet allVars = m_ws->allVars();
        setParameters(indexBase, &allVars);

        if(m_opts["debug"].EqualTo("true"))
        {
            LOG(logDEBUG)<<"---------------- Start of Toy fit Setup ------------------";
            m_ws->allVars().Print("v");
            LOG(logDEBUG)<<"---------------- End of Toy fit Setup ------------------";
        }

    }
    else 
    {
        LOG(logERROR)<<"Can't find "<<indexBase;
        exit(1);
    }
}


void runToys::getToy()
{

    if(m_toyDatasets.size() > 0)
    {
        LOG(logINFO)<<"Getting toys from file "<<m_toyDatasets.size();
        if(m_toyDatasets.empty())
        {
            LOG(logERROR)<<"Ran out of toys";
            exit(1);

        }


        m_dataSet = (RooDataSet*) *m_toyDatasets.begin();
        m_toyDatasets.pop_front();

    }   
    else
    {
        LOG(logINFO)<<"Generating toys data ";

        if(m_ToyData) delete m_ToyData;

        // load that special snap
        m_ws->loadSnapshot("ToySetup");

        randomizeNP();

        RooArgSet* obsList= (RooArgSet*) m_mc->GetObservables();
        m_dataSet = (RooDataSet*) m_toy->GenerateToyData(*obsList);
        // For caching the results;
        m_ToyData = m_dataSet;
    }
}

void runToys::randomizeNP()
{
    auto sysList = m_mc->GetNuisanceParameters();

    if(m_opts.find("randomizeNPGenToys") == m_opts.end()) return;
    if(!m_opts["randomizeNPGenToys"].EqualTo("true")) return;

    LOG(logINFO)<<"Randomize NP";


    TIterator* itr = sysList->createIterator();
    RooRealVar* var;
    while ((var = (RooRealVar*)itr->Next())) 
    {
        double ranVal = m_randomGenerator.Gaus();
        cout<<var->GetName()<<" "<<ranVal<<endl;

        var->setVal(ranVal);
    }

}


void runToys::addVars()
{
    scanBase::addVars();
    m_outVarsDouble["deltaNLL"]  = -999;
    m_outVarsDouble["seed"]         = 0;
}


void runToys::runScan()
{   
    
    modifyToyVars();
    modifyPOIVars();
    modifyNPVars();
    // set systematics
    scanTypeSetup();
    
    m_ws->saveSnapshot("ToySetup", m_ws->allVars());

    m_toy = new ToyMCSampler();
    m_toy->SetPdf( *m_mc->GetPdf() );
    m_toy->SetObservables( *m_mc->GetObservables() );
    m_toy->SetNuisanceParameters(*m_mc->GetNuisanceParameters());

    // generate a random seed
    int cSeed = 0; 
    // get seeds
    cSeed = m_opts["seed"].Atoi();
    RooRandom::randomGenerator()->SetSeed(cSeed);
    m_randomGenerator.SetSeed(cSeed);


    // set parameters from config and save snapshot
    for(int i = 0; i < m_Toys; i++)
    {
        LOG(logDEBUG)<<"Toy i: "<<i;

        // gen the toy
        getToy();

        // Get the toy
        toyFitSetup();

        LOG(logDEBUG)<<"Doing first toy fit";
        // do the fit and save the results
        RooFitResult* fitresult = NULL;
        RooNLLVar* nll = NULL;
        int status = -999;

        fitPoint(fitresult, status, nll);

        double conditionalNLL = 2.*nll->getVal();

        // save
        fillVar();
        m_outVarsDouble["isUnconditional"]   = 0;
        m_outVarsDouble["fitType"]           = -1;
        m_outVarsDouble["deltaNLL"]          = -1;
        if(m_toyDatasets.size() > 0)
        {
            m_outVarsDouble["seed"] = m_nToystoSkip + i;
        }
        else m_outVarsDouble["seed"]       = cSeed; 
        m_outVarsDouble["nll"]        = 2.*nll->getVal();
        m_outVarsDouble["status"]     = status;

        m_outFile->cd();
        m_outTree->Fill();

        delete fitresult;
        delete nll;

        // set POIs
        setPOIs();
        // set systematics
        scanTypeSetup();
        // set any other variables
        modifyVars();

        // Scan the toy to find the minima
        runScanFit();

        LOG(logDEBUG)<<"Doing second toy fit";
        // fit toy again
        fitPoint(fitresult, status, nll, NULL, m_mcUnc);

        // compute shit
        double deltaNLL =  conditionalNLL - 2.*nll->getVal();

        // save
        fillVar();
        m_outVarsDouble["isUnconditional"]   = 0;
        m_outVarsDouble["fitType"]           = 0;
        m_outVarsDouble["deltaNLL"]   = deltaNLL;
        if(m_toyDatasets.size() > 0)
        {
            m_outVarsDouble["seed"] = m_nToystoSkip + i;
        }
        else m_outVarsDouble["seed"]       = cSeed; 
        m_outVarsDouble["nll"]        = 2.*nll->getVal();
        m_outVarsDouble["status"]     = status;

        m_outFile->cd();
        m_outTree->Fill();

        delete fitresult;
        delete nll;

    }
}

void runToys::runScanFit()
{
    
    // If the outputside world hasn't specified this scan
    // Just simply return   
    if(!m_opts["scanToy"].EqualTo("true")) return;


    // Get all the options:
    TString toyScanKey = "ToysInfo_scanSetup_";
    map<TString, TString> scanOpts;
    int index = 0;
    bool isInfoAvail = true;
    do
    {
        TString indexName = toyScanKey + to_string(index);
        if(m_opts.find(indexName) == m_opts.end()) {isInfoAvail = false; continue;}
        // Do the parsing
        auto splitStr = tokenizeStr(m_opts.at(indexName), ":");

        vector<TString> remainVect{splitStr.begin() + 1, splitStr.end()};
        scanOpts[splitStr.at(0)] = joinList(remainVect, ":");
        index++;
    } while(isInfoAvail);


    if(scanOpts.find("Dimension") == scanOpts.end())
    {
        LOG(logERROR)<<"Can't find the dimension key for the toy scan";
        exit(1);
    }
    if(scanOpts.find("poiInfo") == scanOpts.end())
    {
        LOG(logERROR)<<"Can't find the poiInfo key for the toy scan";
        exit(1);
    }


    if(scanOpts.at("Dimension").EqualTo("2")) runScanFit2D(scanOpts.at("poiInfo"));
    else if(scanOpts.at("Dimension").EqualTo("1")) runScanFit1D(scanOpts.at("poiInfo"));
    else
    {
        LOG(logERROR)<<"Can't handle this dimension of the toy scan";
        LOG(logERROR)<<"Dim: "<<scanOpts.at("Dimension");
        exit(1);
    }
}


void runToys::runScanFit2D(TString poiInfo)
{

    LOG(logDEBUG)<<"poiInfo: "<<poiInfo;

    std::vector<TString> splitPOIs = tokenizeStr(poiInfo, ",");

    std::vector<TString> POIvec1 = tokenizeStr(splitPOIs.at(0), ":");
    std::vector<TString> POIvec2 = tokenizeStr(splitPOIs.at(1), ":");

    auto poiName_1 = POIvec1[0];
    auto steps_1   = POIvec1[1].Atoi();
    auto lwrLim_1  = POIvec1[2].Atof();
    auto uprLim_1  = POIvec1[3].Atof();
    auto delta_1   = (uprLim_1 - lwrLim_1)/steps_1;

    auto poiName_2 = POIvec2[0];
    auto steps_2   = POIvec2[1].Atoi();
    auto lwrLim_2  = POIvec2[2].Atof();
    auto uprLim_2  = POIvec2[3].Atof();
    auto delta_2   = (uprLim_2 - lwrLim_2)/steps_2;

    LOG(logDEBUG) << "poiName_1: " <<poiName_1;
    LOG(logDEBUG) << "steps_1: "   <<steps_1;
    LOG(logDEBUG) << "lwrLim_1: "  <<lwrLim_1;
    LOG(logDEBUG) << "uprLim_1: "  <<uprLim_1;

    LOG(logDEBUG) << "poiName_2: " <<poiName_2;
    LOG(logDEBUG) << "steps_2: "   <<steps_2;
    LOG(logDEBUG) << "lwrLim_2: "  <<lwrLim_2;
    LOG(logDEBUG) << "uprLim_2: "  <<uprLim_2;

    auto poi_1 = m_ws->var(poiName_1);
    auto poi_2 = m_ws->var(poiName_2);

    double bestFitNll = -std::numeric_limits<double>::infinity();
    m_ws->saveSnapshot("bestFitPointToyScan", m_ws->allVars());

    bool isPOI_1Const = poi_1->isConstant();
    bool isPOI_2Const = poi_2->isConstant();

    // Scan
    for(int i = 0; i < steps_1; i++)
    {
        for(int j = 0; j < steps_2; j++)
        {

            LOG(logDEBUG)<<"Setting poi_1 to : "<<lwrLim_1 + i * delta_1;
            LOG(logDEBUG)<<"Setting poi_2 to : "<<lwrLim_2 + j * delta_2;


            poi_1->setVal(lwrLim_1 + i * delta_1);
            poi_1->setConstant(true);

            poi_2->setVal(lwrLim_2 + j * delta_2);
            poi_2->setConstant(true);
        
            RooFitResult* fitresult = NULL;
            RooNLLVar* nll = NULL;
            int status = -999;

            fitPoint(fitresult, status, nll);

            double cNLL = 2*nll->getVal();

            if(cNLL < bestFitNll)
            {
                bestFitNll = cNLL;
                // Snapshot
                m_ws->saveSnapshot("bestFitPointToyScan", m_ws->allVars());
            }

            delete fitresult;
            delete nll;
        }
    }

    // load the snap shot
    m_ws->loadSnapshot("bestFitPointToyScan");
    poi_1->setConstant(isPOI_1Const);
    poi_2->setConstant(isPOI_2Const);


}
void runToys::runScanFit1D(TString poiInfo)
{
    std::vector<TString> parsedScanVec = tokenizeStr(poiInfo, ":");

    // Sanity check
    if(parsedScanVec.size() != 4)
    {
        LOG(logERROR) << "poiScan var not in the correct format";
        LOG(logERROR) << "poiScan is " << m_opts["poiScan"];
        exit(1);
    }

    // Parse the information
    auto poiName   = parsedScanVec[0];
    double steps     = parsedScanVec[1].Atoi();
    double lwrLim    = parsedScanVec[2].Atof();
    double uprLim    = parsedScanVec[3].Atof();
    double delta     = (uprLim - lwrLim)/steps;
    //double xsVal  = getXS(poiName, ws, mc, opts);
    
    LOG(logDEBUG) << "poiName: " << poiName;
    LOG(logDEBUG) << "steps: "   << steps;
    LOG(logDEBUG) << "lwrLim: "  << lwrLim;
    LOG(logDEBUG) << "uprLim: "  << uprLim;

    auto poi = m_ws->var(poiName);

    double bestFitNll = -std::numeric_limits<double>::infinity();
    m_ws->saveSnapshot("bestFitPointToyScan", m_ws->allVars());
    bool isPOIConst = poi->isConstant();

    // Scan
    for(int i = 0; i < steps; i++)
    {
        if(m_opts.find("loadunconditionalSnapBeforeEveryFit") != m_opts.end())
        {        
            if(m_opts["loadunconditionalSnapBeforeEveryFit"].EqualTo("true"))
            {
                m_ws->loadSnapshot(m_opts["unconditionalSnap"]);
                // set scan type override
                scanTypeSetup();
            }
        }
        


        poi->setVal(lwrLim + i * delta);
        poi->setConstant(true);
    
        RooFitResult* fitresult = NULL;
        RooNLLVar* nll = NULL;
        int status = -999;

        fitPoint(fitresult, status, nll);

        double cNLL = 2*nll->getVal();
        if(cNLL < bestFitNll)
        {
            bestFitNll = cNLL;
            // Snapshot
            m_ws->saveSnapshot("bestFitPointToyScan", m_ws->allVars());
        }

        delete fitresult;
        delete nll;

    }

    // load the snap shot
    m_ws->loadSnapshot("bestFitPointToyScan");
    poi->setConstant(isPOIConst);
}










