#ifndef _METADATACLASS_H
#define _METADATACLASS_H

// c++ includes
#include <memory>
#include <vector>
#include <string.h>
#include <map>
#include <iostream>
#include <memory>

// singleton patternn -- only one copy of this class will ever exist https://refactoring.guru/design-patterns/singleton/cpp/example#example-0
// hold maps 
class MetadataContainer
{
   public:

      static MetadataContainer* GetInstance()
      {
          if(m_MDC != nullptr)
          {
             return m_MDC;
          }
          else
          {
              m_MDC = new MetadataContainer();
          }

          return m_MDC;
          
      };

      std::vector<std::string> getFlavourCategory();
      std::vector<std::string> getFitCategories();

      std::string getAnalType(){return m_analType;};
      std::string getSampleName(){return m_sampleName;};
      std::string getInputFileList(){return m_inputFileList;};
      std::string getOutputFileName(){return m_outputFileName;};
      std::string getProcessName(){return m_processName;};
      std::string getSOWJSONFile(){return m_SOWJSONfile;};
      std::string getCustomSOWJSONFile(){return m_CustomSOWJSONfile;};
      int getNEvents(){return m_nEvents;};
      
      void setMetadata(std::string key, std::string value);
      void setMetadata(std::string key, int value);

   protected:
      MetadataContainer();
      inline ~MetadataContainer(){};
      
      // singleton class pointer
      static MetadataContainer* m_MDC;

      std::string m_analType;
      std::string m_inputFileList;
      std::string m_outputFileName;
      std::string m_sampleName;
      std::string m_processName;
      std::string m_SOWJSONfile;
      std::string m_CustomSOWJSONfile;
      int m_nEvents;
};


typedef MetadataContainer MDC;


#endif
