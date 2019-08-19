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


//template <typename o_type, typename i_type>
template <>
//void ArgoneXrayAna::copyTTreeReaderArrayToVector( o_type *output_v, TTreeReaderArray<i_type> *input_v){
void BetaScope::copyTTreeReaderArrayToVector<std::vector<double>>( std::string oBranchName, std::string iBranchName, int entry){
//void copyTTreeReaderArrayToVector( o_type *output_v, TTreeReaderArray<i_type> *input_v){
  //std::cout<< this->beta_scope.iTreeDoubleArrayMap[branchName]->GetSize() << std::endl;
  //std::cout<< this->iTreeDoubleArrayMap[iBranchName]->GetSize() << std::endl;
  //TThread::Lock();
  //TTreeReaderArray<double> *mylocalArray = this->iTreeDoubleArrayMap[iBranchName];
  TTreeReader *mylocalReader =  new TTreeReader( "wfm", this->iFile );
  TTreeReaderArray<double> *mylocalArray = new TTreeReaderArray<double>( *mylocalReader, iBranchName.c_str());
  mylocalReader->SetEntry(entry);
  //std::cout << "local reader " << entry  << std::endl;
  for(int i = 0, max = mylocalArray->GetSize(); i < max; i++)
  {
    //std::cout<< i << std::endl;
    //std::cout << this->iTreeDoubleArrayMap[iBranchName]->At(i) << std::endl;
    this->oTreeVecDoubleMap[oBranchName]->push_back( mylocalArray->At(i) );//this->iTreeDoubleArrayMap[iBranchName]->At(i) );
  }
  //TThread::UnLock();
  delete mylocalArray;
  delete mylocalReader;
}
