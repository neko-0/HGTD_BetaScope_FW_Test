#ifndef WAVEFORM_ANA_H
#define WAVEFORM_ANA_H

#include <mutex>
#include <string>
#include <vector>

#include "General/logger/include/logger.h"
#include "WaveformAna/include/WaveformAna.hpp"

class WaveformAnalysis
{

    bool supressNoisy = false;
    int supressNoisyCounter = 0;

    int Find_Bunch_Negative_Signal_Maximum_counter = 0;

    std::vector<double> buffer_[6];

    std::mutex mu;

public:
    WaveformAnalysis()
    {
      for(auto &b: buffer_)
      {
        b.reserve(2000);
      }
    };
    ~WaveformAnalysis() {};

    WaveformAna<double, double>
    analyze_waveform(
        std::vector<double> *t,
        std::vector<double> *w,
        const bool &limiting_search_region_OnOff,
        double pmaxSearchRange[2]
    );


    WaveformAna<double, double>
    analyze_waveform(
      WaveformAna<double, double> &waveform,
      const bool &limiting_search_region_OnOff,
      double pmaxSearchRange[2]
    );


    std::vector<double> loop_helper2(double (WaveformAnalysis::*func)(),
                                     WaveformAna<double, double> waveform,
                                     std::pair<double, unsigned int> pmaxHolder,
                                     int max);
    std::vector<double> loop_helper1(double (WaveformAnalysis::*func)(),
                                     WaveformAna<double, double> waveform,
                                     std::pair<double, unsigned int> pmaxHolder,
                                     int max);

    //==========================================================================
    // Baseline Correction
    double Correct_Baseline(std::vector<double> &voltageVec, const int &ptN);

    double Correct_Baseline2(
        std::vector<double> &voltageVec,
        const double &fractional_pts
    );

    double Correct_Baseline3(
        std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const double tRange[2]
    ); // special treatment of ill-signal baseline.

    bool Correct_Baseline4(
        std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        std::vector<double> &pmax,
        const std::vector<double> &tmax
    );

    void SSRL_Baseline(std::vector<double> &w, const std::string &workerID);

    void SSRL_Baseline(std::vector<double> &w, double &RMS);

    void SSRL_Dynamic_Noise_And_Baseline(
        std::vector<double> &w, // voltage
        const std::vector<double> &t,  // time
        const double &assisting_threshold,
        const double &time_separation,
        double &RMS);

    //==========================================================================
    // Pulse Maximum (Pmax)
    std::pair<double, unsigned int>
    Find_Singal_Maximum(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const bool &confineSearchRegion,
        const double searchRange[2]
    );

    std::pair<double, unsigned int>
    Find_Negative_Signal_Maximum(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const bool &confineSearchRegion,
        const double searchRange[2]
    );

    double Get_Tmax(
        const std::vector<double> &timeVec,
        const std::pair<double, unsigned int> &Pmax
    );

    std::pair<double, double>
    Get_Fit_Tmax(
      std::vector<double> timeVec,
      std::vector<double> voltageVec,
      const std::pair<double, unsigned int> Pmax
    );

    std::pair<double, double>
    Get_Zero_Cross_Tmax(
      std::vector<double> timeVec,
      std::vector<double> voltageVec,
      const std::pair<double, unsigned int> Pmax
    );

    void Find_Bunch_Negative_Signal_Maximum(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const std::vector<double> &pmax,
        const std::vector<double> &tmax,
        std::vector<double> &negPmax,
        std::vector<double> &negTmax
    );

    //==========================================================================
    // Pulse Area
    double Find_Pulse_Area(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const std::pair<double, unsigned int> &Pmax
    );

    double Find_Pulse_Area(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const std::pair<double, unsigned int> &Pmax,
        const double &artificial_baseline
    );

    double Find_Udershoot_Area(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const std::pair<double, unsigned int> &Pmax
    );

    double Find_Udershoot_Area(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const std::pair<double, unsigned int> &Pmax,
        const double &extendedIntRange
    );

    double Pulse_Area_With_Linear_Interpolate_Edge(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const std::pair<double, unsigned int> &Pmax,
        const std::string &integration_option = "Simpson"
    );

    double Pulse_Area_With_Linear_Interpolate_Edge(
        const std::vector<double> &voltageVec, // voltage
        const std::vector<double> &timeVec,    // time
        const std::pair<double, unsigned int> &Pmax,
        const std::string &integration_option,
        const bool &relativeTimeWindow,
        const double &stopTime
    );

    double Pulse_Integration_with_Fixed_Window_Size(
        const std::vector<double> &voltageVec, // voltage
        const std::vector<double> &timeVec, // time
        const std::pair<double, unsigned int> &pmax_holder,
        const std::string &integration_option = "Simpson",
        const double &t_beforeSignal = 1000.0,
        const double &t_afterSignal = 3000.0
    );

    //==========================================================================
    // CFD
    double Rising_Edge_CFD_Time(
        const double &fraction,
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const std::pair<double, unsigned int> &Pmax
    );

    double Falling_Edge_CFD_Time(
        const int &fraction,
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const std::pair<double, unsigned int> &Pmax);

    //==========================================================================
    // Rise Time
    double Find_Rise_Time(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const std::pair<double, unsigned int> &Pmax,
        const double &bottom = 0.1,
        const double &top = 0.9
    );

    //==========================================================================
    // Noise
    double Find_Noise(
        const std::vector<double> &voltageVec,
        const unsigned int &inoise
    );

    double Find_Noise2(
        const std::vector<double> &voltageVec,
        const double &fractional_pts
    );

    double Find_Noise_On_Back_Baseline(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const double &start_time,
        const double &end_time
    );

    //==========================================================================
    // Dvdt

    double Find_Dvdt(
        const int &fraction,
        const int &ndif,
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const std::pair<double, unsigned int> &Pmax
    );

    //==========================================================================
    // Multiple Peak

    std::pair<double, unsigned int> Find_Identical_Peak(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const unsigned int &StartIndex,
        const bool &limitSearchRegion = false,
        const double &min_search_range = -1000.0,
        const double &max_search_range = 1000.0
    );

    void Get_PmaxTmax_Of_Multiple_Singal(
        const double &assist_threshold,
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        std::vector<double> &multiple_singal_pmax_v,
        std::vector<double> &multiple_singal_tmax_v,
        std::vector<int> &indexing_v,
        const double &scale = 2.0
    );

    int Signal_Peak_Counter(
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const double &assisting_threshold
    );

    int Get_Number_Of_Multiple_Signals(
      const double &threshold,
      const std::vector<double> &voltageVec,
      const double &scale = 1.0
    );

    //==========================================================================
    // time at threshold
    double Find_Time_At_Threshold(
        const double &thresholdLevel,
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        const std::pair<double, unsigned int> &Pmax
    );

    double Find_Time_At_Threshold(
        const double &thresholdLevel,
        const std::vector<double> &voltage_v,
        const std::vector<double> &time_v
    );

    double Get_TimeAcrossThreshold(
        const double &thresholdLevel,
        const std::vector<double> &voltageVec,
        const std::vector<double> &timeVec,
        std::vector<double> &time_at_threshold_v,
        const unsigned int &expect_count = 6
    );
};


#endif // WAVEFORM_ANA_H
