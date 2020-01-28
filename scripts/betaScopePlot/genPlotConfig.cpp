#include "plotConfigMgr.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

void GenerateRunInfo() {
  std::ifstream raw_list;
  raw_list.open("Runlist_raw.txt");
  if (raw_list.is_open())
    printf("preparing run_info.txt \n");
  else
    printf("unable to produce run_info.txt \n");

  std::string raw_list_line;
  std::vector<std::string> file_name;
  while (std::getline(raw_list, raw_list_line)) {
    file_name.push_back(raw_list_line);
  }

  std::ofstream run_info;
  run_info.open("run_info_v08022018.ini");

  run_info << "["
           << "header"
           << "]" << std::endl;
  run_info << "sensor=?" << std::endl;
  run_info << "fluence=?" << std::endl;
  run_info << "temperature=?" << std::endl;
  run_info << "time_bin=50" << std::endl;
  run_info << "time_range_min=-50000" << std::endl;
  run_info << "time_range_max=50000" << std::endl;
  run_info << "self_correct=true" << std::endl;
  run_info << "average_pulse_ofile_name=run_avePulse.root" << std::endl;
  run_info << "number_of_runs=" << file_name.size() << std::endl;
  run_info << "trigger_channel=3" << std::endl;
  run_info << "dut_channel=2" << std::endl;
  run_info << "use_selected_events=false" << std::endl;

  run_info << std::endl;

  // writing parameters for each run.
  for (unsigned int i = 0; i < file_name.size(); i++) {
    run_info << "["
             << "run" << i << "]" << std::endl;
    run_info << "bias=?" << std::endl;
    run_info << "file_name=" << file_name.at(i) << std::endl;
    run_info << "cut_1= " << std::endl;
    run_info << "cut_2= " << std::endl;
    run_info << "cut_3= " << std::endl;
    run_info << "cut_4= " << std::endl;
    run_info << "CFD=20" << std::endl;
    run_info << "temperature=" << std::endl;
    run_info << "trigger_bias=" << std::endl;
    run_info << std::endl;
  }

  run_info.close();
  raw_list.close();
  printf("run_info.ini is ready\n");
  printf("\n");
}
