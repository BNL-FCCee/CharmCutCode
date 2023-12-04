#include "CharmCutCode/MetadataContainer.h"
#include "CharmCutCode/TreeContainer.h"

#include "TChain.h"
#include <TObjString.h>

TreeContainer::TreeContainer()
{
    auto fileList = splitString(MDC::GetInstance()->getInputFileList(), ",");
    TChain* chain = new TChain("events");
    for(const auto& fileName: fileList) chain->Add(fileName);
    m_tree = (TTree*) chain;

    // Stop all branches from reading
    m_tree->SetBranchStatus("*", false);

}


std::vector<TString> TreeContainer::splitString(TString string, TString delimiter)
{
    TObjArray *Varparts = string.Tokenize(delimiter);
    std::vector<TString> varNameVec;
    if(Varparts->GetEntriesFast()) {
        TIter iString(Varparts);
        TObjString* os=0;
        while ((os=(TObjString*)iString())) {
            varNameVec.push_back(os->GetString());
        }
    }
    delete Varparts;
    return varNameVec;
}