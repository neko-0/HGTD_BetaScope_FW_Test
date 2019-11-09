#ifndef FITTER_H
#define FITTER_H

//-------c++----------------//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
//------ROOT----------------//
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <TTree.h>
#include <iomanip>
#include <TFile.h>
#include <TF1.h>
#include <TLeaf.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TSystem.h>
#include <TImage.h>
#include <TPaveText.h>
//======RooFit=================//
#include <RooFit.h>
#include <RooDataHist.h>
#include <RooHistPdf.h>
#include <RooRealVar.h>
#include <RooPlot.h>
#include <RooGaussian.h>
#include <RooLandau.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooFFTConvPdf.h>
#include <RooAbsPdf.h>
#include <RooAddPdf.h>
#include <RooExtendPdf.h>

#include "histoPackage.h"

struct FitResult
{
  double param;
  double param_err;
  double xmin;
  double xmax;
  double ndf;
  double chi_square;
  double prob;

  FitResult(){};
  
  FitResult(double param, double param_err, double xmin, double xmax, double ndf, double chi, double prob)
  {
      this->param = param;
      this->param_err = param;
      this->xmin = xmin;
      this->xmax = xmax;
      this->ndf = ndf;
      this->chi_square = chi;
      this->prob = prob;
  };

  ~FitResult(){};

};

class Fitter
{
  private:
    TF1 *fitter;
    std::string fitFunc = "";
    std::string fitOpt = "Q0";

  public:
    Fitter(){};
    virtual ~Fitter(){};

    void set_fitter( std::string fitName ){ this->fitter = new TF1( fitName.c_str(), fitName.c_str() ); this->fitFunc = fitName; };
    TF1 *get_fitter(){ return this->fitter; };

    void set_param( int param, double iValue ){ this->fitter->SetParameter(param, iValue); }

    FitResult fitter_RooLanGausArea( HistoPackage &i_hist, HistoPackage frontBaseArea, HistoPackage backBaseArea, bool savePlot=true);
    FitResult fitter_RooLanGaus( HistoPackage &i_hist, bool savePlot=true);
    FitResult fitter_fit( HistoPackage &i_hist, bool savePlot=true);
};

#endif
