#ifndef _SCAN2DTOYS_H
#define _SCAN2DTOYS_H

#include <string>
#include <iostream>



class scan2DToys: public scanBase
{
   	public:
   		scan2DToys(std::map<TString, TString>& opts);
   		~scan2DToys();

    protected:
        void parseScanOpt() override;

        void showPOIError();

        void fillVar(RooFitResult* fitresult = NULL, RooNLLVar* nll = NULL) override;

        void addVars() override;

        void runScan() override;

        TString m_poiName_1;
        int   m_steps_1;
        float m_lwrLim_1;
        float m_uprLim_1;
        float m_delta_1;

        TString m_poiName_2;
        int   m_steps_2;
        float m_lwrLim_2;
        float m_uprLim_2;
        float m_delta_2;
        int   m_nToys;


        RooRealVar* m_poi_1;
        RooRealVar* m_poi_2;


};


#endif


