#ifndef _WSMAKER_H
#define _WSMAKER_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

// ROOT include
#include <TDirectoryFile.h>
#include <TH1.h>

// local include
#include "WSMaker/CategoryContainer.h"
#include "WSMaker/CommonCode.h"
#include <nlohmann/json.hpp>

class WSMaker 
{
   public:
          WSMaker(std::string fileName, std::string outputDir, std::string jsonConfig)
          {
               m_fileName = fileName;
               m_outputDir = outputDir;
               m_jsonFile = jsonConfig;
               m_doSys = false; // NOT tested!!
          };
          virtual ~WSMaker() {};

          // event loop 
          void run();

   protected:
          std::string m_fileName;
          std::string m_jsonFile;
          std::string m_outputDir;
          bool m_doSys;

          // Parses the json config
          void parseConfig();
          nlohmann::json m_jsonConfigData;
          std::vector<CatName> m_categoryList;
          std::vector<CatName> m_categoryForStatErrorList;
          std::vector<ProcessName> m_processList;
          std::vector<CatName> m_processHeldConstantList;

          // To read the input file
          std::map<NomSysName, std::vector<TH1*>> readFile();

          // Helper to read a given folder inside the 
          std::vector<TH1*> readFromFolder(TDirectoryFile* obj);

          // Storing the points to the class that processes each category
          std::map<CatName, std::shared_ptr<CategoryContainer>> m_categoryCont;

          void WriteXML();


};


#endif

