#include "BetaScopeWaveformAna.h"

#include <fstream>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "General/WaveformAna/include/Waveform_Analysis.hpp"
#include "General/WaveformAna/include/general.hpp"

#include <ctime>

void BetaScopeWaveformAna::initialize()
{
  //required
  this->beta_scope.fileIO_Open( ifile.c_str() );
  BetaScope_AnaFramework::initialize("/home/yuzhan/HGTD_BetaScope_FW_Test/BetaScope_Ana/BetaScopeWaveformAna/AnaTemplate/myOwnTree.ini" );
  //----------------------

  bool branch_checker;
  int branch_counter = 0;

  for( auto ch : this->beta_scope.channel )
  {
    ColorCout::print("  CH:", std::to_string(ch), CYAN);

    if( !this->skipWaveform )
    {
      ColorCout::print("  ", "Creating branches for storing scope channels: ", YELLOW);
      branch_checker = makeBranch<std::vector<double>>(this->beta_scope.oTree, Form("w%i", ch ), Form("w%i", ch ), &this->beta_scope.oTreeVecDoubleMap, this->beta_scope.oTreeVecDouble[branch_counter], branch_counter, &this->beta_scope.oTreeVecDoubleMapIndex, this->beta_scope.newBranchCounterKeeper );
      this->beta_scope.oTreeVecDouble[branch_counter-1]->reserve(1000000);

      branch_checker = makeBranch<std::vector<double>>(this->beta_scope.oTree, Form("t%i", ch ), Form("t%i", ch ), &this->beta_scope.oTreeVecDoubleMap, this->beta_scope.oTreeVecDouble[branch_counter], branch_counter, &this->beta_scope.oTreeVecDoubleMapIndex, this->beta_scope.newBranchCounterKeeper );
      this->beta_scope.oTreeVecDouble[branch_counter-1]->reserve(1000000);

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
    /*
    this->w[ch] = this->beta_scope.oTreeVecDoubleMap["w"+std::to_string(ch)]; this->w[ch]->reserve(1000000000);
    this->t[ch] = this->beta_scope.oTreeVecDoubleMap["t"+std::to_string(ch)]; this->t[ch]->reserve(1000000000);
    this->wRaw[ch] = this->beta_scope.oTreeVecDoubleMap["wRaw"+std::to_string(ch)]; this->wRaw[ch]->reserve(1000000000);
    this->tRaw[ch] = this->beta_scope.oTreeVecDoubleMap["tRaw"+std::to_string(ch)]; this->tRaw[ch]->reserve(1000000000);
    this->pmax[ch] = this->beta_scope.oTreeVecDoubleMap["pmax"+std::to_string(ch)]; this->pmax[ch]->reserve(1000000000);
    this->tmax[ch] = this->beta_scope.oTreeVecDoubleMap["tmax"+std::to_string(ch)]; this->tmax[ch]->reserve(1000000000);
    this->neg_pmax[ch] = this->beta_scope.oTreeVecDoubleMap["neg_pmax"+std::to_string(ch)]; this->neg_pmax[ch]->reserve(1000000000);
    this->neg_tmax[ch] = this->beta_scope.oTreeVecDoubleMap["neg_tmax"+std::to_string(ch)]; this->neg_tmax[ch]->reserve(1000000000);

    this->riseTime[ch] = this->beta_scope.oTreeVecDoubleMap["riseTime"+std::to_string(ch)]; this->riseTime[ch]->reserve(1000000000);
    this->dvdt[ch] = this->beta_scope.oTreeVecDoubleMap["dvdt"+std::to_string(ch)]; this->dvdt[ch]->reserve(1000000000);
    this->cfd_fall[ch] = this->beta_scope.oTreeVecDoubleMap["cfd_fall"+std::to_string(ch)]; this->cfd_fall[ch]->reserve(1000000000);
    this->cfd[ch] = this->beta_scope.oTreeVecDoubleMap["cfd"+std::to_string(ch)]; this->cfd[ch]->reserve(1000000000);
    this->fineCFDRise[ch] = this->beta_scope.oTreeVecDoubleMap["fineCFDRise"+std::to_string(ch)]; this->fineCFDRise[ch]->reserve(1000000000);
    this->thTime[ch] = this->beta_scope.oTreeVecDoubleMap["thTime"+std::to_string(ch)]; this->thTime[ch]->reserve(1000000000);
    this->rms[ch] = this->beta_scope.oTreeVecDoubleMap["rms"+std::to_string(ch)]; this->rms[ch]->reserve(1000000000);
    this->pulseArea_withUndershoot[ch] = this->beta_scope.oTreeVecDoubleMap["pulseArea_withUnderShoot"+std::to_string(ch)]; this->pulseArea_withUndershoot[ch]->reserve(1000000000);
    this->pulseArea_withZeroCross[ch] = this->beta_scope.oTreeVecDoubleMap["pulseArea_withZeroCross"+std::to_string(ch)]; this->pulseArea_withZeroCross[ch]->reserve(1000000000);
    */

    if( !this->skipWaveform )
    {
      this->w[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["w"+std::to_string(ch)]];
      this->t[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["t"+std::to_string(ch)]];
    }
    else
    {
      this->w[ch] = &this->localW[ch];
      this->t[ch] = &this->localT[ch];
    }


    //this->w[ch]->resize(this->beta_scope.iTreeDoubleArray[this->beta_scope.iTreeDoubleArrayMapIndex["w"+std::to_string(ch)]]->GetSize());
    //for(int k =0,max = this->w[ch]->size();k++;){this->w[ch]->push_back(0.0);}

    //this->t[ch]->resize(this->beta_scope.iTreeDoubleArray[this->beta_scope.iTreeDoubleArrayMapIndex["t"+std::to_string(ch)]]->GetSize());
    //for(int k =0,max = this->t[ch]->size();k++;){this->t[ch]->push_back(0.0);}

    //this->wRaw[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["wRaw"+std::to_string(ch)]];
    //this->tRaw[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["tRaw"+std::to_string(ch)]];

    //this->wRaw[ch]->resize(this->beta_scope.iTreeDoubleArray[this->beta_scope.iTreeDoubleArrayMapIndex["w"+std::to_string(ch)]]->GetSize());
    //for(int k =0,max = this->wRaw[ch]->size();k++;){this->wRaw[ch]->push_back(0.0);}

    //this->tRaw[ch]->resize(this->beta_scope.iTreeDoubleArray[this->beta_scope.iTreeDoubleArrayMapIndex["t"+std::to_string(ch)]]->GetSize());
    //for(int k =0,max = this->tRaw[ch]->size();k++;){this->tRaw[ch]->push_back(0.0);}
    this->pmax[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["pmax"+std::to_string(ch)]];
    this->tmax[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["tmax"+std::to_string(ch)]];
    this->neg_pmax[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["neg_pmax"+std::to_string(ch)]];
    this->neg_tmax[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["neg_tmax"+std::to_string(ch)]];

    this->riseTime[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["riseTime"+std::to_string(ch)]];
    this->dvdt[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["dvdt"+std::to_string(ch)]];
    this->cfd_fall[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["cfd_fall"+std::to_string(ch)]];
    this->cfd[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["cfd"+std::to_string(ch)]];
    this->fineCFDRise[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["fineCFDRise"+std::to_string(ch)]];
    this->thTime[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["thTime"+std::to_string(ch)]];
    this->rms[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["rms"+std::to_string(ch)]];
    this->pulseArea_withUndershoot[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["pulseArea_withUndershoot"+std::to_string(ch)]];
    this->pulseArea_withZeroCross[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["pulseArea_withZeroCross"+std::to_string(ch)]];
    this->frontBaselineInt_indepBaseCorr[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["frontBaselineInt_indepBaseCorr"+std::to_string(ch)]];
    this->backBaselineInt_indepBaseCorr[ch] = this->beta_scope.oTreeVecDouble[this->beta_scope.oTreeVecDoubleMapIndex["backBaselineInt_indepBaseCorr"+std::to_string(ch)]];

    //this->i_w2[ch] = this->beta_scope.iTreeDoubleArrayMap["w"+std::to_string(ch)];
    //this->i_t[ch] = this->beta_scope.iTreeDoubleArrayMap["t"+std::to_string(ch)];
    this->i_w[ch] = this->beta_scope.iTreeDoubleArray[this->beta_scope.iTreeDoubleArrayMapIndex["w"+std::to_string(ch)]];
    //std::cout << this->i_w[ch] << std::endl;
    //std::cout << this->i_w2[ch] << std::endl;
    //std::cout << this->i_w2[ch] << std::endl;
    this->i_t[ch] = this->beta_scope.iTreeDoubleArray[this->beta_scope.iTreeDoubleArrayMapIndex["t"+std::to_string(ch)]];
  }
  this->i_current = this->beta_scope.iTreeDoubleValueMap["i_current"];
  this->i_timestamp = this->beta_scope.iTreeDoubleValueMap["i_timestamp"];

  this->current = this->beta_scope.oTreeDouble[this->beta_scope.oTreeDoubleMapIndex["current"]];
  this->timestamp = this->beta_scope.oTreeDouble[this->beta_scope.oTreeDoubleMapIndex["timestamp"]];

  if(this->beta_scope.ieventFromDAQ)
  {
    auto branch_checker = makeBranch<int>(this->beta_scope.oTree, "ievent", "ievent", &this->beta_scope.oTreeIntMap, this->beta_scope.oTreeInt[this->beta_scope.newBranchCounterKeeper], this->beta_scope.newBranchCounterKeeper, &this->beta_scope.oTreeIntMapIndex );
    if(branch_checker)
    {
      ColorCout::print("  Successful type I: ", "ievent", CYAN);
    }
  }
  //this->beta_scope.treeReader->Restart();
}

void BetaScopeWaveformAna::loopEvents()
{
  //fill up your own analysis in the while loop
  bool limiting_search_region_OnOff = this->limitPmaxSearchRange;
  double pmaxSearchRange[2] = {this->pmaxSearchMinRange, this->pmaxSearchMaxRange};
  ColorCout::print( "   "+beta_scope.ifileName, " BetaScopeWaveformAna::loopEvents: Start event processing: ", BOLDYELLOW);
  WaveformAnalysis WaveAna;

  while( this->beta_scope.treeReader->Next() )
  {
    if(this->beta_scope.ieventFromDAQ)*this->beta_scope.oTreeIntMap["ievent"] = **this->beta_scope.iTreeIntValueMap["ievent"];

    //std::time_t t1 = std::time(nullptr);
    //loop through all the possible channels
    for(int chh = 0; chh < this->activeChannels.size(); chh ++)
    {
      int ch = this->activeChannels.at(chh);
      //if(std::find(this->activeChannels.begin(), this->activeChannels.end(), ch) != this->activeChannels.end())
      if(true)
      {
        //if( this->beta_scope.iTreeDoubleArrayMap.count("w"+std::to_string(ch) ))
        if(true)
        {
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

            //this->wRaw[ch]->push_back( this->i_w[ch]->At(i) * this->voltageMultiFactor );
            //this->tRaw[ch]->push_back( this->i_t[ch]->At(i) * this->timeMultiFactor );

            /*
            this->t[ch]->at(i) = this->i_t[ch]->At(i) * this->timeMultiFactor;
            this->wRaw[ch]->at(i) = this->i_w[ch]->At(i) * this->voltageMultiFactor;
            this->tRaw[ch]->at(i) = this->i_t[ch]->At(i) * this->timeMultiFactor;
            */
          }

          // Base line
          std::vector<double> front_temp_voltage = *this->w[ch];
          std::vector<double> front_temp_time = *this->t[ch];

          std::vector<double> back_temp_voltage = *this->w[ch];
          std::vector<double> back_temp_time = *this->t[ch];


          std::pair<double,unsigned int> pmax_before_baseline = WaveAna.Find_Singal_Maximum( *this->w[ch], *this->t[ch], limiting_search_region_OnOff, pmaxSearchRange);
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

          if( ch == this->triggerCh )limiting_search_region_OnOff=false;
          else limiting_search_region_OnOff = true;

          std::pair<double,unsigned int> pmaxHolder = WaveAna.Find_Singal_Maximum( *this->w[ch], *this->t[ch], limiting_search_region_OnOff, pmaxSearchRange );
          std::pair<double,unsigned int> neg_pmaxHolder = WaveAna.Find_Negative_Signal_Maximum( *this->w[ch], *this->t[ch], limiting_search_region_OnOff, pmaxSearchRange);

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
      }
    }

    //filling value that's indep of scope channels
    if( this->beta_scope.currentFromDAQ )*this->current = **this->i_current;
    if( this->beta_scope.timestampFromDAQ )*this->timestamp = **this->i_timestamp;


    //required
    BetaScope_AnaFramework::filldata();
    //std::time_t t2 = std::time(nullptr);
    //std::cout << t2-t1 << std::endl;
    //std::cout << this->beta_scope.oTreeVecDoubleMap["thTime2"]->size() <<std::endl;
  }
}

void BetaScopeWaveformAna::finalize()
{
  //do your own stuffs here


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
