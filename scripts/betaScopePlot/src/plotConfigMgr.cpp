#include "plotConfigMgr.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

PlotConfigMgr::PlotConfigMgr(std::string ifile) {
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(ifile, pt);

  this->sensor = pt.get<std::string>("header.sensor");
  this->fluence = pt.get<std::string>("header.fluence");
  this->temperature = pt.get<std::string>("header.temperature");
  this->time_bin = pt.get<int>("header.time_bin");
  this->time_range_min = pt.get<double>("header.time_range_min");
  this->time_range_max = pt.get<double>("header.time_range_max");
  this->self_correct = pt.get<bool>("header.self_correct");
  this->average_pulse_ofile_name =
      pt.get<std::string>("header.average_pulse_ofile_name");
  this->number_of_runs = pt.get<int>("header.number_of_runs");
  this->trigger_channel = pt.get<int>("header.trigger_channel");
  this->dut_channel = pt.get<int>("header.dut_channel");
  this->use_selected_events = pt.get<bool>("header.use_selected_events", false);

  for (std::size_t i = 0, max = this->number_of_runs; i < max; i++) {
    std::string run_section = "run" + std::to_string(i) + ".";
    std::string bias = run_section + "bias";
    std::string file_name = run_section + "file_name";
    std::string cut_1 = run_section + "cut_1";
    std::string cut_2 = run_section + "cut_2";
    std::string cut_3 = run_section + "cut_3";
    std::string cut_4 = run_section + "cut_4";
    std::string CFD = run_section + "CFD";
    std::string run_temperature = run_section + "temperature";
    std::string trigger_bias = run_section + "trigger_bias";

    this->bias[i].push_back(pt.get<std::string>(bias));
    if (this->use_selected_events) {
      this->file_name[i].push_back("Selected_" +
                                   pt.get<std::string>(file_name));
    } else {
      this->file_name[i].push_back(pt.get<std::string>(file_name));
    }
    this->cut[0][i].push_back(pt.get<std::string>(cut_1));
    this->cut[1][i].push_back(pt.get<std::string>(cut_2));
    this->cut[2][i].push_back(pt.get<std::string>(cut_3));
    this->cut[3][i].push_back(pt.get<std::string>(cut_4));
    this->CFD[i].push_back(pt.get<double>(CFD, 20.0));
    this->run_temperature[i].push_back(pt.get<int>(run_temperature));
    this->trigger_bias[i].push_back(pt.get<int>(trigger_bias));
  }
}

void PlotConfigMgr::PrintContent() {
  std::cout << std::endl;
  std::cout << "sensor= " << this->sensor << std::endl;
  std::cout << "fluence= " << this->fluence << std::endl;
  std::cout << "temperature= " << this->temperature << std::endl;
  std::cout << "time_bin= " << this->time_bin << std::endl;
  std::cout << "average_pulse_ofile_name= " << this->average_pulse_ofile_name
            << std::endl;

  std::cout << "number of run = " << this->number_of_runs << std::endl;

  for (int i = 0; i < this->number_of_runs; i++) {
    std::cout << "bias= " << this->bias[i].at(0) << std::endl;
    std::cout << this->file_name[i].at(0) << std::endl;
    for (unsigned int j = 0; j < this->cut[0][i].size(); j++) {
      for (std::size_t ch = 0, max = 4; ch < max; ch++) {
        std::cout << this->cut[ch][i].at(j) << " ";
      }
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;
}
