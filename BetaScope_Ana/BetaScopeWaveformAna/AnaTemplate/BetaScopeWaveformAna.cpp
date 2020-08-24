#include "BetaScopeWaveformAna.h"

#include <fstream>
#include <iostream>
#include <functional>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "General/WaveformAna/include/general.hpp"
#include "General/Trigger/include/Trigger.hpp"

#include <ctime>
#include <future>
#include <thread>
#include <memory>

#include <Math/MinimizerOptions.h>


bool BetaScopeWaveformAna::isGoodTrig( const WaveformAna<double,double> &waveform )
{
  if( waveform.pmax() >= 70 && waveform.pmax() < 350  && waveform.tmax() > 0 )
  {
    return true;
  }
  else{ return false; }
}

int BetaScopeWaveformAna::event_ana(int ch, WaveformAna<double, double> waveform)
{
  WaveformAnalysis WaveAna;

  bool confine_search;

  if (ch == this->triggerCh){ confine_search = false; }
  else{ confine_search = true; }

  //WaveformAna<double, double> waveform =
  WaveAna.analyze_waveform(
    waveform,
    confine_search,
    this->my_anaParam.pmaxSearchRange
  );

  this->frontBaselineInt_indepBaseCorr[ch]->emplace_back(waveform.front_baseline_int());
  this->backBaselineInt_indepBaseCorr[ch]->emplace_back(waveform.back_baseline_int());
  this->pmax[ch]->emplace_back(waveform.pmax());
  this->neg_pmax[ch]->emplace_back(waveform.neg_pmax());

  this->tmax[ch]->emplace_back(waveform.tmax());
  this->neg_tmax[ch]->emplace_back(waveform.neg_tmax());

  this->rms[ch]->emplace_back(waveform.rms());

  this->pulseArea_withUndershoot[ch]->emplace_back(waveform.pulse_area_undershoot());
  this->pulseArea_withZeroCross[ch]->emplace_back(waveform.pulse_area());

  this->riseTime[ch]->emplace_back(waveform.rise_time());

  int thcount =  WaveAna.Get_Number_Of_Multiple_Signals( 20, waveform.get_v2() );
  this->beta_scope.SetOutBranchValue( Form("countTH20_%i", ch), thcount );
  this->beta_scope.SetOutBranchValue( Form("undershoot_pmax%i", ch), waveform.undershoot_pmax() );
  this->beta_scope.SetOutBranchValue( Form("undershoot_tmax%i", ch), waveform.undershoot_tmax() );
  this->beta_scope.SetOutBranchValue( Form("isGoodTrig%i", ch), (isGoodTrig(waveform) && thcount<3) ? 1:0 );
  this->beta_scope.SetOutBranchValue( Form("CalTrig%i", ch), Trigger::Calibrated_LGAD_Trigger(waveform));


  *this->cfd[ch] = waveform.cfd();
  *this->cfd_fall[ch] = waveform.cfd_fall();
  *this->dvdt[ch] = waveform.dvdt();
  *this->thTime[ch] = waveform.threshold_time();
  *this->fineCFDRise[ch] = waveform.fine_cfd();
  *this->tot[ch] = waveform.tot();

  if( !this->skipWaveform )
  {
    *this->w[ch] = waveform.v2();
    *this->t[ch] = waveform.v1();
    //this->waveform_ana[ch]->emplace_back(waveform);
  }

  if( this->enable_fit )
  {
    WaveformAnalysis::FitResult tmaxFitHolder = WaveAna.Get_Fit_Tmax( waveform.get_v2(), waveform.get_v1(), waveform.max_index() );
    WaveformAnalysis::FitResult tmaxZeroHolder = WaveAna.Get_Zero_Cross_Tmax( waveform.get_v2(), waveform.get_v1(), waveform.max_index() );
    WaveformAnalysis::FitResult fit_cfd_50 = WaveformAnalysis::Fit_CFD<double>(waveform, 0.5);
    WaveformAnalysis::FitResult fit_cfd_20 = WaveformAnalysis::Fit_CFD<double>(waveform, 0.2);

    this->fit_tmax[ch]->emplace_back(tmaxFitHolder.value );
    this->fit_tmax_chi[ch]->emplace_back(tmaxFitHolder.chi );


    this->zero_cross_tmax[ch]->emplace_back(tmaxZeroHolder.value);
    this->zero_cross_tmax_chi[ch]->emplace_back(tmaxZeroHolder.chi);


    this->beta_scope.SetOutBranchValue(Form("fit_50cfd%i_chi", ch), fit_cfd_50.chi);
    this->beta_scope.SetOutBranchValue(Form("fit_50cfd%i", ch), fit_cfd_50.value);

    this->beta_scope.SetOutBranchValue(Form("fit_20cfd%i_chi", ch), fit_cfd_20.chi);
    this->beta_scope.SetOutBranchValue(Form("fit_20cfd%i", ch), fit_cfd_20.value);

    if(!this->skipWaveform)
    {
      this->beta_scope.SetOutBranchValue(Form("fit_tmax%i_g", ch), tmaxFitHolder.graph);
      this->beta_scope.SetOutBranchValue(Form("zero_cross_tmax%i_g", ch), tmaxZeroHolder.graph);
      this->beta_scope.SetOutBranchValue(Form("fit_20cfd%i_g", ch), fit_cfd_20.graph);
      this->beta_scope.SetOutBranchValue(Form("fit_50cfd%i_g", ch), fit_cfd_50.graph);
    }
  }

  return 0;
}


void BetaScopeWaveformAna::Analysis()
{
  // fill up your own analysis in the while loop

  if (!this->isProcessing)
  {
    LOG_INFO( beta_scope.GetInFileNickName() + " BetaScopeWaveformAna::analysis: Start event processing: ");
    this->isProcessing = true;
  }

  // loop through all the possible channels

  //std::vector<std::thread> workers;
  std::vector<std::future<int>> workers;
  //boost::asio::thread_pool pool(activeChannels.size());
  for( std::size_t chh = 0; chh < this->activeChannels.size(); chh++ )
  {
    int ch = this->activeChannels.at(chh);

    // using one time traces;
    if( this->resample_time && this->dt <= 0.0)
    {
      this->xorigin = this->i_t[ch]->At(0);
      this->dt = this->i_t[ch]->At(1) - this->i_t[ch]->At(0);
    }

    // workers.emplace_back( new std::thread( &BetaScopeWaveformAna::event_ana, this, ch) );
    WaveformAna<double, double> waveform(
      this->i_w[ch],
      this->i_t[ch],
      this->invertChannels.at(chh),
      this->voltageMultiFactor,
      this->timeMultiFactor,
      this->resample_time,
      this->xorigin,
      this->dt
    );

    /*
    workers.push_back(
        //std::async( std::launch::async | std::launch::deferred, &BetaScopeWaveformAna::event_ana, this, ch, waveform)
      std::async( std::launch::deferred, &BetaScopeWaveformAna::event_ana, this, ch, waveform)
    );
    */
    if(this->internal_mp)
    {
      //boost::asio::post(pool, boost::bind(&BetaScopeWaveformAna::event_ana, this, ch, waveform) );
      ///*
      workers.push_back(
          //std::async( std::launch::async | std::launch::deferred, &BetaScopeWaveformAna::event_ana, this, ch, waveform)
        std::async( std::launch::deferred, &BetaScopeWaveformAna::event_ana, this, ch, waveform)
      );
      //*/
    }
    else
    {
      BetaScopeWaveformAna::event_ana(ch, waveform);
    }
  }
  if(this->internal_mp)
  {
    for( auto &worker : workers ){ worker.wait(); }
    //pool.join();
  }

  //BetaScopeWaveformAna::event_ana(ch, this->invertChannels.at(chh),this->voltageMultiFactor,this->timeMultiFactor,this->resample_time,this->xorigin,this->dt);

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


bool BetaScopeWaveformAna::Initialize()
{
  // required
  bool file_opened = this->beta_scope.FileOpen(ifile.c_str());
  if(!file_opened){return false;}

  BETA_LOG::LOG_LEVEL = 5;

  gErrorIgnoreLevel = kFatal;

  char *check_path = getenv("BETASCOPE_SCRIPTS");
  if (check_path != NULL)
  {
    LOG_INFO("Found myOwnTree.ini");
    std::string beta_scope_path(getenv("BETASCOPE_SCRIPTS"));
    BetaScope_AnaFramework::Initialize( beta_scope_path + "/../BetaScope_Ana/BetaScopeWaveformAna/AnaTemplate/myOwnTree.ini");
  }
  else
  {
    LOG_INFO("Did not find myOwnTree.ini");
    BetaScope_AnaFramework::Initialize("");
  }

  this->my_anaParam.limiting_search_region_OnOff = this->limitPmaxSearchRange;
  this->my_anaParam.pmaxSearchRange[0] = this->pmaxSearchMinRange;
  this->my_anaParam.pmaxSearchRange[1] = this->pmaxSearchMaxRange;
  //----------------------

  if( !this->skipWaveform )
  {
    for( auto ch : this->beta_scope.channel )
    {
      LOG_INFO("CH:"+std::to_string(ch) );
      LOG_INFO("Creating branches for storing scope channels: " );
      if( this->beta_scope.BuildOutBranch<std::vector<double>>(Form("w%i", ch)) )
      {
        LOG_INFO("Successful: voltage ch-" + std::to_string(ch) );
      }

      if( this->beta_scope.BuildOutBranch<std::vector<double>>(Form("t%i", ch)) )
      {
        LOG_INFO("Successful: time ch-" + std::to_string(ch) );
      }
    }
  }

  // do your own stuffs here
  // this->beta_scope.treeReader->Next();
  for( const int &ch : this->activeChannels )
  {

    if( !this->skipWaveform)
    {
      this->w[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "w" + std::to_string(ch));
      this->t[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "t" + std::to_string(ch));
      //this->beta_scope.BuildOutBranch<std::vector<WaveformAna<double, double>>>("waveform" + std::to_string(ch));
      //this->waveform_ana[ch] = this->beta_scope.GetOutBranch<std::vector<WaveformAna<double, double>>>("waveform" + std::to_string(ch));
    }
    else
    {
      this->w[ch] = &localW[ch]; // new std::vector<double>;
      this->w[ch]->reserve(10000);
      this->t[ch] = &localT[ch]; // new std::vector<double>;
      this->t[ch]->reserve(10000);
    }

    // build extra branches
    this->beta_scope.BuildOutBranch<int>(Form("countTH20_%i", ch));
    this->beta_scope.BuildOutBranch<double>(Form("undershoot_pmax%i", ch) );
    this->beta_scope.BuildOutBranch<double>(Form("undershoot_tmax%i", ch) );
    this->beta_scope.BuildOutBranch<bool>(Form("isGoodTrig%i", ch) );
    this->beta_scope.BuildOutBranch<bool>(Form("CalTrig%i", ch) );
    this->beta_scope.BuildOutBranch<std::vector<double>>(Form("tot%i", ch) );

    if( this->enable_fit )
    {
      this->beta_scope.BuildOutBranch<double>(Form("fit_20cfd%i", ch) );
      this->beta_scope.BuildOutBranch<double>(Form("fit_20cfd%i_chi", ch) );

      this->beta_scope.BuildOutBranch<double>(Form("fit_50cfd%i", ch) );
      this->beta_scope.BuildOutBranch<double>(Form("fit_50cfd%i_chi", ch) );

      this->fit_tmax[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "fit_tmax"+std::to_string(ch) );
      this->fit_tmax_chi[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "fit_tmax_chi"+std::to_string(ch) );
      this->zero_cross_tmax[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "zero_cross_tmax"+std::to_string(ch) );
      this->zero_cross_tmax_chi[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "zero_cross_tmax_chi"+std::to_string(ch) );

      if(!this->skipWaveform)
      {
        this->beta_scope.BuildOutBranch<TGraph>(Form("zero_cross_tmax%i_g", ch) );
        this->beta_scope.BuildOutBranch<TGraph>(Form("fit_tmax%i_g", ch) );
        this->beta_scope.BuildOutBranch<TGraph>(Form("fit_20cfd%i_g", ch) );
        this->beta_scope.BuildOutBranch<TGraph>(Form("fit_50cfd%i_g", ch) );
      }
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
    this->tot[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("tot"+std::to_string(ch));
    if( !this->i_w[ch] )
    {
      std::cout << this->i_w[ch] << std::endl;
      std::cout << this->beta_scope.GetInBranch<TTreeReaderArray, double>("w" + std::to_string(ch)) << std::endl;
    }
  }

  if( beta_scope.IsBranchExists("ievent") )
  {
    this->has_daq_eventNum = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, int>("ievent", "ievent");
    this->beta_scope.BuildOutBranch<int>("ievent");
  }
  if( beta_scope.IsBranchExists("temperature") )
  {
    this->has_daq_temperature = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("temperature", "temperature");
    this->beta_scope.BuildOutBranch<double>("temperature");
  }
  if( beta_scope.IsBranchExists("humidity"))
  {
    this->has_daq_humidity = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("humidity", "humidity");
    this->beta_scope.BuildOutBranch<double>("humidity");
  }
  if( beta_scope.IsBranchExists("i_timestamp"))
  {
    this->has_daq_timestamp = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("i_timestamp", "timestamp");
    this->beta_scope.BuildOutBranch<double>("timestamp");
  }
  if( beta_scope.IsBranchExists("i_current"))
  {
    this->has_daq_current = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("i_current", "current");
    this->beta_scope.BuildOutBranch<double>("current");
  }
  if( beta_scope.IsBranchExists("bias"))
  {
    this->has_daq_bias = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("bias", "bias");
    this->beta_scope.BuildOutBranch<double>("bias");
  }
  if( beta_scope.IsBranchExists("cycle"))
  {
    this->has_daq_cycle = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, int>("cycle", "cycle");
    this->beta_scope.BuildOutBranch<int>("cycle");
  }

  BETA_LOG::LOG_LEVEL = 0;

  // this->beta_scope.treeReader->Restart();
  return true;
}

void BetaScopeWaveformAna::LoopEvents()
{
  LOG_INFO("running Derived class LoopEvents." );
  if(this->skim_output)
  {
    BetaScope_AnaFramework::LoopEvents(&BetaScope_AnaFramework::Analysis, std::bind(&BetaScopeWaveformAna::Selector, this) );
  }
  else
  {
    BetaScope_AnaFramework::LoopEvents(&BetaScope_AnaFramework::Analysis);
  }
}

bool BetaScopeWaveformAna::Selector()
{
  return *(this->beta_scope.GetOutBranch<bool>("isGoodTrig3"));
}

void BetaScopeWaveformAna::Finalize()
{
  // do your own stuffs here

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
  this->enable_fit = pt.get<bool>("General.enable_fit");

  for (int i = 1; i < 7; i++)
  {
    std::string ch = "channel_" + std::to_string(i);
    if (pt.get<int>("Channel_Activation." + ch) != 0)
    {
      this->activeChannels.emplace_back(i);
      if (pt.get<int>("Channel_Invertion." + ch) != 0)
        this->invertChannels.emplace_back(1);
      else
        this->invertChannels.emplace_back(0);
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
  configFile << "rawFilesDir = ./raw/" << std::endl;
  configFile << "enable_fit = false" << std::endl;

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
