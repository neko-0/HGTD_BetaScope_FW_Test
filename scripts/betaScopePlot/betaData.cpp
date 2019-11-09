#include "dataOutputFormat.h"

void DataOutputFormat::CreateBetaScopeOutputFile(
  std::string biasVoltage,
  std::map<std::string, FitResult> outData,
  int temperature,
  int trigger_bias )
{

  std::string dutHeader = "DUT_"+biasVoltage + "_" + std::to_string(temperature);
  DataOutputFormat::WriteHeader( dutHeader );

  DataOutputFormat::WriteKey( "PulseArea", outData["area_zero2"].param );
  DataOutputFormat::WriteKey( "PulseArea_Error", outData["area_zero2"].param_err );
  DataOutputFormat::WriteKey( "PulseArea_NDF", outData["area_zero2"].ndf );
  DataOutputFormat::WriteKey( "PulseArea_CHI", outData["area_zero2"].chi_square );
  DataOutputFormat::WriteKey( "PulseArea_PROB", outData["area_zero2"].prob );

  DataOutputFormat::WriteKey( "Pmax", outData["pmax2"].param );
  DataOutputFormat::WriteKey( "Pmax_Error", outData["pmax2"].param_err );
  DataOutputFormat::WriteKey( "Pmax_NDF", outData["pmax2"].ndf );
  DataOutputFormat::WriteKey( "Pmax_CHI", outData["pmax2"].chi_square );
  DataOutputFormat::WriteKey( "Pmax_PROB", outData["pmax2"].prob );

  DataOutputFormat::WriteKey( "RMS", outData["rms2"].param );
  DataOutputFormat::WriteKey( "RMS_Error", outData["rms2"].param_err );
  DataOutputFormat::WriteKey( "RMS_NDF", outData["rms2"].ndf );
  DataOutputFormat::WriteKey( "RMS_CHI", outData["rms2"].chi_square );
  DataOutputFormat::WriteKey( "RMS_PROB", outData["rms2"].prob );

  DataOutputFormat::WriteKey( "Rise_Time", outData["rise2"].param );
  DataOutputFormat::WriteKey( "Rise_Time_Error", outData["rise2"].param_err );
  DataOutputFormat::WriteKey( "Rise_Time_NDF", outData["rise2"].ndf );
  DataOutputFormat::WriteKey( "Rise_Time_CHI", outData["rise2"].chi_square );
  DataOutputFormat::WriteKey( "Rise_Time_PROB", outData["rise2"].prob );

  DataOutputFormat::WriteKey( "dvdt", outData["dvdt2"].param );
  DataOutputFormat::WriteKey( "dvdt_Error", outData["dvdt2"].param_err );
  DataOutputFormat::WriteKey( "dvdt_NDF", outData["dvdt2"].ndf );
  DataOutputFormat::WriteKey( "dvdt_CHI", outData["dvdt2"].chi_square );
  DataOutputFormat::WriteKey( "dvdt_PROB", outData["dvdt2"].prob );

  DataOutputFormat::WriteKey( "FWHM", outData["fwhm2"].param );
  DataOutputFormat::WriteKey( "FWHM_Error", outData["fwhm2"].param_err );
  DataOutputFormat::WriteKey( "FWHM_NDF", outData["fwhm2"].ndf );
  DataOutputFormat::WriteKey( "FWHM_CHI", outData["fwhm2"].chi_square );
  DataOutputFormat::WriteKey( "FWHM_PROB", outData["fwhm2"].prob );

  DataOutputFormat::WriteKey( "jitter_20", outData["jitter2"].param );
  DataOutputFormat::WriteKey( "jitter_20_Error", outData["jitter2"].param_err );
  DataOutputFormat::WriteKey( "jitter_20_NDF", outData["jitter2"].ndf );
  DataOutputFormat::WriteKey( "jitter_20_CHI", outData["jitter2"].chi_square );
  DataOutputFormat::WriteKey( "jitter_20_PROB", outData["jitter2"].prob );

  DataOutputFormat::WriteKey( "NewPulseArea", outData["area_under2"].param );
  DataOutputFormat::WriteKey( "NewPulseArea_Error", outData["area_under2"].param_err );
  DataOutputFormat::WriteKey( "NewPulseArea_NDF", outData["area_under2"].ndf );
  DataOutputFormat::WriteKey( "NewPulseArea_CHI", outData["area_under2"].chi_square );
  DataOutputFormat::WriteKey( "NewPulseArea_PROB", outData["area_under2"].prob );

  DataOutputFormat::WriteKey( "FallTime", outData["fall2"].param );
  DataOutputFormat::WriteKey( "FallTime_Error", outData["fall2"].param_err );
  DataOutputFormat::WriteKey( "FallTime_NDF", outData["fall2"].ndf );
  DataOutputFormat::WriteKey( "FallTime_CHI", outData["fall2"].chi_square );
  DataOutputFormat::WriteKey( "FallTime_PROB", outData["fall2"].prob );

  DataOutputFormat::WriteKey( "temperature", temperature);
  DataOutputFormat::WriteKey( "trigger_bias", trigger_bias);

  std::string triggerHeader = "Trig_"+biasVoltage + "_" + std::to_string(temperature);
  DataOutputFormat::WriteHeader( triggerHeader );

  DataOutputFormat::WriteKey( "PulseArea", 0.0);
  DataOutputFormat::WriteKey( "PulseArea_Error", 0.0 );
  DataOutputFormat::WriteKey( "PulseArea_NDF", 0.0 );
  DataOutputFormat::WriteKey( "PulseArea_CHI", 0.0 );
  DataOutputFormat::WriteKey( "PulseArea_PROB", 0.0 );

  DataOutputFormat::WriteKey( "Pmax", outData["pmax3"].param );
  DataOutputFormat::WriteKey( "Pmax_Error", outData["pmax3"].param_err );
  DataOutputFormat::WriteKey( "Pmax_NDF", outData["pmax3"].ndf );
  DataOutputFormat::WriteKey( "Pmax_CHI", outData["pmax3"].chi_square );
  DataOutputFormat::WriteKey( "Pmax_PROB", outData["pmax3"].prob );

  DataOutputFormat::WriteKey( "RMS", outData["rms3"].param );
  DataOutputFormat::WriteKey( "RMS_Error", outData["rms3"].param_err );
  DataOutputFormat::WriteKey( "RMS_NDF", outData["rms3"].ndf );
  DataOutputFormat::WriteKey( "RMS_CHI", outData["rms3"].chi_square );
  DataOutputFormat::WriteKey( "RMS_PROB", outData["rms3"].prob );

  DataOutputFormat::WriteKey( "Rise_Time", outData["rise3"].param );
  DataOutputFormat::WriteKey( "Rise_Time_Error", outData["rise3"].param_err );
  DataOutputFormat::WriteKey( "Rise_Time_NDF", outData["rise3"].ndf );
  DataOutputFormat::WriteKey( "Rise_Time_CHI", outData["rise3"].chi_square );
  DataOutputFormat::WriteKey( "Rise_Time_PROB", outData["rise3"].prob );

  DataOutputFormat::WriteKey( "dvdt", outData["dvdt3"].param );
  DataOutputFormat::WriteKey( "dvdt_Error", outData["dvdt3"].param_err );
  DataOutputFormat::WriteKey( "dvdt_NDF", outData["dvdt3"].ndf );
  DataOutputFormat::WriteKey( "dvdt_CHI", outData["dvdt3"].chi_square );
  DataOutputFormat::WriteKey( "dvdt_PROB", outData["dvdt3"].prob );

  DataOutputFormat::WriteKey( "FWHM", outData["fwhm3"].param );
  DataOutputFormat::WriteKey( "FWHM_Error", outData["fwhm3"].param_err );
  DataOutputFormat::WriteKey( "FWHM_NDF", outData["fwhm3"].ndf );
  DataOutputFormat::WriteKey( "FWHM_CHI", outData["fwhm3"].chi_square );
  DataOutputFormat::WriteKey( "FWHM_PROB", outData["fwhm3"].prob );

  DataOutputFormat::WriteKey( "jitter_20", outData["jitter3"].param );
  DataOutputFormat::WriteKey( "jitter_20_Error", outData["jitter3"].param_err );
  DataOutputFormat::WriteKey( "jitter_20_NDF", outData["jitter3"].ndf );
  DataOutputFormat::WriteKey( "jitter_20_CHI", outData["jitter3"].chi_square );
  DataOutputFormat::WriteKey( "jitter_20_PROB", outData["jitter3"].prob );

  DataOutputFormat::WriteKey( "NewPulseArea", outData["area_under3"].param );
  DataOutputFormat::WriteKey( "NewPulseArea_Error", outData["area_under3"].param_err );
  DataOutputFormat::WriteKey( "NewPulseArea_NDF", outData["area_under3"].ndf );
  DataOutputFormat::WriteKey( "NewPulseArea_CHI", outData["area_under3"].chi_square );
  DataOutputFormat::WriteKey( "NewPulseArea_PROB", outData["area_under3"].prob );

  DataOutputFormat::WriteKey( "FallTime", outData["fall3"].param );
  DataOutputFormat::WriteKey( "FallTime_Error", outData["fall3"].param_err );
  DataOutputFormat::WriteKey( "FallTime_NDF", outData["fall3"].ndf );
  DataOutputFormat::WriteKey( "FallTime_CHI", outData["fall3"].chi_square );
  DataOutputFormat::WriteKey( "FallTime_PROB", outData["fall3"].prob );

  DataOutputFormat::WriteKey( "temperature", temperature);
  DataOutputFormat::WriteKey( "trigger_bias", trigger_bias);
}
