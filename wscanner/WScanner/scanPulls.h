#ifndef _SCANPULLS_H
#define _SCANPULLS_H

#include <string>
#include <iostream>



class scanPulls: public scanBase
{
    public:
        scanPulls(std::map<TString, TString>& opts);
        ~scanPulls();

    protected:
        void parseScanOpt() override;

        void fillVar(RooFitResult* fitresult = NULL, RooNLLVar* nll = NULL) override;

        void addVars() override;

        void runScan() override;

};


#endif


