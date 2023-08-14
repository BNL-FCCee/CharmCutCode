#include "CharmCutCode/MetadataContainer.h"


MetadataContainer* MetadataContainer::m_MDC = nullptr;


MetadataContainer::MetadataContainer()
{
}


std::vector<std::string> MetadataContainer::getFlavourCategory()
{
    std::vector<std::string> vlist {"B", "C", "S", "G"};
    return vlist;
}

std::vector<std::string> MetadataContainer::getFitCategories()
{   
    // Get the main flavour list
    auto flavourList = getFlavourCategory();
    // We want to further split this into 3 categories
    std::vector<std::string> vlist {"L", "M", "H"};

    // Do a catesian product between the two and return those as the fit categories
    std::vector<std::string> finalCat;
    for(const auto& flav: flavourList)
    {
        for(const auto& finCat: vlist)
        {
            finalCat.push_back(flav + finCat);
        }
    }

    return finalCat;
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