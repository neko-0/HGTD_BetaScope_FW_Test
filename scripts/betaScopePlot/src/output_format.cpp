#include "betaScopePlot/include/output_format.h"

#include <utility>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

void DataOutputFormat::CreateBetaScopeOutputFile(
    std::string biasVoltage,
    std::map<std::string, FitResult> outData,
    int temperature,
    int trigger_bias
)
{
  std::vector<std::pair<std::string, std::string>> output_name_ref;
  output_name_ref.push_back( std::make_pair<std::string,std::string>("PulseArea", "area_zero") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("Pmax", "pmax") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("RMS", "rms") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("Rise_Time", "rise") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("dvdt", "dvdt") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("FWHM", "fwhm") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("jitter_20", "jitter") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("NewPulseArea", "area_under") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("FallTime", "fall") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("DeltaT_Res50", "res50") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("DeltaT_Res20", "res20") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("DeltaT_resTmax", "resTmax") );
  output_name_ref.push_back( std::make_pair<std::string,std::string>("DeltaT_resZeroCross", "resZeroCross") );


  std::string dutHeader = "DUT," + biasVoltage + "," + std::to_string(temperature);
  DataOutputFormat::WriteHeader(dutHeader);

  for(auto &pair : output_name_ref)
  {
    try
    {
      auto &my_outData = outData.at(fmt::format("{}2", pair.second));
      DataOutputFormat::WriteKey( pair.first, my_outData.param);
      DataOutputFormat::WriteKey( fmt::format("{}_Error", pair.first), my_outData.param_err);
      DataOutputFormat::WriteKey( fmt::format("{}_NDF", pair.first), my_outData.ndf);
      DataOutputFormat::WriteKey( fmt::format("{}_CHI", pair.first), my_outData.chi_square);
      DataOutputFormat::WriteKey( fmt::format("{}_PROB", pair.first), my_outData.prob);
      DataOutputFormat::WriteKey( fmt::format("{}_CHI_NDF", pair.first), my_outData.chi_ndf);
      DataOutputFormat::WriteKey( fmt::format("{}_Param2", pair.first), my_outData.param2);
      DataOutputFormat::WriteKey( fmt::format("{}_Param2_Error", pair.first), my_outData.param2_err);
    }
    catch(...)
    {
      fmt::print("cannot write DUT {} {}\n", pair.first, pair.second);
    }
  }

  DataOutputFormat::WriteKey("temperature", temperature);
  DataOutputFormat::WriteKey("trigger_bias", trigger_bias);

  std::string triggerHeader = "Trig," + biasVoltage + "," + std::to_string(temperature);
  DataOutputFormat::WriteHeader(triggerHeader);

  for(auto &pair : output_name_ref)
  {
    try
    {
      auto &my_outData = outData.at(fmt::format("{}3", pair.second));
      DataOutputFormat::WriteKey( pair.first, my_outData.param);
      DataOutputFormat::WriteKey( fmt::format("{}_Error", pair.first), my_outData.param_err);
      DataOutputFormat::WriteKey( fmt::format("{}_NDF", pair.first), my_outData.ndf);
      DataOutputFormat::WriteKey( fmt::format("{}_CHI", pair.first), my_outData.chi_square);
      DataOutputFormat::WriteKey( fmt::format("{}_PROB", pair.first), my_outData.prob);
      DataOutputFormat::WriteKey( fmt::format("{}_CHI_NDF", pair.first), my_outData.chi_ndf);
      DataOutputFormat::WriteKey( fmt::format("{}_Par2", pair.first), my_outData.param2);
      DataOutputFormat::WriteKey( fmt::format("{}_Par2Err", pair.first), my_outData.param2_err);
    }
    catch(...)
    {
      fmt::print("cannot write Trig {} {}\n", pair.first, pair.second);
    }
  }

  DataOutputFormat::WriteKey("temperature", temperature);
  DataOutputFormat::WriteKey("trigger_bias", trigger_bias);
}

/*
Automaticlly parse outData into a ini file.
*/
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

  fmt::print("writing {},{}\n", biasVoltage, temperature);
  for(const auto &odata : outData )
  {
    pt.put(fmt::format("{},{}.{}", biasVoltage, temperature, odata.first), odata.second.param );
    pt.put(fmt::format("{},{}.{}_Error", biasVoltage, temperature, odata.first), odata.second.param_err );
    pt.put(fmt::format("{},{}.{}_NDF", biasVoltage, temperature, odata.first), odata.second.ndf );
    pt.put(fmt::format("{},{}.{}_CHI", biasVoltage, temperature, odata.first), odata.second.chi_square );
    pt.put(fmt::format("{},{}.{}_PROB", biasVoltage, temperature, odata.first), odata.second.prob );
    pt.put(fmt::format("{},{}.{}_CHI_NDF", biasVoltage, temperature, odata.first), odata.second.chi_ndf );
    pt.put(fmt::format("{},{}.{}_Par2", biasVoltage, temperature, odata.first), odata.second.param2 );
    pt.put(fmt::format("{},{}.{}_Par2Err", biasVoltage, temperature, odata.first), odata.second.param2_err );
  }

  boost::property_tree::ini_parser::write_ini("raw_results.ini", pt);
}
