#include "General/Trigger/include/Trigger.hpp"


bool Trigger::MatchPmax(const WaveformAna<double, double> &waveform, const double &start, const double &end)
{
  if( waveform.pmax() >= start && waveform.pmax() < end )
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Trigger::MatchTmax(const WaveformAna<double, double> &waveform, const double &start, const double &end)
{
  if( waveform.tmax()-waveform.cfd().at(20) >= start && waveform.tmax()-waveform.cfd().at(20) < end )
  {
    return true;
  }
  else
  {
    return false;
  }
}


bool Trigger::LGAD_Trigger( const WaveformAna <double, double> &waveform )
{
  WaveformAnalysis WaveAna;
  int count_over_th = WaveAna.Get_Number_Of_Multiple_Signals( 20, waveform.get_v2() );

  if( Trigger::MatchPmax(waveform) && Trigger::MatchTmax(waveform) && count_over_th < 3 )
  {
    return true;
  }
  else
  {
    return false;
  }
}
