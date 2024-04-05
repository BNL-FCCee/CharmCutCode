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
#include "ROOT/RVec.hxx"

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


/// template specialisation for ROOT::VecOps::RVec branches/ 
/// These essentially behave like scalars, but provide additional 
/// convenient element-level accessors as well as iterators. 
template <typename T>
class varMember<ROOT::VecOps::RVec<T>> {
    public:
        inline varMember(TTree* tree, std::string name)
        {
            // make the branch active to be read
            tree->SetBranchStatus(name.c_str(), true);
            // Connect the branch
            tree->SetBranchAddress(name.c_str(), &var);
        };

        ROOT::VecOps::RVec<T>* getVar() {return var;};

        // get the element at index
        T at(int i) {return var->at(i); }

        // get the size of the vector
        int size() {return var->size(); }

        // Return the base object
        ROOT::VecOps::RVec<T>* operator() (){return var;}

    private:
        ROOT::VecOps::RVec<T>* var = NULL;

};
template <typename T>
class varMemberMap {
    public:
        inline varMemberMap(TTree* tree, std::vector<std::string> nameList, T defaultVal)
        {
            // Initialize the map with default value
            for(const auto& name: nameList) var[name]  = defaultVal;

            for (const auto& name: nameList)
            {
                // make the branch active to be read
                tree->SetBranchStatus(name.c_str(), true);
                // Connect the branch
                tree->SetBranchAddress(name.c_str(), &(var[name]));
            }
        };

        // inline T operator();
        T getVal(std::string name) {return var.at(name); }

    private:
        std::map<std::string, T> var;

};

template <typename T>
class varMemberVector {
    public:
        inline varMemberVector(TTree* tree, std::vector<std::string> nameList, T defaultVal)
        {
            // Initialize the vector with default value
            var.reserve(nameList.size());
            for(const auto& name: nameList) var.push_back(defaultVal);

            // Connect it
            for (size_t i = 0; i < nameList.size(); i++)
            {
                // make the branch active to be read
                tree->SetBranchStatus(nameList[i].c_str(), true);
                // Connect the branch
                tree->SetBranchAddress(nameList[i].c_str(), &(var.at(i)));
            }
        };

        // inline T operator();
        T getVal(int index) {return var.at(index); }

    private:
        std::vector<T> var;

};

class TreeContainer 
{
   public:
        TreeContainer();
        virtual ~TreeContainer() {};

        // Get the raw number of entries in the tree
        int getEntries(){return m_tree->GetEntries();};

        // Get events to run on
        int getEventsToRun()
        {
            int nEntries = getEntries();
            if(MDC::GetInstance()->getNEvents() > 0) nEntries = MDC::GetInstance()->getNEvents();
            return nEntries;
        }


        TTree* getTree(){return m_tree;};
        void getEntry(int i){ m_tree->GetEntry(i);};


   protected:
        TTree* m_tree;

        std::vector<TString> splitString(TString string, TString delimiter);

};



#endif

