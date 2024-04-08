#include "scanDatasets.h"

#include "TCollection.h"
#include "TKey.h"
#include "TObject.h"
#include "TObjArray.h"
#include <algorithm>

using namespace std;
using namespace RooStats;

scanDatasets::scanDatasets(std::map<TString, TString>& opts):scanBase(opts){}
scanDatasets::~scanDatasets(){}


void scanDatasets::doUnconditional()
{
    cout << "Skipping doUnconditional" << endl;
}
void scanDatasets::doUnconditionalUnparam()
{

    cout << "Skipping doUnconditionalUnparam" << endl;
}
 

void scanDatasets::parseScanOpt()
{

    auto scanInfoList = tokenizeStr(m_opts["poiScan"], ":");

    // check if the datasets to scan are in the ws or provided in another file
    TString datasetFileName = scanInfoList[1];
    int nDatasets       = scanInfoList[2].Atoi();
    int nDatasetstoSkip = scanInfoList[3].Atoi();

    // Get all the dataset name
    if(datasetFileName.EqualTo("inWorkspace")) 
    {
        m_datasetWS = m_ws;
    }
    else
    {
        TFile* cFile = new TFile(datasetFileName);
        TIter nextkey(cFile->GetListOfKeys());
        TKey *key;
        while ((key = (TKey*)nextkey())) 
        {
            TObject *Obj = key->ReadObj();
            if(Obj->IsA()->InheritsFrom("RooWorkspace"))
            {
                m_datasetWS = (RooWorkspace*) Obj;
                break;
            }
        }
    }

    if(!m_datasetWS)
    {
        LOG(logERROR)<<"Cannot find the workspace in the file";
        exit(1);
    }

    // Get All the RooDataset
    auto listDatasets = m_datasetWS->allData();
    auto startPOS = std::next(listDatasets.begin(), std::min(nDatasetstoSkip, int(listDatasets.size())));
    auto endPOS = std::next(listDatasets.begin(), std::min(nDatasets + nDatasetstoSkip, int(listDatasets.size())));
    list<RooAbsData*> dataSetList(startPOS, endPOS);
    LOG(logINFO)<<"Org Dataset list size "<<listDatasets.size();
    LOG(logINFO)<<"Dataset list size "<<dataSetList.size();
    m_datasets = dataSetList;
    m_nDatasets = nDatasets; 

}

void scanDatasets::fillVar(RooFitResult* fitresult, RooNLLVar* nll)
{
    scanBase::fillVar(fitresult, nll);
    // cout<<"Poi: "<<m_poiName<<endl;
    // m_outVarsDouble["poi"] = m_ws->var(m_poiName)->getVal();
}

void scanDatasets::addVars()
{
    scanBase::addVars();
    m_outVarsDouble["uprErr"]  = -999;
    m_outVarsDouble["lwrErr"]  = -999;
    m_outVarsDouble["cenVal"]  = -999;
    m_outVarsDouble["fitType"] = -999;
}

void scanDatasets::runScan()
{
    // // set parameters from config
    for(int i = 0; i < m_nDatasets; i++)
    {
        LOG(logDEBUG)<<"Doing dataset number: "<<i;

        setDataset();

        LOG(logDEBUG)<<"Doing fit";
        // do the fit and save the results

        // exit(1);
        // Scan the toy to find the minima
        runScanFit();

        fillVar();
        m_outVarsDouble["isUnconditional"]  = 0;
        m_outVarsDouble["fitType"]          = 5;
        m_outVarsDouble["nll"]              = m_scanVals["nll"];
        m_outVarsDouble["status"]           = m_scanVals["status"];
        m_outVarsDouble["uprErr"]           = m_scanVals["uprErr"];
        m_outVarsDouble["lwrErr"]           = m_scanVals["lwrErr"];
        m_outVarsDouble["cenVal"]           = m_scanVals["cenVal"];

        // save
        m_outFile->cd();
        m_outTree->Fill();


    }
}


void scanDatasets::setDataset()
{
    if(m_datasets.size() > 0)
    {
        LOG(logINFO)<<"Getting dataset from file "<<m_datasets.size();
        if(m_datasets.empty())
        {
            LOG(logERROR)<<"Ran out of datasets";
            exit(1);
        }
        m_dataSet = (RooDataSet*) *m_datasets.begin();
        m_datasets.pop_front();
    }   
    else
    {
        LOG(logINFO)<<"Ran out of datasets";
        exit(1);
    }
}

void scanDatasets::runScanFit()
{
    // Get all the options:
    TString toyScanKey = "scanDatasetsInfo_scanSetup_";
    map<TString, TString> scanOpts;
    int index = 0;
    bool isInfoAvail = true;
    bool scanToy = false;
    bool minosFit = false;
    cout << "MinosFit: " << minosFit << " scanToy " << scanToy << endl;

    std::map<TString, TString> scanDatasetOpts;

    do
    {
        TString indexName = toyScanKey + to_string(index);
        if(m_opts.find(indexName) == m_opts.end()) {isInfoAvail = false; continue;}

        
        // Do the parsing
        auto splitStr = tokenizeStr(m_opts.at(indexName), ":");
        scanDatasetOpts[splitStr.at(0)] = splitStr.at(1);

        vector<TString> remainVect{splitStr.begin() + 1, splitStr.end()};
        scanOpts[splitStr.at(0)] = joinList(remainVect, ":");
        index++;
    } while(isInfoAvail);

    if(scanDatasetOpts.find("Dimension") == scanDatasetOpts.end())
    {
        LOG(logERROR)<<"Can't find the dimension key for the toy scan";
        exit(1);
    }

    if(scanDatasetOpts.find("MinosFit") != scanDatasetOpts.end()) minosFit = true;
    else if(scanDatasetOpts.find("poiInfo") != scanDatasetOpts.end()) scanToy = true;

    if(minosFit && scanToy)
    {
        LOG(logERROR)<<"User is requesting a minosFit and a scan of the POI, please choose only 1";
        exit(1);
    }

    if(minosFit)
    {
        LOG(logINFO) << "Will use Minuit to get +- 1 sigma error on fit";
        runMinosFit(scanDatasetOpts.at("poiName"));
    } 

    else if(scanToy)
    {
         LOG(logINFO) << "Will do a nll scan to get +- 1 sigma error on fit";
        if(scanDatasetOpts.at("Dimension").EqualTo("1")) runScanFit1D(scanOpts.at("poiInfo"));
        else
        {
            LOG(logERROR)<<"Can't handle this dimension of the toy scan";
            LOG(logERROR)<<"Dim: "<<scanDatasetOpts.at("Dimension");
            exit(1);
        }
    }
}

void scanDatasets::runMinosFit(TString poiName)
{
    scanTypeSetup();

    auto poi = m_ws->var(poiName);

    RooFitResult* fitresult = NULL;
    RooNLLVar* nll = NULL;
    int status = -999;

    ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);

    fitPoint(fitresult, status, nll);

    m_scanVals["nll"]              = 2.*nll->getVal();
    m_scanVals["status"]           = status;
    m_scanVals["cenVal"] = poi->getVal();
    m_scanVals["lwrErr"] = poi->getErrorLo();
    m_scanVals["uprErr"] = poi->getErrorHi();

    delete nll;
    delete fitresult;

}

void scanDatasets::runScanFit1D(TString poiInfo)
{
    cout << "poiInfo:" << poiInfo << endl;
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
    bool isPOIConst = poi->isConstant();


    /// Uncond fit on the dataset
    std::map<double, double> nllMap;
    std::map<double, double> rawNLLMap;

    m_ws->loadSnapshot(m_opts["unconditionalSnap"]);

    RooFitResult* uncondfitresult = NULL;
    RooNLLVar* uncondnll = NULL;
    int uncondstatus = -999;

    fitPoint(uncondfitresult, uncondstatus, uncondnll);

    double uncondnllVal = 2*uncondnll->getVal();

    nllMap[poi->getVal()] = 0;


    // // comment in for the minos error
    // m_scanVals["cenVal"] = poi->getVal();
    // m_scanVals["lwrErr"] = poi->getErrorLo();
    // m_scanVals["uprErr"] = poi->getErrorHi();


    delete uncondnll;
    delete uncondfitresult;

    scanTypeSetup();

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
        // cout<<"----------------------------------"<<endl;
        poi->setVal(lwrLim + i * delta);
        poi->setConstant(true);
        // poi->Print();
    
        RooFitResult* fitresult = NULL;
        RooNLLVar* nll = NULL;
        int status = -999;

        fitPoint(fitresult, status, nll);

        nllMap[poi->getVal()] =  2*nll->getVal() - uncondnllVal;
        rawNLLMap[poi->getVal()] =  2*nll->getVal();

        delete fitresult;
        delete nll;

    }

    double outMinValX  = -999;
    double outLeftErr  = -999;
    double outRightErr = -999;


    // for(const auto& fit: nllMap)
    // {
    //     cout<<"NLL: "<<fit.first<<" "<<fit.second<<endl;
    // }

    fixOffset(nllMap);

    for(const auto& fit: nllMap)
    {
        cout<<"nllMap: "<<fit.first<<" "<<fit.second<<endl;
    }

    double outLeft2SigErr  = 0;
    double outRight2sigErr = 0;
    printFitInfo(nllMap, outMinValX, outLeftErr, outRightErr, outLeft2SigErr, outRight2sigErr);


    poi->setConstant(isPOIConst);


    m_scanVals["cenVal"] = outMinValX;
    m_scanVals["lwrErr"] = outLeftErr;
    m_scanVals["uprErr"] = outRightErr;


    for(const auto& fit: m_scanVals)
    {
        cout<<fit.first<<" "<<fit.second<<endl;
    }

    m_ws->loadSnapshot(m_opts["unconditionalSnap"]);
    poi->setVal(m_scanVals["cenVal"]);    

    RooFitResult* uncondfitresult1 = NULL;
    RooNLLVar* uncondnll1 = NULL;
    int uncondstatus1 = -999;

    fitPoint(uncondfitresult1, uncondstatus1, uncondnll1);
    rawNLLMap[poi->getVal()] =  2*uncondnll1->getVal();


    for(auto& rawInfo: rawNLLMap) rawInfo.second -= 2*uncondnll1->getVal();

    fixOffset(rawNLLMap);
    outLeft2SigErr  = 0;
    outRight2sigErr = 0;
    printFitInfo(rawNLLMap, outMinValX, outLeftErr, outRightErr, outLeft2SigErr, outRight2sigErr);

    // for(const auto& fit: rawNLLMap)
    // {
    //     cout<<"Raw nLL: "<<fit.first<<" "<<fit.second<<endl;
    // }
    m_scanVals["nll"]    = 2.*uncondnll1->getVal();
    m_scanVals["status"] = uncondstatus1;
    m_scanVals["cenVal"] = outMinValX;
    m_scanVals["lwrErr"] = outLeftErr;
    m_scanVals["uprErr"] = outRightErr;

    // for(const auto& fit: m_scanVals)
    // {
    //     cout<<fit.first<<" "<<fit.second<<endl;
    // }



}









