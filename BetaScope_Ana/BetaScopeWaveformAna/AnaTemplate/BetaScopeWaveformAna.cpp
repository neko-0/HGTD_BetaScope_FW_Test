#include "BetaScopeWaveformAna.h"

#include <fstream>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "General/WaveformAna/include/Waveform_Analysis.hpp"
#include "General/WaveformAna/include/general.hpp"

#include <ctime>
#include <thread>


int THREAD_COUNT = 0;

void BetaScopeWaveformAna::thread_it( int ch)
{
  WaveformAnalysis WaveAna;

  if( THREAD_COUNT < 100){
    ColorCout::print("  Running thread_it: thread num ", std::to_string(THREAD_COUNT), CYAN);
    THREAD_COUNT++;
  }
  // Base line
  std::vector<double> front_temp_voltage = *this->w[ch];
  std::vector<double> front_temp_time = *this->t[ch];

  std::vector<double> back_temp_voltage = *this->w[ch];
  std::vector<double> back_temp_time = *this->t[ch];

  std::pair<double,unsigned int> pmax_before_baseline = WaveAna.Find_Singal_Maximum( *this->w[ch], *this->t[ch], this->my_anaParam.limiting_search_region_OnOff, this->my_anaParam.pmaxSearchRange);
  double tmax_for_baseline = this->t[ch]->at(pmax_before_baseline.second);
  double temp_riseTime = WaveAna.Find_Rise_Time( *this->w[ch], *this->t[ch], pmax_before_baseline, 0.1, 0.9 );

  double temp_front_searchRange[2] = {tmax_for_baseline-temp_riseTime-15000.0, tmax_for_baseline-temp_riseTime-10000.0};
  std::pair<double,unsigned int> front_baseline_pmax = WaveAna.Find_Singal_Maximum( *this->w[ch], *this->t[ch], true, temp_front_searchRange );
  double front_baseline_tmax = this->t[ch]->at(front_baseline_pmax.second);
  double front_baseline_riseTime = WaveAna.Find_Rise_Time( *this->w[ch], *this->t[ch], front_baseline_pmax, 0.1, 0.9 );
  double temp_baselineRange[2] = {front_baseline_tmax-front_baseline_riseTime-5000.0, front_baseline_tmax+5000.0};
  WaveAna.Correct_Baseline3(front_temp_voltage, front_temp_time, temp_baselineRange );
  std::pair<double,unsigned int> front_baseline_pmax_corr = WaveAna.Find_Singal_Maximum( front_temp_voltage, front_temp_time, true, temp_front_searchRange );
  this->frontBaselineInt_indepBaseCorr[ch]->push_back( WaveAna.Pulse_Integration_with_Fixed_Window_Size( front_temp_voltage, front_temp_time, front_baseline_pmax_corr, "Simpson", 1000.0, 3000.0) );


  double temp_back_searchRange[2] = {tmax_for_baseline+10000.0, tmax_for_baseline+15000.0 };
  std::pair<double,unsigned int> back_baseline_pmax = WaveAna.Find_Singal_Maximum ( *this->w[ch], *this->t[ch], true, temp_back_searchRange);
  double back_baseline_tmax = this->t[ch]->at(back_baseline_pmax.second);
  double back_baseline_riseTime = WaveAna.Find_Rise_Time( *this->w[ch], *this->t[ch], back_baseline_pmax, 0.1, 0.9 );
  double temp_back_baselineRange[2] = {back_baseline_tmax-back_baseline_riseTime-5000.0, back_baseline_tmax+5000.0 };
  WaveAna.Correct_Baseline3(back_temp_voltage, back_temp_time, temp_back_baselineRange );
  std::pair<double,unsigned int> back_baseline_pmax_corr = WaveAna.Find_Singal_Maximum( back_temp_voltage, back_temp_time, true, temp_back_searchRange );
  this->backBaselineInt_indepBaseCorr[ch]->push_back( WaveAna.Pulse_Integration_with_Fixed_Window_Size( back_temp_voltage, back_temp_time, back_baseline_pmax_corr, "Simpson", 1000.0, 3000.0) );

  double baselineRange[2] = {tmax_for_baseline-temp_riseTime-3000.0, tmax_for_baseline-temp_riseTime-1000.0 };
  WaveAna.Correct_Baseline3( *this->w[ch], *this->t[ch], baselineRange);

  if( ch == this->triggerCh )this->my_anaParam.limiting_search_region_OnOff=false;
  else this->my_anaParam.limiting_search_region_OnOff = true;

  std::pair<double,unsigned int> pmaxHolder = WaveAna.Find_Singal_Maximum( *this->w[ch], *this->t[ch], this->my_anaParam.limiting_search_region_OnOff, this->my_anaParam.pmaxSearchRange );
  std::pair<double,unsigned int> neg_pmaxHolder = WaveAna.Find_Negative_Signal_Maximum( *this->w[ch], *this->t[ch], this->my_anaParam.limiting_search_region_OnOff, this->my_anaParam.pmaxSearchRange);

  this->pmax[ch]->push_back( pmaxHolder.first );
  this->neg_pmax[ch]->push_back( neg_pmaxHolder.first );

  for(int k =0; k<101; k++){ double percentage = 1.0*k; this->cfd[ch]->push_back( WaveAna.Rising_Edge_CFD_Time( percentage, *this->w[ch], *this->t[ch], pmaxHolder ) ); }

  for(int k =0; k<101; k++){ this->cfd_fall[ch]->push_back( WaveAna.Falling_Edge_CFD_Time( k, *this->w[ch], *this->t[ch], pmaxHolder ) ); }

  for( int step = 0; step < 500; step++ ){ double percentage = 0.2 * step; this->fineCFDRise[ch]->push_back( WaveAna.Rising_Edge_CFD_Time( percentage, *this->w[ch], *this->t[ch], pmaxHolder ) ); }

  //========================================================================
  //Filling Tmax
  this->tmax[ch]->push_back( WaveAna.Get_Tmax( *this->t[ch], pmaxHolder) );
  this->neg_tmax[ch]->push_back( WaveAna.Get_Tmax( *this->t[ch], neg_pmaxHolder) );

  this->rms[ch]->push_back( WaveAna.Find_Noise( *this->w[ch], 0.25*this->w[ch]->size() ) );

  this->pulseArea_withUndershoot[ch]->push_back( WaveAna.Pulse_Integration_with_Fixed_Window_Size( *this->w[ch], *this->t[ch], pmaxHolder, "Simpson", 1000.0, 3000.0) );
  this->pulseArea_withZeroCross[ch]->push_back( WaveAna.Find_Pulse_Area( *this->w[ch], *this->t[ch], pmaxHolder) );

  //this->frontBaselineInt[b]->push_back( WaveAna.Pulse_Integration_with_Fixed_Window_Size( this->w[ch], this->t[ch], pmaxHolder, "Simpson", 12000.0, -8000.0) );
  //this->backBaselineInt[b]->push_back( WaveAna.Pulse_Integration_with_Fixed_Window_Size( this->w[ch], this->t[ch], pmaxHolder, "Simpson", -8000.0, 12000.0) );

  //------------------Rise Time 10 to 90---------------------------------------------------------

  this->riseTime[ch]->push_back( WaveAna.Find_Rise_Time( *this->w[ch], *this->t[ch], pmaxHolder, 0.1, 0.9 ) );

  //--------------------------------------------------------------------------------------------

  for(int d =0; d<101; d++){ this->dvdt[ch]->push_back( WaveAna.Find_Dvdt( d, 0, *this->w[ch], *this->t[ch], pmaxHolder ) ); }

  for(int k = 0; k < 2000; k++){ this->thTime[ch]->push_back( WaveAna.Find_Time_At_Threshold( double(k), *this->w[ch], *this->t[ch], pmaxHolder ) ); }

  if( !this->skipWaveform )
  {
    for( unsigned int invI = 0; invI < this->w[ch]->size(); invI++){
      this->w[ch]->at(invI) = -1.0 * this->w[ch]->at(invI);
    }
  }
  else
  {
    this->w[ch]->clear();
    this->t[ch]->clear();
  }

}

void BetaScopeWaveformAna::analysis()
{
  //fill up your own analysis in the while loop

  //WaveformAnalysis WaveAna;

  if(! this->isProcessing)
  {
    ColorCout::print( "   " + beta_scope.get_ifile_name(), " BetaScopeWaveformAna::analysis: Start event processing: ", BOLDYELLOW);
    this->isProcessing = true;
  }

  //if(this->beta_scope.ieventFromDAQ)*this->beta_scope.oTreeIntMap["ievent"] = **this->beta_scope.iTreeIntValueMap["ievent"];

  //std::time_t t1 = std::time(nullptr);
  //loop through all the possible channels
  std::vector<std::thread*> workers;
  for(int chh = 0; chh < this->activeChannels.size(); chh ++)
  {
    int ch = this->activeChannels.at(chh);
    //if(std::find(this->activeChannels.begin(), this->activeChannels.end(), ch) != this->activeChannels.end())
    if(true)
    {
      //if( this->beta_scope.iTreeDoubleArrayMap.count("w"+std::to_string(ch) ))
      if(true)
      {
        if(!this->i_w[ch])std::cout<< ch << std::endl;
        for(std::size_t i=0, max = this->i_w[ch]->GetSize(); i<max; i++)
        {
          //std::cout<<  <<std::endl;
          //if( std::find(this->invertChannels.begin(), this->invertChannels.end(), ch) != this->invertChannels.end() )
          if(this->invertChannels.at(chh) != 0)
          {
            this->w[ch]->push_back( this->i_w[ch]->At(i) * -1.0 * this->voltageMultiFactor );
            //this->w[ch]->at(i) = this->i_w[ch]->At(i) * -1.0 * this->voltageMultiFactor;
          }
          else
          {
            this->w[ch]->push_back( this->i_w[ch]->At(i) * this->voltageMultiFactor );
            //this->w[ch]->at(i) = this->i_w[ch]->At(i) * this->voltageMultiFactor;
          }
          this->t[ch]->push_back( this->i_t[ch]->At(i) * this->timeMultiFactor );

          //std::cout << this->w[ch]->at(5) << std::endl;
          //std::cout << this->w[ch]->size() << std::endl;

          //this->wRaw[ch]->push_back( this->i_w[ch]->At(i) * this->voltageMultiFactor );
          //this->tRaw[ch]->push_back( this->i_t[ch]->At(i) * this->timeMultiFactor );

          /*
          this->t[ch]->at(i) = this->i_t[ch]->At(i) * this->timeMultiFactor;
          this->wRaw[ch]->at(i) = this->i_w[ch]->At(i) * this->voltageMultiFactor;
          this->tRaw[ch]->at(i) = this->i_t[ch]->At(i) * this->timeMultiFactor;
          */
        }

        workers.push_back( new std::thread( &BetaScopeWaveformAna::thread_it, this, ch) );

      }
    }
  }

  for(std::size_t id=0; id < workers.size(); id++ )
  {
    workers[id]->join();
    delete workers[id];
  }

  // filling value that's indep of scope channels
  if( this->has_daq_cycle )this->beta_scope.set_oTree_value<int>("cycle", this->beta_scope.get_iBranch_value<TTreeReaderValue,int>("cycle") );
  if( this->has_daq_temperature )this->beta_scope.set_oTree_value<double>("temperature", this->beta_scope.get_iBranch_value<TTreeReaderValue,double>("temperature") );
  if( this->has_daq_humidity )this->beta_scope.set_oTree_value<double>("humidity", this->beta_scope.get_iBranch_value<TTreeReaderValue,double>("humidity") );
  if( this->has_daq_bias )this->beta_scope.set_oTree_value<double>("bias", this->beta_scope.get_iBranch_value<TTreeReaderValue,double>("bias") );
  if( this->has_daq_timestamp )this->beta_scope.set_oTree_value<double>("timestamp", this->beta_scope.get_iBranch_value<TTreeReaderValue,double>("timestamp") );
  if( this->has_daq_current )this->beta_scope.set_oTree_value<double>("current", this->beta_scope.get_iBranch_value<TTreeReaderValue,double>("current") );
  if( this->has_daq_eventNum )this->beta_scope.set_oTree_value<double>("ievent", this->beta_scope.get_iBranch_value<TTreeReaderValue,int>("ievent") );

}


void BetaScopeWaveformAna::initialize()
{
  //required
  this->beta_scope.fileIO_Open( ifile.c_str() );
  BetaScope_AnaFramework::initialize("/home/white_meow/scripts/HGTD_BetaScope_FW_Test/BetaScope_Ana/BetaScopeWaveformAna/AnaTemplate/myOwnTree.ini" );

  this->my_anaParam.limiting_search_region_OnOff = this->limitPmaxSearchRange;
  this->my_anaParam.pmaxSearchRange[0] = this->pmaxSearchMinRange;
  this->my_anaParam.pmaxSearchRange[1] = this->pmaxSearchMaxRange;
  //----------------------

  if( !this->skipWaveform )
  {
    for( auto ch : this->beta_scope.channel )
    {
      bool branch_checker;
      int branch_counter = 0;
      ColorCout::print("  CH:", std::to_string(ch), CYAN);

      ColorCout::print("  ", "Creating branches for storing scope channels: ", YELLOW);
      branch_checker = this->beta_scope.buildPrimitiveBranch<std::vector<double>>( Form("w%i", ch ) );

      branch_checker = this->beta_scope.buildPrimitiveBranch<std::vector<double>>( Form("t%i", ch ) );

      if(branch_checker)
      {
        ColorCout::print("  Successful:", std::to_string(ch), CYAN);
      }
    }
  }

  //do your own stuffs here
  //this->beta_scope.treeReader->Next();
  for(int chh = 0; chh < this->activeChannels.size(); chh ++)
  {
    int ch = this->activeChannels.at(chh);

    if( !this->skipWaveform )
    {
      this->w[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "w"+std::to_string(ch) );
      this->t[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "t"+std::to_string(ch) );
    }
    else
    {
      this->w[ch] = &localW[ch];//new std::vector<double>;
      this->w[ch]->reserve(10000);
      this->t[ch] = &localT[ch];//new std::vector<double>;
      this->t[ch]->reserve(10000);
    }

    this->pmax[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "pmax"+std::to_string(ch) );
    this->tmax[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "tmax"+std::to_string(ch) );
    this->neg_pmax[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "neg_pmax"+std::to_string(ch) );
    this->neg_tmax[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "neg_tmax"+std::to_string(ch) );

    this->riseTime[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "riseTime"+std::to_string(ch) );
    this->dvdt[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "dvdt"+std::to_string(ch) );
    this->cfd_fall[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "cfd_fall"+std::to_string(ch) );
    this->cfd[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "cfd"+std::to_string(ch) );
    this->fineCFDRise[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "fineCFDRise"+std::to_string(ch) );
    this->thTime[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "thTime"+std::to_string(ch) );
    this->rms[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "rms"+std::to_string(ch) );
    this->pulseArea_withUndershoot[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "pulseArea_withUndershoot"+std::to_string(ch) );
    this->pulseArea_withZeroCross[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "pulseArea_withZeroCross"+std::to_string(ch) );
    this->frontBaselineInt_indepBaseCorr[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "frontBaselineInt_indepBaseCorr"+std::to_string(ch) );
    this->backBaselineInt_indepBaseCorr[ch] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( "backBaselineInt_indepBaseCorr"+std::to_string(ch) );

    this->i_w[ch] = this->beta_scope.get_iBranch<TTreeReaderArray,double>("w"+std::to_string(ch) );
    this->i_t[ch] = this->beta_scope.get_iBranch<TTreeReaderArray,double>("t"+std::to_string(ch) );
    if(!this->i_w[ch])
    {
      std::cout<<this->i_w[ch]<<std::endl;
      std::cout<<this->beta_scope.get_iBranch<TTreeReaderArray,double>("w"+std::to_string(ch) )<<std::endl;
    }
  }

  if( beta_scope.isBranchExists("ievent") )
  {
    this->has_daq_eventNum = true;
    this->beta_scope.set_iBranch<TTreeReaderValue,int>("ievent", "ievent");
    this->beta_scope.buildPrimitiveBranch<int>("ievent");
  }
  if( beta_scope.isBranchExists("temperature") )
  {
    this->has_daq_temperature = true;
    this->beta_scope.set_iBranch<TTreeReaderValue,double>("temperature", "temperature");
    this->beta_scope.buildPrimitiveBranch<double>("temperature");
  }
  if( beta_scope.isBranchExists("humidity") )
  {
    this->has_daq_humidity = true;
    this->beta_scope.set_iBranch<TTreeReaderValue,double>("humidity", "humidity");
    this->beta_scope.buildPrimitiveBranch<double>("humidity");
  }
  if( beta_scope.isBranchExists("i_timestamp") )
  {
    this->has_daq_timestamp = true;
    this->beta_scope.set_iBranch<TTreeReaderValue,double>("i_timestamp", "timestamp");
    this->beta_scope.buildPrimitiveBranch<double>("timestamp");
  }
  if( beta_scope.isBranchExists("i_current") )
  {
    this->has_daq_current = true;
    this->beta_scope.set_iBranch<TTreeReaderValue,double>("i_current", "current");
    this->beta_scope.buildPrimitiveBranch<double>("current");
  }
  if( beta_scope.isBranchExists("bias") )
  {
    this->has_daq_bias = true;
    this->beta_scope.set_iBranch<TTreeReaderValue,double>("bias", "bias");
    this->beta_scope.buildPrimitiveBranch<double>("bias");
  }
  if( beta_scope.isBranchExists("cycle") )
  {
    this->has_daq_cycle = true;
    this->beta_scope.set_iBranch<TTreeReaderValue,int>("cycle", "cycle");
    this->beta_scope.buildPrimitiveBranch<int>("cycle");
  }

  //this->beta_scope.treeReader->Restart();
}

void BetaScopeWaveformAna::loopEvents()
{
  BetaScope_AnaFramework::loopEvents( &BetaScope_AnaFramework::analysis );
}

void BetaScopeWaveformAna::finalize()
{
  //do your own stuffs here
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

  //required
  BetaScope_AnaFramework::finalize();
}











//custom class methods start from here
void BetaScopeWaveformAna::readWaveformConfig( std::string configName )
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

  for( int i = 1; i < 7; i++)
  {
    std::string ch = "channel_"+std::to_string(i);
    if( pt.get<int>("Channel_Activation."+ch) !=0 )
    {
      this->activeChannels.push_back(i);
      if( pt.get<int>("Channel_Invertion."+ch) !=0 )this->invertChannels.push_back(1);
      else this->invertChannels.push_back(0);
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
  for( int i = 1; i < 7; i++)
  {
    configFile << "channel_" << i << " = 0" << std::endl;
  }

  configFile << std::endl;

  configFile << "[Channel_Invertion]" << std::endl;
  for( int i = 1; i < 7; i++)
  {
    configFile << "channel_" << i << " = 0" << std::endl;
  }
}
