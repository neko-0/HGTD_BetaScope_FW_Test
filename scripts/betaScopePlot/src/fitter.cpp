#include <RooFit.h>
#include <fmt/format.h>

#include <RooMinimizer.h>

#include "betaScopePlot/include/fitter.h"

FitResult Fitter::fitter_RooLanGausArea(
  HistoPackage &i_histo,
  HistoPackage frontBaseArea,
  HistoPackage backBaseArea,
  bool savePlot)
{
  gStyle->SetStatW(0.125);
  gStyle->SetStatH(0.125);

  RooRealVar x(
    fmt::format("x_{}",i_histo.get_tag()).c_str(), i_histo.get_observable().c_str(),
    i_histo.get_xmin(), i_histo.get_xmax()
  );

  /*
  RooRealVar *roo_gaus_mean;
  RooRealVar *roo_gaus_sigma;
  RooGaussian *roo_gaus;

  RooRealVar *roo_landau_mean;
  RooRealVar *roo_landau_sigma;
  RooLandau *roo_landau;
  */

  double sampleMean = i_histo.get_histo()->GetMean();
  double sampleSigma = i_histo.get_histo()->GetStdDev();
  int bins = i_histo.get_histo()->GetEntries();
  double histo_max = i_histo.get_max_bin_xvalue();

  TF1 frontBaselineFitter(fmt::format("frontBaselineFitter_{}",i_histo.get_tag()).c_str(), "gaus");
  frontBaseArea.get_histo()->Fit(&frontBaselineFitter, "Q0");

  double baseline_area_mean = frontBaselineFitter.GetParameter(1);
  double baseline_area_sigma = frontBaselineFitter.GetParameter(2);
  double baseline_area_sigma_error = frontBaselineFitter.GetParError(2);

  TF1 backBaselineFitter(fmt::format("backBaselineFitter_{}",i_histo.get_tag()).c_str(), "gaus");
  backBaseArea.get_histo()->Fit(&backBaselineFitter, "Q0");

  double backBaseline_param1 = backBaselineFitter.GetParameter(1);
  double backBaseline_param2 = backBaselineFitter.GetParameter(2);
  baseline_area_mean = (backBaseline_param1 + baseline_area_mean) / 2.0;
  baseline_area_sigma = (backBaseline_param2 + baseline_area_sigma) / 2.0;
  baseline_area_sigma_error = (backBaseline_param2 + baseline_area_sigma_error) / 2.0;

  RooRealVar roo_gaus_mean(fmt::format("Roo_gaus_mean_",i_histo.get_tag()).c_str(), "Roo_gaus_mean", 0.0);
  RooRealVar roo_gaus_sigma(fmt::format("Roo_gaus_sigma_",i_histo.get_tag()).c_str(), "Roo_gaus_sigma", std::abs(sampleSigma), 0.001, 3*std::abs(sampleSigma));
  RooGaussian roo_gaus(fmt::format("Roo_gauss_",i_histo.get_tag()).c_str(), "Roo_gauss", x, roo_gaus_mean, roo_gaus_sigma);

  RooRealVar roo_landau_mean(fmt::format("Roo_landau_mean_",i_histo.get_tag()).c_str(), "Roo_landau_mean", 1.0, 0.0, i_histo.get_xmax());
  RooRealVar roo_landau_sigma(fmt::format("Roo_landau_sigma_",i_histo.get_tag()).c_str(), "Roo_landau_sigma", std::abs(sampleSigma), 0.1, 3 * std::abs(sampleSigma));
  RooLandau roo_landau(fmt::format("Roo_landau_",i_histo.get_tag()).c_str(), "Roo_landau", x, roo_landau_mean, roo_landau_sigma);

  /*
  roo_gaus_mean = new RooRealVar("Roo_gaus_mean", "Roo_gaus_mean", 0.0);
  roo_gaus_sigma = new RooRealVar("Roo_gaus_sigma", "Roo_gaus_sigma", std::abs(sampleSigma), 0.1, 3*std::abs(sampleSigma));
  roo_gaus = new RooGaussian("Roo_gauss", "Roo_gauss", x, *roo_gaus_mean, *roo_gaus_sigma);

  roo_landau_mean = new RooRealVar("Roo_landau_mean", "Roo_landau_mean", 1.0, 0.0, i_histo.get_xmax());
  roo_landau_sigma = new RooRealVar("Roo_landau_sigma", "Roo_landau_sigma", std::abs(sampleSigma), 0.1, 3 * std::abs(sampleSigma));
  roo_landau = new RooLandau("Roo_landau", "Roo_landau", x, *roo_landau_mean, *roo_landau_sigma);
  */

  x.setBins(10000, "cache");

  RooFFTConvPdf lxg( fmt::format("lxg_{}",i_histo.get_tag()).c_str(), "landau conv gauss", x, roo_landau, roo_gaus);

  RooDataHist dataHist( fmt::format("dataHist_{}",i_histo.get_tag()).c_str(), i_histo.get_title().c_str(), x, RooFit::Import(*i_histo.get_histo()));

  //std::unique_lock<std::mutex> lck(MTX);
  //auto my_mini = RooFi::Minimizer("Minutit2","Migrad");
  //auto my_mini = RooMinimizer(lxg);
  //my_mini.setMinimizerType("Minutit2");
  //RooFit::Minimizer("Minutit2","Migrad"), RooFit::NumCPU(3)
  RooFitResult *emptyFit = lxg.fitTo(dataHist, RooFit::Save()); // Normalization(histo->GetEntries(),RooAbsReal::NumEvent)
  //RooFitResult *emptyFit = my_mini.fit(fmt::format("dataHist_{}",i_histo.get_tag()).c_str());
  //lck.unlock();

  double landau_mean_error = roo_landau_mean.getPropagatedError(*emptyFit);

  TF1 *lxg_tf = lxg.asTF(RooArgList(x));
  double peak = lxg_tf->GetMaximumX();
  double peak_y = lxg_tf->Eval(peak);
  double left_50per = lxg_tf->GetX(peak_y / 2.0, -1000, peak);
  double right_50per = lxg_tf->GetX(peak_y / 2.0, peak, 10.0 * peak);
  double landau_FWHM = right_50per - left_50per;

  double landau_left = 2.0;
  double landau_right = 7.0;

  double range_min = peak-landau_left*sampleSigma; //(landau_left * sampleSigma)/1.5;//Roo_lsigma.getVal();
  double range_max = peak+landau_right*sampleSigma; //(landau_right * sampleSigma)/2.0;//Roo_lsigma.getVal();

  if(range_min != range_max)
  {
    i_histo.set_min(range_min);
    i_histo.set_max(range_max);
  }
  else if (sampleSigma > 0)
  {
    i_histo.set_min(sampleMean-5*sampleSigma);
    i_histo.set_max(sampleMean+5*sampleSigma);
  }
  else
  {
    i_histo.set_min(i_histo.get_xmin()-500);
    i_histo.set_max(i_histo.get_xmax()+500);
  }

  double Par = roo_landau_mean.getVal();
  double ParErr = roo_landau_mean.getAsymErrorHi();

  double Par2 = roo_landau_sigma.getVal();
  double Par2Err = roo_landau_sigma.getAsymErrorHi();

  RooPlot *frame = x.frame(RooFit::Title(i_histo.get_title().c_str()));

  if( savePlot )
  {
    TCanvas *oCanvas = new TCanvas(Form("%s_canvas", i_histo.get_histoName().c_str()));
    //TPaveText *paveText = new TPaveText;
    //TF1 *histo_fit = new TF1;
    TPaveText paveText(0.7, 0.9, 0.5, 0.7, "brNDC");

    TF1 histo_fit(Form("%s_root_fit", i_histo.get_histoName().c_str()), "landau");
    histo_fit.SetLineColor(kGreen);
    i_histo.get_histo()->SetLineColor(kRed);
    i_histo.get_histo()->SetLineWidth(3);
    if( i_histo.get_histo()->Fit(&histo_fit, "SQR", "", i_histo.get_xmin(), i_histo.get_xmax())){}
    else{ i_histo.get_histo()->Fit(&histo_fit, "SQ"); }
    i_histo.get_histo()->GetYaxis()->SetRangeUser(0, i_histo.get_max_bin_xvalue()*1.5);
    i_histo.get_histo()->Draw("same");

    dataHist.plotOn(frame, RooFit::Name(fmt::format("dataHist_{}",i_histo.get_tag()).c_str())); //, Normalization(histo->GetEntries(),// RooAbsReal::NumEvent));
    dataHist.statOn(frame, RooFit::Layout(0.45, 0.90, 0.6));
    lxg.plotOn(frame, RooFit::Name(fmt::format("lxg_{}",i_histo.get_tag()).c_str())); //, Normalization(histo->GetEntries(), // RooAbsReal::NumEvent));
    lxg.paramOn(frame, RooFit::Layout(0.45, 0.90, 0.4));
    roo_landau.plotOn(frame, RooFit::LineStyle(kDashed),RooFit::LineColor(kOrange),RooFit::Name("landau")); //,// Normalization(histo->GetEntries(),RooAbsReal::NumEvent)//);
    roo_gaus.plotOn(frame, RooFit::LineStyle(kDashed),RooFit::LineColor(kPink),RooFit::Name("gaus"));
    frame->Draw("same");

    //paveText = new TPaveText(0.7, 0.9, 0.5, 0.7, "brNDC");
    paveText.AddText(Form("Peak = %lf", peak));
    paveText.AddText(Form("Error = %lf", landau_mean_error));
    paveText.AddText(Form("FWHM = %lf", landau_FWHM));
    paveText.Draw();

    TImage *img = TImage::Create();
    img->FromPad(oCanvas);
    img->WriteImage(Form("%s_%s_lxg_RooFit.png", i_histo.get_tfile_name().c_str(), i_histo.get_simple_tag().c_str()));
    delete img;

    //if (histo_fit != NULL)delete histo_fit;
    //if (paveText != NULL)delete paveText;
    if (oCanvas != NULL){ delete oCanvas; }
  }

  double ndf = lxg_tf->GetNDF();
  double chi_square = lxg_tf->GetChisquare();
  double prob = lxg_tf->GetProb();
  double chi_ndf = frame->chiSquare(fmt::format("lxg_{}",i_histo.get_tag()).c_str(), fmt::format("dataHist_{}",i_histo.get_tag()).c_str(), 3);

  FitResult fitResult{
    Par, ParErr, range_min, range_max, ndf, chi_square, prob, chi_ndf, Par2, Par2Err
  };

  /*
  if (roo_gaus != NULL)
    delete roo_gaus;
  if (roo_gaus_mean != NULL)
    delete roo_gaus_mean;
  if (roo_gaus_sigma != NULL)
    delete roo_gaus_sigma;
  if (roo_landau != NULL)
    delete roo_landau;
  if (roo_landau_mean != NULL)
    delete roo_landau_mean;
  if (roo_landau_sigma != NULL)
    delete roo_landau_sigma;
  */

  return fitResult; // std::make_tuple( Par, ParErr, range_min, range_max );
}

FitResult Fitter::fitter_RooLanGaus(HistoPackage &i_histo, bool savePlot)
{
  gStyle->SetStatW(0.125);
  gStyle->SetStatH(0.125);

  RooRealVar x(
    fmt::format("x_{}",i_histo.get_tag()).c_str(), i_histo.get_observable().c_str(),
    i_histo.get_xmin(),i_histo.get_xmax()
  );

  double sampleMean = i_histo.get_histo()->GetMean();
  double sampleSigma = i_histo.get_histo()->GetStdDev();
  int bins = i_histo.get_histo()->GetEntries();
  double histo_max = i_histo.get_max_bin_xvalue();

  RooRealVar roo_gaus_mean( fmt::format("Roo_gmean_",i_histo.get_tag()).c_str(), "Roo_gmean", 0.0);
  RooRealVar roo_gaus_sigma( fmt::format("Roo_gsigma_",i_histo.get_tag()).c_str(), "Roo_gsigma", std::abs(sampleSigma), 0.1, 3*std::abs(sampleSigma));
  RooGaussian roo_gaus( fmt::format("Roo_gauss_",i_histo.get_tag()).c_str(), "Roo_gauss", x, roo_gaus_mean, roo_gaus_sigma);

  RooRealVar roo_landau_mean( fmt::format("Roo_landau_mean_",i_histo.get_tag()).c_str(), "Roo_landau_mean", 1.0, i_histo.get_xmin(), i_histo.get_xmax() );
  RooRealVar roo_landau_sigma( fmt::format("Roo_landau_sigma_",i_histo.get_tag()).c_str(), "Roo_landau_sigma",std::abs(sampleSigma), 0.001, 3 * std::abs(sampleSigma));
  RooLandau roo_landau( fmt::format("Roo_landau_",i_histo.get_tag()).c_str(), "Roo_landau", x, roo_landau_mean, roo_landau_sigma);

  x.setBins(10000, "cache");

  RooFFTConvPdf lxg( fmt::format("lxg_{}",i_histo.get_tag()).c_str(), "landau conv gauss", x, roo_landau, roo_gaus);

  RooDataHist dataHist( fmt::format("dataHist_{}",i_histo.get_tag()).c_str(), i_histo.get_title().c_str(), x, RooFit::Import(*i_histo.get_histo()));

  //std::unique_lock<std::mutex> lck(MTX);
  //auto my_mini = RooFit::Minimizer("Minutit2","Migrad");
  //auto my_mini = RooMinimizer(lxg);
  //my_mini.setMinimizerType("Minutit2");
  //RooFit::Minimizer("Minutit2","Migrad"), RooFit::NumCPU(3)
  RooFitResult *emptyFit = lxg.fitTo( dataHist, RooFit::Save());
  //RooFitResult *emptyFit = my_mini.fit(fmt::format("dataHist_{}",i_histo.get_tag()).c_str());
  //lck.unlock();

  double landau_mean_error = roo_landau_mean.getPropagatedError(*emptyFit);

  TF1 *lxg_tf = lxg.asTF(RooArgList(x));
  double peak = lxg_tf->GetMaximumX();
  double peak_y = lxg_tf->Eval(peak);
  double left_50per = lxg_tf->GetX(peak_y / 2.0, -1000, peak);
  double right_50per = lxg_tf->GetX(peak_y / 2.0, peak, 10.0 * peak);
  double landau_FWHM = right_50per - left_50per;

  double landau_left = 2.0;
  double landau_right = 7.0;

  double range_min = peak-landau_left*sampleSigma;
  double range_max = peak+landau_right*sampleSigma;

  if(range_min != range_max)
  {
    i_histo.set_min(range_min);
    i_histo.set_max(range_max);
  }
  else if (sampleSigma > 0)
  {
    i_histo.set_min(sampleMean-5*sampleSigma);
    i_histo.set_max(sampleMean+5*sampleSigma);
  }
  else
  {
    i_histo.set_min(i_histo.get_xmin()-500);
    i_histo.set_max(i_histo.get_xmax()+500);
  }

  double Par = roo_landau_mean.getVal();
  double ParErr = roo_landau_mean.getAsymErrorHi();

  double Par2 = roo_landau_sigma.getVal();
  double Par2Err = roo_landau_mean.getAsymErrorHi();

  RooPlot *frame = x.frame(RooFit::Title(i_histo.get_title().c_str()));

  if( savePlot )
  {
    TCanvas *oCanvas = new TCanvas(Form("%s_canvas", i_histo.get_histoName().c_str()));
    //TPaveText *paveText = new TPaveText;
    //TF1 *histo_fit = new TF1;

    TF1 histo_fit(Form("%s_root_fit", i_histo.get_histoName().c_str()), "landau");
    histo_fit.SetLineColor(kGreen);
    i_histo.get_histo()->SetLineColor(kRed);
    i_histo.get_histo()->SetLineWidth(3);
    if( i_histo.get_histo()->Fit(&histo_fit, "SQR", "", i_histo.get_xmin(), i_histo.get_xmax())){}
    else{ i_histo.get_histo()->Fit(&histo_fit, "SQ"); }
    i_histo.get_histo()->GetYaxis()->SetRangeUser(0, i_histo.get_max_bin_xvalue() * 1.5);
    i_histo.get_histo()->Draw("same");

    dataHist.plotOn( frame, RooFit::Name(fmt::format("dataHist_{}",i_histo.get_tag()).c_str()));
    dataHist.statOn(frame, RooFit::Layout(0.45, 0.90, 0.6));
    lxg.plotOn(frame, RooFit::Name(fmt::format("lxg_{}",i_histo.get_tag()).c_str()));
    lxg.paramOn(frame, RooFit::Layout(0.45, 0.90, 0.4));
    roo_landau.plotOn( frame, RooFit::LineStyle(kDashed), RooFit::LineColor(kOrange), RooFit::Name("landau"));
    roo_gaus.plotOn(frame, RooFit::LineStyle(kDashed), RooFit::LineColor(kPink), RooFit::Name("gaus"));
    frame->Draw("same");

    TPaveText paveText(0.7, 0.9, 0.5, 0.7, "brNDC");
    paveText.AddText(Form("Peak = %lf", peak));
    paveText.AddText(Form("Error = %lf", landau_mean_error));
    paveText.AddText(Form("FWHM = %lf", landau_FWHM));
    paveText.Draw();

    TImage *img = TImage::Create();
    img->FromPad(oCanvas);
    img->WriteImage(Form("%s_%s_lxg_RooFit.png", i_histo.get_tfile_name().c_str(), i_histo.get_simple_tag().c_str()));
    delete img;

    if( oCanvas != NULL ){ delete oCanvas; }
  }

  double ndf = lxg_tf->GetNDF();
  double chi_square = lxg_tf->GetChisquare();
  double prob = lxg_tf->GetProb();
  double chi_ndf = frame->chiSquare( fmt::format("lxg_{}",i_histo.get_tag()).c_str(), fmt::format("dataHist_{}",i_histo.get_tag()).c_str(), 3);

  FitResult fitResult{
    Par, ParErr, range_min, range_max, ndf, chi_square, prob, chi_ndf, Par2, Par2Err
  };

  return fitResult;
}

FitResult Fitter::fitter_fit(HistoPackage &i_histo, std::string fitName, bool savePlot)
{  
  TF1 fitter( fmt::format("{}_{}",fitName, std::rand()).c_str(), fitName.c_str());
  fitter.AddToGlobalList(false);

  double sampleMean = i_histo.get_histo()->GetMean();
  double sampleSigma = i_histo.get_histo()->GetStdDev();

  fitter.SetParameter(1, sampleMean);
  fitter.SetParameter(2, sampleSigma);

  int fitSatus = i_histo.get_histo()->Fit(&fitter, this->fitOpt.c_str(), "", i_histo.get_xmin(), i_histo.get_xmax());

  double param1 = fitter.GetParameter(1);
  double param2 = fitter.GetParameter(2);

  double range_min = param1 - 5 * param2;
  double range_max = param1 + 5 * param2;

  if( fitSatus == 0 )
  {
    if(range_min != range_max)
    {
      i_histo.set_min(range_min);
      i_histo.set_max(range_max);
    }
    else if (sampleSigma > 0)
    {
      i_histo.set_min(sampleMean-sampleSigma);
      i_histo.set_max(sampleMean+sampleSigma);
    }
    else
    {
      i_histo.set_min(i_histo.get_xmin()-500);
      i_histo.set_max(i_histo.get_xmax()+500);
    }
    this->fitOpt = "SQR";
  }
  else
  {
    std::cout << i_histo.get_histoName() << "no fit results\n";
    i_histo.set_min(1);
    i_histo.set_max(1);
    this->fitOpt = "SQ";
  }


  double Par = fitter.GetParameter(1);
  double ParErr = fitter.GetParError(1);

  double Par2 = fitter.GetParameter(2);
  double Par2Err = fitter.GetParError(2);

  if (savePlot)
  {
    TCanvas *oCanvas = new TCanvas(Form("%s_canvas", i_histo.get_histoName().c_str()));
    oCanvas->cd();
    i_histo.get_histo()->Draw();
    //fitter.Draw("same");
    TImage *img = TImage::Create();
    img->FromPad(oCanvas);
    img->WriteImage(Form("%s_%s.png", i_histo.get_tfile_name().c_str(), i_histo.get_simple_tag().c_str()));
    delete img;
    delete oCanvas;
  }

  double ndf = fitter.GetNDF();
  double chi_square = fitter.GetChisquare();
  double prob = fitter.GetProb();
  double chi_ndf = chi_square / ndf;

  FitResult fitResult{
    Par, ParErr, range_min, range_max, ndf, chi_square, prob, chi_ndf, Par2, Par2Err
  };

  return fitResult;
}
