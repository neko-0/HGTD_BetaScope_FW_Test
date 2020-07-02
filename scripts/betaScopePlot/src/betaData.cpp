#include "betaScopePlot/include/dataOutputFormat.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

void DataOutputFormat::CreateBetaScopeOutputFile(
    std::string biasVoltage,
    std::map<std::string, FitResult> outData,
    int temperature,
    int trigger_bias
)
{

  std::string dutHeader = "DUT_" + biasVoltage + "_" + std::to_string(temperature);
  DataOutputFormat::WriteHeader(dutHeader);

  DataOutputFormat::WriteKey("PulseArea", outData["area_zero2"].param);
  DataOutputFormat::WriteKey("PulseArea_Error", outData["area_zero2"].param_err);
  DataOutputFormat::WriteKey("PulseArea_NDF", outData["area_zero2"].ndf);
  DataOutputFormat::WriteKey("PulseArea_CHI", outData["area_zero2"].chi_square);
  DataOutputFormat::WriteKey("PulseArea_PROB", outData["area_zero2"].prob);
  DataOutputFormat::WriteKey("PulseArea_CHI_NDF", outData["area_zero2"].chi_ndf);

  DataOutputFormat::WriteKey("Pmax", outData["pmax2"].param);
  DataOutputFormat::WriteKey("Pmax_Error", outData["pmax2"].param_err);
  DataOutputFormat::WriteKey("Pmax_NDF", outData["pmax2"].ndf);
  DataOutputFormat::WriteKey("Pmax_CHI", outData["pmax2"].chi_square);
  DataOutputFormat::WriteKey("Pmax_PROB", outData["pmax2"].prob);
  DataOutputFormat::WriteKey("Pmax_CHI_NDF", outData["pmax2"].chi_ndf);

  DataOutputFormat::WriteKey("RMS", outData["rms2"].param);
  DataOutputFormat::WriteKey("RMS_Error", outData["rms2"].param_err);
  DataOutputFormat::WriteKey("RMS_NDF", outData["rms2"].ndf);
  DataOutputFormat::WriteKey("RMS_CHI", outData["rms2"].chi_square);
  DataOutputFormat::WriteKey("RMS_PROB", outData["rms2"].prob);
  DataOutputFormat::WriteKey("RMS_CHI_NDF", outData["rms2"].chi_ndf);

  DataOutputFormat::WriteKey("Rise_Time", outData["rise2"].param);
  DataOutputFormat::WriteKey("Rise_Time_Error", outData["rise2"].param_err);
  DataOutputFormat::WriteKey("Rise_Time_NDF", outData["rise2"].ndf);
  DataOutputFormat::WriteKey("Rise_Time_CHI", outData["rise2"].chi_square);
  DataOutputFormat::WriteKey("Rise_Time_PROB", outData["rise2"].prob);
  DataOutputFormat::WriteKey("Rise_Time_CHI_NDF", outData["rise2"].chi_ndf);

  DataOutputFormat::WriteKey("dvdt", outData["dvdt2"].param);
  DataOutputFormat::WriteKey("dvdt_Error", outData["dvdt2"].param_err);
  DataOutputFormat::WriteKey("dvdt_NDF", outData["dvdt2"].ndf);
  DataOutputFormat::WriteKey("dvdt_CHI", outData["dvdt2"].chi_square);
  DataOutputFormat::WriteKey("dvdt_PROB", outData["dvdt2"].prob);
  DataOutputFormat::WriteKey("dvdt_CHI_NDF", outData["dvdt2"].chi_ndf);

  DataOutputFormat::WriteKey("FWHM", outData["fwhm2"].param);
  DataOutputFormat::WriteKey("FWHM_Error", outData["fwhm2"].param_err);
  DataOutputFormat::WriteKey("FWHM_NDF", outData["fwhm2"].ndf);
  DataOutputFormat::WriteKey("FWHM_CHI", outData["fwhm2"].chi_square);
  DataOutputFormat::WriteKey("FWHM_PROB", outData["fwhm2"].prob);
  DataOutputFormat::WriteKey("FWHM_CHI_NDF", outData["fwhm2"].chi_ndf);

  DataOutputFormat::WriteKey("jitter_20", outData["jitter2"].param);
  DataOutputFormat::WriteKey("jitter_20_Error", outData["jitter2"].param_err);
  DataOutputFormat::WriteKey("jitter_20_NDF", outData["jitter2"].ndf);
  DataOutputFormat::WriteKey("jitter_20_CHI", outData["jitter2"].chi_square);
  DataOutputFormat::WriteKey("jitter_20_PROB", outData["jitter2"].prob);
  DataOutputFormat::WriteKey("jitter_20_CHI_NDF", outData["jitter2"].chi_ndf);

  DataOutputFormat::WriteKey("NewPulseArea", outData["area_under2"].param);
  DataOutputFormat::WriteKey("NewPulseArea_Error",outData["area_under2"].param_err);
  DataOutputFormat::WriteKey("NewPulseArea_NDF", outData["area_under2"].ndf);
  DataOutputFormat::WriteKey("NewPulseArea_CHI",outData["area_under2"].chi_square);
  DataOutputFormat::WriteKey("NewPulseArea_PROB", outData["area_under2"].prob);
  DataOutputFormat::WriteKey("NewPulseArea_CHI_NDF",outData["area_under2"].chi_ndf);

  DataOutputFormat::WriteKey("FallTime", outData["fall2"].param);
  DataOutputFormat::WriteKey("FallTime_Error", outData["fall2"].param_err);
  DataOutputFormat::WriteKey("FallTime_NDF", outData["fall2"].ndf);
  DataOutputFormat::WriteKey("FallTime_CHI", outData["fall2"].chi_square);
  DataOutputFormat::WriteKey("FallTime_PROB", outData["fall2"].prob);
  DataOutputFormat::WriteKey("FallTime_CHI_NDF", outData["fall2"].chi_ndf);

  DataOutputFormat::WriteKey("DeltaT_Res50", outData["res50"].param);
  DataOutputFormat::WriteKey("DeltaT_Res50_Error", outData["res50"].param_err);
  DataOutputFormat::WriteKey("DeltaT_Res50_NDF", outData["res50"].ndf);
  DataOutputFormat::WriteKey("DeltaT_Res50_CHI", outData["res50"].chi_square);
  DataOutputFormat::WriteKey("DeltaT_Res50_PROB", outData["res50"].prob);
  DataOutputFormat::WriteKey("DeltaT_Res50_CHI_NDF", outData["res50"].chi_ndf);

  DataOutputFormat::WriteKey("DeltaT_Res20", outData["res20"].param);
  DataOutputFormat::WriteKey("DeltaT_Res20_Error", outData["res20"].param_err);
  DataOutputFormat::WriteKey("DeltaT_Res20_NDF", outData["res20"].ndf);
  DataOutputFormat::WriteKey("DeltaT_Res20_CHI", outData["res20"].chi_square);
  DataOutputFormat::WriteKey("DeltaT_Res20_PROB", outData["res20"].prob);
  DataOutputFormat::WriteKey("DeltaT_Res20_CHI_NDF", outData["res20"].chi_ndf);

  DataOutputFormat::WriteKey("DeltaT_resTmax", outData["resTmax"].param);
  DataOutputFormat::WriteKey("DeltaT_resTmax_Error", outData["resTmax"].param_err);
  DataOutputFormat::WriteKey("DeltaT_resTmax_NDF", outData["resTmax"].ndf);
  DataOutputFormat::WriteKey("DeltaT_resTmax_CHI", outData["resTmax"].chi_square);
  DataOutputFormat::WriteKey("DeltaT_resTmax_PROB", outData["resTmax"].prob);
  DataOutputFormat::WriteKey("DeltaT_resTmax_CHI_NDF", outData["resTmax"].chi_ndf);

  DataOutputFormat::WriteKey("DeltaT_resZeroCross", outData["resZeroCross"].param);
  DataOutputFormat::WriteKey("DeltaT_resZeroCross_Error", outData["resZeroCross"].param_err);
  DataOutputFormat::WriteKey("DeltaT_resZeroCross_NDF", outData["resZeroCross"].ndf);
  DataOutputFormat::WriteKey("DeltaT_resZeroCross_CHI", outData["resZeroCross"].chi_square);
  DataOutputFormat::WriteKey("DeltaT_resZeroCross_PROB", outData["resZeroCross"].prob);
  DataOutputFormat::WriteKey("DeltaT_resZeroCross_CHI_NDF", outData["resZeroCross"].chi_ndf);

  DataOutputFormat::WriteKey("temperature", temperature);
  DataOutputFormat::WriteKey("trigger_bias", trigger_bias);

  std::string triggerHeader = "Trig_" + biasVoltage + "_" + std::to_string(temperature);
  DataOutputFormat::WriteHeader(triggerHeader);

  DataOutputFormat::WriteKey("PulseArea", 0.0);
  DataOutputFormat::WriteKey("PulseArea_Error", 0.0);
  DataOutputFormat::WriteKey("PulseArea_NDF", 0.0);
  DataOutputFormat::WriteKey("PulseArea_CHI", 0.0);
  DataOutputFormat::WriteKey("PulseArea_PROB", 0.0);
  DataOutputFormat::WriteKey("PulseArea_CHI_NDF", 0.0);

  DataOutputFormat::WriteKey("Pmax", outData["pmax3"].param);
  DataOutputFormat::WriteKey("Pmax_Error", outData["pmax3"].param_err);
  DataOutputFormat::WriteKey("Pmax_NDF", outData["pmax3"].ndf);
  DataOutputFormat::WriteKey("Pmax_CHI", outData["pmax3"].chi_square);
  DataOutputFormat::WriteKey("Pmax_PROB", outData["pmax3"].prob);
  DataOutputFormat::WriteKey("Pmax_CHI_NDF", outData["pmax3"].chi_ndf);

  DataOutputFormat::WriteKey("RMS", outData["rms3"].param);
  DataOutputFormat::WriteKey("RMS_Error", outData["rms3"].param_err);
  DataOutputFormat::WriteKey("RMS_NDF", outData["rms3"].ndf);
  DataOutputFormat::WriteKey("RMS_CHI", outData["rms3"].chi_square);
  DataOutputFormat::WriteKey("RMS_PROB", outData["rms3"].prob);
  DataOutputFormat::WriteKey("RMS_CHI_NDF", outData["rms3"].chi_ndf);

  DataOutputFormat::WriteKey("Rise_Time", outData["rise3"].param);
  DataOutputFormat::WriteKey("Rise_Time_Error", outData["rise3"].param_err);
  DataOutputFormat::WriteKey("Rise_Time_NDF", outData["rise3"].ndf);
  DataOutputFormat::WriteKey("Rise_Time_CHI", outData["rise3"].chi_square);
  DataOutputFormat::WriteKey("Rise_Time_PROB", outData["rise3"].prob);
  DataOutputFormat::WriteKey("Rise_Time_CHI_NDF", outData["rise3"].chi_ndf);

  DataOutputFormat::WriteKey("dvdt", outData["dvdt3"].param);
  DataOutputFormat::WriteKey("dvdt_Error", outData["dvdt3"].param_err);
  DataOutputFormat::WriteKey("dvdt_NDF", outData["dvdt3"].ndf);
  DataOutputFormat::WriteKey("dvdt_CHI", outData["dvdt3"].chi_square);
  DataOutputFormat::WriteKey("dvdt_PROB", outData["dvdt3"].prob);
  DataOutputFormat::WriteKey("dvdt_CHI_NDF", outData["dvdt3"].chi_ndf);

  DataOutputFormat::WriteKey("FWHM", outData["fwhm3"].param);
  DataOutputFormat::WriteKey("FWHM_Error", outData["fwhm3"].param_err);
  DataOutputFormat::WriteKey("FWHM_NDF", outData["fwhm3"].ndf);
  DataOutputFormat::WriteKey("FWHM_CHI", outData["fwhm3"].chi_square);
  DataOutputFormat::WriteKey("FWHM_PROB", outData["fwhm3"].prob);
  DataOutputFormat::WriteKey("FWHM_CHI_NDF", outData["fwhm3"].chi_ndf);

  DataOutputFormat::WriteKey("jitter_20", outData["jitter3"].param);
  DataOutputFormat::WriteKey("jitter_20_Error", outData["jitter3"].param_err);
  DataOutputFormat::WriteKey("jitter_20_NDF", outData["jitter3"].ndf);
  DataOutputFormat::WriteKey("jitter_20_CHI", outData["jitter3"].chi_square);
  DataOutputFormat::WriteKey("jitter_20_PROB", outData["jitter3"].prob);
  DataOutputFormat::WriteKey("jitter_20_CHI_NDF", outData["jitter3"].chi_ndf);

  DataOutputFormat::WriteKey("NewPulseArea", outData["area_under3"].param);
  DataOutputFormat::WriteKey("NewPulseArea_Error", outData["area_under3"].param_err);
  DataOutputFormat::WriteKey("NewPulseArea_NDF", outData["area_under3"].ndf);
  DataOutputFormat::WriteKey("NewPulseArea_CHI", outData["area_under3"].chi_square);
  DataOutputFormat::WriteKey("NewPulseArea_PROB", outData["area_under3"].prob);
  DataOutputFormat::WriteKey("NewPulseArea_CHI_NDF", outData["area_under3"].chi_ndf);

  DataOutputFormat::WriteKey("FallTime", outData["fall3"].param);
  DataOutputFormat::WriteKey("FallTime_Error", outData["fall3"].param_err);
  DataOutputFormat::WriteKey("FallTime_NDF", outData["fall3"].ndf);
  DataOutputFormat::WriteKey("FallTime_CHI", outData["fall3"].chi_square);
  DataOutputFormat::WriteKey("FallTime_PROB", outData["fall3"].prob);
  DataOutputFormat::WriteKey("FallTime_CHI_NDF", outData["fall3"].chi_ndf);

  DataOutputFormat::WriteKey("temperature", temperature);
  DataOutputFormat::WriteKey("trigger_bias", trigger_bias);
}

void DataOutputFormat::ParseRawOutputToINI(
    std::string biasVoltage,
    std::map<std::string, FitResult> outData,
    int temperature
)
{
  boost::property_tree::ptree pt;
  try
  {
    boost::property_tree::ini_parser::read_ini("raw_results.ini", pt);
  }
  catch(std::exception &pt_ex )
  {
    std::cerr << pt_ex.what() << std::endl;
  }

  fmt::print("writing {}_{}\n", biasVoltage, temperature);
  for(const auto &odata : outData )
  {
    pt.put(fmt::format("{}_{}.{}", biasVoltage, temperature, odata.first), odata.second.param );
    pt.put(fmt::format("{}_{}.{}_Error", biasVoltage, temperature, odata.first), odata.second.param_err );
    pt.put(fmt::format("{}_{}.{}_NDF", biasVoltage, temperature, odata.first), odata.second.ndf );
    pt.put(fmt::format("{}_{}.{}_CHI", biasVoltage, temperature, odata.first), odata.second.chi_square );
    pt.put(fmt::format("{}_{}.{}_PROB", biasVoltage, temperature, odata.first), odata.second.prob );
    pt.put(fmt::format("{}_{}.{}_CHI_NDF", biasVoltage, temperature, odata.first), odata.second.chi_ndf );
    pt.put(fmt::format("{}_{}.{}_Par2", biasVoltage, temperature, odata.first), odata.second.param2 );
    pt.put(fmt::format("{}_{}.{}_Par2Err", biasVoltage, temperature, odata.first), odata.second.param2_err );
  }

  boost::property_tree::ini_parser::write_ini("raw_results.ini", pt);
}
