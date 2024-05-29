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

      int getMaxScoreFlav(const std::vector<int>& vec, const std::map<int, std::vector<float>>& mapFlav);
      void getMissingPair(const std::vector<int>& foundPair, std::vector<int>& missingPair);


      // std::vector<unsigned int> missing_pair(std::vector<int>);

      void run() override;

   protected:


};


#endif

