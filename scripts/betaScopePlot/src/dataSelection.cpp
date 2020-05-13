#include "betaScopePlot/include/dataSelection.h"

#include <fmt/format.h>
#include <stdexcept>

#include <TCut.h>
#include <TString.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>


DataSelection::DataSelection(std::vector<std::string> icut, int dut, int trig)
{
  std::string Dut_tcut = fmt::format("tmax{}[0] - cfd{}[20]", dut, trig);
  std::string Trig_tcut = fmt::format("tmax{}[0] - cfd{}[20]", trig, trig);

  std::string Dut_pcut = fmt::format("pmax{}[0]", dut);
  std::string Trig_pcut = fmt::format("pmax{}[0]", trig);

  if( icut.size() == 8)
  {
    DataSelection::Create_TmaxPmax_Selection(icut, dut, trig);
  }
  else if( icut.size() == 9)
  {
    std::cout << "detect 9 elements in selection\n";
    DataSelection::Create_TmaxPmax_Selection(std::vector<std::string>(icut.begin(), icut.begin()+8), dut, trig);
    this->cuts = fmt::format("{} && {}", this->cuts, icut.at(8));
  }
  else if( icut.size() == 1 )
  {
    this->cuts = icut.at(0);
  }
  else
  {
    std::cout << "invalid cut or not yet implemented. Using empty cut" << std::endl;
    this->cuts = " ";
  }
}

DataSelection::DataSelection(std::vector<double> icut, int dut, int trig)
{
  const char *Dut_tcut = Form("tmax%d[0] - cfd%d[20]", dut, trig);
  const char *Trig_tcut = Form("tmax%d[0] - cfd%d[20]", trig, trig);

  const char *Dut_pcut = Form("pmax%d[0]", dut);
  const char *Trig_pcut = Form("pmax%d[0]", trig);

  if (icut.size() == 8)
  {
    double dutTmaxMin = icut.at(0);
    double dutTmaxMax = icut.at(1);
    double dutPmaxMin = icut.at(2);
    double dutPmaxMax = icut.at(3);
    double trigTmaxMin = icut.at(4);
    double trigTmaxMax = icut.at(5);
    double trigPmaxMin = icut.at(6);
    double trigPmaxMax = icut.at(7);

    this->cuts = fmt::format(
      " {} > {} && {} < {} && "
      " {} > {} && {} < {} && "
      " {} > {} && {} < {} && "
      " {} > {} && {} < {}  ",
      Dut_tcut, dutTmaxMin, Dut_tcut, dutTmaxMax,
      Dut_pcut, dutPmaxMin, Dut_pcut, dutPmaxMax,
      Trig_tcut, trigTmaxMin, Trig_tcut, trigTmaxMax,
      Trig_pcut, trigPmaxMin, Trig_pcut, trigPmaxMax
    );

    this->sideband1 = fmt::format(
      " {} > {} && {} < {} && "
      " {} > {} && {} < {} ",
      Dut_tcut, dutTmaxMin - 1000 - this->sideband_size,
      Dut_tcut, dutTmaxMin - 1000.0, Dut_pcut, 0.0, Dut_pcut, dutPmaxMax
    );

    this->sideband2 = fmt::format(
      " %s > %f && %s < %f && "
      " %s > %f && %s < %f ",
      Dut_tcut, dutTmaxMax + 1000.0, Dut_tcut,
      dutTmaxMax + 1000.0 + this->sideband_size,
      Dut_pcut, 0.0, Dut_pcut, dutPmaxMax
    );
  }
}

void DataSelection::Create_TmaxPmax_Selection( std::vector<std::string> icut, int dut, int trig )
{
  std::string Dut_tcut = fmt::format("tmax{}[0] - cfd{}[20]", dut, trig);
  std::string Trig_tcut = fmt::format("tmax{}[0] - cfd{}[20]", trig, trig);

  std::string Dut_pcut = fmt::format("pmax{}[0]", dut);
  std::string Trig_pcut = fmt::format("pmax{}[0]", trig);

  if (icut.size() == 8)
  {
    double dutTmaxMin = std::stod(icut.at(0));
    double dutTmaxMax = std::stod(icut.at(1));
    double dutPmaxMin = std::stod(icut.at(2));
    double dutPmaxMax = std::stod(icut.at(3));
    double trigTmaxMin = std::stod(icut.at(4));
    double trigTmaxMax = std::stod(icut.at(5));
    double trigPmaxMin = std::stod(icut.at(6));
    double trigPmaxMax = std::stod(icut.at(7));

    this->cuts = fmt::format(
      " {} > {} && {} < {} && "
      " {} > {} && {} < {} && "
      " {} > {} && {} < {} && "
      " {} > {} && {} < {} ",
      Dut_tcut, dutTmaxMin, Dut_tcut, dutTmaxMax,
      Dut_pcut, dutPmaxMin, Dut_pcut, dutPmaxMax,
      Trig_tcut, trigTmaxMin, Trig_tcut, trigTmaxMax,
      Trig_pcut, trigPmaxMin, Trig_pcut, trigPmaxMax
    );


    this->sideband1 = fmt::format(
      " {} > {} && {} < {} && "
      " {} > {} && {} < {} ",
      Dut_tcut, dutTmaxMin - 1000 - this->sideband_size,
      Dut_tcut, dutTmaxMin - 1000.0, Dut_pcut, 0.0,
      Dut_pcut, dutPmaxMax
    );

    this->sideband2 = fmt::format(
      " {} > {} && {} < {} && "
      " {} > {} && {} < {} ",
      Dut_tcut, dutTmaxMax + 1000.0, Dut_tcut,
      dutTmaxMax + 1000.0 + this->sideband_size,
      Dut_pcut, 0.0, Dut_pcut, dutPmaxMax
    );
  }
  else
  {
    throw std::invalid_argument( "icut size need to be 8." );
  }
}
