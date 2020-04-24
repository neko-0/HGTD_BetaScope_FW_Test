#ifndef TRIGGER_H
#define TRIGGER_H

#include "General/WaveformAna/include/Waveform_Analysis.hpp"
#include "General/WaveformAna/include/WaveformAna.hpp"

class Trigger
{
public:
  Trigger(){};
  ~Trigger(){};

  static bool MatchPmax( const WaveformAna<double, double> &waveform, const double &start=70, const double &end=350);
  static bool MatchTmax( const WaveformAna<double, double> &waveform, const double &start=0, const double &end=500);

  static bool LGAD_Trigger( const WaveformAna <double, double> &waveform );
};

#endif //TRIGGER
