#include <fmt/format.h>

#include "betaScopePlot/include/plotJobMgr.h"


void PlotJobMgr::Fill(const int &channel, const std::string &fitFunc,
  const HistoPackage &histoPack, const std::string &tag
)
{
  this->jobs.push_back( PlotList{channel, fitFunc, histoPack, tag} );
}


PlotJobMgr PlotJobMgr::Create_Default_List( std::string tfile_name )
{
  PlotJobMgr defaultPlotMgr;

  int channel_list[2] = {2, 3};

  for( int &ch : channel_list )
  {
    defaultPlotMgr.Fill(
      ch, "LanGaus",
      HistoPackage(tfile_name, fmt::format("pmax{}[0]",ch) , fmt::format("Pmax Ch{} [mV]",ch), 100, 0, 400, fmt::format("pmax{}",ch)),
      fmt::format("pmax{}",ch)
    );

    defaultPlotMgr.Fill(
      ch, "gaus",
      HistoPackage(tfile_name, fmt::format("rms{}[0]",ch) , fmt::format("RMS Ch{} [mV]",ch), 100, 0, 10, fmt::format("rms{}",ch)),
      fmt::format("rms{}",ch)
    );

    defaultPlotMgr.Fill(
      ch, "gaus",
      HistoPackage(tfile_name, fmt::format("riseTime{}[0]",ch) , fmt::format("Rise Time Ch{} [ps]",ch), 100, 0, 2000, fmt::format("rise{}",ch)),
      fmt::format("rise{}",ch)
    );

    defaultPlotMgr.Fill(
      ch, "gaus",
      HistoPackage(tfile_name, fmt::format("cfd_fall{}[10]-cfd_fall{}[90]",ch, ch) , fmt::format("Fall Time Ch{} [ps]",ch), 100, 0, 5000, fmt::format("fall{}",ch)),
      fmt::format("fall{}",ch)
    );

    defaultPlotMgr.Fill(
      ch, "gaus",
      HistoPackage(tfile_name, fmt::format("dvdt{}[20]",ch) , fmt::format("dvdt 20% Ch{} [V/ps]",ch),  100, 0, 1000, fmt::format("dvdt{}",ch)),
      fmt::format("dvdt{}",ch)
    );

    defaultPlotMgr.Fill(
      ch, "landau",
      HistoPackage(tfile_name, fmt::format("rms{}[0]/dvdt{}[20]",ch, ch) , fmt::format("Jitter 20% Ch{} [ps]",ch), 100, 0, 200, fmt::format("jitter{}",ch)),
      fmt::format("jitter{}",ch)
    );

    defaultPlotMgr.Fill(
      ch, "gaus",
      HistoPackage(tfile_name, fmt::format("cfd_fall{}[50]-cfd{}[50]",ch,ch) , fmt::format("FWHM Ch{} [ps]",ch), 100, 0, 5000, fmt::format("fwhm{}",ch)),
      fmt::format("fwhm{}",ch)
    );

    defaultPlotMgr.Fill(
      ch, "LanGausArea",
      HistoPackage(tfile_name, fmt::format("pulseArea_withZeroCross{}[0]/1.0E-15",ch) , fmt::format("Area 0X Ch{} [ps]",ch), 100, 0, 80e4, fmt::format("area_zero{}",ch)),
      fmt::format("area_zero{}",ch)
    );

    defaultPlotMgr.Fill(
      ch, "LanGausArea",
      HistoPackage(tfile_name, fmt::format("pulseArea_withUndershoot{}[0]/1.0E-15",ch) , fmt::format("Area Under Ch{} [ps]",ch), 100, 0, 80e4, fmt::format("area_under{}",ch)),
      fmt::format("area_under{}",ch)
    );
  }

  defaultPlotMgr.Fill(
    2, "gaus",
    HistoPackage(tfile_name, "cfd2[50]-cfd3[50]" , "Time resolution CFD50% [ps]", 100, -100, 500, "res50"),
    "res50"
  );

  defaultPlotMgr.Fill(
    2, "gaus",
    HistoPackage(tfile_name, "cfd2[20]-cfd3[20]" , "Time resolution CFD20% [ps]", 100, -100, 500, "res20"),
    "res20"
  );

  defaultPlotMgr.Fill(
    2, "gaus",
    HistoPackage(tfile_name, "tmax2[0]-cfd3[20]" , "Time resolution Tmax [ps]", 100, -100, 500, "resTmax"),
    "resTmax"
  );

  defaultPlotMgr.Fill(
    2, "gaus",
    HistoPackage(tfile_name, "zero_cross_tmax2[0]-cfd3[20]" , "Time resolution Zero Cross Tmax Ch2 [ps]", 100, -100, 500, "resZeroCross"),
    "resZeroCross"
  );

  return defaultPlotMgr;
}
