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
  std::string function_name = "WaveformAnalysis::Find_Negative_Signal_Maximum";

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
std::pair<double, unsigned int>
  WaveformAnalysis::Find_Bunch_Negative_Signal_Maximum

  usage:
    finding negative peak between bunches. Mainly use for (SSRL) bunch signals.
  It use the pmaxs and tmaxs to define a search range for the minimum peak.

  paramters:

    std::vector<double>
      voltageVec := vector of voltage value.

    std::vector<double>
      timeVec := vector of time value.

    std::vector<double>
      pmax := vector of pmax value (found using multiple peak function)

    std::vector<double>
      tmax := vector of tmax value (found using multiple peak function)

    std::vector<double> &
      negPmax := vector for negative pmax, filled up by reference.

    std::vector<double> &
      negTmax := vector for negative tmax, filled up by reference.

  return : void
==============================================================================*/
void WaveformAnalysis::Find_Bunch_Negative_Signal_Maximum(
  std::vector<double> voltageVec,
  std::vector<double> timeVec,
  std::vector<double> pmax,
  std::vector<double> tmax,
  std::vector<double> &negPmax,
  std::vector<double> &negTmax
)
{
  std::string function_name = "WaveformAnalysis::Find_Bunch_Negative_Signal_Maximum";

  if( voltageVec.size() != timeVec.size() && pmax.size() != tmax.size() )
  {
    if(this->Find_Bunch_Negative_Signal_Maximum_counter>100)ColorCout::ErrorMsg(function_name, "Size dose not match! fill with 10e11." );
    negPmax.push_back( 10e11 );
    negTmax.push_back( 10e11 );
    this->Find_Bunch_Negative_Signal_Maximum_counter++;
  }
  else
  {
    if( pmax.size()==1 )
    {
      if(this->Find_Bunch_Negative_Signal_Maximum_counter>100)ColorCout::Msg(function_name, "Only one pmax, no needed to use this function, set value to -10e11" );
      negPmax.push_back( -10e11 );
      negTmax.push_back( -10e11 );
      this->Find_Bunch_Negative_Signal_Maximum_counter++;
    }
    else
    {
      for(std::size_t i=0, max=pmax.size()-1; i<max; i++)
      {
        double searchRange[2] = {tmax.at(i), tmax.at(i+1) };
        std::pair<double, unsigned int> my_negPmax = WaveformAnalysis::Find_Negative_Signal_Maximum(voltageVec, timeVec, true, searchRange);
        negPmax.push_back( my_negPmax.first );
        negTmax.push_back( timeVec.at(my_negPmax.second) );
      }
    }
  }
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
