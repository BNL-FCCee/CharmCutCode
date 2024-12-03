#ifndef _AnalysisZHAllHad6J_H
#define _AnalysisZHAllHad6J_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

// local include
#include "PostCutCode/AnalysisBase.h"

class AnalysisZHAllHad6J: public AnalysisBase
{
   public:
      bool m_debug;
      AnalysisZHAllHad6J();
      virtual ~AnalysisZHAllHad6J();

      // std::vector<unsigned int> missing_pair(std::vector<int>);

      void run() override;

   protected:


};


#endif

