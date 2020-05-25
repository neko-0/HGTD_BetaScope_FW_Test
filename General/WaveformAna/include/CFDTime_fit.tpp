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
#include <TVectorD.h>
#include <TVector.h>
#include <Math/MinimizerOptions.h>

#include <fmt/format.h>

#include "WaveformAna/include/Waveform_Analysis.hpp"

template <class data_type>
WaveformAnalysis::FitResult WaveformAnalysis::Fit_CFD(
  WaveformAna<data_type,data_type> &waveform, const double &cfd
)
{
  gROOT->SetBatch(true);
  double range[2] = {-1000, 1000};
  if( waveform.pmax() < 0 )
  {
    WaveformAnalysis::Find_Singal_Maximum(
      waveform.get_v2(), waveform.get_v1(), false, range
    );
  }

  double value_at_cfd = waveform.pmax()*cfd;
  int index_at_cfd = 1;
  double time_at_cfd = -9999.0;
  for(std::size_t j = waveform.max_index(); j>1; j--)
  {
    if( waveform.get_v2_value(j) <= value_at_cfd )
    {
      index_at_cfd = j;
      break;
    }
  }

  if(index_at_cfd == waveform.size()){index_at_cfd = waveform.size()-2;}
  auto sub_waveform = waveform.sub_waveform(index_at_cfd-1, index_at_cfd+2);
  std::string title = fmt::format("fit_cfd_id{}_{}_{}", std::rand(), index_at_cfd, value_at_cfd);
  TVectorD x(sub_waveform.v1().size(), &sub_waveform.v1()[0]);
  TVectorD y(sub_waveform.v2().size(), &sub_waveform.v2()[0]);
  TGraph gr(x,y);
  gr.SetTitle(title.c_str());
  gr.SetName(title.c_str());

  title = "f_" + title;
  TF1 fu(title.c_str(), "[0]*x+[1]");
  fu.AddToGlobalList(false);
  TFitResultPtr res = gr.Fit(&fu, "SQ");
  time_at_cfd = fu->GetX(value_at_cfd);
  double chi2 = res->Chi2();
  if(TMath::IsNaN(time_at_cfd)|| TMath::IsNaN(chi2))
  {
    return WaveformAnalysis::FitResult(-9999.0, -9999.0, gr);
  }
  else
  {
    return WaveformAnalysis::FitResult(time_at_cfd, chi2, gr);
  }
}
