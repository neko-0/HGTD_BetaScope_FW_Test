//////////////////////////////////////
////                                //
////  Dvdt implementation           //
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

//===========================================================================================================================================

double WaveformAnalysis::Find_Dvdt(
    const int &fraction,
    const int &ndif,
    const std::vector<double> &voltageVec,
    const std::vector<double> &timeVec,
    const std::pair<double, unsigned int> &Pmax)
{
    // function to dv/dt at a given constant fraction value.

    double time_difference = 0.0;
    double dvdt = 0.0;

    int ifraction = 0;

    time_difference = timeVec.at(1) - timeVec.at(0);

    double pmax = Pmax.first;
    unsigned int imax = Pmax.second;

    for (int j = imax; j > -1; j--)
    {
        if (voltageVec.at(j) <= pmax * double(fraction) / 100)
        {
            ifraction = j;
            break;
        }
    } // find index of first point before constant fraction of pulse

    if (ifraction == voltageVec.size() - 1)
        ifraction--;

    if (ndif == 0)
    {
        dvdt = (voltageVec.at(ifraction + 1) -voltageVec.at(ifraction)) /time_difference;
    }

    else
    {
        dvdt = (voltageVec.at(ifraction + ndif) -voltageVec.at(ifraction - ndif)) /(time_difference * (ndif * 2));
    }

    return dvdt;
}
