#include "BetaScopeWaveformAna.h"

#include <fstream>
#include <iostream>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "General/WaveformAna/include/general.hpp"

#include <ctime>
#include <future>
#include <thread>
#include <memory>

#include <TGraph.h>
#include <TF1.h>
#include <TH1.h>
#include <TROOT.h>
#include <Math/MinimizerOptions.h>


bool BetaScopeWaveformAna::isGoodTrig( const WaveformAna<double,double> &waveform )
{
  if( waveform.pmax() >= 70 && waveform.pmax() < 350  && waveform.tmax() > 0 )
  {
    return true;
  }
  else{ return false; }
}

void BetaScopeWaveformAna::event_ana(int ch, WaveformAna<double, double> waveform)
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
  this->fit_tmax[ch]->emplace_back(waveform.fit_tmax());
  //this->fit_tmax[ch]->emplace_back( WaveAna.Get_Fit_Tmax( *this->t[ch], *this->w[ch], pmaxHolder) );

  this->rms[ch]->emplace_back(waveform.rms());

  this->pulseArea_withUndershoot[ch]->emplace_back(waveform.pulse_area_undershoot());
  this->pulseArea_withZeroCross[ch]->emplace_back(waveform.pulse_area());

  this->riseTime[ch]->emplace_back(waveform.rise_time());

  int thcount =  WaveAna.Get_Number_Of_Multiple_Signals( 20, waveform.get_v2() );
  this->beta_scope.SetOutBranchValue( Form("countTH20_%i", ch), thcount );
  this->beta_scope.SetOutBranchValue( Form("undershoot_pmax%i", ch), waveform.undershoot_pmax() );
  this->beta_scope.SetOutBranchValue( Form("undershoot_tmax%i", ch), waveform.undershoot_tmax() );
  this->beta_scope.SetOutBranchValue( Form("isGoodTrig%i", ch), (isGoodTrig(waveform) && thcount<3) ? 1:0 );


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
    this->waveform_ana[ch]->emplace_back(waveform);
  }

  auto subwaveform = waveform.sub_waveform(waveform.max_index()-4, waveform.max_index()+4);
  auto deri_subwaveform = subwaveform.derivative();
  TGraph g(deri_subwaveform.size(), &deri_subwaveform.get_v1()[0], &deri_subwaveform.get_v2()[0]);
  g.SetName(Form("%s_%i_ch%i",this->beta_scope.GetInFileNickName().c_str(), this->counter_, ch));
  this->counter_++;
  TF1 linear("linear", "[0]*x+[1]", deri_subwaveform.get_v1()[0]-300, deri_subwaveform.get_v1()[deri_subwaveform.size()-1]+300);
  linear.AddToGlobalList(false);
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
  TThread::Lock();
  //g.Fit(&linear, "Q");
  TThread::UnLock();
  this->beta_scope.SetOutBranchValue( Form("deri_tmax%i_g", ch), g );
  if(std::all_of(std::begin(deri_subwaveform.get_v2()), std::end(deri_subwaveform.get_v2()), [](double value){return value > 0; }))
  {
    this->beta_scope.SetOutBranchValue( Form("deri_tmax%i_value", ch), 10e11 );
  }
  else if( std::all_of( std::begin(deri_subwaveform.get_v2()), std::end(deri_subwaveform.get_v2()), [](double value){return value < 0; } ) )
  {
    this->beta_scope.SetOutBranchValue( Form("deri_tmax%i_value", ch), 10e11 );
  }
  else
  {
    auto deri_zero_cross = linear.GetX(0, waveform.get_v1()[0], waveform.get_v1()[waveform.size()-1]);
    if(true)//TMath::IsNaN(deri_zero_cross))
    {
      this->beta_scope.SetOutBranchValue( Form("deri_tmax%i_value", ch), 10e11 );
    }
    else
    {
      this->beta_scope.SetOutBranchValue( Form("deri_tmax%i_value", ch), deri_zero_cross );
    }
  }

  /*
  if( !this->skipWaveform )
  {
    for( const auto &value : waveform.get_v2())
    {
      this->w[ch]->emplace_back(-value);
    }
    for( const auto &value : waveform.get_v1())
    {
      this->t[ch]->emplace_back(value);
    }
  }
  else
  {
    this->w[ch]->clear();
    this->t[ch]->clear();
  }
  */


  //this->waveform_ana[ch]->emplace_back(waveform);
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
  std::vector<std::future<void>> workers;
  for (int chh = 0; chh < this->activeChannels.size(); chh++)
  {
    int ch = this->activeChannels.at(chh);

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

    workers.emplace_back(
        std::async( std::launch::async | std::launch::deferred, &BetaScopeWaveformAna::event_ana, this, ch, waveform)
    );
  }

    //BetaScopeWaveformAna::event_ana(ch, waveform);

    //BetaScopeWaveformAna::event_ana(ch, this->invertChannels.at(chh),this->voltageMultiFactor,this->timeMultiFactor,this->resample_time,this->xorigin,this->dt);

  for( std::size_t id = 0; id < workers.size(); id++ )
  {
     //workers[id].join();
     //delete workers[id];
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
  this->beta_scope.FileOpen(ifile.c_str());

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

  if (!this->skipWaveform)
  {
    for (auto ch : this->beta_scope.channel)
    {
      bool branch_checker;
      int branch_counter = 0;
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
  for (int chh = 0; chh < this->activeChannels.size(); chh++)
  {
    int ch = this->activeChannels.at(chh);

    if (!this->skipWaveform)
    {
      this->w[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "w" + std::to_string(ch));
      this->t[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "t" + std::to_string(ch));
      this->beta_scope.BuildOutBranch<std::vector<WaveformAna<double, double>>>("waveform" + std::to_string(ch));
      this->waveform_ana[ch] = this->beta_scope.GetOutBranch<std::vector<WaveformAna<double, double>>>("waveform" + std::to_string(ch));

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
    this->beta_scope.BuildOutBranch<std::vector<double>>(Form("tot%i", ch) );

    this->beta_scope.BuildOutBranch<TGraph>(Form("deri_tmax%i_g", ch) );
    this->beta_scope.BuildOutBranch<double>(Form("deri_tmax%i_value", ch) );

    this->pmax[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("pmax" + std::to_string(ch));
    this->tmax[ch] = this->beta_scope.GetOutBranch<std::vector<double>>("tmax" + std::to_string(ch));
    this->fit_tmax[ch] = this->beta_scope.GetOutBranch<std::vector<double>>( "fit_tmax"+std::to_string(ch) );
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
    if (!this->i_w[ch])
    {
      std::cout << this->i_w[ch] << std::endl;
      std::cout << this->beta_scope.GetInBranch<TTreeReaderArray, double>("w" + std::to_string(ch)) << std::endl;
    }
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
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("temperature", "temperature");
    this->beta_scope.BuildOutBranch<double>("temperature");
  }
  if (beta_scope.IsBranchExists("humidity"))
  {
    this->has_daq_humidity = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("humidity", "humidity");
    this->beta_scope.BuildOutBranch<double>("humidity");
  }
  if (beta_scope.IsBranchExists("i_timestamp"))
  {
    this->has_daq_timestamp = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("i_timestamp", "timestamp");
    this->beta_scope.BuildOutBranch<double>("timestamp");
  }
  if (beta_scope.IsBranchExists("i_current"))
  {
    this->has_daq_current = true;
    this->beta_scope.SetInBranch<TTreeReaderValue, double>("i_current", "current");
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
  LOG_INFO("running Derived class LoopEvents." );
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
