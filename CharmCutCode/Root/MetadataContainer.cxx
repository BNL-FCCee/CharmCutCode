#include "CharmCutCode/MetadataContainer.h"


MetadataContainer* MetadataContainer::m_MDC = nullptr;


MetadataContainer::MetadataContainer()
{
}

void MetadataContainer::setMetadata(std::string key, std::string value)
{
    if(key == "inputFileList") m_inputFileList = value;
    else if(key == "outputFileName") m_outputFileName = value;
    else if(key == "sampleName") m_sampleName = value;
    else if(key == "processName") m_processName = value;
    else if(key == "analType") m_analType = value;
    else if(key == "SOWJSONfile") m_SOWJSONfile = value;
    else if(key == "CustomSOWJSONfile") m_CustomSOWJSONfile = value;
    else if(key == "DetectorVarFile") m_DetectorVarFile = value;
    else if(key == "DetectorVar") m_DetectorVar = value;
    else
    {
        std::cout<<"Did not recognize the key: "<<key<<std::endl;
        exit(1);
    }
}

void MetadataContainer::setMetadata(std::string key, int value)
{
    if(key == "nEvents") m_nEvents = value;
    else
    {
        std::cout<<"Did not recognize the key: "<<key<<std::endl;
        exit(1);
    }
}