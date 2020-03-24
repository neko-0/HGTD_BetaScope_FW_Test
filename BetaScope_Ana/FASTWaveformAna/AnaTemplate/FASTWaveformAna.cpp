#include "FASTWaveformAna.h"
#include "General/WaveformAna/include/Waveform_Analysis.hpp"
#include "General/WaveformAna/include/WaveformAna.hpp"
#include "General/Trigger/include/Trigger.hpp"

#include <iostream>



void FASTWaveformAna::Analysis()
{
  // fill up your own analysis in the while loop

  WaveformAnalysis WaveAna;

  WaveformAna<double,double> dut_waveform(
    this->beta_scope.GetInBranch<TTreeReaderArray,double>("w1"),
    this->beta_scope.GetInBranch<TTreeReaderArray,double>("t1"),
    false, //inverting waveform
    1000.0, //vscale factor
    1000.0, //hscale factor
    false, // time resample
    0,
    50
  );

  WaveformAna<double,double> trigger_waveform(
    this->beta_scope.GetInBranch<TTreeReaderArray,double>("w3"),
    this->beta_scope.GetInBranch<TTreeReaderArray,double>("t3"),
    false, //inverting waveform
    1000.0, //vscale factor
    1000.0, //hscale factor
    false, // time resample
    0,
    50
  );

  double range[2] = {-1000, 1000};
  WaveAna.analyze_waveform(trigger_waveform, false, range );
  this->beta_scope.SetOutBranchValue("isGoodTrig", Trigger::LGAD_Trigger(trigger_waveform));

  std::vector<double> tot_buffer;
  tot_buffer.reserve(1000);
  std::vector<double> tat_buffer;
  tat_buffer.reserve(1000);

  std::vector<double> buffer;

  for (int k = 0; k < 1000; k++)
  {
      tat_buffer.emplace_back( WaveAna.Find_Time_At_Threshold( double(k), dut_waveform.get_v2(), dut_waveform.get_v1()) );
      tot_buffer.emplace_back( WaveAna.Get_TimeAcrossThreshold( double(k), dut_waveform.get_v2(), dut_waveform.get_v1(), buffer, 2));
  }

  this->beta_scope.SetOutBranchValue("ToT15", tot_buffer[15] );
  this->beta_scope.SetOutBranchValue("ToT20", tot_buffer[20] );
  this->beta_scope.SetOutBranchValue("ToT25", tot_buffer[25] );
  this->beta_scope.SetOutBranchValue("ToT30", tot_buffer[30] );
  this->beta_scope.SetOutBranchValue("ToT40", tot_buffer[40] );
  this->beta_scope.SetOutBranchValue("ToT50", tot_buffer[50] );
  this->beta_scope.SetOutBranchValue("ToT100", tot_buffer[100] );

  this->beta_scope.SetOutBranchValue("TaT15", tat_buffer[15] );
  this->beta_scope.SetOutBranchValue("TaT20", tat_buffer[20] );
  this->beta_scope.SetOutBranchValue("TaT25", tat_buffer[25] );
  this->beta_scope.SetOutBranchValue("TaT30", tat_buffer[30] );
  this->beta_scope.SetOutBranchValue("TaT40", tat_buffer[40] );
  this->beta_scope.SetOutBranchValue("TaT50", tat_buffer[50] );
  this->beta_scope.SetOutBranchValue("TaT100", tat_buffer[100] );

  bool found_pmax = false;
  for( int pmax = 0; pmax < 2000; pmax++)
  {
    int num_pulses = WaveAna.Get_Number_Of_Multiple_Signals(pmax, dut_waveform.get_v2() );
    if(num_pulses < 1 && pmax > 6)
    {
      this->beta_scope.SetOutBranchValue("pmax", pmax);
      found_pmax = true;
      break;
    }
  }
  if(!found_pmax){this->beta_scope.SetOutBranchValue("pmax", 1000);}

  *this->beta_scope.GetOutBranch<std::vector<double>>("w1") = dut_waveform.v2();
  *this->beta_scope.GetOutBranch<std::vector<double>>("t1") = dut_waveform.v1();
  *this->beta_scope.GetOutBranch<std::vector<double>>("w3") = trigger_waveform.v2();
  *this->beta_scope.GetOutBranch<std::vector<double>>("t3") = trigger_waveform.v1();

}

void FASTWaveformAna::Initialize()
{
  // required
  std::string function_name = "FASTWaveformAna::initialize";
  this->beta_scope.FileOpen(ifile.c_str());

  BetaScope_AnaFramework::Initialize("");

  this->beta_scope.BuildOutBranch<bool>("isGoodTrig");
  this->beta_scope.BuildOutBranch<double>("ToT15");
  this->beta_scope.BuildOutBranch<double>("ToT20");
  this->beta_scope.BuildOutBranch<double>("ToT25");
  this->beta_scope.BuildOutBranch<double>("ToT30");
  this->beta_scope.BuildOutBranch<double>("ToT40");
  this->beta_scope.BuildOutBranch<double>("ToT50");
  this->beta_scope.BuildOutBranch<double>("ToT100");

  this->beta_scope.BuildOutBranch<double>("TaT15");
  this->beta_scope.BuildOutBranch<double>("TaT20");
  this->beta_scope.BuildOutBranch<double>("TaT25");
  this->beta_scope.BuildOutBranch<double>("TaT30");
  this->beta_scope.BuildOutBranch<double>("TaT40");
  this->beta_scope.BuildOutBranch<double>("TaT50");
  this->beta_scope.BuildOutBranch<double>("TaT100");

  this->beta_scope.BuildOutBranch<double>("pmax");

  this->beta_scope.BuildOutBranch<std::vector<double>>("w1");
  this->beta_scope.BuildOutBranch<std::vector<double>>("w3");
  this->beta_scope.BuildOutBranch<std::vector<double>>("t1");
  this->beta_scope.BuildOutBranch<std::vector<double>>("t3");

}

void FASTWaveformAna::LoopEvents()
{
  BetaScope_AnaFramework::LoopEvents(&BetaScope_AnaFramework::Analysis);
}

void FASTWaveformAna::Finalize()
{
  // required
  BetaScope_AnaFramework::Finalize();
}

// custom class methods start from here
