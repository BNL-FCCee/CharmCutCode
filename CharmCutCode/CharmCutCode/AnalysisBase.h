#ifndef _AnalysisBASEFCEE_H
#define _AnalysisBASEFCEE_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

// local include
#include "CharmCutCode/MetadataContainer.h"
#include "CharmCutCode/HistContainer.h"

class AnalysisBase 
{
    public:
        AnalysisBase() {};
        virtual ~AnalysisBase() {};

        virtual void initialize() = 0;

        virtual void run() = 0;

        virtual void finalize() = 0;

    protected:
        std::shared_ptr<HistContainer> m_histContainer;


};


#endif

