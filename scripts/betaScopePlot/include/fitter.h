#ifndef FITTER_H
#define FITTER_H

//-------c++----------------//
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fmt/format.h>

//------ROOT----------------//
#include <TCanvas.h>
#include <TCut.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TImage.h>
#include <TLeaf.h>
#include <TPaveText.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>
#include <assert.h>
#include <fcntl.h>
#include <iomanip>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <mutex>
//======RooFit=================//
#include <RooAbsPdf.h>
#include <RooAddPdf.h>
#include <RooDataHist.h>
#include <RooDataSet.h>
#include <RooExtendPdf.h>
#include <RooFFTConvPdf.h>
#include <RooFit.h>
#include <RooGaussian.h>
#include <RooHistPdf.h>
#include <RooLandau.h>
#include <RooPlot.h>
#include <RooRealVar.h>

#include "histoPackage.h"


static std::mutex MTX;

struct FitResult {
  double param;
  double param_err;
  double xmin;
  double xmax;
  double ndf;
  double chi_square;
  double prob;
  double chi_ndf;
};

class Fitter {
private:
  TF1 *fitter;
  std::string fitFunc = "";
  std::string fitOpt = "Q0";

public:
  Fitter(){};
  virtual ~Fitter(){};

  void set_fitter(std::string fitName) {
    this->fitter = new TF1( fmt::format("{}_{}",fitName, std::rand()).c_str(), fitName.c_str());
    this->fitFunc = fitName;
  };
  TF1 *get_fitter() { return this->fitter; };

  void set_param(int param, double iValue) {
    this->fitter->SetParameter(param, iValue);
  }

  FitResult fitter_RooLanGausArea(
    HistoPackage &i_hist,
    HistoPackage frontBaseArea,
    HistoPackage backBaseArea,
    bool savePlot = true
  );

  FitResult fitter_RooLanGaus( HistoPackage &i_hist, bool savePlot = true );
  FitResult fitter_fit( HistoPackage &i_hist, bool savePlot = true );
};

#endif
