#include "fitter.h"

FitResult Fitter::fitter_RooLanGausArea( HistoPackage &i_histo, HistoPackage frontBaseArea, HistoPackage backBaseArea, bool savePlot)
{
  gStyle->SetStatW(0.125);
  gStyle->SetStatH(0.125);

  RooRealVar x( "x", i_histo.get_observable().c_str(), i_histo.get_xmin(), i_histo.get_xmax() );

  RooRealVar *roo_gaus_mean;
  RooRealVar *roo_gaus_sigma;
  RooGaussian *roo_gaus;

  RooRealVar *roo_landau_mean;
  RooRealVar *roo_landau_sigma;
  RooLandau *roo_landau;

  double sampleMean = i_histo.get_histo()->GetMean();
  double sampleSigma = i_histo.get_histo()->GetStdDev();
  int bins = i_histo.get_histo()->GetEntries();
  double histo_max = i_histo.get_histo()->GetXaxis()->GetBinCenter( i_histo.get_histo()->GetMaximumBin() );

  TF1 *frontBaselineFitter = new TF1( "frontBaselineFitter", "gaus");
  frontBaseArea.get_histo()->Fit( frontBaselineFitter, "Q0" );

  double baseline_area_mean = frontBaselineFitter->GetParameter(1);
  double baseline_area_sigma = frontBaselineFitter->GetParameter(2);
  double baseline_area_sigma_error = frontBaselineFitter->GetParError(2);

  TF1 *backBaselineFitter = new TF1( "backBaselineFitter", "gaus");
  backBaseArea.get_histo()->Fit( backBaselineFitter, "Q0" );

  baseline_area_mean = (backBaselineFitter->GetParameter(1)+baseline_area_mean)/2.0; //std::cout << baseline_area_mean << std::endl; std::cin.get();
  baseline_area_sigma = (backBaselineFitter->GetParameter(2)+baseline_area_sigma)/2.0;
  baseline_area_sigma_error = (backBaselineFitter->GetParError(2)+baseline_area_sigma_error)/2.0;

  delete frontBaselineFitter;
  delete backBaselineFitter;

  roo_gaus_mean = new RooRealVar( "Roo_gaus_mean", "Roo_gaus_mean", 0.0);
  roo_gaus_sigma = new RooRealVar( "Roo_gaus_sigma", "Roo_gaus_sigma", std::abs(sampleSigma), 0.1, 3*std::abs(sampleSigma) );
  roo_gaus = new RooGaussian( "Roo_gauss", "Roo_gauss", x, *roo_gaus_mean, *roo_gaus_sigma );

  roo_landau_mean = new RooRealVar( "Roo_landau_mean", "Roo_landau_mean", 1.0, 0.0, i_histo.get_xmax() );
  roo_landau_sigma = new RooRealVar( "Roo_landau_sigma", "Roo_landau_sigma", std::abs(sampleSigma), 0.1, 3*std::abs(sampleSigma));
  roo_landau = new RooLandau( "Roo_landau", "Roo_landau", x, *roo_landau_mean, *roo_landau_sigma );

  x.setBins(10000,"cache");

  RooFFTConvPdf lxg( "lxg", "landau conv gauss", x, *roo_landau, *roo_gaus );

  RooDataHist dataHist("dataHist", i_histo.get_title().c_str(), x, RooFit::Import(*i_histo.get_histo()) );

  RooPlot *frame = x.frame( RooFit::Title( i_histo.get_title().c_str() ) );
  RooFitResult *emptyFit = lxg.fitTo( dataHist, RooFit::Save() );//, Normalization(histo->GetEntries(),RooAbsReal::NumEvent) );

  double landau_mean_error = roo_landau_mean->getPropagatedError( *emptyFit );

  TF1 * lxg_tf = lxg.asTF( RooArgList(x) );
  double peak = lxg_tf->GetMaximumX();
  double peak_y = lxg_tf->Eval(peak);
  double left_50per = lxg_tf->GetX(peak_y/2.0, -1000, peak);
  double right_50per = lxg_tf->GetX(peak_y/2.0, peak, 10.0*peak);
  double landau_FWHM = right_50per - left_50per;

  double landau_left = 2.0;
  double landau_right = 7.0;

  double range_min= peak - landau_left * sampleSigma; //(landau_left * sampleSigma)/1.5;//Roo_lsigma.getVal();
  double range_max = peak + landau_right * sampleSigma;//(landau_right * sampleSigma)/2.0;//Roo_lsigma.getVal();

  i_histo.set_min( range_min );
  i_histo.set_max( range_max );

  double Par = roo_landau_mean->getVal();
  double ParErr = roo_landau_mean->getAsymErrorHi();

  if(savePlot)
  {
    TCanvas *oCanvas = new TCanvas;
    TPaveText *paveText = new TPaveText;
    TF1 *histo_fit = new TF1;

    histo_fit = new TF1( Form("%s_root_fit", i_histo.get_histoName().c_str() ), "landau" );
    histo_fit->SetLineColor(kGreen);
    i_histo.get_histo()->SetLineColor(kRed);
    i_histo.get_histo()->SetLineWidth(3);
    //histo->Scale(1.0/histo->Integral(), "width");
    i_histo.get_histo()->Fit( histo_fit, "Q");
    //if(i_histo.get_histo()->GetEntries() > 700 && i_histo.get_histo()->GetEntries() < 2000 ) i_histo.get_histo()->GetYaxis()->SetRangeUser(0, 400);
    //else if( i_histo.get_histo()->GetEntries() > 2000 ) i_histo.get_histo()->GetYaxis()->SetRangeUser(0, 1000);
    //else i_histo.get_histo()->GetYaxis()->SetRangeUser(0, 50);
    i_histo.get_histo()->GetYaxis()->SetRangeUser(0, i_histo.get_max_bin_value()*1.5);
    i_histo.get_histo()->Draw("same");

    dataHist.plotOn( frame );//, Normalization(histo->GetEntries(), RooAbsReal::NumEvent));
    dataHist.statOn(frame, RooFit::Layout(0.45,0.90,0.6));
    lxg.plotOn( frame );//, Normalization(histo->GetEntries(), RooAbsReal::NumEvent));
    lxg.paramOn(frame, RooFit::Layout(0.45, 0.90, 0.4));
    roo_landau->plotOn( frame, RooFit::LineStyle(kDashed), RooFit::LineColor(kOrange) );//, Normalization(histo->GetEntries(),RooAbsReal::NumEvent) );
    roo_gaus->plotOn( frame, RooFit::LineStyle(kDashed), RooFit::LineColor(kPink) );
    frame->Draw("same");

    paveText = new TPaveText(0.7, 0.9, 0.5, 0.7, "brNDC");
    paveText->AddText( Form("Peak = %lf", peak) );
    paveText->AddText( Form("Error = %lf", landau_mean_error) );
    paveText->AddText( Form("FWHM = %lf", landau_FWHM) );
    paveText->Draw();

    TImage *img = TImage::Create();
    img->FromPad(oCanvas);
    img->WriteImage( Form("%s_%s_lxg_RooFit.png", i_histo.get_fname().c_str(), i_histo.get_title().c_str()) );
    delete img;
    if( histo_fit != NULL ) delete histo_fit;
    if( paveText != NULL ) delete paveText;
    if( oCanvas != NULL ) delete oCanvas;
  }

  double ndf = lxg_tf->GetNDF();
  double chi_square = lxg_tf->GetChisquare();
  double prob = lxg_tf->GetProb();

  FitResult fitResult(Par, ParErr, range_min, range_max, ndf, chi_square, prob);

  if( roo_gaus != NULL ) delete roo_gaus;
  if( roo_gaus_mean != NULL ) delete roo_gaus_mean;
  if( roo_gaus_sigma != NULL ) delete roo_gaus_sigma;
  if( roo_landau != NULL ) delete roo_landau;
  if( roo_landau_mean != NULL ) delete roo_landau_mean;
  if( roo_landau_sigma != NULL ) delete roo_landau_sigma;

  return fitResult; //std::make_tuple( Par, ParErr, range_min, range_max );
}

FitResult Fitter::fitter_RooLanGaus( HistoPackage &i_histo, bool savePlot)
{
  gStyle->SetStatW(0.125);
  gStyle->SetStatH(0.125);

  RooRealVar x( "x", i_histo.get_observable().c_str(), i_histo.get_xmin(), i_histo.get_xmax() );

  RooRealVar *roo_gaus_mean;
  RooRealVar *roo_gaus_sigma;
  RooGaussian *roo_gaus;

  RooRealVar *roo_landau_mean;
  RooRealVar *roo_landau_sigma;
  RooLandau *roo_landau;

  double sampleMean = i_histo.get_histo()->GetMean();
  double sampleSigma = i_histo.get_histo()->GetStdDev();
  int bins = i_histo.get_histo()->GetEntries();
  double histo_max = i_histo.get_histo()->GetXaxis()->GetBinCenter( i_histo.get_histo()->GetMaximumBin() );

  roo_gaus_mean = new RooRealVar( "Roo_gmean", "Roo_gmean", 0.0);
  roo_gaus_sigma = new RooRealVar( "Roo_gsigma", "Roo_gsigma", std::abs(sampleSigma), 0.1, 3*std::abs(sampleSigma) );
  roo_gaus = new RooGaussian( "Roo_gauss", "Roo_gauss", x, *roo_gaus_mean, *roo_gaus_sigma );

  roo_landau_mean = new RooRealVar( "Roo_landau_mean", "Roo_landau_mean", sampleMean, 0.0, i_histo.get_xmax() );
  roo_landau_sigma = new RooRealVar( "Roo_landau_sigma", "Roo_landau_sigma", std::abs(sampleSigma), 0.1, 3*std::abs(sampleSigma));
  roo_landau = new RooLandau( "Roo_landau", "Roo_landau", x, *roo_landau_mean, *roo_landau_sigma );

  x.setBins(10000,"cache");

  RooFFTConvPdf lxg( "lxg", "landau conv gauss", x, *roo_landau, *roo_gaus );

  RooDataHist dataHist("dataHist", i_histo.get_title().c_str(), x, RooFit::Import(*i_histo.get_histo()) );

  RooPlot *frame = x.frame( RooFit::Title( i_histo.get_title().c_str() ) );
  RooFitResult *emptyFit = lxg.fitTo( dataHist, RooFit::Save() );//, Normalization(histo->GetEntries(),RooAbsReal::NumEvent) );

  double landau_mean_error = roo_landau_mean->getPropagatedError( *emptyFit );

  TF1 * lxg_tf = lxg.asTF( RooArgList(x) );
  double peak = lxg_tf->GetMaximumX();
  double peak_y = lxg_tf->Eval(peak);
  double left_50per = lxg_tf->GetX(peak_y/2.0, -1000, peak);
  double right_50per = lxg_tf->GetX(peak_y/2.0, peak, 10.0*peak);
  double landau_FWHM = right_50per - left_50per;

  double landau_left = 2.0;
  double landau_right = 7.0;

  double range_min= peak - landau_left * sampleSigma; //(landau_left * sampleSigma)/1.5;//Roo_lsigma.getVal();
  double range_max = peak + landau_right * sampleSigma;//(landau_right * sampleSigma)/2.0;//Roo_lsigma.getVal();

  i_histo.set_min( range_min );
  i_histo.set_max( range_max );

  double Par = roo_landau_mean->getVal();
  double ParErr = roo_landau_mean->getAsymErrorHi();

  if(savePlot)
  {
    TCanvas *oCanvas = new TCanvas;
    TPaveText *paveText = new TPaveText;
    TF1 *histo_fit = new TF1;

    histo_fit = new TF1( Form("%s_root_fit", i_histo.get_histoName().c_str() ), "landau" );
    histo_fit->SetLineColor(kGreen);
    i_histo.get_histo()->SetLineColor(kRed);
    i_histo.get_histo()->SetLineWidth(3);
    //histo->Scale(1.0/histo->Integral(), "width");
    i_histo.get_histo()->Fit( histo_fit, "Q");
    //if(i_histo.get_histo()->GetEntries() > 700 && i_histo.get_histo()->GetEntries() < 2000 ) i_histo.get_histo()->GetYaxis()->SetRangeUser(0, 400);
    //else if( i_histo.get_histo()->GetEntries() > 2000 ) i_histo.get_histo()->GetYaxis()->SetRangeUser(0, peak*2);
    //else i_histo.get_histo()->GetYaxis()->SetRangeUser(0, 50);
    i_histo.get_histo()->GetYaxis()->SetRangeUser(0, i_histo.get_max_bin_value()*1.5);
    i_histo.get_histo()->Draw("same");

    dataHist.plotOn( frame );//, Normalization(histo->GetEntries(), RooAbsReal::NumEvent));
    dataHist.statOn(frame, RooFit::Layout(0.45,0.90,0.6));
    lxg.plotOn( frame );//, Normalization(histo->GetEntries(), RooAbsReal::NumEvent));
    lxg.paramOn(frame, RooFit::Layout(0.45, 0.90, 0.4));
    roo_landau->plotOn( frame,  RooFit::LineStyle(kDashed),  RooFit::LineColor(kOrange) );//, Normalization(histo->GetEntries(),RooAbsReal::NumEvent) );
    roo_gaus->plotOn( frame,  RooFit::LineStyle(kDashed),  RooFit::LineColor(kPink) );
    frame->Draw("same");

    paveText = new TPaveText(0.7, 0.9, 0.5, 0.7, "brNDC");
    paveText->AddText( Form("Peak = %lf", peak) );
    paveText->AddText( Form("Error = %lf", landau_mean_error) );
    paveText->AddText( Form("FWHM = %lf", landau_FWHM) );
    paveText->Draw();

    TImage *img = TImage::Create();
    img->FromPad (oCanvas);
    img->WriteImage( Form("%s_%s_lxg_RooFit.png", i_histo.get_fname().c_str(), i_histo.get_title().c_str()) );
    delete img;
    if( histo_fit != NULL ) delete histo_fit;
    if( paveText != NULL ) delete paveText;
    if( oCanvas != NULL ) delete oCanvas;
  }

  double ndf = lxg_tf->GetNDF();
  double chi_square = lxg_tf->GetChisquare();
  double prob = lxg_tf->GetProb();

  FitResult fitResult(Par, ParErr, range_min, range_max, ndf, chi_square, prob);

  if( roo_gaus != NULL ) delete roo_gaus;
  if( roo_gaus_mean != NULL ) delete roo_gaus_mean;
  if( roo_gaus_sigma != NULL ) delete roo_gaus_sigma;
  if( roo_landau != NULL ) delete roo_landau;
  if( roo_landau_mean != NULL ) delete roo_landau_mean;
  if( roo_landau_sigma != NULL ) delete roo_landau_sigma;

  return fitResult; //std::make_tuple( Par, ParErr, range_min, range_max );
}

FitResult Fitter::fitter_fit( HistoPackage &i_histo, bool savePlot)
{
  double sampleMean = i_histo.get_histo()->GetMean();
  double sampleSigma = i_histo.get_histo()->GetStdDev();

  this->fitter->SetParameter(1, sampleMean);
  this->fitter->SetParameter(2, sampleSigma);

  i_histo.get_histo()->Fit(this->fitter);

  double range_min= this->fitter->GetParameter(1) - 5 * this->fitter->GetParameter(2);
  double range_max = this->fitter->GetParameter(1) + 5 * this->fitter->GetParameter(2);

  i_histo.set_min(range_min);
  i_histo.set_max(range_max);

  double Par = this->fitter->GetParameter(1);
  double ParErr = this->fitter->GetParError(1);

  if(savePlot)
  {
    TCanvas *oCanvas = new TCanvas( Form("%s_canvas", i_histo.get_histoName().c_str() ) );
    oCanvas->cd();
    i_histo.get_histo()->Draw();
    gStyle->SetOptFit(1);
    gSystem->ProcessEvents();
    TImage *img = TImage::Create();
    img->FromPad (oCanvas);
    img->WriteImage( Form("%s_%s.png", i_histo.get_fname().c_str(), i_histo.get_histoName().c_str()) );
    delete img;
    delete oCanvas;
  }

  double ndf = this->fitter->GetNDF();
  double chi_square = this->fitter->GetChisquare();
  double prob = this->fitter->GetProb();

  FitResult fitResult(Par, ParErr, range_min, range_max, ndf, chi_square, prob);
  return fitResult; //std::make_tuple( Par, ParErr, range_min, range_max );
}
