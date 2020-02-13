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

WaveformAna<double, double>
WaveformAnalysis::analyze_waveform(
    std::vector<double> *t, std::vector<double> *w,
    bool limiting_search_region_OnOff,
    double pmaxSearchRange[2])
{
    WaveformAna<double, double> waveform(t, w);

    if (THREAD_COUNT < 100)
    {
        ColorCout::print("  Running thread_it: thread num ", std::to_string(THREAD_COUNT), CYAN);
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
    waveform.set_front_baseline_int( WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( front_temp_voltage, front_temp_time,front_baseline_pmax_corr, "Simpson", 1000.0, 3000.0));

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
    waveform.set_back_baseline_int( WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( back_temp_voltage, back_temp_time, back_baseline_pmax_corr, "Simpson",1000.0, 3000.0));

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

    waveform.set_pmax(pmaxHolder.first);
    waveform.set_tmax(waveform.get_v1_value( pmaxHolder.second));
    waveform.set_max_index(pmaxHolder.second);

    waveform.set_neg_pmax(neg_pmaxHolder.first);
    waveform.set_neg_tmax(waveform.get_v1_value(neg_pmaxHolder.second));
    waveform.set_neg_max_index(neg_pmaxHolder.second);

    waveform.set_rms(WaveformAnalysis::Find_Noise( waveform.get_v2(), 0.25 * waveform.get_v2().size()));

    waveform.set_pulse_area_undershoot( WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size(waveform.get_v2(), waveform.get_v1(), pmaxHolder, "Simpson", 1000.0, 3000.0));
    waveform.set_pulse_area( WaveformAnalysis::Find_Pulse_Area( waveform.get_v2(), waveform.get_v1(), pmaxHolder));

    // this->frontBaselineInt[b]->push_back(
    // WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( this->w[ch],
    // this->t[ch], pmaxHolder, "Simpson", 12000.0, -8000.0) );
    // this->backBaselineInt[b]->push_back(
    // WaveformAnalysis::Pulse_Integration_with_Fixed_Window_Size( this->w[ch],
    // this->t[ch], pmaxHolder, "Simpson", -8000.0, 12000.0) );

    //------------------Rise Time 10 to
    //90---------------------------------------------------------

    waveform.set_rise_time(WaveformAnalysis::Find_Rise_Time(waveform.get_v2(), waveform.get_v1(), pmaxHolder, 0.1, 0.9));

    //--------------------------------------------------------------------------------------------

    std::vector<double> cfd;
    for (int k = 0; k < 101; k++)
    {
        double percentage = 1.0 * k;
        cfd.push_back( WaveformAnalysis::Rising_Edge_CFD_Time( percentage, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.set_cfd(cfd);

    std::vector<double> cfd_fall;
    for (int k = 0; k < 101; k++)
    {
        cfd_fall.push_back( WaveformAnalysis::Falling_Edge_CFD_Time(k, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.set_cfd_fall(cfd_fall);

    std::vector<double> dvdt;
    for (int d = 0; d < 101; d++)
    {
        dvdt.push_back(WaveformAnalysis::Find_Dvdt(d, 0, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.set_dvdt(dvdt);

    std::vector<double> fine_cfd;
    for (int step = 0; step < 500; step++)
    {
        double percentage = 0.2 * step;
        fine_cfd.push_back( WaveformAnalysis::Rising_Edge_CFD_Time( percentage, waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.set_fine_cfd(fine_cfd);

    std::vector<double> thTime;
    for (int k = 0; k < 2000; k++)
    {
        thTime.push_back( WaveformAnalysis::Find_Time_At_Threshold( double(k), waveform.get_v2(), waveform.get_v1(), pmaxHolder));
    }
    waveform.set_threashold_time(thTime);

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
