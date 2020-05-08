#ifndef BETASCOPE_MEASUREMENT_INFO_HPP_MEASUREMENT_INFO_H
#define BETASCOPE_MEASUREMENT_INFO_HPP_MEASUREMENT_INFO_H

#include <string>
#include <vector>

extern const std::string PLOT_CONFIG_VER;

struct PlotConfigMgr {

  // header section
  struct ConfigHeader
  {
    std::string sensor;
    std::string fluence;
    std::string temperature;
    int time_bin;
    double time_range_min;
    double time_range_max;
    bool self_correct;
    std::string average_pulse_ofile_name;
    int number_of_runs;
    int trigger_channel;
    int dut_channel;
    bool use_selected_events;
  };

  struct ConfigSection
  {
    int run_index;
    std::string bias;
    std::string file_name;
    std::string cut[4];
    double cfd;
    int trigger_bias;
    int temperature;
  };


  ConfigHeader header;
  std::vector<ConfigSection> sections;

  PlotConfigMgr(){};

  PlotConfigMgr(ConfigHeader header, std::vector<ConfigSection> sections)
  :header(header), sections(sections){};

  virtual ~PlotConfigMgr(){};

  static void PrintContent( PlotConfigMgr config);
  static PlotConfigMgr ParseConfig(std::string ifile);
  static bool GenerateRunInfo();
};

#endif // BETASCOPE_MEASUREMENT_INFO_HPP_MEASUREMENT_INFO_H
