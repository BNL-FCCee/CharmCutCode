#ifndef _AnalysisZHAllHad6J_H
#define _AnalysisZHAllHad6J_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>
//ROOT things

// local include
#include "PostCutCode/AnalysisBase.h"

class AnalysisZHAllHad6J: public AnalysisBase
{
   public:
      bool m_debug;
      AnalysisZHAllHad6J();
      virtual ~AnalysisZHAllHad6J();
      

      int  getMaxScoreFlav(const std::vector<int>& vec, const std::map<int, std::vector<float>>& mapFlav);
      void getMissingPair(const std::vector<int>& foundPair, std::vector<int>& missingPair);
      void printZcandidate(struct ZCandidate zCandidate);


      // std::vector<unsigned int> missing_pair(std::vector<int>);

      void run() override;

   protected:
      double mH_ZZ;
      double mZ1_jj;
      double mZ2_jj;
      double mZ3_jj;
      double Hscore;
      double Zscore;
      int Hflav;
      int Zflav;
      double ChiH;
      double ChiZ;
      double mc_weight;


};


#endif

