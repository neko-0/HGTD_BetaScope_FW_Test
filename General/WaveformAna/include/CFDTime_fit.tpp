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

  auto sub_waveform = waveform.sub_waveform(index_at_cfd-1, index_at_cfd+1);
  std::string title = fmt::format("{}_{}", index_at_cfd, value_at_cfd);
  TGraph gr(sub_waveform.size(), &sub_waveform.get_v1()[0], &sub_waveform.get_v2()[0] );
  gr.SetTitle(title.c_str());
  gr.SetName(title.c_str());

  title = "f_" + title;
  TF1 fu(title.c_str(), "[0]*x+[1]", sub_waveform.get_v1_value(0)-50, sub_waveform.get_v1_value(sub_waveform.size()-1)+50);
  fu.AddToGlobalList(false);
  TFitResultPtr res = gr.Fit(&fu, "SQ");
  time_at_cfd = fu.GetX(value_at_cfd);
  if(TMath::IsNaN(time_at_cfd))
  {
    return WaveformAnalysis::FitResult{time_at_cfd, -9999.0, gr};
  }
  else
  {
    return WaveformAnalysis::FitResult{time_at_cfd, res->Chi2(), gr};
  }
}
