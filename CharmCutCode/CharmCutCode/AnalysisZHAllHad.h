#ifndef _AnalysisZHAllHad_H
#define _AnalysisZHAllHad_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

// local include
#include "CharmCutCode/AnalysisBase.h"

class AnalysisZHAllHad: public AnalysisBase
{
   public:
      AnalysisZHAllHad();
      virtual ~AnalysisZHAllHad();

      void run() override;

   protected:


};


#endif

