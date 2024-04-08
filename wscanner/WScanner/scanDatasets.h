#ifndef _SCANDATASETS_H
#define _SCANDATASETS_H

#include "scanBase.h"


#include <string>
#include <iostream>
#include <list>
#include "RooAbsData.h"

#include "RooStats/ToyMCImportanceSampler.h"
#include "RooStats/SamplingDistribution.h"
#include "RooStats/AsymptoticCalculator.h"

class scanDatasets: public scanBase
{
   	public:
   		scanDatasets(std::map<TString, TString>& opts);
   		~scanDatasets();

   	protected:
		void parseScanOpt() override;

		void runScan() override;

		void doUnconditional() override;

		void doUnconditionalUnparam() override;

   		void fillVar(RooFitResult* fitresult = NULL, RooNLLVar* nll = NULL) override;

		
		int m_nDatasets;
		RooWorkspace* m_datasetWS;
		std::list<RooAbsData*> m_datasets;

		std::map<TString, float> m_scanVals;


		void addVars() override;

		// Scan
		void runScanFit();
		void runScanFit2D(TString poiInfo);
		void runScanFit1D(TString poiInfo);

		void runMinosFit(TString poiName);

		void setDataset();

   		TString m_poiName;
    	int   m_steps;
    	float m_lwrLim;
    	float m_uprLim;
    	float m_delta;

    	RooRealVar* m_poi;


};


#endif


