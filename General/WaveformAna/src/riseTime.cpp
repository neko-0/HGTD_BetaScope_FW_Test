//////////////////////////////////////
////                                //
////  Rise Time implementation      //
////                                //
//////////////////////////////////////

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
//------ROOT----------------//
#include <TCanvas.h>
#include <TF1.h>
#include <TGraph.h>
#include <TH1.h>
#include <TImage.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TThread.h>

/*==============================================================================
Find the rise time
  param npoints     := number of sampled points in a waveform.
  param w           := waveform
  param t           := time trace of the waveform
  param pmax_holder := the Pmax tuple that contains the index
  param bottom      := percentage of the start point
  param top         := percentage of the end point

  return rise time of the signal
==============================================================================*/
double WaveformAnalysis::Find_Rise_Time(
    const std::vector<double> &voltageVec,
    const std::vector<double> &timeVec,
    const std::pair<double, unsigned int> &Pmax,
    const double &bottom,
    const double &top
)
{
    double rise = 0.0;

    unsigned int itop = voltageVec.size() - 2, ibottom = 0;

    bool ten = true, ninety = true;

    unsigned int imax = Pmax.second;
    double pmax = Pmax.first;

    double lowerval = pmax * bottom;
    double upperval = pmax * top;

    for (int j = imax; j > -1; j--)
    {
        if (ninety && voltageVec.at(j) < upperval)
        {
            itop = j; // find the index right below 90%
            ninety = false;
        }
        if (ten && voltageVec.at(j) < lowerval)
        {
            ibottom = j; // find the index right below 10%
            ten = false;
        }
        if (!ten && !ninety)
        {
            break;
        }
    }
    if (ibottom == voltageVec.size() - 1)
    {
        ibottom--;
    }
    if (itop == voltageVec.size() - 1)
    {
        itop--;
    }
    // std::cout<<itop<<std::endl;
    // std::cout<<ibottom<<std::endl;
    double tbottom = xlinearInter(timeVec.at(ibottom), voltageVec.at(ibottom), timeVec.at(ibottom + 1), voltageVec.at(ibottom + 1), lowerval);
    double ttop = xlinearInter(timeVec.at(itop), voltageVec.at(itop), timeVec.at(itop + 1), voltageVec.at(itop + 1), upperval);

    rise = ttop - tbottom; // rise
    return rise;
}
