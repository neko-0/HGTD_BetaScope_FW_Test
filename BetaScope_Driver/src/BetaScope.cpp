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
double *BetaScope::getD(std::string key){ return this->oTreeDoubleMap[key]; }
int *BetaScope::getI(std::string key){ return this->oTreeIntMap[key]; }


template <>
DataType<double>::type *BetaScope::get_oTreeBranch<double>(std::string branchName){ return this->oTreeDoubleMap[branchName]; }

template <>
DataType<int>::type *BetaScope::get_oTreeBranch<int>(std::string branchName){ return this->oTreeIntMap[branchName]; }

template <>
DataType<std::vector<double>>::type *BetaScope::get_oTreeBranch<std::vector<double>>(std::string branchName){ return this->oTreeVecDoubleMap[branchName]; }

template <>
DataType<std::vector<int>>::type *BetaScope::get_oTreeBranch<std::vector<int>>(std::string branchName){ return this->oTreeVecIntMap[branchName]; }
