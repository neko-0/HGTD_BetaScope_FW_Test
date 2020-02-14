#include "SelfTrigger.h"
#include "General/WaveformAna/include/Waveform_Analysis.hpp"
#include "General/WaveformAna/include/WaveformAna.hpp"

#include <iostream>



void SelfTrigger::Analysis()
{
  // fill up your own analysis in the while loop

  WaveformAnalysis WaveAna;

  WaveformAna<double,double> waveform(
    this->beta_scope.GetInBranch<TTreeReaderArray,double>("w2"),
    this->beta_scope.GetInBranch<TTreeReaderArray,double>("t2")
  );

  const double verScaler = -1.0;
  const double horScaler = 1.0;

  float time_window = waveform.get_v1_value( waveform.size()-1 ) - waveform.get_v1_value(0);

  std::vector<float> threshold = {
    -0.2, -0.1, -0.05, -0.04, -0.03, -0.02, -0.01, -0.007, -0.005, -0.003, -0.002, -0.001,
    0.001, 0.002, 0.003, 0.004, 0.005, 0.006, 0.007, 0.01, 0.02, 0.03, 0.04, 0.05, 0.1, 0.2
  };

  for( const auto &th : threshold)
  {
    int num_pulses = WaveAna.Get_Number_Of_Multiple_Signals(th, waveform.get_v2() );
    this->beta_scope.GetOutBranch<std::vector<int>>("npulses")->push_back(num_pulses);
    this->beta_scope.GetOutBranch<std::vector<double>>("threshold")->push_back(th);
    this->beta_scope.GetOutBranch<std::vector<double>>("rate")->push_back( num_pulses/time_window );
  }
}

void SelfTrigger::Initialize()
{
  // required
  std::string function_name = "SelfTrigger::initialize";
  this->beta_scope.FileOpen(ifile.c_str());

  BetaScope_AnaFramework::Initialize("");

  this->beta_scope.SetInBranch<TTreeReaderValue, double>("i_timestamp", "timestamp");

  this->beta_scope.BuildOutBranch<std::vector<double>>("threshold");
  this->beta_scope.BuildOutBranch<std::vector<int>>("npulses");
  this->beta_scope.BuildOutBranch<std::vector<double>>("rate");
}

void SelfTrigger::LoopEvents()
{
  BetaScope_AnaFramework::LoopEvents(&BetaScope_AnaFramework::Analysis);
}

void SelfTrigger::Finalize()
{
  // required
  BetaScope_AnaFramework::Finalize();
}

// custom class methods start from here
