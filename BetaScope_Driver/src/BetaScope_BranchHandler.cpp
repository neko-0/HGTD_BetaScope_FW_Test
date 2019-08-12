#include "BetaScope_Driver/include/BetaScope_Class.h"

#include "Colorful_Cout/include/Colorful_Cout.h"

#include <string>
#include <iostream>
#include <utility>      // std::pair, std::make_pair

template <>
bool BetaScope::buildBranch<std::vector<double>>( std::string branchName )
{
  try{
    this->oTreeVecDouble[this->newBranchCounterKeeper] = new std::vector<double>;
    this->oTreeVecDoubleMap.insert( std::pair<std::string, std::vector<double>*>(branchName, this->oTreeVecDouble[this->newBranchCounterKeeper]) );
    this->oTree->Branch( branchName.c_str(), this->oTreeVecDouble[this->newBranchCounterKeeper] );
    this->oTreeVecDoubleMapIndex.insert( std::pair<std::string , int>(branchName, this->newBranchCounterKeeper) );
    this->reserved_vec_d.push_back( this->newBranchCounterKeeper );
    this->newBranchCounterKeeper ++;
    return true;
  }
  catch(...){
    return false;
  }
}

template <>
bool BetaScope::buildBranch<std::vector<int>>( std::string branchName )
{
  try{
    this->oTreeVecInt[this->newBranchCounterKeeper] = new std::vector<int>;
    this->oTreeVecIntMap.insert( std::pair<std::string, std::vector<int>*>(branchName, this->oTreeVecInt[this->newBranchCounterKeeper]) );
    this->oTree->Branch( branchName.c_str(), this->oTreeVecInt[this->newBranchCounterKeeper] );
    this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, this->newBranchCounterKeeper) );
    this->reserved_vec_i.push_back( this->newBranchCounterKeeper );
    this->newBranchCounterKeeper ++;
    return true;
  }
  catch(...){
    return false;
  }
}
