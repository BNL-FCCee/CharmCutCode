////////////////////////////////////////////////////////
// Code by Tim Adye, copied directly his implementation
// For interpolating better than linearly between the points
////////////////////////////////////////////////////////

#include "InterpolatedGraph.h"

using std::cout;
using std::cerr;
using std::endl;

TSpline* spline (TGraph* g, int ord)
{
    if      (ord==1) return new TMVA::TSpline1 (g->GetTitle(), g);
    else if (ord==2) return new TMVA::TSpline2 (g->GetTitle(), g);
    else if (ord==3) return new       TSpline3 (g->GetTitle(), g);
    else if (ord==5) return new       TSpline5 (g->GetTitle(), g);
    else {
        cerr << Form("TSpline%d is not supported",ord) << endl;
        return 0;
    }
}
//


Double_t findmin (const InterpolatedGraph& f,
        Double_t xmin,
        Double_t xmax,
        int verbose,
        int maxIter, double absTol, double relTol)
{
    // return the X value of the minimum of the graph

    if (!f.ok()) return TMath::QuietNaN();

    const TGraph& graph= *f.GetGraph();
    ROOT::Math::WrappedFunction<const InterpolatedGraph&> wf(f);

    if (xmin >= xmax) {
        xmin= -TMath::Infinity();
        xmax=  TMath::Infinity();
    }
    if (xmin == -TMath::Infinity()) xmin= graph.GetX()[0];
    if (xmax ==  TMath::Infinity()) xmax= graph.GetX()[graph.GetN()-1];

    if (verbose>=1)
        cout << "Do " << f.Spline().ClassName() << " minimisation between " << xmin << "  and " << xmax << endl;

    ROOT::Math::BrentMinimizer1D bmz;
    bmz.SetFunction (wf, xmin, xmax);
    bool ret= bmz.Minimize (maxIter, absTol, relTol);
    if (!ret) {
        cerr << "BrentMinimizer1D failed - return NaN" << endl;
        return TMath::QuietNaN();
    }
    Double_t x0= bmz.XMinimum();
    if (verbose>=1) cout << "BrentMinimizer1D returns x0 = " << x0 << endl;

    return x0;
}

Double_t findy (const InterpolatedGraph& f,
        Double_t y0,
        Double_t xmin,
        Double_t xmax,
        bool lowSearch,
        int verbose,
        int maxIter, double absTol, double relTol)
{
    // return the X value of the given graph for the target value y0
    // Based on RooStats::HypoTestInverterResult::GetGraphX()

    if (!f.ok()) return TMath::QuietNaN();

    const TGraph& graph= *f.GetGraph();
    ROOT::Math::WrappedFunction<const InterpolatedGraph&> wf(f);

    // find reasanable xmin and xmax if not given
    const Double_t* gx= graph.GetX();
    const Double_t* gy= graph.GetY();
    Int_t n= graph.GetN();

    // case no range is given check if need to extrapolate to lower/upper values
    if (xmin >= xmax) {
        xmin= -TMath::Infinity();
        xmax=  TMath::Infinity();
    }

    Double_t axmin= xmin;
    Double_t axmax= xmax;
    // need lower extrapolation?
    if (axmin == -TMath::Infinity() && !(lowSearch ? (TMath::MinElement(n,gy) > y0) : (TMath::MaxElement(n,gy) < y0)))
        axmin= gx[0];
    // need upper extrapolation?
    if (axmax ==  TMath::Infinity() && !(lowSearch ? (TMath::MaxElement(n,gy) < y0) : (TMath::MinElement(n,gy) > y0)))
        axmax= gx[n-1];

    if (verbose>=1) {
        if (lowSearch) cout << "Lower x search : ";
        else           cout << "Upper x search : ";
        cout << "do " << f.Spline().ClassName() << " interpolation between " << axmin << "  and " << axmax << endl;
    }

    ROOT::Math::BrentRootFinder brf;
    brf.SetFunction (wf, axmin, axmax);
    bool ret= brf.Solve (maxIter, absTol, relTol);
    if (!ret) {
        cerr << "BrentRootFinder interpolation failed - return NaN" << endl;
        return TMath::QuietNaN();
    }
    Double_t x0= brf.Root();
    if (verbose>=1) cout << "BrentRootFinder returns x0 = " << x0 << endl;

    // look in case if a new interseption exists
    // only when boundaries are not given
    if (xmin == -TMath::Infinity() && xmax == TMath::Infinity()) {
        Int_t index= TMath::BinarySearch(n, gx, x0);
        if (verbose>=1) cout << "Do new interpolation dividing from " << index << " and " << gy[index] << endl;
        if      ( lowSearch && index>=1  && (gy[0]  -y0)*(gy[index]  -y0) < 0)
            // search if another interseption exists at a lower value
            x0= findy (f, y0, gx[0],       gx[index], lowSearch, verbose, maxIter, absTol, relTol);
        else if (!lowSearch && index<n-2 && (gy[n-1]-y0)*(gy[index+1]-y0) < 0)
            // search if another interseption exists at a higher value
            x0= findy (f, y0, gx[index+1], gx[n-1],   lowSearch, verbose, maxIter, absTol, relTol);
    }
    return x0;
}

Double_t eval (const TGraph* g, Double_t x, int ord, int verbose)
{
    InterpolatedGraph f (g, 0.0, ord, verbose);
    return f(x);
}

TF1* func (const TGraph* g,
        int ord,
        Double_t y0,
        Double_t xmin,
        Double_t xmax,
        Int_t npx,
        int verbose)
{
    if (TMath::IsNaN(xmin) || TMath::IsNaN(xmax)) {
        const TAxis* ax= g->GetXaxis();
        if (TMath::IsNaN(xmin)) xmin= ax->GetXmin();
        if (TMath::IsNaN(xmax)) xmax= ax->GetXmax();
    }
    InterpolatedGraph* f= new InterpolatedGraph (g, y0, ord, verbose);  // no way to delete f
    TF1* f1= new TF1 (g->GetName(), *f, xmin, xmax, 0);
    f1->SetNpx(npx);
    return f1;
}

void draw (TGraph* g,
        int ord,
        Double_t xmin,
        Double_t xmax,
        Option_t* opt,
        Int_t npx,
        int verbose)
{
    // Draw graph and spline function (including with extrapolation)
    // note can modify graph style and limits.
    if (g->GetMarkerStyle() == 1) {
        g->SetMarkerStyle(20);
        g->SetMarkerSize(0.7);
    }
    TAxis* ax= g->GetXaxis();
    bool setlim= (!TMath::IsNaN(xmin) || !TMath::IsNaN(xmax));
    if (TMath::IsNaN(xmin)) xmin= ax->GetXmin();
    if (TMath::IsNaN(xmax)) xmax= ax->GetXmax();
    if (setlim) ax->SetLimits (xmin, xmax);
    g->Draw(opt);
    InterpolatedGraph f (g, 0.0, ord, verbose, true);
    TF1* f1= new TF1 (g->GetName(), f, xmin, xmax, 0);
    f1->SetNpx(npx);
    f1->Draw("same");
}

Double_t findmin (const TGraph* g,
        Double_t xmin,
        Double_t xmax,
        int ord,
        int verbose,
        int maxIter, double absTol, double relTol)
{
    // return the X value of the minimum of the given graph. The graph will be sorted if needed.
    // the graph is evaluated using TSpline3 interpolation by default.
    InterpolatedGraph f (g, 0.0, ord, verbose);
    return findmin (f, xmin, xmax, verbose, maxIter, absTol, relTol);
}

Double_t findy (const TGraph* g,
        Double_t y0,
        Double_t xmin,
        Double_t xmax,
        bool lowSearch,
        int ord,
        int verbose,
        int maxIter, double absTol, double relTol)
{
    // return the X value of the given graph for the target value y0. The graph will be sorted if needed.
    // the graph is evaluated using TSpline3 interpolation by default.
    InterpolatedGraph f (g, y0, ord, verbose);
    return findy (f, y0, xmin, xmax, lowSearch, verbose, maxIter, absTol, relTol);
}
