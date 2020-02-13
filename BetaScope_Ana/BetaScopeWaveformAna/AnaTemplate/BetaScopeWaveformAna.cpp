#include "BetaScopeWaveformAna.h"

#include <fstream>
#include <iostream>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "General/WaveformAna/include/general.hpp"

#include <ctime>
#include <future>
#include <thread>

void fill_worker_here(std::vector<double> *buffer, std::vector<double> input) {
  for (auto value : input) {
    buffer->push_back(value);
  }
}

void BetaScopeWaveformAna::event_ana(int ch) {
  WaveformAnalysis WaveAna;

  if (ch == this->triggerCh)
    this->my_anaParam.limiting_search_region_OnOff = false;
  else
    this->my_anaParam.limiting_search_region_OnOff = true;

  WaveformAna<double, double> waveform = WaveAna.analyze_waveform(
    this->t[ch],
    this->w[ch],
    this->my_anaParam.limiting_search_region_OnOff,
    this->my_anaParam.pmaxSearchRange
  );

  this->frontBaselineInt_indepBaseCorr[ch]->push_back(waveform.get_front_baseline_int());
  this->backBaselineInt_indepBaseCorr[ch]->push_back(waveform.get_back_baseline_int());
  this->pmax[ch]->push_back(waveform.get_pmax());
  this->neg_pmax[ch]->push_back(waveform.get_neg_pmax());

  this->tmax[ch]->push_back(waveform.get_tmax());
  this->neg_tmax[ch]->push_back(waveform.get_neg_tmax());

  this->rms[ch]->push_back(waveform.get_rms());

  this->pulseArea_withUndershoot[ch]->push_back(waveform.get_pulse_area_undershoot());
  this->pulseArea_withZeroCross[ch]->push_back(waveform.get_pulse_area());

  this->riseTime[ch]->push_back(waveform.get_rise_time());

  // std::vector<std::thread*> workers;
  // std::vector<std::future<void>> workers;

  /*
  workers.push_back( std::async( fill_worker_here, this->cfd[ch],
  waveform.get_cfd() ) ); workers.push_back( std::async( fill_worker_here,
  this->cfd_fall[ch], waveform.get_cfd_fall() ) ); workers.push_back(
  std::async( fill_worker_here, this->dvdt[ch], waveform.get_dvdt() ) );
  workers.push_back( std::async( fill_worker_here, this->thTime[ch],
  waveform.get_threshold_time() ) ); workers.push_back( std::async(
  fill_worker_here, this->fineCFDRise[ch], waveform.get_fine_cfd() ) );
  */

  ///*
  for (auto value : waveform.get_cfd()) { this->cfd[ch]->push_back(value); }
  for (auto value : waveform.get_cfd_fall()) { this->cfd_fall[ch]->push_back(value); }
  for (auto value : waveform.get_dvdt()) { this->dvdt[ch]->push_back(value); }
  for (auto value : waveform.get_threshold_time()) { this->thTime[ch]->push_back(value); }
  for (auto value : waveform.get_fine_cfd()) { this->fineCFDRise[ch]->push_back(value); }
  //*/

  if (!this->skipWaveform)
  {
    for (auto value : waveform.get_v2())
    {
      this->w[ch]->push_back(-value);
    }
    for (auto value : waveform.get_v1())
    {
      this->t[ch]->push_back(value);
    }
    // workers.push_back( std::async( fill_worker_here, this->w[ch],
    // waveform.get_v2() ) ); workers.push_back( std::async( fill_worker_here,
    // this->t[ch], waveform.get_v1() ) );
  }
  else
  {
    this->w[ch]->clear();
    this->t[ch]->clear();
  }

  /*
  for(std::size_t id=0; id < workers.size(); id++ )
  {
      workers[id].wait();
      //workers[id]->join();
      //delete workers[id];
  }
  */

  this->waveform_ana[ch]->push_back(waveform);
}

void BetaScopeWaveformAna::Analysis() {
  // fill up your own analysis in the while loop

  // WaveformAnalysis WaveAna;

  if (!this->isProcessing)
  {
    ColorCout::print(
        "   " + beta_scope.GetInFileNickName(),
        " BetaScopeWaveformAna::analysis: Start event processing: ",
        BOLDYELLOW
    );
    this->isProcessing = true;
  }

  // if(this->beta_scope.ieventFromDAQ)*this->beta_scope.oTreeIntMap["ievent"] =
  // **this->beta_scope.iTreeIntValueMap["ievent"];

  // std::time_t t1 = std::time(nullptr);
  // loop through all the possible channels
  // std::vector<std::thread*> workers;
  std::vector<std::future<void>> workers;
  for (int chh = 0; chh < this->activeChannels.size(); chh++)
  {
    int ch = this->activeChannels.at(chh);
    // if(std::find(this->activeChannels.begin(), this->activeChannels.end(),
    // ch) != this->activeChannels.end())
    if (this->resample_time && this->dt <= 0.0)
    {
      this->xorigin = this->i_t[ch]->At(0);
      this->dt = this->i_t[ch]->At(1) - this->i_t[ch]->At(0);
    }
    if (true)
    {
      // if( this->beta_scope.iTreeDoubleArrayMap.count("w"+std::to_string(ch)
      // ))
      if (true)
      {
        if (!this->i_w[ch])
          std::cout << ch << std::endl;
        for (std::size_t i = 0, max = this->i_w[ch]->GetSize(); i < max; i++)
        {
          // std::cout<<  <<std::endl;
          // if( std::find(this->invertChannels.begin(),
          // this->invertChannels.end(), ch) != this->invertChannels.end() )
          if (this->invertChannels.at(chh) != 0)
          {
            this->w[ch]->push_back(this->i_w[ch]->At(i) * -1.0 * this->voltageMultiFactor);
            // this->w[ch]->at(i) = this->i_w[ch]->At(i) * -1.0 *
            // this->voltageMultiFactor;
          }
          else
          {
            this->w[ch]->push_back(this->i_w[ch]->At(i) * this->voltageMultiFactor);
            // this->w[ch]->at(i) = this->i_w[ch]->At(i) *
            // this->voltageMultiFactor;
          }

          if (this->resample_time)
          {
            this->t[ch]->push_back((this->xorigin + i * this->dt) * this->timeMultiFactor);
            // this->xorigin = this->xorigin + this->dt;
          }
          else {
            this->t[ch]->push_back(this->i_t[ch]->At(i) * this->timeMultiFactor);
          }
          // std::cout << this->w[ch]->at(5) << std::endl;
          // std::cout << this->w[ch]->size() << std::endl;

          // this->wRaw[ch]->push_back( this->i_w[ch]->At(i) *
          // this->voltageMultiFactor ); this->tRaw[ch]->push_back(
          // this->i_t[ch]->At(i) * this->timeMultiFactor );

          /*
          this->t[ch]->at(i) = this->i_t[ch]->At(i) * this->timeMultiFactor;
          this->wRaw[ch]->at(i) = this->i_w[ch]->At(i) *
          this->voltageMultiFactor; this->tRaw[ch]->at(i) = this->i_t[ch]->At(i)
          * this->timeMultiFactor;
          */
        }
        // this->xorigin = 0.0;
        // this->dt = 0.0;

        // workers.push_back( new std::thread( &BetaScopeWaveformAna::event_ana,
        // this, ch) );
        workers.push_back( std::async(&BetaScopeWaveformAna::event_ana, this, ch));
      }
    }
  }

  for (std::size_t id = 0; id < workers.size(); id++)
  {
    // workers[id]->join();
    // delete workers[id];
    workers[id].wait();
  }

  // filling value that's indep of scope channels
  if (this->has_daq_cycle)
  {
    this->beta_scope.SetOutBranchValue( "cycle", this->beta_scope.GetInBranchValue<TTreeReaderValue, int>("cycle"));
  }
  if (this->has_daq_temperature)
  {
    this->beta_scope.SetOutBranchValue( "temperature", this->beta_scope.GetInBranchValue<TTreeReaderValue, double>("temperature"));
  }
  if (this->has_daq_humidity)
  {
    this->beta_scope.SetOutBranchValue( "humidity", this->beta_scope.GetInBranchValue<TTreeReaderValue, double>("humidity"));
  }
  if (this->has_daq_bias)
  {
    this->beta_scope.SetOutBranchValue( "bias", this->beta_scope.GetInBranchValue<TTreeReaderValue, double>("bias"));
  }
  if (this->has_daq_timestamp)
  {
    this->beta_scope.SetOutBranchValue("timestamp", this->beta_scope.GetInBranchValue<TTreeReaderValue, double>("timestamp"));
  }
  if (this->has_daq_current)
  {
    this->beta_scope.SetOutBranchValue("current", this->beta_scope.GetInBranchValue<TTreeReaderValue, double>("current"));
  }
  if (this->has_daq_eventNum)
  {
    this->beta_scope.SetOutBranchValue( "ievent", this->beta_scope.GetInBranchValue<TTreeReaderValue, int>("ievent"));
  }
}

void BetaScopeWaveformAna::Initialize() {
  // required
  std::string function_name = "BetaScopeWaveformAna::initialize";
  this->beta_scope.FileOpen(ifile.c_str());

  char *check_path = getenv("BETASCOPE_SCRIPTS");
  if (check_path != NULL)
  {
    ColorCout::Msg(function_name, "Found myOwnTree.ini");
    std::string beta_scope_path(getenv("BETASCOPE_SCRIPTS"));
    BetaScope_AnaFramework::Initialize( beta_scope_path + "/../BetaScope_Ana/BetaScopeWaveformAna/AnaTemplate/myOwnTree.ini");
  }
  else
  {
    ColorCout::Msg(function_name, "Did not find myOwnTree.ini");
    BetaScope_AnaFramework::Initialize("");
  }

  this->my_anaParam.limiting_search_region_OnOff = this->limitPmaxSearchRange;
  this->my_anaParam.pmaxSearchRange[0] = this->pmaxSearchMinRange;
  this->my_anaParam.pmaxSearchRange[1] = this->pmaxSearchMaxRange;
  //----------------------

  if (!this->skipWaveform)
  {
    for (auto ch : this->beta_scope.channel)
    {
      bool branch_checker;
      int branch_counter = 0;
      ColorCout::print("  CH:", std::to_string(ch), CYAN);

      ColorCout::print("  ", "Creating branches for storing scope channels: ", YELLOW);
      branch_checker = this->beta_scope.BuildOutBranch<std::vector<double>>(Form("w%i", ch));

      branch_checker = this->beta_scope.BuildOutBranch<std::vector<double>>(Form("t%i", ch));

      if (branch_checker)
      {
        ColorCout::print("  Successful:", std::to_string(ch), CYAN);
      }
    }
  }

  // do your own stuffs here
  // this->beta_scope.treeReader->Next();
  for (int chh = 0; chh < this->activeChannels.size(); chh++)
  {
    int ch = this->activeChannels.at(chh);

    if (!this->skipWaveform)
    {
      this->w[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "w" + std::to_string(ch));
      this->t[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "t" + std::to_string(ch));
    }
    else
    {
      this->w[ch] = &localW[ch]; // new std::vector<double>;
      this->w[ch]->reserve(10000);
      this->t[ch] = &localT[ch]; // new std::vector<double>;
      this->t[ch]->reserve(10000);
    }

    this->pmax[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("pmax" + std::to_string(ch));
    this->tmax[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("tmax" + std::to_string(ch));
    this->neg_pmax[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("neg_pmax" + std::to_string(ch));
    this->neg_tmax[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("neg_tmax" + std::to_string(ch));
    this->riseTime[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("riseTime" + std::to_string(ch));
    this->dvdt[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("dvdt" + std::to_string(ch));
    this->cfd_fall[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("cfd_fall" + std::to_string(ch));
    this->cfd[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("cfd" + std::to_string(ch));
    this->fineCFDRise[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("fineCFDRise" + std::to_string(ch));
    this->thTime[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("thTime" + std::to_string(ch));
    this->rms[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("rms" + std::to_string(ch));
    this->pulseArea_withUndershoot[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "pulseArea_withUndershoot" + std::to_string(ch));
    this->pulseArea_withZeroCross[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("pulseArea_withZeroCross" + std::to_string(ch));
    this->frontBaselineInt_indepBaseCorr[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("frontBaselineInt_indepBaseCorr" + std::to_string(ch));
    this->backBaselineInt_indepBaseCorr[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("backBaselineInt_indepBaseCorr" + std::to_string(ch));
    this->i_w[ch] = this->beta_scope.GetInBranch<TTreeReaderArray, double>("w" + std::to_string(ch));
    this->i_t[ch] = this->beta_scope.GetInBranch<TTreeReaderArray, double>("t" + std::to_string(ch));
    if (!this->i_w[ch])
    {
      std::cout << this->i_w[ch] << std::endl;
      std::cout << this->beta_scope.GetInBranch<TTreeReaderArray, double>("w" + std::to_string(ch)) << std::endl;
    }
    this->beta_scope.BuildOutBranch<std::vector<WaveformAna<double, double>>>("waveform" + std::to_string(ch));
    this->waveform_ana[ch] = this->beta_scope.GetOutBranch<std::vector<WaveformAna<double, double>>>("waveform" + std::to_string(ch));
  }

  if (beta_scope.IsBranchExists("ievent"))
  {
    this->has_daq_eventNum = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, int>("ievent", "ievent");
    this->beta_scope.BuildOutBranch<int>("ievent");
  }
  if (beta_scope.IsBranchExists("temperature"))
  {
    this->has_daq_temperature = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("temperature",
                                                           "temperature");
    this->beta_scope.BuildOutBranch<double>("temperature");
  }
  if (beta_scope.IsBranchExists("humidity"))
  {
    this->has_daq_humidity = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("humidity",
                                                           "humidity");
    this->beta_scope.BuildOutBranch<double>("humidity");
  }
  if (beta_scope.IsBranchExists("i_timestamp"))
  {
    this->has_daq_timestamp = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("i_timestamp",
                                                           "timestamp");
    this->beta_scope.BuildOutBranch<double>("timestamp");
  }
  if (beta_scope.IsBranchExists("i_current"))
  {
    this->has_daq_current = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("i_current",
                                                           "current");
    this->beta_scope.BuildOutBranch<double>("current");
  }
  if (beta_scope.IsBranchExists("bias"))
  {
    this->has_daq_bias = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("bias", "bias");
    this->beta_scope.BuildOutBranch<double>("bias");
  }
  if (beta_scope.IsBranchExists("cycle"))
  {
    this->has_daq_cycle = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, int>("cycle", "cycle");
    this->beta_scope.BuildOutBranch<int>("cycle");
  }

  // this->beta_scope.treeReader->Restart();
}

void BetaScopeWaveformAna::LoopEvents()
{
  BetaScope_AnaFramework::LoopEvents(&BetaScope_AnaFramework::Analysis);
}

void BetaScopeWaveformAna::Finalize()
{
  // do your own stuffs here
  /*
  if(this->skipWaveform)
  {
    for(int chh = 0; chh < this->activeChannels.size(); chh ++)
    {
      int ch = this->activeChannels.at(chh);
      if(this->w[ch]!=NULL)delete this->w[ch];
      if(this->t[ch]!=NULL)delete this->t[ch];
    }
  }
  */

  // required
  BetaScope_AnaFramework::Finalize();
}

// custom class methods start from here
void BetaScopeWaveformAna::readWaveformConfig(std::string configName)
{
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(configName, pt);

  this->voltageMultiFactor = pt.get<double>("General.voltageMultiFactor");
  this->timeMultiFactor = pt.get<double>("General.timeMultiFactor");
  this->voltageMultiFactor = pt.get<double>("General.voltageMultiFactor");
  this->limitPmaxSearchRange = pt.get<bool>("General.limitPmaxSearchRange");
  this->pmaxSearchMinRange = pt.get<double>("General.pmaxSearchMinRange");
  this->pmaxSearchMaxRange = pt.get<double>("General.pmaxSearchMaxRange");
  this->assistThreshold = pt.get<double>("General.assistThreshold");
  this->triggerCh = pt.get<int>("General.triggerCh");
  this->rawFilesDir = pt.get<std::string>("General.rawFilesDir");

  for (int i = 1; i < 7; i++)
  {
    std::string ch = "channel_" + std::to_string(i);
    if (pt.get<int>("Channel_Activation." + ch) != 0)
    {
      this->activeChannels.push_back(i);
      if (pt.get<int>("Channel_Invertion." + ch) != 0)
        this->invertChannels.push_back(1);
      else
        this->invertChannels.push_back(0);
    }
  }
}

void BetaScopeWaveformAna::generateWaveformConfig()
{
  std::ofstream configFile("WaveformAnaConfig.ini");

  configFile << "[General]" << std::endl;
  configFile << "voltageMultiFactor = 1000.0" << std::endl;
  configFile << "timeMultiFactor = 1.0e12" << std::endl;
  configFile << "limitPmaxSearchRange = true" << std::endl;
  configFile << "pmaxSearchMinRange = -2200.0 " << std::endl;
  configFile << "pmaxSearchMaxRange = 2200.0 " << std::endl;
  configFile << "assistThreshold = 10.0 " << std::endl;
  configFile << "triggerCh = 3 " << std::endl;
  configFile << "rawFilesDir = " << std::endl;

  configFile << std::endl;

  configFile << "[Channel_Activation]" << std::endl;
  for (int i = 1; i < 7; i++)
  {
    configFile << "channel_" << i << " = 0" << std::endl;
  }

  configFile << std::endl;

  configFile << "[Channel_Invertion]" << std::endl;
  for (int i = 1; i < 7; i++)
  {
    configFile << "channel_" << i << " = 0" << std::endl;
  }
}
