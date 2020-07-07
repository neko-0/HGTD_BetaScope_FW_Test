#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TImage.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TThread.h>
#include <TCanvas.h>
#include <TFitResult.h>
#include <TVectorF.h>
#include <Math/MinimizerOptions.h>

#include "General/WaveformAna/include/Waveform_Analysis.hpp"


/*==============================================================================
Give the time of the singal maximum (Tmax) using Gaussian fit

==============================================================================*/
WaveformAnalysis::FitResult
WaveformAnalysis::Get_Fit_Tmax(
  const std::vector<double> &voltageVec,
  const std::vector<double> &timeVec,
  const unsigned int &pmax_index
)
{
  //TODO: add fitting code here
  TH1::AddDirectory(kFALSE);
  gROOT->SetBatch(true);
  double tmax_fitted = -9999.;
  double chi2_fitted = -9999.;
  int n_points = 4;
  TVectorF small_voltageVec (2*n_points); // = new float[2*n_points];
  TVectorF small_timeVec (2*n_points);// = new float[2*n_points];

  if(timeVec.at(pmax_index) > -1000. and timeVec.at(pmax_index) < 1000.)
  {
    if(pmax_index > 10 and pmax_index < (timeVec.size() - 10))
    {
      for(int i = 0; i < 2*n_points; i++)
      {
          double t_point = timeVec.at(pmax_index - n_points + i);
          double v_point = voltageVec.at(pmax_index - n_points + i);

          small_timeVec[i] = t_point;
          small_voltageVec[i] = v_point;
      }

      std::string title = std::to_string(timeVec.at(pmax_index) + std::rand());
      TGraph gr(small_timeVec, small_voltageVec);
      gr.SetTitle(title.c_str());
      gr.SetName(title.c_str());

      TThread::Lock();
      //static std::mutex mu;
      //std::unique_lock<std::mutex> lck(mu);
      title = "f_" + title;
      TF1 fu(title.c_str(), "gaus", timeVec.at(pmax_index) - 300., timeVec.at(pmax_index) + 300.);
      fu.AddToGlobalList(false);
      fu.SetParameter(0, timeVec.at(pmax_index));
      fu.SetParameter(1, 100.);
      TFitResultPtr res = gr.Fit(&fu, "SQR");
      //lck.unlock();
      TThread::UnLock();

      tmax_fitted = fu.GetParameter(1);
      chi2_fitted = res->Chi2();

      if(TMath::IsNaN(tmax_fitted))
      {
        TGraph loc_gr(n_points);
        return WaveformAnalysis::FitResult{-9999.0, -9999.0, loc_gr};
      }
      return WaveformAnalysis::FitResult{tmax_fitted, chi2_fitted, gr};
    }
    else
    {
      TGraph gr(n_points);
      return WaveformAnalysis::FitResult{-9999.0, -9999.0, gr};
    }
  }
  else
  {
    TGraph gr(n_points);
    return WaveformAnalysis::FitResult{tmax_fitted, chi2_fitted, gr};
  }
}

/*==============================================================================
Give the time of the singal maximum (Tmax) using linear fit to the 1st derivatve.

==============================================================================*/
WaveformAnalysis::FitResult
WaveformAnalysis::Get_Zero_Cross_Tmax(
  const std::vector<double> &voltageVec,
  const std::vector<double> &timeVec,
  const unsigned int &pmax_index
)
{
  TH1::AddDirectory(kFALSE);

  double tmax_zerocross = -9999.;
  double chi2_fitted = -9999.;

  int n_points = 4;
  //std::cout<<"\n**** start *** \n";
  TVectorF small_voltageVec (2*n_points); // = new float[2*n_points];
  TVectorF small_timeVec (2*n_points);// = new float[2*n_points];

  if(timeVec.at(pmax_index) > -1000. and timeVec.at(pmax_index) < 1000.)
  {
    if(pmax_index > 10 and pmax_index < (timeVec.size() - 10))
    {
      for(int i = 0; i < 2*n_points; i++)
      {
          double t_point = timeVec.at(pmax_index - n_points + i);
          double v_inc = (voltageVec.at(pmax_index - n_points + i + 1) - voltageVec.at(pmax_index - n_points + i));
          double t_inc = (timeVec.at(pmax_index - n_points + i + 1) - timeVec.at(pmax_index - n_points + i));
          double v_point = v_inc / t_inc;

          small_timeVec[i] = t_point;
          small_voltageVec[i] = v_point;
      }

      std::string title = std::to_string(timeVec.at(pmax_index) + std::rand());
      TGraph gr(small_timeVec, small_voltageVec);
      gr.SetTitle(title.c_str());
      gr.SetName(title.c_str());

      TThread::Lock();
      //static std::mutex mu;
      //std::unique_lock<std::mutex> lck(mu);
      title = "f_" + title;
      TF1 fu(title.c_str(), "[0]*x+[1]", timeVec.at(pmax_index) - 300., timeVec.at(pmax_index) + 300.);
      fu.AddToGlobalList(false);
      TFitResultPtr res = gr.Fit(&fu, "SQR");
      //lck.unlock();
      TThread::UnLock();

      tmax_zerocross = fu.GetX(0);
      chi2_fitted = res->Chi2();

      if(TMath::IsNaN(tmax_zerocross))
      {
        TGraph loc_gr(n_points);
        return WaveformAnalysis::FitResult{-9999.0, -9999.0, loc_gr};
      }
      return WaveformAnalysis::FitResult(tmax_zerocross, chi2_fitted, gr);
    }
    else
    {
      TGraph gr(n_points);
      return WaveformAnalysis::FitResult(-9999.0, -9999.0, gr);
    }
  }
  else
  {
    TGraph gr(n_points);
    return WaveformAnalysis::FitResult(tmax_zerocross, chi2_fitted, gr);
  }
}

/*==============================================================================
Give the time of the singal maximum (Tmax) using Gaussian fit

Reimplementation
==============================================================================*/
template <class data_type>
WaveformAnalysis::FitResult WaveformAnalysis::Get_Fit_Tmax( WaveformAna<data_type,data_type> &waveform, const unsigned int &npt)
{

  TH1::AddDirectory(kFALSE);
  gROOT->SetBatch(true);
  double tmax_fitted = -9999.;
  double chi2_fitted = -9999.;

  if(waveform.tmax() > -1000 && waveform.tmax() < 1000)
  {
    auto sub_waveform = waveform.sub_waveform(waveform.max_index()-npt, waveform.max_index()+npt);

    std::string title = std::to_string(waveform.max_index()+std::rand());
    TGraph gr(sub_waveform.size(), &sub_waveform.get_v1()[0], &sub_waveform.get_v2()[0] );
    gr.SetTitle(title.c_str());
    gr.SetName(title.c_str());

    TThread::Lock();
    //static std::mutex mu;
    //std::unique_lock<std::mutex> lck(mu);
    title = "f_" + title;
    TF1 fu(title.c_str(), "gaus", waveform.tmax()-300.0, waveform.tmax()+300.0);
    fu.AddToGlobalList(false);
    fu.SetParameter(0, waveform.tmax());
    fu.SetParameter(1, 100.);
    TFitResultPtr res = gr.Fit(&fu, "SQR");
    //lck.unlock();
    TThread::UnLock();

    tmax_fitted = fu.GetParameter(1);
    chi2_fitted = res->Chi2();
    if(TMath::IsNaN(tmax_fitted))
    {
      tmax_fitted = -9999.0;
      chi2_fitted = -9999.0;
    }

    waveform.fit_tmax(tmax_fitted);
    waveform.fit_tmax_chi(chi2_fitted);
    return FitResult(tmax_fitted, chi2_fitted, gr);
  }
  else
  {
    TGraph gr(waveform.size());

    waveform.fit_tmax(tmax_fitted);
    waveform.fit_tmax_chi(chi2_fitted);
    return WaveformAnalysis::FitResult(tmax_fitted, chi2_fitted, gr);
  }
}


/*==============================================================================
Give the time of the singal maximum (Tmax) using linear fit to the 1st derivatve.

Reimplementation
==============================================================================*/
template <class data_type>
WaveformAnalysis::FitResult WaveformAnalysis::Get_Zero_Cross_Tmax(
  WaveformAna<data_type,data_type> &waveform,
  const unsigned int &npt
)
{
  TH1::AddDirectory(kFALSE);

  double tmax_zerocross = -9999.;
  double chi2_fitted = -9999.;

  if(waveform.tmax() > -1000 && waveform.tmax() < 1000)
  {
    auto sub_waveform = waveform.sub_waveform(waveform.max_index()-npt, waveform.max_index()+npt);
    auto deri_subwaveform = sub_waveform.derivatve();

    std::string title = std::to_string(waveform.max_index()+std::rand());
    TGraph gr(deri_subwaveform.size(), &deri_subwaveform.get_v1()[0], &deri_subwaveform.get_v2()[0] );
    gr.SetTitle(title.c_str());
    gr.SetName(title.c_str());

    TThread::Lock();
    //static std::mutex mu;
    //std::unique_lock<std::mutex> lck(mu);
    title = "f_" + title;
    TF1 fu(title.c_str(), "[0]*x+[1]", waveform.tmax()-300.0, waveform.tmax()+300.0);
    fu.AddToGlobalList(false);
    TFitResultPtr res = gr.Fit(&fu, "SQ");
    //lck.unlock();
    TThread::UnLock();

    tmax_zerocross = fu.GetX(0);
    chi2_fitted = res->Chi2();
    if(TMath::IsNaN(tmax_zerocross))
    {
      tmax_zerocross = -9999.0;
      chi2_fitted = -9999.0;
    }

    waveform.zero_cross_tmax(tmax_zerocross);
    waveform.zero_cross_tmax_chi(chi2_fitted);
    return FitResult(tmax_zerocross, chi2_fitted, gr);
  }
  else
  {
    TGraph gr(waveform.size());

    waveform.zero_cross_tmax(tmax_zerocross);
    waveform.zero_cross_tmax_chi(chi2_fitted);
    return WaveformAnalysis::FitResult(tmax_zerocross, chi2_fitted, gr);
  }
}
