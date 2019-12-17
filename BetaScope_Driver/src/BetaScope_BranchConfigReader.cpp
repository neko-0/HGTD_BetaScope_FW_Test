#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include <tuple>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

std::vector<
  std::tuple<
    int, std::string, std::string, std::string
  >
>
  BetaScope::BranchConfigReader( std::string configName )
  {
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(configName, pt);

    std::vector<std::tuple<int, std::string, std::string, std::string>> output;

    for( auto section : pt)
    {
      output.push_back(
        std::make_tuple(
          pt.get<int>(section.first+".mode"),
          pt.get<std::string>(section.first+".type"),
          pt.get<std::string>(section.first+".key"),
          pt.get<std::string>(section.first+".branch_name")
        )
      );
    }

    return output;
  }
