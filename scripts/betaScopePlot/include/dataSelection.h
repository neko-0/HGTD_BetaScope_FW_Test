#ifndef BETASCOPE_SRC_DATASELECTION_HPP_H_
#define BETASCOPE_SRC_DATASELECTION_HPP_H_

#include <string>
#include <vector>

struct DataSelection {
  std::string cuts;
  std::string sideband1;
  std::string sideband2;

  double sideband_size = 1000.0;

  DataSelection( std::vector<std::string> icut, int dut = 2, int trig = 3);

  DataSelection( std::vector<double> icut, int dut = 2, int trig = 3 );

  void Create_TmaxPmax_Selection( std::vector<std::string> icut, int dut, int trig );

};

#endif // BETASCOPE_SRC_DATASELECTION_HPP_H_
