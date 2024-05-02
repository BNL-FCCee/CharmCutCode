#ifndef _AnalysisZHvvJJ_H
#define _AnalysisZHvvJJ_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

// local include
#include "PostCutCode/AnalysisBase.h"

class AnalysisZHvvJJ: public AnalysisBase
{
   public:
      AnalysisZHvvJJ();
      virtual ~AnalysisZHvvJJ();

      void run() override;

   protected:


};


#endif

