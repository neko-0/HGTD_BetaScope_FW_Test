#ifndef BETASCOPE_SRC_DATASELECTION_HPP_H_
#define BETASCOPE_SRC_DATASELECTION_HPP_H_

#include <TCut.h>
#include <string>
#include <vector>

struct DataSelection {
  TCut cuts;
  TCut sideband1;
  TCut sideband2;

  double sideband_size = 1000.0;

  DataSelection(std::vector<std::string> icut, int dut = 2, int trig = 3);

  DataSelection(std::vector<double> icut, int dut = 2, int trig = 3);

  /*DataSelection(
    double dutTmaxMin,
    double dutTmaxMax,
    double dutPmaxMin,
    double dutPmaxMax,
    double trigTmaxMin,
    double trigTmaxMax,
    double trigPmaxMin,
    double trigPmaxMax,
    const char* extraCut,
    int dut = 2,
    int trig = 3
  );

  DataSelection(
    const char* customCut
  );*/
};

#endif // BETASCOPE_SRC_DATASELECTION_HPP_H_
