#ifndef WAVEFORM_ANA_H
#define WAVEFORM_ANA_H

#include <vector>
#include <string>

#include "General/Colorful_Cout/include/Colorful_Cout.h"

class WaveformAnalysis
{
  bool supressNoisy = false;
  int supressNoisyCounter = 0;

  public:

    WaveformAnalysis(){};
    ~WaveformAnalysis(){};

    //==========================================================================
    // Baseline Correction
    void Correct_Baseline( std::vector<double> &voltageVec, int ptN );
    void Correct_Baseline2( std::vector<double> &voltageVec, double fractional_pts );
    void Correct_Baseline3( std::vector<double> &voltageVec, std::vector<double> timeVec, double tRange[2] ); //special treatment of ill-signal baseline.
    bool Correct_Baseline4( std::vector<double> &voltageVec, std::vector<double> timeVec, std::vector<double> &pmax, std::vector<double> tmax);
    void SSRL_Baseline( std::vector<double> &w, std::string workerID );
    void SSRL_Baseline( std::vector<double> &w, double &RMS);
    void SSRL_Dynamic_Noise_And_Baseline(
      std::vector<double> &w, //voltage
      std::vector<double> t, //time
      double assisting_threshold,
      const double time_separation,
      double &RMS
    );

    //==========================================================================
    // Pulse Maximum (Pmax)
    std::pair<double, unsigned int> Find_Singal_Maximum(
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      bool confineSearchRegion,
      double searchRange[2]
    );

    std::pair<double, unsigned int> Find_Negative_Signal_Maximum(
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      bool confineSearchRegion,
      double searchRange[2]
    );

    double Get_Tmax(
      std::vector<double> timeVec,
      const std::pair<double, unsigned int> Pmax
    );

    void Find_Bunch_Negative_Signal_Maximum(
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      std::vector<double> pmax,
      std::vector<double> tmax,
      std::vector<double> &negPmax,
      std::vector<double> &negTmax
    );

    //==========================================================================
    // Pulse Area
    double Find_Pulse_Area(
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      const std::pair<double,unsigned int> Pmax
    );

    double Find_Pulse_Area(
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      const std::pair<double,unsigned int> Pmax,
      const double artificial_baseline
    );

    double Find_Udershoot_Area(
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      const std::pair<double, unsigned int> Pmax
    );

    double Find_Udershoot_Area(
     std::vector<double> voltageVec,
     std::vector<double> timeVec,
     const std::pair<double,unsigned int> Pmax,
     double extendedIntRange
    );

    double Pulse_Area_With_Linear_Interpolate_Edge(
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      const std::pair<double,unsigned int> Pmax,
      std::string integration_option = "Simpson"
    );

    double Pulse_Area_With_Linear_Interpolate_Edge(
      std::vector<double> voltageVec, //voltage
      std::vector<double> timeVec, //time
      const std::pair<double,unsigned int> Pmax,
      std::string integration_option,
      bool relativeTimeWindow,
      double stopTime
    );

    double Pulse_Integration_with_Fixed_Window_Size(
      std::vector<double> w, //voltage
      std::vector<double> t, //time
      const std::pair<double,unsigned int> pmax_holder,
      std::string integration_option = "Simpson",
      const double t_beforeSignal = 1000.0,
      const double t_afterSignal = 3000.0
    );

    //==========================================================================
    // CFD
    double Rising_Edge_CFD_Time(
      const double fraction,
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      const std::pair<double,unsigned int> Pmax
    );

    double Falling_Edge_CFD_Time(
      const int           fraction,
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      const std::pair<double,unsigned int> Pmax
    );

    //==========================================================================
    // Rise Time
    double Find_Rise_Time(
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      const std::pair<double, unsigned int> Pmax,
      double bottom = 0.1,
      double top = 0.9
    );

    //==========================================================================
    // Noise
    double Find_Noise(
      std::vector<double> voltageVec,
      const unsigned int inoise
    );

    double Find_Noise2(
      std::vector<double> voltageVec,
      const double fractional_pts
    );

    double Find_Noise_On_Back_Baseline(
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      double start_time,
      double end_time
    );

    //==========================================================================
    // Dvdt

    double Find_Dvdt(
      const int           fraction,
      const int           ndif,
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      const std::pair<double,unsigned int> Pmax
    );

    //==========================================================================
    // Multiple Peak

    std::pair <double, unsigned int> Find_Identical_Peak(
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      unsigned int StartIndex,
      bool limitSearchRegion = false,
      double min_search_range = -1000.0,
      double max_search_range = 1000.0
    );

    void Get_PmaxTmax_Of_Multiple_Singal(
      const double        assist_threshold,
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      std::vector<double> &multiple_singal_pmax_v,
      std::vector<double> &multiple_singal_tmax_v,
      std::vector<int>    &indexing_v,
      const double scale = 2.0
    );

    int Signal_Peak_Counter(
      std::vector<double> &voltageVec,
      std::vector<double> timeVec,
      double assisting_threshold
    );

    //==========================================================================
    // time at threshold
    double Find_Time_At_Threshold(
      const double                thresholdLevel,
      std::vector<double>         voltageVec,
      std::vector<double>         timeVec,
      const std::pair<double,unsigned int> Pmax
    );

    double Find_Time_At_Threshold(
      const double                thresholdLevel,
      std::vector<double>         voltage_v,
      std::vector<double>         time_v
    );

    void Get_TimeAcrossThreshold(
      const double        thresholdLevel,
      std::vector<double> voltageVec,
      std::vector<double> timeVec,
      std::vector<double> &time_at_threshold_v,
      const unsigned int  expect_count = 6
    );
};



#endif // WAVEFORM_ANA_H
