#include "commonCode.h"
#include "InterpolatedGraph.h"
#include "log.h"

#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <set>

using namespace std;
 
std::vector<TString> tokenizeStr(TString str, TString key)
{
    TObjArray *Varparts = str.Tokenize(key);
    vector<TString> varNameVec;
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

std::map<TString, TString> readMetadata(TTree* tree)
{
    map<TString,TString> metaData;

    TObjArray* listLf = tree->GetListOfLeaves();

    std::map<std::string, TString*> m_stringMetaDataVar;

    for (Int_t i = 0; i < listLf->GetEntries(); i++)
    {
        TLeaf* lf = (TLeaf*) listLf->At(i);

        TString typeName   = lf->GetTypeName();
        TString titleName  = lf->GetTitle();

        if(typeName.Contains("TString"))
        {
            m_stringMetaDataVar[lf->GetName()] = 0;
        }
    }   

    for (auto it = m_stringMetaDataVar.begin(); it != m_stringMetaDataVar.end(); ++it)
    {
        TString varName = it->first;
        tree->SetBranchAddress(varName, &it->second);
    }

    tree->GetEntry(0);

    // Copy over the map
    for (auto it = m_stringMetaDataVar.begin(); it != m_stringMetaDataVar.end(); ++it)
    {
        TString varName = it->first;
        metaData[it->first] = *it->second;
    }

    TString dataType = "";
    if(metaData["doData"].Contains("true")) dataType = "data";
    else dataType = "asimov";

    std::map<TString, TString> infoMap;

    infoMap["scanType"] = metaData["scanType"];
    infoMap["dataType"] = dataType;
    infoMap["label"] = "";
    dataType.ToLower();
    
    if(dataType.Contains("asimov")) infoMap["label"] += "Exp";
    else                            infoMap["label"] += "Obs";
    
    metaData["scanType"].ToLower();
    if(metaData["scanType"].Contains("statonly"))   infoMap["label"] += "-noSys";
    else if(metaData["scanType"].Contains("modtheory"))   infoMap["label"] += "-modTh";
    else                                            infoMap["label"] += "-Sys";

    // cout<<metaData["scanType"]<<" "<<infoMap["label"]<<endl;

    infoMap["dimension"]        = metaData["dimension"];
    infoMap["plotConfig"]       = metaData["plotConfig"];

    if(infoMap["dimension"].Contains("1D") or infoMap["dimension"].EqualTo("1"))
    {
        std::vector<TString> poiName = tokenizeStr(metaData["poiScan"], ":");
        infoMap["poi"] = poiName[0];
    }
    else if(infoMap["dimension"].Contains("2D") or infoMap["dimension"].EqualTo("2"))
    {
        std::vector<TString> poiName = tokenizeStr(metaData["poiScan"], ",");
        if(poiName.size() != 2)
        {
            cout<<"number of POI specified don't match what is required for 2D"<<endl;
            cout<<metaData["poiScan"]<<endl;
        }

        auto poi1Info = tokenizeStr(poiName[0], ":");
        auto poi2Info = tokenizeStr(poiName[1], ":");
        infoMap["poi_1"] = poi1Info[0];
        infoMap["poi_2"] = poi2Info[0];

        double deltaPOI1 = (poi1Info.at(3).Atof() - poi1Info.at(2).Atof())/poi1Info.at(1).Atof();
        double deltaPOI2 = (poi2Info.at(3).Atof() - poi2Info.at(2).Atof())/poi2Info.at(1).Atof();


        infoMap["poi_1_delta"] = std::to_string(deltaPOI1);
        infoMap["poi_2_delta"] = std::to_string(deltaPOI2);
    }
    else if(infoMap["dimension"].EqualTo("correlationHist"))
    {
        // Do nothing for now
    }
    else
    {
        cout<<"Input dimension not recognized"<<endl;
        cout<<"Input option: "<<infoMap["dimension"]<<endl;
        throw "Input dimension not recognized";
    }


    infoMap["fileName"]        = metaData.at("fileName");
    infoMap["workspaceName"]   = metaData.at("workspaceName");
    infoMap["modelConfigName"] = metaData.at("modelConfigName");


    return infoMap;
}






TString getFancyLegname(TString name)
{
    return name.ReplaceAll("noSys", "Stat. only").ReplaceAll("noTh", "No Theory");
}

vector<TString> getVariableList(TTree *tree)
{
    TObjArray* listLf = tree->GetListOfLeaves();

    vector<TString> varList;
    for (Int_t i = 0; i < listLf->GetEntries(); i++)
    {
        TLeaf* lf = (TLeaf*) listLf->At(i);

        TString typeName   = lf->GetTypeName();
        std::string brName = lf->GetTitle();
        TString titleName  = lf->GetTitle();

        if(titleName.Contains("alpha"))
        {
            varList.push_back(titleName);
        }
    }
    return varList;
}

std::vector<std::map<TString, TString>> getMetadata(std::vector<TString> fileNameList)
{

    std::vector<std::map<TString, TString>> myLabels;
    for(auto file:fileNameList)
    {
        TFile *fileName = TFile::Open(file, "r");
        TTree *tree = (TTree*)fileName->Get("metaData");
        // // read the metaData

        std::map<TString, TString> infoMap = readMetadata(tree);
        myLabels.push_back(infoMap);

        fileName->Close();
    }
    return myLabels;
}
map<double,double> read1DVar(TString varName, TTree* tree)
{
    map<double,double> info;

    double var;
    double poi;
    double isUnconditional;
    double fitType;

    tree->SetBranchAddress("poi", &poi);         
    tree->SetBranchAddress(varName, &var);
    tree->SetBranchAddress("isUnconditional",   &isUnconditional);
    tree->SetBranchAddress("fitType",   &fitType);

    double min = 0;
    if(varName.EqualTo("nll"))
    {
        min = tree->GetMinimum("nll");
    }

    double sumVarConditional = 0;
    int    countVarConditional = 0;
    double averVarConditional = 0;

    // Calculate the average of the unconditional NLL
    if(varName.EqualTo("nll"))
    {
        bool hasUnparmUnconditional = (tree->GetEntries("fitType == 2")>0 ? 1:0);

        for(int i = 0 ; i < tree->GetEntries(); i++)
        {
            tree->GetEntry(i);
            if(hasUnparmUnconditional)
            {
                if(fitType > 1.5 && fitType < 2.5)
                {
                    countVarConditional++;
                    sumVarConditional  += var;
                }
            }
            else
            {
                if(fitType > 0.5 && fitType < 1.5)
                {
                    countVarConditional++;
                    sumVarConditional  += var;
                }
            }
        }
        averVarConditional = sumVarConditional/countVarConditional;
    }
    min = averVarConditional;


    for(int i = 0; i < tree->GetEntries(); i++)
    {
        tree->GetEntry(i);
        //cout<<"reading 1D var poi: "<<poi<<" "<<varName<<" "<<var<<" "<<(var-min)<<endl;


        if(!(fitType > -0.5 && fitType < 1.5)) continue;
        
        if(std::isnan(var)) continue;
        if(varName.EqualTo("nll") && (var-min) > 10) continue;
        info[poi] = var - min;
    }

    return info;
}

TGraph* get1DGraph(map<double,double>& varInfo)
{
    vector<double> xVal;
    vector<double> yVal;

    for (auto it = varInfo.begin(); it != varInfo.end(); ++it)
    {
        xVal.push_back(it->first);       
        yVal.push_back(it->second);       
    }

    TGraph *g = new TGraph(xVal.size(),&xVal[0],&yVal[0]);

    return g;
}

void fixOffset(map<double,double>& varInfo)
{
    double minVal = 99999;

    for (auto it = varInfo.begin(); it != varInfo.end(); ++it)
    {
        if(it->second < minVal) minVal = it->second;
    }

    // do the actual subtraction
    for (auto it = varInfo.begin(); it != varInfo.end(); ++it)
    {
        it->second -=  minVal;
    }
}

void printFitInfo(map<double,double>& NLLVar, double& outMinValX, double& outLeftErr, double& outRightErr,
                  double& outLeft2sigErr, double& outRight2sigErr, bool doBetterError)
{
    double minValX = 99999;
    double minValY = std::numeric_limits<double>::infinity();

    for (auto it = NLLVar.begin(); it != NLLVar.end(); ++it)
    {
        if(it->second < minValY)
        {
            minValX = it->first;
            minValY = it->second;
        }
    }

    outMinValX = minValX;


    // Error band
    vector<double> xValLeft;
    vector<double> yValLeft;
    
    vector<double> xValRight;
    vector<double> yValRight;

    for (auto it = NLLVar.begin(); it != NLLVar.end(); ++it)
    {
        if(it->first < minValX)
        {
            xValLeft.push_back(it->first);
            yValLeft.push_back(it->second);
        }
        else if(it->first > minValX)
        {
            xValRight.push_back(it->first);
            yValRight.push_back(it->second);
        }
    }

    TGraph* gLeft  = NULL; 
    if(xValLeft.size() > 0) gLeft = new TGraph(xValLeft.size(),&yValLeft[0],&xValLeft[0]);
    TGraph* gRight = NULL;
    if(xValRight.size() > 0) gRight = new TGraph(xValRight.size(),&yValRight[0],&xValRight[0]);

    double leftErr     = 0;
    double left2sigErr = 0;
    if(gLeft) {
        leftErr        = gLeft->Eval(1) - minValX;
        left2sigErr    = gLeft->Eval(4) - minValX;
    }
    double rightErr     = 0;
    double right2sigErr = 0;
    if(gRight) {
        rightErr        = gRight->Eval(1) - minValX;
        right2sigErr    = gRight->Eval(4) - minValX;
    }
    
    // if(opts["doUpperLim"].Contains("true"))
    // {
    //     leftErr = gLeft->Eval(4) - minValX;
    //     rightErr = gRight->Eval(4) - minValX;
    // }

    outLeftErr      = leftErr;
    outRightErr     = rightErr;
    outLeft2sigErr  = left2sigErr;
    outRight2sigErr = right2sigErr;

    // cout<<"outMinValX: "<<outMinValX<<endl;
    // cout<<"outLeftErr: "<<outLeftErr<<endl;
    // cout<<"outRightErr: "<<outRightErr<<endl;


    if(doBetterError)
    {
        // Get better graph
        TGraph* g = get1DGraph(NLLVar);
        // outMinValX = findmin (g, outMinValX + 2*outLeftErr, outMinValX + 2*outRightErr, 3, 0);

        // cout<<"outMinValX: "<<outMinValX<<" outLeftErr: "<<outLeftErr<<" outRightErr: "<<outRightErr<<endl;

        outLeftErr      = findy (g, 1, outMinValX + 1.1*outLeftErr, outMinValX, true, 3) - outMinValX;
        outRightErr     = findy (g, 1, outMinValX, outMinValX + 1.1*outRightErr, true, 3)  - outMinValX;
        outLeft2sigErr  = findy (g, 4, outMinValX + 1.1*outLeft2sigErr, outMinValX, true, 3) - outMinValX;
        outRight2sigErr = findy (g, 4, outMinValX, outMinValX + 1.1*outRight2sigErr, true, 3)  - outMinValX;

        delete g;
    }
    // cout<<"outMinValX: "<<outMinValX<<endl;
    // cout<<"outLeftErr: "<<outLeftErr<<endl;
    // cout<<"outRightErr: "<<outRightErr<<endl;

}


PlottingInfo parsePlottingInfo(std::vector<TString> fileNameList)
{

    LOG(logINFO)<<"Parsing plotting style info from the fileName list - if plots look weird grep for this line and modify accordingly";
    bool hasStat = false;
    bool hasSys = false;


    std::set<TString> uniqueFolder;
    for (const auto& fileName: fileNameList)
    {
        if(fileName.Contains("statOnly")) hasStat = true;
        if(fileName.Contains("allSys")) hasSys = true;

        vector<TString> filePartList = tokenizeStr(fileName,"/");

        if(filePartList.size() > 4) uniqueFolder.insert(filePartList.at(filePartList.size() - 4));
    }

    PlottingInfo plotInfo;
    plotInfo.hasStatSys = hasStat &&  hasSys;
    plotInfo.hasMultipleFolder = uniqueFolder.size() > 1;

    return plotInfo;
}
bool doesExist(const std::string& name) 
{
    ifstream f(name.c_str());
    return f.good();
}

vector<TString> splitString(TString string, TString delimiter)
{
    TObjArray *Varparts = string.Tokenize(delimiter);
    vector<TString> varNameVec;
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

TString joinList(vector<TString> arr, TString delimiter)
{
    if (arr.empty()) return "";

    string str;
    for (auto i : arr)
        str += i + delimiter;
    str = str.substr(0, str.size() - delimiter.Length());
    return TString(str);
}



void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}


void drawToy(TH1D & toyHist, double nomNLLVal, double percExcluded, TString subDirectoryName, TString histName)
{
    TCanvas *c = new TCanvas("", "", 0, 0, 600, 600);
    toyHist.GetXaxis()->SetTitle("#Delta nll");

    toyHist.Scale(1/toyHist.Integral());

    TLine *line = new TLine(nomNLLVal, 0, nomNLLVal, toyHist.GetMaximum());
    line->SetLineColor(kRed);
    line->SetLineWidth(2);
    toyHist.SetLineWidth(2);
    toyHist.SetMaximum(toyHist.GetMaximum()*1.3);
    toyHist.Draw("hist");
    line->Draw();

    double leftDist = 0.19;

    // ATLASLabel(leftDist, 0.875, "Internal", 1);
    TString channelStr = "H #rightarrow ZZ* #rightarrow 4l";
    TLatex* tMain = new TLatex (leftDist, 0.81, channelStr);
    tMain->SetNDC();
    tMain->SetTextSize(0.0425);
    tMain->SetTextFont(42);
    tMain->Draw();

    TString lumiString = "13 TeV, 139 fb^{-1}";
    TLatex* lumInfo = new TLatex (leftDist, 0.76, lumiString);
    lumInfo->SetNDC();
    lumInfo->SetTextSize(0.0325);
    lumInfo->SetTextFont(42);
    lumInfo->Draw();

    double rightDist = 0.6;
    double topDist = 0.86;
    TString numToys = Form("nToys: %.2f", toyHist.GetEntries());
    TLatex* numToysInfo = new TLatex (rightDist, topDist, numToys);
    numToysInfo->SetNDC();
    numToysInfo->SetTextSize(0.0325);
    numToysInfo->SetTextFont(42);
    numToysInfo->Draw();

    TString exclStr = Form("Perc. Excluded: %.3f", percExcluded);
    TLatex* exclInfo = new TLatex (rightDist, topDist - 0.04, exclStr);
    exclInfo->SetNDC();
    exclInfo->SetTextSize(0.0325);
    exclInfo->SetTextFont(42);
    exclInfo->Draw();

    TString nllStr = Form("nll Val: %.3f", nomNLLVal);
    TLatex* nllInfo = new TLatex (rightDist, topDist - 0.08, nllStr);
    nllInfo->SetNDC();
    nllInfo->SetTextSize(0.0325);
    nllInfo->SetTextFont(42);
    nllInfo->Draw();


    c->SaveAs(subDirectoryName + histName + ".pdf");
    delete c;
}


double plotToy(const std::vector<double> & toyResults, double nomNLLVal, TString subDirectoryName, TString histName)
{

    /// populate a histo for visualisation purposes
    TH1D toyHist (histName,histName,toyResults.size() / 4, 0,toyResults.back()*1.2);
    for (auto r : toyResults) {
        toyHist.Fill(r);
    } 

    /// get the quantile from our toy vector
    double perExc = (toyResults.end() - std::lower_bound(toyResults.begin(),toyResults.end(),nomNLLVal))/(double)toyResults.size(); 
    if(std::isnan(perExc)) perExc = -999;
    cout << "Perc: " << perExc << " nomNLLVal: " << nomNLLVal << endl;

    /// and draw a plot as well
    drawToy(toyHist, nomNLLVal, perExc, subDirectoryName, histName); 

    return perExc;
}


// Update the input string.
void autoExpandEnvironmentVariables( std::string & text ) {
    static std::regex env( "\\$\\{([^}]+)\\}" );
    std::smatch match;
    while ( std::regex_search( text, match, env ) ) {
        const char * s = getenv( match[1].str().c_str() );
        const std::string var( s == NULL ? "" : s );
        text.replace( match[0].first, match[0].second, var );
    }
}

// Leave input alone and return new string.
std::string expandEnvironmentVariables( const std::string & input ) {
    std::string text = input;
    autoExpandEnvironmentVariables( text );
    return text;
}
