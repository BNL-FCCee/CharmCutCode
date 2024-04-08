#ifndef _RANKNP_H
#define _RANKNP_H

#include <string>
#include <iostream>

class rankNP: public scanBase
{
    public:
        rankNP(std::map<TString, TString>& opts);
        ~rankNP();

    protected:
        void parseScanOpt() override;

        void fillVar(RooFitResult* fitresult = NULL, RooNLLVar* nll = NULL) override;

        void addVars() override;

        void runScan() override;

        void scanPointHelper(TString fitType, TString NPName, RooArgSet* minosSet = NULL);




      std::vector<TString> m_paramsToRank;

};


#endif


