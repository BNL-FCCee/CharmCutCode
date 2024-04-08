#ifndef _SCAN1D_H
#define _SCAN1D_H

#include <string>
#include <iostream>



class scan1D: public scanBase
{
   	public:
   		scan1D(std::map<TString, TString>& opts);
   		~scan1D();

   	protected:
   		void parseScanOpt() override;

   		void fillVar(RooFitResult* fitresult = NULL, RooNLLVar* nll = NULL) override;

		void addVars() override;

		void runScan() override;

   		TString m_poiName;
    	int   m_steps;
    	float m_lwrLim;
    	float m_uprLim;
    	float m_delta;

    	RooRealVar* m_poi;

};


#endif


