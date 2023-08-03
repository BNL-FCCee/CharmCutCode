#ifndef _TREECONTAINERBASE_H
#define _TREECONTAINERBASE_H

// c++ includes
#include <map>
#include <vector>
#include <iostream>
#include <memory>

// local include
#include "CharmCutCode/MetadataContainer.h"

// Root include
#include <TString.h>
#include <TTree.h>
#include <algorithm>

template <typename T>
class varMember {
    public:
        inline varMember(TTree* tree, std::string name)
        {
            // make the branch active to be read
            tree->SetBranchStatus(name.c_str(), true);
            // Connect the branch
            tree->SetBranchAddress(name.c_str(), &var);
        };

        // inline T operator();
        T getVal() {return var; }

        T operator() (){return var;}


  private:
        T var;

};

class TreeContainer 
{
   public:
        TreeContainer();
        virtual ~TreeContainer() {};

        int getEntries(){return m_tree->GetEntries();};

        TTree* getTree(){return m_tree;};

        void getEntry(int i){ m_tree->GetEntry(i);};
        
        std::vector<TString> splitString(TString string, TString delimiter);

   protected:
        TTree* m_tree;

};



#endif

