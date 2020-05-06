#include "BetaScope_Driver/include/BetaScope_AnaFramework.h"
#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "General/logger/include/logger.h"
#include "General/WaveformAna/include/Waveform_Analysis.hpp"
#include <iostream>
#include <string>

struct AnaParam {
  bool limiting_search_region_OnOff;
  double pmaxSearchRange[2];
};

class BetaScopeWaveformAna : public BetaScope_AnaFramework<BetaScope> {
private:
  bool isProcessing = false;
  bool has_daq_current = false;
  bool has_daq_timestamp = false;
  bool has_daq_temperature = false;
  bool has_daq_humidity = false;
  bool has_daq_eventNum = false;
  bool has_daq_cycle = false;
  bool has_daq_bias = false;

  bool resample_time = true;
  double dt = 0.0;
  double xorigin = 0.0;

public:
  // this is are required.
  std::string ifile;

  // user define class variables
  std::vector<int> invertChannels;
  std::vector<int> activeChannels;

  bool skipWaveform = false;
  bool skim_output = false;

  bool limitPmaxSearchRange = true;
  double pmaxSearchMinRange = -1000.0; // unit of ps
  double pmaxSearchMaxRange = 1000.0;
  double voltageMultiFactor = 1000.0; // convert Volt to mili-Volt.
  double timeMultiFactor = 1.0e12;    // convert second to pico-second
  double assistThreshold = 10.0;      // unit of mili-Volt
  int triggerCh = 3;
  int counter_ = 0;

  std::vector<double> *w[8];
  std::vector<double> *t[8];
  std::vector<double> *wRaw[8];
  std::vector<double> *tRaw[8];
  std::vector<double> *pmax[8];
  std::vector<double> *tmax[8];
  std::vector<double> *riseTime[8];
  std::vector<double> *cfd[8];
  std::vector<double> *cfd_fall[8];
  std::vector<double> *fineCFDRise[8];
  std::vector<double> *dvdt[8];
  std::vector<double> *thTime[8];
  std::vector<double> *neg_pmax[8];
  std::vector<double> *neg_tmax[8];
  std::vector<double> *fit_tmax[8];
  std::vector<double> *fit_tmax_chi[8];
  std::vector<double> *zero_cross_tmax[8];
  std::vector<double> *zero_cross_tmax_chi[8];
  std::vector<double> *rms[8];
  std::vector<double> *pulseArea_withUndershoot[8];
  std::vector<double> *pulseArea_withZeroCross[8];
  std::vector<double> *backBaselineInt_indepBaseCorr[8];
  std::vector<double> *frontBaselineInt_indepBaseCorr[8];
  std::vector<double> *tot[8];

  std::vector<double> localW[8];
  std::vector<double> localT[8];

  std::vector<WaveformAna<double, double>> *waveform_ana[8];

  double *current;
  double *timestamp;

  TTreeReaderArray<double> *i_w[8];
  TTreeReaderArray<double> *i_w2[8];
  TTreeReaderArray<double> *i_t[8];

  TTreeReaderValue<double> *i_current;
  TTreeReaderValue<double> *i_timestamp;


  AnaParam my_anaParam;

public:

  BetaScopeWaveformAna(){};

  BetaScopeWaveformAna(std::string ifile) : ifile(ifile) {
    LOG_INFO("Using user template: BetaScopeWaveformAna");
  };

  ~BetaScopeWaveformAna(){};

  // required, user can add more to the existing methods;
  void Initialize();
  void Analysis();
  void LoopEvents();
  void Finalize();

  void run(){
    Initialize();
    LoopEvents();
    Finalize();
  };

  void event_ana(int ch, WaveformAna<double, double> waveform);

  void fill_worker(std::vector<double> *buffer, std::vector<double> input);

  bool isGoodTrig( const WaveformAna<double,double> &waveform );

  // custom methods.
  void readWaveformConfig(std::string configName);
  static void generateWaveformConfig();
  void loadFile(std::string ifile) { this->ifile = ifile; };
  void setWaveform(bool value) { this->skipWaveform = value; };

  std::string rawFilesDir = "";
};
