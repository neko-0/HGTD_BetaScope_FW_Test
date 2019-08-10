#include "BetaScope_Driver/include/BetaScope_Class.h"

#include "Colorful_Cout/include/Colorful_Cout.h"

#include <string>
#include <iostream>
#include <map>

template <typename dataType>
dataType* BetaScope::get(std::string key, std::string dtype){
  if(dtype.compare("vector<double>")==0 )
  {
    return NULL;//&this->oTreeVecDoubleMap[key];
  }
  else if( dtype.compare("vector<int>")==0 )
  {
    return NULL;//&this->oTreeVecIntMap[key];
  }
  else
  {
      return NULL;
  }
};

std::vector<double> *BetaScope::getDV(std::string key){ return this->oTreeVecDoubleMap[key]; }
std::vector<int> *BetaScope::getIV(std::string key){ return this->oTreeVecIntMap[key]; }
