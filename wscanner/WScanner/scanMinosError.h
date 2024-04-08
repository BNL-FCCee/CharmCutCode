#ifndef _SCANMIONSERROR_H
#define _SCANMIONSERROR_H

#include <string>
#include <iostream>

class scanMinosError: public scanBase
{
    public:
        scanMinosError(std::map<TString, TString>& opts);
        ~scanMinosError();

    protected:
        void parseScanOpt() override;

        void fillVar(RooFitResult* fitresult = NULL, RooNLLVar* nll = NULL) override;

        void addVars() override;

        void runScan() override;

        std::vector<TString> m_paramsToFit;

};


#endif


