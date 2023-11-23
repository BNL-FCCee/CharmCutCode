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

        virtual void initialize()
        {
            m_histContainer = std::make_shared<HistContainer>();
        }


        // Main logic of analysis in here
        virtual void run() = 0;

        // Called to finalize the analysis
        // Write output, for now
        virtual void finalize()
        {
            writeHistogram();
        }

    protected:
        std::shared_ptr<HistContainer> m_histContainer;

        void writeHistogram();


};


#endif

