#ifndef _AnalysisZHvvJJ_H
#define _AnalysisZHvvJJ_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

// local include
#include "CharmCutCode/MetadataContainer.h"
#include "CharmCutCode/HistContainer.h"

class AnalysisZHvvJJ 
{
   public:
      AnalysisZHvvJJ();
      virtual ~AnalysisZHvvJJ();

      // event loop and systematic loop
      void run();

      void finalize();

   protected:
      std::shared_ptr<HistContainer> m_histContainer;


};


#endif

