#include "betaScopePlot/include/plotJobMgr.h"

#include <fmt/format.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

PlotJobMgr PlotJobMgr::operator +(const PlotJobMgr &inputConfigMgr)
{
  PlotJobMgr merged;
  merged.jobs.insert(this->jobs.end(), this->jobs.begin(), this->jobs.end() );
  merged.jobs.insert(merged.jobs.end(), inputConfigMgr.jobs.begin(), inputConfigMgr.jobs.end() );
  return merged;
}

PlotJobMgr &PlotJobMgr::operator +=(const PlotJobMgr &inputConfigMgr)
{
  this->jobs.insert(this->jobs.end(), inputConfigMgr.jobs.begin(), inputConfigMgr.jobs.end() );
  return *this;
}


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

  return defaultPlotMgr;
}

//==============================================================================
//==============================================================================
PlotJobMgr PlotJobMgr::Read_List(std::string tfile_name, std::string job_list)
{
    std::string file_loc;
    if(job_list == "")
    {
      std::cout << "opening additional plot list from default location\n";

      std::string beta_scope_path(getenv("BETASCOPE_SCRIPTS"));
      file_loc = fmt::format("{}/betaScopePlot/src/plot_list.json", beta_scope_path);
    }
    else
    {
      file_loc = job_list;
    }

    boost::property_tree::ptree pt;
    boost::property_tree::read_json(file_loc, pt);

    PlotJobMgr output_plotJobMgr;

    for(const auto &item : pt.get_child("plot_list"))
    {
      const auto &job = item.second;
      const auto &bin = job.get_child("bin");

      output_plotJobMgr.Fill(
        job.get<int>("channel", 2),
        job.get<std::string>("fit"),
        HistoPackage(
          tfile_name,
          job.get<std::string>("expression"),
          job.get<std::string>("title"),
          bin.get<int>("nbin"), bin.get<double>("xmin"), bin.get<double>("xmax"),
          job.get<std::string>("histo_tag")
        ),
        job.get<std::string>("tag")
      );
    }

    return output_plotJobMgr;
}
