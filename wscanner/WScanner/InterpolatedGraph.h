#ifndef INTERPOLATEDGRAPH_H
#define INTERPOLATEDGRAPH_H

#include <iostream>
#include <vector>
#include "TGraph.h"
#include "TMath.h"
#include "Math/BrentRootFinder.h"
#include "Math/BrentMinimizer1D.h"
#include "Math/WrappedFunction.h"
#include "TAxis.h"
#include "TF1.h"
#include "TSpline.h"
#include "TMVA/TSpline1.h"
#include "TMVA/TSpline2.h"


TSpline* spline (TGraph* g, int ord=3);


// Forward declare it
struct InterpolatedGraph {
    InterpolatedGraph (const TGraph* g, Double_t y0=0.0, int ord=3, int verbose=0, bool alwaysClone=false)
        : _y0(y0), _ord(ord), _verbose(verbose), _graph(0), _ownedGraph(0), _spline(0)
    {
        Int_t n= g->GetN();
        if (n < 2) {
            std::cerr << "Need at least 2 points for interpolation (n=" << n << ")" << std::endl;
            return;
        }
        if        (ord==1 || ord==2) {
            _graph= new TGraph (*g);
            if      (!g->TestBit(1UL<<19)) _graph->Sort();
        } else if (!g->TestBit(1UL<<19)) {  //  BIT(19)=kIsSortedX
            _graph= new TGraph (*g);
            _graph->Sort();
            _ownedGraph= _graph;  // otherwise owned by TMVA::TSpline1/2
        } else if (alwaysClone) {
            _graph= new TGraph (*g);
            _ownedGraph= _graph;
        } else
            _graph= const_cast<TGraph*>(g);
        _spline= spline (_graph, _ord);
    }

    InterpolatedGraph (const InterpolatedGraph& other)
        : _y0(other._y0), _ord(other._ord), _verbose(other._verbose), _graph(other._graph), _ownedGraph(0), _spline(0)
    {
        if (_ord==1 || _ord==2) _graph= new TGraph (*_graph);
        _spline= spline (_graph, _ord);
    }

    ~InterpolatedGraph() {
        delete _ownedGraph; _ownedGraph= 0;
        delete _spline;     _spline=     0;
    }

    // return interpolated value for x - target
    Double_t operator() (Double_t x) const {
        Double_t y= (_spline) ? (_spline->Eval(x)-_y0) : TMath::QuietNaN();
        if (_verbose>=2) std::cout << x << " -> " << y << std::endl;
        return y;
    }
    // provided for TF1 functor compatibility
    Double_t operator() (Double_t* x, double* /*p=0*/) const {
        Double_t y= (_spline) ? (_spline->Eval(x[0])-_y0) : TMath::QuietNaN();
        if (_verbose>=2) std::cout << x[0] << " -> " << y << std::endl;
        return y;
    }
    TGraph*        GetGraph() const { return _graph;   }
    bool           ok()       const { return  _spline; }
    const TSpline& Spline()   const { return *_spline; }

    private:
    Double_t _y0;
    int _ord, _verbose;
    TGraph *_graph, *_ownedGraph;
    TSpline* _spline;
};


Double_t findmin (const InterpolatedGraph& f,
        Double_t xmin=-TMath::Infinity(),
        Double_t xmax=TMath::Infinity(),
        int verbose=0,
        int maxIter=100, double absTol=1e-8, double relTol=1e-10);

Double_t findy (const InterpolatedGraph& f,
        Double_t y0,
        Double_t xmin=-TMath::Infinity(),
        Double_t xmax= TMath::Infinity(),
        bool lowSearch=true,
        int verbose=0,
        int maxIter=100, double absTol=1e-8, double relTol=1e-10);

Double_t eval (const TGraph* g, Double_t x, int ord=3, int verbose=0);

TF1* func (const TGraph* g,
        int ord=3,
        Double_t y0= 0.0,
        Double_t xmin=TMath::QuietNaN(),
        Double_t xmax=TMath::QuietNaN(),
        Int_t npx=500,
        int verbose=0);

void draw (TGraph* g,
        int ord=3,
        Double_t xmin=TMath::QuietNaN(),
        Double_t xmax=TMath::QuietNaN(),
        Option_t* opt="ap",
        Int_t npx=500,
        int verbose=0);

Double_t findmin (const TGraph* g,
        Double_t xmin=-TMath::Infinity(),
        Double_t xmax= TMath::Infinity(),
        int ord=3,
        int verbose=0,
        int maxIter=100, double absTol=1e-8, double relTol=1e-10);

Double_t findy (const TGraph* g,
        Double_t y0=0.0,
        Double_t xmin=-TMath::Infinity(),
        Double_t xmax= TMath::Infinity(),
        bool lowSearch=true,
        int ord=3,
        int verbose=0,
        int maxIter=100, double absTol=1e-8, double relTol=1e-10);


#endif




