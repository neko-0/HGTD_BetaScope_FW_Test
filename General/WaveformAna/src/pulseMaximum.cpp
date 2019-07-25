//////////////////////////////////////
////                                //
////  Pulse Maximum implementation  //
////                                //
//////////////////////////////////////


//==============================================================================
// Headers

#include "WaveformAna/include/Waveform_Analysis.hpp"
#include "WaveformAna/include/general.hpp"

//-------c++----------------//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <numeric>
#include <functional>
//------ROOT----------------//
#include <TH1.h>
#include <TF1.h>
#include <TGraph.h>
#include <TThread.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TImage.h>
#include <TCanvas.h>

//==============================================================================


/*==============================================================================
Finding the Signal Maximum (Pmax)
  param voltageVec := waveform
  param timeVect   := time trace of the waveform
  param confineSearchRegion := turn ON of OFF the search region.
  param searchRage := the range for the Pmax search

  return : Pmax value and its index in the waveform vector.
==============================================================================*/
std::pair<double, unsigned int> WaveformAnalysis::Find_Singal_Maximum(
  std::vector<double> voltageVec,
  std::vector<double> timeVec,
  bool confineSearchRegion,
  double searchRange[2]
)
{
    double          pmax        = 0.0;
    unsigned int    pmaxIndex  = 0;
    //bool   strangePeak = true;
    bool   firstPoint  = true;
    std::size_t npoints = voltageVec.size();

    if( confineSearchRegion )
    {
      for( std::size_t j = 0; j < npoints; j++)
      {
        if( searchRange[0] <= timeVec.at(j) && timeVec.at(j) <= searchRange[1] ) //zoom in to find the Pmax
        {
            if( firstPoint ){ pmaxIndex = j; firstPoint = false; }
            if( voltageVec.at(j) > pmax )
            {
              pmax      = voltageVec.at(j);
              pmaxIndex = j;
            }
        }
      }
    }
    else
    {
      for( std::size_t j = 0; j < npoints; j++ )
      {
          if (j == 0)
          {
            pmax = voltageVec[j];
            pmaxIndex = j;
          }
          if (j != 0 && voltageVec.at(j) > pmax)
          {
            pmax = voltageVec.at(j);
            pmaxIndex = j;
        }
      }
    }

    return std::make_pair( pmax, pmaxIndex);
}


/*==============================================================================
Find the negative signal maximum (indeed the signal minimum)
  param voltageVec := waveform (stl vector)
  param timeVec    := time trace of the waveform
  param confineSearchRegion := turn ON of OFF the search region.
  param searchRage := the range for the Pmax search

  return : Negative Pmax value and its index in the waveform vector.
==============================================================================*/
std::pair<double, unsigned int> WaveformAnalysis::Find_Negative_Signal_Maximum(
  std::vector<double> voltageVec,
  std::vector<double> timeVec,
  bool confineSearchRegion,
  double searchRange[2]
)
{
    double pmax = 0.0;
    unsigned int pmaxIndex  = 0;
    //bool strangePeak = true;
    bool firstPoint  = true;
    std::size_t npoints = voltageVec.size();

    if( confineSearchRegion )
    {
      for( std::size_t j = 0; j < npoints; j++)
      {
        if( searchRange[0] <= timeVec.at(j) && timeVec.at(j) <= searchRange[1] ) //zoom in to find the Pmax
        {
            if( firstPoint ){ pmaxIndex = j; firstPoint = false; }
            if( voltageVec.at(j) < pmax )
            {
              pmax = voltageVec.at(j);
              pmaxIndex = j;
            }
        }
      }
    }
    else
    {
      for( std::size_t j = 0, max = npoints; j < max; j++ )
      {
          if (j == 0)
          {
            pmax = voltageVec.at(j);
            pmaxIndex = j;
          }
          if (j != 0 && voltageVec.at(j) < pmax)
          {
            pmax = voltageVec.at(j);
            pmaxIndex = j;
        }
      }
    }

    return std::make_pair( pmax, pmaxIndex);
}


/*==============================================================================
Give the time of the singal maximum (Tmax)
  param t           := time trace of the waveform
  param pmax_holder := the Pmax tuple that contains the index

  return : Tmax
==============================================================================*/
double WaveformAnalysis::Get_Tmax(
  std::vector<double> timeVec,
  const std::pair<double, unsigned int> Pmax
)
{
  double tmax = timeVec.at(Pmax.second);
  return tmax;
}
