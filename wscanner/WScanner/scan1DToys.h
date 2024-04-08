#ifndef _SCAN1DTOYS_H
#define _SCAN1DTOYS_H

#include <string>
#include <iostream>



class scan1DToys: public scanBase
{
   	public:
   		scan1DToys(std::map<TString, TString>& opts);
   		~scan1DToys();

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
        int   m_nToys;

        RooRealVar* m_poi;


};


#endif


