#include "dataOutputFormat.h"


void DataOutputFormat::CreateBetaScopeOutputFile(
  std::string biasVoltage,
  std::map<std::string,std::pair<double,double>> outData,
  int temperature,
  int trigger_bias )
{

  std::string dutHeader = "DUT_"+biasVoltage + "_" + std::to_string(temperature);
  DataOutputFormat::WriteHeader( dutHeader );

  DataOutputFormat::WriteKey( "PulseArea", outData["area_zero2"].first );
  DataOutputFormat::WriteKey( "PulseArea_Error", outData["area_zero2"].second );

  DataOutputFormat::WriteKey( "Pmax", outData["pmax2"].first );
  DataOutputFormat::WriteKey( "Pmax_Error", outData["pmax2"].second );

  DataOutputFormat::WriteKey( "RMS", outData["rms2"].first );
  DataOutputFormat::WriteKey( "RMS_Error", outData["rms2"].second );

  DataOutputFormat::WriteKey( "Rise_Time", outData["rise2"].first );
  DataOutputFormat::WriteKey( "Rise_Time_Error", outData["rise2"].second );

  DataOutputFormat::WriteKey( "dvdt", outData["dvdt2"].first );
  DataOutputFormat::WriteKey( "dvdt_Error", outData["dvdt2"].second );

  DataOutputFormat::WriteKey( "FWHM", outData["fwhm2"].first );
  DataOutputFormat::WriteKey( "FWHM_Error", outData["fwhm2"].second );

  DataOutputFormat::WriteKey( "jitter_20", outData["jitter2"].first );
  DataOutputFormat::WriteKey( "jitter_20_Error", outData["jitter2"].second );

  DataOutputFormat::WriteKey( "NewPulseArea", outData["area_under2"].first );
  DataOutputFormat::WriteKey( "NewPulseArea_Error", outData["area_under2"].second );

  DataOutputFormat::WriteKey( "FallTime", outData["fall2"].first );
  DataOutputFormat::WriteKey( "FallTime_Error", outData["fall2"].second );

  DataOutputFormat::WriteKey( "temperature", temperature);
  DataOutputFormat::WriteKey( "trigger_bias", trigger_bias);

  std::string triggerHeader = "Trig_"+biasVoltage + "_" + std::to_string(temperature);
  DataOutputFormat::WriteHeader( triggerHeader );

  DataOutputFormat::WriteKey( "PulseArea", 0.0);
  DataOutputFormat::WriteKey( "PulseArea_Error", 0.0 );

  DataOutputFormat::WriteKey( "Pmax", outData["pmax3"].first );
  DataOutputFormat::WriteKey( "Pmax_Error", outData["pmax3"].second );

  DataOutputFormat::WriteKey( "RMS", outData["rms3"].first );
  DataOutputFormat::WriteKey( "RMS_Error", outData["rms3"].second );

  DataOutputFormat::WriteKey( "Rise_Time", outData["rise3"].first );
  DataOutputFormat::WriteKey( "Rise_Time_Error", outData["rise3"].second );

  DataOutputFormat::WriteKey( "dvdt", outData["dvdt3"].first );
  DataOutputFormat::WriteKey( "dvdt_Error", outData["dvdt3"].second );

  DataOutputFormat::WriteKey( "FWHM", outData["fwhm3"].first );
  DataOutputFormat::WriteKey( "FWHM_Error", outData["fwhm3"].second );

  DataOutputFormat::WriteKey( "jitter_20", outData["jitter3"].first );
  DataOutputFormat::WriteKey( "jitter_20_Error", outData["jitter3"].second );

  DataOutputFormat::WriteKey( "NewPulseArea", outData["area_under3"].first );
  DataOutputFormat::WriteKey( "NewPulseArea_Error", outData["area_under3"].second );

  DataOutputFormat::WriteKey( "FallTime", outData["fall3"].first );
  DataOutputFormat::WriteKey( "FallTime_Error", outData["fall3"].second );

  DataOutputFormat::WriteKey( "temperature", temperature);
  DataOutputFormat::WriteKey( "trigger_bias", trigger_bias);
}
