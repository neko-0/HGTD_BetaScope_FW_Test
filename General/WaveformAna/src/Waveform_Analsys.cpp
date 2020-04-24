#include "General/WaveformAna/include/general.hpp"
#include "WaveformAna/include/Waveform_Analysis.hpp"

int THREAD_COUNT = 0;

/*
std::vector<double> WaveformAnalysis::loop_helper1( double
(WaveformAnalysis::*func)(), WaveformAna<double, double> waveform,
std::pair<double,unsigned int> pmaxHolder, int max)
{
  std::vector<double> fine_cfd;
  for( int step = 0; step < max; step++ ) {
      double percentage = 0.2 * step;
      fine_cfd.push_back( (this->*func)( percentage, waveform.get_v2(),
waveform.get_v1(), pmaxHolder ) );
  }
  return fine_cfd;
}

std::vector<double> WaveformAnalysis::loop_helper2( double
(WaveformAnalysis::*func)(), WaveformAna<double, double> waveform,
std::pair<double,unsigned int> pmaxHolder, int max)
{
  std::vector<double> v;
  for(int k =0; k<max; k++) {
      v.push_back( (this->*func)( k, waveform.get_v2(), waveform.get_v1(),
pmaxHolder ) );
  }
  return v;
}
*/

WaveformAna<double, double> WaveformAnalysis::analyze_waveform(
  std::vector<double> *t,
  std::vector<double> *w,
  const bool &limiting_search_region_OnOff,
  double pmaxSearchRange[2]
)
{
    WaveformAna<double, double> waveform(w, t);

    if (THREAD_COUNT < 100)
    {
        LOG_INFO(boost::str(boost::format("thread count %1%") % THREAD_COUNT) );
        THREAD_COUNT++;
    }
    // Base line
    std::vector<double> front_temp_voltage = *w;
    std::vector<double> front_temp_time = *t;

    std::vector<double> back_temp_voltage = *w;
    std::vector<double> back_temp_time = *t;

    std::pair<double, unsigned int> pmax_before_baseline = WaveformAnalysis::Find_Singal_Maximum( *w, *t, limiting_search_region_OnOff, pmaxSearchRange);
    double tmax_for_baseline = t->at( pmax_before_baseline.second);
    double temp_riseTime = WaveformAnalysis::Find_Rise_Time(*w, *t, pmax_before_baseline, 0.1, 0.9);

    double temp_front_searchRange[2] = {
        tmax_for_baseline - temp_riseTime - 15000.0,
        tmax_for_baseline - temp_riseTime - 10000.0
    };

    std::pair<double, unsigned int>
    front_baseline_pmax = WaveformAnalysis::Find_Singal_Maximum(*w, *t, true, temp_front_searchRange);
    double front_baseline_tmax = t->at(front_baseline_pmax.second);
    double front_baseline_riseTime = WaveformAnalysis::Find_Rise_Time(*w, *t, front_baseline_pmax, 0.1, 0.9);
    double temp_baselineRange[2] = {front_baseline_tmax - front_baseline_riseTime - 5000.0, front_baseline_tmax + 5000.0 };
    WaveformAnalysis::Correct_Baseline3( front_temp_voltage, front_temp_time, temp_baselineRange);
    std::pair<double, unsigned int> front_baseline_pmax_corr = WaveformAnalysis::Find_Singal_Maximum( front_temp_voltage, front_temp_time,true, temp_front_searchRange);
    waveform.front_baseline_int( WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( front_temp_voltage, front_temp_time,front_baseline_pmax_corr, "Simpson", 1000.0, 3000.0));

    double temp_back_searchRange[2] = {
      tmax_for_baseline + 10000.0,
      tmax_for_baseline + 15000.0
    };

    std::pair<double, unsigned int> back_baseline_pmax = WaveformAnalysis::Find_Singal_Maximum(*w, *t, true, temp_back_searchRange);
    double back_baseline_tmax = t->at( back_baseline_pmax.second);
    double back_baseline_riseTime = WaveformAnalysis::Find_Rise_Time(*w, *t, back_baseline_pmax, 0.1, 0.9);
    double temp_back_baselineRange[2] = {back_baseline_tmax - back_baseline_riseTime - 5000.0, back_baseline_tmax + 5000.0};
    WaveformAnalysis::Correct_Baseline3( back_temp_voltage, back_temp_time, temp_back_baselineRange);
    std::pair<double, unsigned int> back_baseline_pmax_corr = WaveformAnalysis::Find_Singal_Maximum(back_temp_voltage, back_temp_time, true, temp_back_searchRange);
    waveform.back_baseline_int( WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( back_temp_voltage, back_temp_time, back_baseline_pmax_corr, "Simpson",1000.0, 3000.0));

    double baselineRange[2] = {
      tmax_for_baseline - temp_riseTime - 3000.0,
      tmax_for_baseline - temp_riseTime - 1000.0
    };
    WaveformAnalysis::Correct_Baseline3(*w, *t, baselineRange);

    waveform.set_v1(*t);
    waveform.set_v2(*w);

    // if( ch == this->triggerCh
    // )this->my_anaParam.limiting_search_region_OnOff=false; else
    // this->my_anaParam.limiting_search_region_OnOff = true;

    std::pair<double, unsigned int> pmaxHolder = WaveformAnalysis::Find_Singal_Maximum( waveform.get_v2(), waveform.get_v1(), limiting_search_region_OnOff, pmaxSearchRange);
    std::pair<double, unsigned int> neg_pmaxHolder = WaveformAnalysis::Find_Negative_Signal_Maximum( waveform.get_v2(), waveform.get_v1(), limiting_search_region_OnOff, pmaxSearchRange);

    waveform.pmax(pmaxHolder.first);
    waveform.tmax(waveform.get_v1_value( pmaxHolder.second));
    waveform.fit_tmax( WaveformAnalysis::Get_Fit_Tmax( waveform.get_v2(), waveform.get_v1(), pmaxHolder) );
    waveform.max_index(pmaxHolder.second);

    waveform.neg_pmax(neg_pmaxHolder.first);
    waveform.neg_tmax(waveform.get_v1_value(neg_pmaxHolder.second));
    waveform.neg_max_index(neg_pmaxHolder.second);

    waveform.rms(WaveformAnalysis::Find_Noise( waveform.get_v2(), 0.25 * waveform.get_v2().size()));

    waveform.pulse_area_undershoot( WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size(waveform.get_v2(), waveform.get_v1(), pmaxHolder, "Simpson", 1000.0, 3000.0));
    waveform.pulse_area( WaveformAnalysis::Find_Pulse_Area( waveform.get_v2(), waveform.get_v1(), pmaxHolder));

    // this->frontBaselineInt[b]->push_back(
    // WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( this->w[ch],
    // this->t[ch], pmaxHolder, "Simpson", 12000.0, -8000.0) );
    // this->backBaselineInt[b]->push_back(
    // WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( this->w[ch],
    // this->t[ch], pmaxHolder, "Simpson", -8000.0, 12000.0) );

    //------------------Rise Time 10 to
    //90---------------------------------------------------------

    waveform.rise_time(WaveformAnalysis::Find_Rise_Time(waveform.get_v2(), waveform.get_v1(), pmaxHolder, 0.1, 0.9));

    //--------------------------------------------------------------------------------------------

    std::vector<double> cfd;
    for (int k = 0; k < 101; k++)
    {
        double percentage = 1.0 * k;
        cfd.push_back( WaveformAnalysis::Rising_Edge_CFD_Time( percentage, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.cfd(cfd);

    std::vector<double> cfd_fall;
    for (int k = 0; k < 101; k++)
    {
        cfd_fall.push_back( WaveformAnalysis::Falling_Edge_CFD_Time(k, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.cfd_fall(cfd_fall);

    std::vector<double> dvdt;
    for (int d = 0; d < 101; d++)
    {
        dvdt.push_back(WaveformAnalysis::Find_Dvdt(d, 0, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.dvdt(dvdt);

    std::vector<double> fine_cfd;
    for (int step = 0; step < 500; step++)
    {
        double percentage = 0.2 * step;
        fine_cfd.push_back( WaveformAnalysis::Rising_Edge_CFD_Time( percentage, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.fine_cfd(fine_cfd);

    std::vector<double> thTime;
    for (int k = 0; k < 2000; k++)
    {
        thTime.push_back( WaveformAnalysis::Find_Time_At_Threshold( double(k), waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.threashold_time(thTime);

    /*
    waveform.set_cfd( loop_helper2( &WaveformAnalysis::Rising_Edge_CFD_Time,
    waveform, pmaxHolder, 101) ); waveform.set_cfd_fall( loop_helper2(
    &WaveformAnalysis::Falling_Edge_CFD_Time, waveform, pmaxHolder, 101) );
    waveform.set_dvdt( loop_helper2( &WaveformAnalysis::Find_Dvdt, waveform,
    pmaxHolder, 101) ); waveform.set_fine_cfd( loop_helper1(
    &WaveformAnalysis::Rising_Edge_CFD_Time, waveform, pmaxHolder, 500) );
    waveform.set_threashold_time( loop_helper2(
    &WaveformAnalysis::Find_Time_At_Threshold, waveform, pmaxHolder, 2000) );
    */

    return waveform;
}

//==============================================================================
WaveformAna<double, double> WaveformAnalysis::analyze_waveform(
  WaveformAna<double, double> &waveform,
  const bool &limiting_search_region_OnOff,
  double pmaxSearchRange[2]
)
{
    //WaveformAna<double, double> waveform(w, t, invert, ampFactor, tFactor, tResample, xori, dt);
    //WaveformAna<double,double>

    //std::cout << waveform.get_v2_value(0) << ", ";

    if (THREAD_COUNT < 100)
    {
        //LOG_INFO(boost::str(boost::format("thread count %1%") % THREAD_COUNT) );
        THREAD_COUNT++;
    }
    // Base line
    std::vector<double> front_temp_voltage = waveform.v2();
    std::vector<double> front_temp_time = waveform.v1();

    std::vector<double> back_temp_voltage = waveform.v2();
    std::vector<double> back_temp_time = waveform.v1();

    std::pair<double, unsigned int> pmax_before_baseline = WaveformAnalysis::Find_Singal_Maximum( waveform.get_v2(), waveform.get_v1(), limiting_search_region_OnOff, pmaxSearchRange);
    double tmax_for_baseline;
    if( pmax_before_baseline.second < waveform.size() )
    {
      tmax_for_baseline = waveform.get_v1_value(pmax_before_baseline.second);
    }
    else
    {
      tmax_for_baseline = waveform.get_v1_value(0);
    }

    double temp_riseTime = WaveformAnalysis::Find_Rise_Time( waveform.get_v2(), waveform.get_v1(), pmax_before_baseline, 0.1, 0.9);

    double temp_front_searchRange[2] = {
        tmax_for_baseline - temp_riseTime - 15000.0,
        tmax_for_baseline - temp_riseTime - 10000.0
    };

    std::pair<double, unsigned int>
    front_baseline_pmax = WaveformAnalysis::Find_Singal_Maximum( waveform.get_v2(), waveform.get_v1(), true, temp_front_searchRange);
    double front_baseline_tmax = waveform.get_v1_value(front_baseline_pmax.second);
    double front_baseline_riseTime = WaveformAnalysis::Find_Rise_Time( waveform.get_v2(), waveform.get_v1(), front_baseline_pmax, 0.1, 0.9);
    double temp_baselineRange[2] = {front_baseline_tmax - front_baseline_riseTime - 5000.0, front_baseline_tmax + 5000.0 };
    WaveformAnalysis::Correct_Baseline3( front_temp_voltage, front_temp_time, temp_baselineRange);
    std::pair<double, unsigned int> front_baseline_pmax_corr = WaveformAnalysis::Find_Singal_Maximum( front_temp_voltage, front_temp_time,true, temp_front_searchRange);
    waveform.front_baseline_int( WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( front_temp_voltage, front_temp_time,front_baseline_pmax_corr, "Simpson", 1000.0, 3000.0));

    double temp_back_searchRange[2] = {
      tmax_for_baseline + 10000.0,
      tmax_for_baseline + 15000.0
    };

    std::pair<double, unsigned int> back_baseline_pmax = WaveformAnalysis::Find_Singal_Maximum( waveform.get_v2(), waveform.get_v1(), true, temp_back_searchRange);
    double back_baseline_tmax = waveform.get_v1_value(back_baseline_pmax.second);
    double back_baseline_riseTime = WaveformAnalysis::Find_Rise_Time( waveform.get_v2(), waveform.get_v1(), back_baseline_pmax, 0.1, 0.9);
    double temp_back_baselineRange[2] = {back_baseline_tmax - back_baseline_riseTime - 5000.0, back_baseline_tmax + 5000.0};
    WaveformAnalysis::Correct_Baseline3( back_temp_voltage, back_temp_time, temp_back_baselineRange);
    std::pair<double, unsigned int> back_baseline_pmax_corr = WaveformAnalysis::Find_Singal_Maximum(back_temp_voltage, back_temp_time, true, temp_back_searchRange);
    waveform.back_baseline_int( WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( back_temp_voltage, back_temp_time, back_baseline_pmax_corr, "Simpson",1000.0, 3000.0));

    double baselineRange[2] = {
      tmax_for_baseline - temp_riseTime - 3000.0,
      tmax_for_baseline - temp_riseTime - 1000.0
    };

    //std::vector<double> corr_v = waveform.get_v2();
    //std::vector<double> corr_t = waveform.get_v1();
    WaveformAnalysis::Correct_Baseline3( waveform.get_v2(), waveform.get_v1(), baselineRange );

    //std::cout << front_temp_voltage.at(0) << ", " << waveform.get_v2_value(0) << std::endl;

    //waveform.set_v1( corr_t );
    //waveform.set_v2( corr_v );

    // if( ch == this->triggerCh
    // )this->my_anaParam.limiting_search_region_OnOff=false; else
    // this->my_anaParam.limiting_search_region_OnOff = true;

    std::pair<double, unsigned int> pmaxHolder = WaveformAnalysis::Find_Singal_Maximum( waveform.get_v2(), waveform.get_v1(), limiting_search_region_OnOff, pmaxSearchRange);
    std::pair<double, unsigned int> neg_pmaxHolder = WaveformAnalysis::Find_Negative_Signal_Maximum( waveform.get_v2(), waveform.get_v1(), limiting_search_region_OnOff, pmaxSearchRange);

    waveform.pmax( pmaxHolder.first);
    waveform.tmax( waveform.get_v1_value( pmaxHolder.second));
    waveform.fit_tmax( WaveformAnalysis::Get_Fit_Tmax( waveform.get_v2(), waveform.get_v1(), pmaxHolder) );
    waveform.max_index( pmaxHolder.second);

    waveform.neg_pmax( neg_pmaxHolder.first);
    waveform.neg_tmax( waveform.get_v1_value(neg_pmaxHolder.second));
    waveform.neg_max_index( neg_pmaxHolder.second);

    waveform.rms( WaveformAnalysis::Find_Noise( waveform.get_v2(), 0.25 * waveform.get_v2().size()));

    waveform.pulse_area_undershoot( WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size(waveform.get_v2(), waveform.get_v1(), pmaxHolder, "Simpson", 1000.0, 3000.0));
    waveform.pulse_area( WaveformAnalysis::Find_Pulse_Area( waveform.get_v2(), waveform.get_v1(), pmaxHolder));

    // this->frontBaselineInt[b]->push_back( WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( this->w[ch], this->t[ch], pmaxHolder, "Simpson", 12000.0, -8000.0) );
    // this->backBaselineInt[b]->push_back( WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( this->w[ch], this->t[ch], pmaxHolder, "Simpson", -8000.0, 12000.0) );

    //------------------Rise Time 10 to 90---------------------------------------------------------

    waveform.rise_time(WaveformAnalysis::Find_Rise_Time(waveform.get_v2(), waveform.get_v1(), pmaxHolder, 0.1, 0.9));

    //--------------------------------------------------------------------------------------------

    double undershoot_pmax_range[2] = {waveform.tmax(), 5000};
    auto undershoot_pmax = WaveformAnalysis::Find_Negative_Signal_Maximum( waveform.get_v2(), waveform.get_v1(), true, undershoot_pmax_range);
    waveform.undershoot_pmax( undershoot_pmax.first );
    waveform.undershoot_tmax( waveform.get_v1_value(undershoot_pmax.second) );

    //--------------------------------------------------------------------------------------------

    for (int k = 0; k < 101; k++)
    {
        double percentage = 1.0 * k;
        buffer_[0].emplace_back( WaveformAnalysis::Rising_Edge_CFD_Time( percentage, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.cfd(buffer_[0]);
    buffer_[0].clear();

    for (int k = 0; k < 101; k++)
    {
        buffer_[0].emplace_back( WaveformAnalysis::Falling_Edge_CFD_Time(k, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.cfd_fall(buffer_[0]);
    buffer_[0].clear();


    for (int d = 0; d < 101; d++)
    {
        buffer_[0].emplace_back(WaveformAnalysis::Find_Dvdt(d, 0, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.dvdt(buffer_[0]);
    buffer_[0].clear();

    std::vector<double> fine_cfd;
    for (int step = 0; step < 500; step++)
    {
        double percentage = 0.2 * step;
        buffer_[0].emplace_back( WaveformAnalysis::Rising_Edge_CFD_Time( percentage, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.fine_cfd(buffer_[0]);
    buffer_[0].clear();

    for (int k = 0; k < 2000; k++)
    {
        buffer_[0].emplace_back( WaveformAnalysis::Find_Time_At_Threshold( double(k), waveform.get_v2(), waveform.get_v1(), pmaxHolder));
        buffer_[1].emplace_back(WaveformAnalysis::Get_TimeAcrossThreshold(double(k), waveform.get_v2(), waveform.get_v1(), buffer_[2], 2));
    }
    waveform.threashold_time(buffer_[0]);
    waveform.tot(buffer_[1]);
    buffer_[0].clear();
    buffer_[1].clear();
    buffer_[2].clear();

    /*
    waveform.set_cfd( loop_helper2( &WaveformAnalysis::Rising_Edge_CFD_Time,
    waveform, pmaxHolder, 101) ); waveform.set_cfd_fall( loop_helper2(
    &WaveformAnalysis::Falling_Edge_CFD_Time, waveform, pmaxHolder, 101) );
    waveform.set_dvdt( loop_helper2( &WaveformAnalysis::Find_Dvdt, waveform,
    pmaxHolder, 101) ); waveform.set_fine_cfd( loop_helper1(
    &WaveformAnalysis::Rising_Edge_CFD_Time, waveform, pmaxHolder, 500) );
    waveform.set_threashold_time( loop_helper2(
    &WaveformAnalysis::Find_Time_At_Threshold, waveform, pmaxHolder, 2000) );
    */

    return waveform;
}
//==============================================================================

int WaveformAnalysis::Get_Number_Of_Multiple_Signals(
  const double &threshold,
  const std::vector<double> &voltageVec,
  const double &scale
)
{
  std::string function_name = "WaveformAnalysis::Get_Number_Of_Multiple_Signals";

  double pmax = 0.0;
  double pmax_i = 0;
  bool candidate_signal = false;
  bool noisy_event = true;
  int num_pulses = 0;

  int index = 0;
  for( const auto &val : voltageVec )
  {
    if( !candidate_signal )
    {
      if( val >= threshold )
      {
        pmax = val;
        pmax_i = index;
        candidate_signal = true;
        noisy_event ? noisy_event = false : 0;
      }
    }
    else
    {
      if( val > pmax )
      {
        pmax = val;
        pmax_i = index;
      }
      else if(
        (val < threshold) &&
        (threshold - abs(val) <= threshold)
      )
      {
        num_pulses++;
        pmax = val;
        pmax_i = index;
        candidate_signal = false;
      }
      else{}
    }
    index++;
  }
    candidate_signal ? num_pulses++ : 0;
    return num_pulses;
}
