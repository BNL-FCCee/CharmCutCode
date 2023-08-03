// C++ includes
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>

// Root includes
#include <TString.h>

#include <WSMaker/WSMaker.h>


using namespace std;

map<TString,std::string> opts;
bool cmdline(int argc, char** argv, map<TString,std::string>& opts);
int main(int argc, char** argv)
{
    if (!cmdline(argc,argv,opts)) return 0;

    auto wsMaker = std::make_shared<WSMaker>(opts["inputFile"], opts["outputDir"], opts["jsonConfig"]);
    wsMaker->run();

    return 0;
}





bool cmdline(int argc, char** argv, map<TString,std::string>& opts)
{
    opts.clear();

    // defaults
    opts["inputFile"]       = "root-files/combinedRaw.root";
    opts["outputDir"]       = "ZHvv_anal_v1";
    opts["jsonConfig"]      = "root-files/combinedRaw.root";

    for (int i=1;i<argc;i++) {

        string opt=argv[i];

        if (opt=="--help") {
            cout<<"--debug           : event number for debugging (delimted by comma)"<<endl;

            return false;
        }

        if(0!=opt.find("--")) {
            cout<<"ERROR: options start with '--'!"<<endl;
            return false;
        }
        opt.erase(0,2);
        if(opt == "isGrid")             {opts["isGrid"] = "true"; continue;}

        if(opts.find(opt)==opts.end()) {
            cout<<"ERROR: invalid option '"<<opt<<"'!"<<endl;
            return false;
        }
        string nxtopt=argv[i+1];
        if(0==nxtopt.find("--")||i+1>=argc) {
            cout<<"ERROR: option '"<<opt<<"' requires value!"<<endl;
            return false;
        }
        opts[opt]=nxtopt;
        i++;
    }

    return true;
}


