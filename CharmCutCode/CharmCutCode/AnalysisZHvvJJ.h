#ifndef _AnalysisZHvvJJ_H
#define _AnalysisZHvvJJ_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

// local include
#include "CharmCutCode/AnalysisBase.h"

class AnalysisZHvvJJ: public AnalysisBase
{
   public:
      AnalysisZHvvJJ();
      virtual ~AnalysisZHvvJJ();

      void initialize() override {};

      void run() override;

      void finalize() override; 

   protected:


};


#endif

