#ifndef _HISTOGRAMCLASS_H
#define _HISTOGRAMCLASS_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>

// root include
#include <TH1F.h>
#include <TH2F.h>

// local include
#include "CharmCutCode/MetadataContainer.h"

// Base class using template method pattern
class HistContainer 
{
   public:
      HistContainer()
      {

      }
      virtual ~HistContainer()
      {

      }

      TH1F* getFlavourCategoryHist()
      {
         static TH1F* hist = NULL;
         if(hist) return hist;

         auto flavour = MDC::GetInstance()->getFlavourCategory();
         auto processName = MDC::GetInstance()->getProcessName();

         hist = new TH1F(("scoreMap_1D_" + processName).c_str(), ("scoreMap_1D_" + processName).c_str(), flavour.size(), 0, flavour.size());
         registerHist(hist);
         int index = 0;
         for(const auto& v: flavour)
         {
            hist->GetXaxis()->SetBinLabel(index + 1, v.c_str());
            index++;
         }
         return hist;

      }

      TH1F* getFitCategoryHist()
      {
         static TH1F* hist = NULL;
         if(hist) return hist;

         auto flavour = MDC::GetInstance()->getFitCategories();
         auto processName = MDC::GetInstance()->getProcessName();

         hist = new TH1F(("scoreMapFitCategory_1D_" + processName).c_str(), ("scoreMapFitCategory_1D_" + processName).c_str(), flavour.size(), 0, flavour.size());
         registerHist(hist);
         int index = 0;
         for(const auto& v: flavour)
         {
            hist->GetXaxis()->SetBinLabel(index + 1, v.c_str());
            index++;
         }
         return hist;


      }

      std::map<std::string, TH2F*> getObsHistinFitCategory()
      {
         static  std::map<std::string, TH2F*> histList;
         if(histList.size() > 0) return histList;

         auto categories = MDC::GetInstance()->getFitCategories();

         for(const auto& c: categories)
         {

            // This is high keywored; It needs to be in ObsHist_Cat_XXX_Process_YYY. The WS building depends on this
            auto name = "ObsHist_Cat_" + c + "_Process_" + MDC::GetInstance()->getProcessName();
            auto hist = new TH2F(name.c_str(), name.c_str(), 250, 0, 250, 250, 0, 250);
            registerHist(hist);
            histList[c] = hist;
         }
      
         return histList;

      }

      std::vector<TH1*> getHistList(){return m_histList;};


   protected:
      std::vector<TH1*> m_histList;
      void registerHist(TH1* h)
      {
         m_histList.push_back(h);
      }

};


#endif

