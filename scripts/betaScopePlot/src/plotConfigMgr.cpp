#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <fmt/format.h>

#include "betaScopePlot/include/plotConfigMgr.h"

PlotConfigMgr PlotConfigMgr::ParseConfig(std::string ifile)
{

  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(ifile, pt);

  PlotConfigMgr::ConfigHeader header = PlotConfigMgr::ConfigHeader{
    pt.get<std::string>("header.sensor"),
    pt.get<std::string>("header.fluence"),
    pt.get<std::string>("header.temperature"),
    pt.get<int>("header.time_bin"),
    pt.get<double>("header.time_range_min"),
    pt.get<double>("header.time_range_max"),
    pt.get<bool>("header.self_correct"),
    pt.get<std::string>("header.average_pulse_ofile_name"),
    pt.get<int>("header.number_of_runs"),
    pt.get<int>("header.trigger_channel"),
    pt.get<int>("header.dut_channel"),
    pt.get<bool>("header.use_selected_events", false)
  };

  std::vector<PlotConfigMgr::ConfigSection> sections;
  for (std::size_t i = 0, max = header.number_of_runs; i < max; i++ )
  {
    std::string run_header = fmt::format("run{}", i);
    sections.push_back(
      PlotConfigMgr::ConfigSection{
        (int)i,
        pt.get<std::string>(fmt::format("{}.bias",run_header)),
        pt.get<std::string>(fmt::format("{}.file_name",run_header)),
        pt.get<std::string>(fmt::format("{}.cut_1",run_header)),
        pt.get<std::string>(fmt::format("{}.cut_2",run_header)),
        pt.get<std::string>(fmt::format("{}.cut_3",run_header)),
        pt.get<std::string>(fmt::format("{}.cut_4",run_header)),
        pt.get<double>(fmt::format("{}.CFD",run_header), 20),
        pt.get<int>(fmt::format("{}.trigger_bias",run_header)),
        pt.get<int>(fmt::format("{}.temperature",run_header))
      }
    );
  }

  return PlotConfigMgr(header, sections);
}

void PlotConfigMgr::PrintContent(PlotConfigMgr config)
{
  fmt::print("Header\n");
  fmt::print("sensor = {}\n", config.header.sensor );
  fmt::print("fluence = {}\n", config.header.fluence );
  fmt::print("temperature = {}\n", config.header.temperature );
  fmt::print("time_bin = {}\n", config.header.time_bin );
  fmt::print("average_pulse_ofile_name = {}\n", config.header.average_pulse_ofile_name );
  fmt::print("number_of_runs = {}\n", config.header.number_of_runs );
  for(auto &sec : config.sections )
  {
    fmt::print("[run{}]\n", sec.run_index);
    fmt::print("bias = {}\n", sec.bias);
    fmt::print("file_name = {}\n", sec.file_name);
    fmt::print("cut1 = {}\n", sec.cut[0]);
    fmt::print("cut2 = {}\n", sec.cut[1]);
    fmt::print("cut3 = {}\n", sec.cut[2]);
    fmt::print("cut4 = {}\n", sec.cut[3]);
    fmt::print("CFD = {}\n", sec.cfd);
    fmt::print("trigger_bias = {}\n", sec.trigger_bias);
    fmt::print("temperature = {}\n", sec.temperature);
  }
}
