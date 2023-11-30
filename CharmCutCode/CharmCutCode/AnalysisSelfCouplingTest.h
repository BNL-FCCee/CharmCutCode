#ifndef _AnalysisSelfCouplingTest_H
#define _AnalysisSelfCouplingTest_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

// local include
#include "CharmCutCode/AnalysisBase.h"

class AnalysisSelfCouplingTest: public AnalysisBase
{
   public:
      AnalysisSelfCouplingTest();
      virtual ~AnalysisSelfCouplingTest();

      void run() override;

   protected:


};


#endif

