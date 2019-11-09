#ifndef DATA_OUTPUT_FORMAT_HPP
#define DATA_OUTPUT_FORMAT_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include "fitter.h"

class DataOutputFormat{

  std::ofstream ofile;

  void WriteHeader(std::string header){
    this->ofile << "[" << header << "]\n";
  };

  void WriteKey(std::string key, double value){
    this->ofile << key << " = " << value << "\n";
  };

  public:
    DataOutputFormat( std::string fileName = "_results.ini" ){
      this->ofile.open(fileName, std::ios::app);
    };

    void CreateBetaScopeOutputFile(
      std::string biasVoltage,
      std::map<std::string, FitResult> outData,
      int temperatur = 20,
      int trigger_bias = 390
    );

};



#endif //DATA_OUTPUT_FORMAT_HPP
