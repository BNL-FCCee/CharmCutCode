#ifndef _AnalysisZHAllHad_H
#define _AnalysisZHAllHad_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

// local include
#include "PostCutCode/AnalysisBase.h"

class AnalysisZHAllHad: public AnalysisBase
{
   public:
      bool m_debug;
      AnalysisZHAllHad();
      virtual ~AnalysisZHAllHad();

      // std::vector<unsigned int> missing_pair(std::vector<int>);

      void run() override;

   protected:


};


#endif

