#ifndef _RUNTOYS_H
#define _RUNTOYS_H

#include "scanBase.h"
#include "TRandom3.h"

#include <string>
#include <iostream>
#include <list>
#include "RooAbsData.h"

#include "RooStats/ToyMCImportanceSampler.h"
#include "RooStats/SamplingDistribution.h"
#include "RooStats/AsymptoticCalculator.h" 

using namespace RooStats;
using namespace RooFit ;

class runToys: public scanBase
{
   	public:
   		runToys(std::map<TString, TString>& opts);
   		~runToys();

   	protected:
        void parseScanOpt() override;

  		void runScan() override;

        int m_Toys;
        ToyMCSampler* m_toy;
        RooDataSet* m_ToyData;
        RooWorkspace* m_ToyWS;

        std::list<RooAbsData*> m_toyDatasets;

        int m_nToys;
        int m_nToystoSkip;

        void modifyToyVars();
        void modifyPOIVars();
        void modifyNPVars();
        void toyFitSetup();
        void getToy();
        void addVars() override;


        void randomizeNP();


        // Scan
        void runScanFit();
        void runScanFit2D(TString poiInfo);
        void runScanFit1D(TString poiInfo);

        TRandom3 m_randomGenerator;



};


#endif


