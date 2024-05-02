#ifndef _HISTOGRAMCLASS_H
#define _HISTOGRAMCLASS_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <fstream>

// root include
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TFile.h>
// local include
#include "PostCutCode/MetadataContainer.h"

// Base class using template method pattern
class HistContainer 
{
   public:
      HistContainer();
      virtual ~HistContainer() {};

      // For knowing know many events were run over
      TH1F* getCountingHist();


      // Create generic TH1F that are registered in the Hist container
      TH1F* get1DHist(std::string name, int binX, double lwrX, double uprX, std::vector<std::string> optionalBinNames = {});

      // Create generic TH2F that are registered in the Hist container
      TH2F* get2DHist(std::string name, int binX, double lwrX, double uprX, int binY, double lwrY, double uprY);

      ////// Please only call one of them in the analysis, otherwise, there will be chaos
      // If it gets too much, we can add runtime check to see one of them has been called
      // Create 2D histograms that will be used for fit observable
      std::map<std::string, TH2F*> getObsHistinFitCategory(std::vector<std::string> fitCategories, int binX, double lwrX, double uprX, int binY, double lwrY, double uprY);
      // Create 1D histograms that will be used for fit observable
      std::map<std::string, TH1F*> getObsHistinFitCategory(std::vector<std::string> fitCategories, int binX, double lwrX, double uprX);
      // Load TH2 for DecVar effect on the score
      std::map<std::string, TH3*> histo_DetVarsScoreSmear(std::vector<std::string> jetFlavs);



      std::vector<TH1*> getHistList(){return m_histList;};


   protected:
      std::map<std::string, TH3*> histos_DetVars;
      std::vector<TH1*> m_histList;
      
      void registerHist(TH1* h)
      {
         m_histList.push_back(h);
      }

      // Check if the hist exists
      TH1* getHist(std::string name)
      {
         for(const auto& h: m_histList)
         {
            if(h->GetTitle() == name) return h;
         }
         return NULL;
      }

};


#endif

