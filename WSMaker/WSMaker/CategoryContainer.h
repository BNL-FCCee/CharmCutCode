#ifndef _CATEGORYCONTAINER_H
#define _CATEGORYCONTAINER_H

// c++ includes
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

// Root includes
#include "TH1.h"
#include "TH1.h"

// local include
#include "WSMaker/CommonCode.h"

class CategoryContainer 
{
   public:
        CategoryContainer(std::string name)
        {
            m_catName = name;
        };
        virtual ~CategoryContainer() {};

        void setXRebin(int _x) {m_rebinX = _x; }
        void setYRebin(int _x) {m_rebinY = _x; }
        void setLumi(double _x) {m_lumi = _x; }
        void setOutputDir(std::string _x) {m_outputDir = _x; }
        void setConstantProc(std::vector<std::string> procList) {m_constProcList = procList; };

        /// Add a list of hist to this category
        /// key: Nominal <or systematic name>. Nominal is keyword
        /// std::map<ProcessName, TH1*> one for each process
        void addHist(std::string key, std::map<ProcessName, TH1*> histList)
        {
            m_histList[key] = histList;
        } 

        // Preprocess all the hist into information that neede
        void processHist();

        // Write the hist such that it will work for histfactory 
        void writeHistToRoot();

        // Write the category specific XML for histfactor
        void writeXML();

        // POI list for this category
        std::vector<std::string> getPOIList() {return m_poiList;};

        // Const vars for this category
        std::vector<std::string> getConstantVarList() {return m_constVarList; };

        // Name with full path for the output root file
        std::string getRootOutputFileName(){return m_outputDir + "/hist/histFile_" + m_catName + ".root"; };
        // Name with full path for the output XML file
        std::string getXMLFileName(){return m_outputDir + "/XML/" + m_catName + ".XML"; };

   protected:
        // Category name
        std::string m_catName;
        // OutputDir
        std::string m_outputDir;
        // Input hist
        std::map<NomSysName, std::map<ProcessName, TH1*>> m_histList;
        // Yields for each process
        std::map<NomSysName, std::map<ProcessName, double>> m_yieldList;
        // 1D histograms which are unrolled if the original list is 2D. Also removes bins with zero yeild
        std::map<NomSysName, std::map<ProcessName, TH1D*>> m_flatHistList;

        // Sum of all nominal 
        TH1* m_sumHist = NULL;


        // Rebin x factor
        int m_rebinX = 1;

        // Rebin y factor
        int m_rebinY = 1;

        // Luminosity - need to figure out units
        double m_lumi = 1;

        // rebins hist based on the settings
        void rebinHist();

        // Extracts yeild info for each checking
        void fillYieldInfo();

        // To clean up histograms; Right not removes process with nominal yield = 0
        void cleanHist();
        
        // Unrolls 2D hist, and removes non-zero bins
        void createFlatNonEmptyHist();
        // Clears over and underflow bins in the histogram
        void clearOverUnderFlow();

        // Gets the list of process in this category
        std::set<std::string> getProcessList();

        // POI list
        std::vector<std::string> m_poiList;

        // const var list
        std::vector<std::string> m_constVarList;

        // const process list
        std::vector<std::string> m_constProcList;



};


#endif

