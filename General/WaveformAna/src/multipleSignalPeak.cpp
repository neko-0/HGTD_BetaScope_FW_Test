////////////////////////////////////////////
////                                      //
////  Multiple Signal Peak implementation //
////                                      //
///////////////////////////////////////////

//==============================================================================
// Headers

#include "WaveformAna/include/Waveform_Analysis.hpp"
#include "WaveformAna/include/general.hpp"

//-------c++----------------//
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>


/*==============================================================================
std::pair <double, unsigned int>
  WaveformAnalysis::Find_Identical_Peak

  if there are multiple peaks with same hight, the usual peak finder will only
  gives the first peak. This peak finder allows user to give a starting point
  for the search.


  std::vector<double>
    w := waveform

  std::vector<double>
    t := time trace of the waveform

  unsigned int
    StartIndex := index of the waveform vector to start the next search

  bool
    limitSearchRegion := option for limiting the pmax search region

  double
    min_search_range := minimum search range in time.

  double
    max_search_range := maximum search range in time.

  return : Pmax value and its index in the waveform vector.
==============================================================================*/
std::pair<double, unsigned int>
WaveformAnalysis::Find_Identical_Peak(
    const std::vector<double> &voltageVec,
    const std::vector<double> &timeVec,
    const unsigned int &StartIndex,
    const bool &limitSearchRegion,
    const double &min_search_range,
    const double &max_search_range)
{
    double pmax = 0.0;
    unsigned int pmax_index = StartIndex;
    bool StrangePeak = true;
    bool FirstPoint = true;
    std::size_t npoints = voltageVec.size();

    // std::cout << "StartIndex: " << StartIndex << std::endl;
    if (StartIndex)
    {
        if (limitSearchRegion)
        {
            for (unsigned int j = StartIndex; j < npoints;j++)
            {
                if (j == StartIndex)
                {
                    pmax = voltageVec.at(j);
                    pmax_index = j;
                    FirstPoint = false;
                }
                if (min_search_range <= timeVec.at(j) &&timeVec.at(j) <=max_search_range)   // zoom in to find the Pmax
                {
                    if (j != StartIndex && voltageVec.at(j) == pmax)
                    {
                        pmax = voltageVec.at(j);
                        pmax_index = j;
                        StrangePeak = false;
                    }
                }
            }
            if (StrangePeak && FirstPoint)   // previous sick point will be sent to here.
            {
                pmax = 10000.0;
                pmax_index = npoints;
            }
        }
        else
        {
            for (unsigned int j = StartIndex; j < npoints;j++)
            {
                if (j == StartIndex)
                {
                    pmax = voltageVec.at(j);
                    pmax_index = j;
                }
                if (j != StartIndex && voltageVec.at(j) == pmax)
                {
                    pmax = voltageVec.at(j);
                    pmax_index = j;
                }
            }
        }
    }
    else
    {
        if (limitSearchRegion)
        {
            for (unsigned int j = 0; j < npoints; j++)
            {
                if (min_search_range <= timeVec.at(j) &&timeVec.at(j) <= max_search_range)   // zoom in to find the Pmax
                {
                    if (FirstPoint && voltageVec.at(j) > 0)
                    {
                        pmax = voltageVec.at(j);
                        pmax_index = j;
                        FirstPoint = false;
                        StrangePeak = false;
                    }
                    if (voltageVec.at(j) > pmax)
                    {
                        pmax = voltageVec.at(j);
                        pmax_index = j;
                    }
                }
            }
            if (StrangePeak) // if it cannot find any positive point within the range,
                // return this number
            {
                pmax = 10000.0;
                pmax_index = npoints;
            }
        }
        else
        {
            for (unsigned int j = 0; j < npoints; j++)
            {
                // std::cout << "here increment " << j << std::endl;
                if (j == 0)
                {
                    pmax = 0.0;
                    pmax_index = 0;
                }
                if (j != 0 && voltageVec.at(j) > pmax)
                {
                    pmax = voltageVec.at(j);
                    pmax_index = j;
                }
            }
        }
    }
    // std::cout << "pmax_index: " << pmax_index << std::endl;

    return std::make_pair(pmax, pmax_index);
}

/*==============================================================================
void
  WaveformAnalysis::Get_PmaxTmax_Of_Multiple_Singal

  usage: get pmax and tmax pair for multiple signals on as time window.
  A threadhold is require to help with dientifying signals

  const double
    assist_threshold := threshold use for detecting signals.

  std::vector<double>
    voltageVec := voltage vector

  std::vector<double>
    timeVec := time vector

  std::vector<dohble> &
    multiple_singal_pmax_v := vector for holding all of the detected signal peak

  std::vector<dohble> &
    multiple_singal_tmax_v := time assiciated with the signal peak

  std::vector<int>  &
    indexing_v := vector for keeping track of the index of found signal peaks

  return : no return

==============================================================================*/
// find all the pmax for multiple signals

void WaveformAnalysis::Get_PmaxTmax_Of_Multiple_Singal(
    const double &assist_threshold,
    const std::vector<double> &voltageVec,
    const std::vector<double> &timeVec,
    std::vector<double> &multiple_singal_pmax_v,
    std::vector<double> &multiple_singal_tmax_v,
    std::vector<int> &indexing_v,
    const double &scale // by default scale = 2.0
)
{
    std::string function_name =
        "WaveformAnalysis::Get_PmaxTmax_Of_Multiple_Singal";

    double pmax = 0.0;
    int pmax_index = 0;
    bool candidate_signal = false;
    bool noisy_event = true;
    std::size_t npoints = voltageVec.size();

    for (unsigned int i = 0; i < npoints; i++)
    {
        if (!candidate_signal)
        {
            if (voltageVec.at(i) >= assist_threshold)
            {
                if (voltageVec.at(i) > pmax)
                {
                    pmax = voltageVec.at(i);
                    pmax_index = i;
                    candidate_signal = true;
                    if (noisy_event)
                        noisy_event = false;
                }
            }
        }
        else
        {
            if (voltageVec.at(i) > pmax)
            {
                pmax = voltageVec.at(i);
                pmax_index = i;
            }
            else if ((voltageVec.at(i) < assist_threshold) &&
                     (assist_threshold - abs(voltageVec.at(i))) <=
                     (assist_threshold / scale))
            {
                multiple_singal_pmax_v.push_back(pmax);
                multiple_singal_tmax_v.push_back(timeVec.at(
                                                     pmax_index));
                indexing_v.push_back(pmax_index);
                pmax = voltageVec.at(i);
                pmax_index = i;
                candidate_signal = false;
            }
            else if (i == npoints - 1)
            {
                multiple_singal_pmax_v.push_back(pmax);
                multiple_singal_tmax_v.push_back(timeVec.at(pmax_index));
                indexing_v.push_back(pmax_index);
            }
            else
            {
            }
        }
    }

    if (noisy_event)
    {
        multiple_singal_pmax_v.push_back(10e11); // default value if nothing is found.
        multiple_singal_tmax_v.push_back(10e11);
        indexing_v.push_back(0);
        if (!this->supressNoisy)
        {
            this->mu.lock();
            wave_logger.warning( __PRETTY_FUNCTION__, "Noisy"); // if too many "Noisy", there might be a problem.
            this->mu.unlock();
        }
        this->supressNoisyCounter++;
        if (this->supressNoisyCounter == 100)
        {
            this->mu.lock();
            wave_logger.warning( __PRETTY_FUNCTION__, "supressNoisyCounter reaches 100");
            this->mu.unlock();
            this->supressNoisy = true;
        }
    }
    else if (multiple_singal_pmax_v.size() == 0)
    {
        multiple_singal_pmax_v.push_back(10e11); // default value if nothing is found.
        multiple_singal_tmax_v.push_back(10e11);
        indexing_v.push_back(0);
        if (!this->supressNoisy)
        {
            this->mu.lock();
            wave_logger.warning( __PRETTY_FUNCTION__, "Method fail, the output size is 0. Assigning ""default value"); // if too many "Noisy", there
            // might be a problem.
            this->mu.unlock();
        }
    }
}

/*==============================================================================
Multiple signal peaks counter.
  param

  return number of signal peaks.
===============================================================================*/
int Signal_Peak_Counter(
  std::vector<double>&voltageVec,
  const std::vector<double> &timeVec,
  const double &assisting_threshold
)
{
    std::vector<double> multiple_singal_pmax_v = {};
    multiple_singal_pmax_v.reserve(timeVec.size());

    double pmax = 0.0;
    // unsigned int pmax_index = 0;

    bool candidate_signal = false;
    bool noisy_event = true;

    std::size_t npoints = voltageVec.size();

    for (unsigned int i = 0; i < npoints; i++)
    {
        if (!candidate_signal)
        {
            if (voltageVec.at(i) >= assisting_threshold)
            {
                if (voltageVec.at(i) > pmax)
                {
                    pmax = voltageVec.at(i);
                    // pmax_index = i;
                    candidate_signal = true;
                }
            }
        }
        else
        {
            if (voltageVec.at(i) > pmax)
            {
                pmax = voltageVec.at(i);
                // pmax_index = i;
            }
            else if ((voltageVec.at(i) < assisting_threshold) && (assisting_threshold - voltageVec.at(i)) <= (assisting_threshold))
            {
                multiple_singal_pmax_v.push_back(pmax);
                pmax = voltageVec.at(i);
                // pmax_index = i;
                candidate_signal = false;
                if (noisy_event)
                    noisy_event = false;
            }
            else
            {
            }
        }
    }

    if (!noisy_event)
    {
        return multiple_singal_pmax_v.size();
    }
    else
        return 0;
}
