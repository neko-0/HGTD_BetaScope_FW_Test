#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <fmt/format.h>

#include "betaScopePlot/include/plotConfigMgr.h"

const std::string PLOT_CONFIG_VER = "08022018";

bool PlotConfigMgr::GenerateRunInfo()
{

  system("ls stats_*root* > Runlist_raw.txt");
  std::ifstream raw_list;
  raw_list.open("Runlist_raw.txt");
  if( raw_list.is_open() )
    printf("preparing config file. \n");
  else
  {
    printf("unable to produce config file. \n");
    return false;
  }

  std::string raw_list_line;
  std::vector<std::string> file_name;
  while( std::getline(raw_list, raw_list_line) )
  {
    file_name.push_back(raw_list_line);
  }
  raw_list.close();

  // using property_tree to produce ini file.
  boost::property_tree::ptree pt;

  // Header
  pt.put("header.sensor", "?");
  pt.put("header.fluence", "?");
  pt.put("header.temperature", "?");
  pt.put("header.time_bin", 50);
  pt.put("header.time_range_min", -50000);
  pt.put("header.time_range_max", 50000);
  pt.put("header.average_pulse_ofile_name", "run_avePluse.root");
  pt.put("header.number_of_runs", "");
  pt.put("header.trigger_channel", 3);
  pt.put("header.dut_channel", 2);
  pt.put("header.use_selected_events", false);

  // writing parameters for each run.
  for (unsigned int i = 0; i < file_name.size(); i++)
  {
    std::string run_header = fmt::format("run{}", i);
    pt.put(fmt::format("{}.{}", run_header, "bias"), "?");
    pt.put(fmt::format("{}.{}", run_header, "file_name"), file_name.at(i) );
    pt.put(fmt::format("{}.{}", run_header, "cut_1"), "");
    pt.put(fmt::format("{}.{}", run_header, "cut_2"), "");
    pt.put(fmt::format("{}.{}", run_header, "cut_3"), "");
    pt.put(fmt::format("{}.{}", run_header, "cut_4"), "");
    pt.put(fmt::format("{}.{}", run_header, "CFD"), 20);
    pt.put(fmt::format("{}.{}", run_header, "temperature"), "");
    pt.put(fmt::format("{}.{}", run_header, "trigger_bias"), "");
  }

  boost::property_tree::ini_parser::write_ini(fmt::format("run_info_v{}.ini", PLOT_CONFIG_VER), pt);

  return true;
}
