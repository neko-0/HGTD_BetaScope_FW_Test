#include "BetaScope_Class.h"


template <typename dtype>
bool BetaScope::buildPrimitiveBranch( std::string branchName )
{
  std::string function_name = "BetaScope::buildPrimitiveBranch";
  try{
    this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter] = new PrimitiveDataType_Container<dtype>();
    this->oTreePrimitiveBranchesMap.insert( std::pair<std::string, PrimitiveDataType_BaseContainer*>( branchName, this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter] ) );
    oTree->Branch( branchName.c_str(), static_cast<PrimitiveDataType_Container<dtype>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter])->get() );
    this->oTreePrimitiveBranchesMapIndex.insert( std::pair<std::string , int>(branchName, this->oTreePrimitiveBranchCounter) );

    if( is_vector<dtype>::value )
    {
      this->oTreeSTLVectorReservedIndex.push_back( this->newBranchCountKeeper );

      typedef typename is_vector<dtype>::T v;
      ///*
      if( std::is_same< v, std::vector<int>>::value )
      {
        ColorCout::Msg(function_name, "Branch:" + branchName + " is std::vector<int>, Handle buffer internally" );
        this->oTreeSTLVecotr_Int_keeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<int>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter])->get() );
        this->oTree_STLVecotrKeeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<int>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter]) );
      }
      if( std::is_same< v, std::vector<double>>::value )
      {
        ColorCout::Msg(function_name, "Branch:" + branchName + " is std::vector<double>, Handle buffer internally" );
        this->oTreeSTLVecotr_Double_keeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<double>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter])->get() );
        this->oTree_STLVecotrKeeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<double>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter]) );
      }
      if( std::is_same< v, std::vector<float>>::value)
      {
        ColorCout::Msg(function_name, "Branch:" + branchName + " is std::vector<float>, Handle buffer internally" );
        this->oTreeSTLVecotr_Float_keeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<float>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter])->get() );
        this->oTree_STLVecotrKeeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<float>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter]) );
      }
      if( std::is_same< v, std::vector<bool>>::value)
      {
        ColorCout::Msg(function_name, "Branch:" + branchName + " is std::vector<bool>, Handle buffer internally" );
        this->oTreeSTLVecotr_Bool_keeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<bool>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter])->get() );
        this->oTree_STLVecotrKeeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<bool>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter]) );
      }
      if( std::is_same< v, std::vector<char>>::value)
      {
        ColorCout::Msg(function_name, "Branch:" + branchName + " is std::vector<char>, Handle buffer internally" );
        this->oTreeSTLVecotr_Char_keeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<char>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter])->get() );
        this->oTree_STLVecotrKeeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<char>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter]) );
      }
      //*/
    }
    else{
      ColorCout::Msg(function_name, "Branch:" + branchName + " is NOT std::vector. No action is needed." );
    }

    this->oTreePrimitiveBranchCounter++;
    return true;
  }
  catch(...){
    return false;
  }
}

template <typename dtype>
typename DataType<dtype>::type *BetaScope::get_oTree_PrimitiveBranch( std::string branchName )
{
  return static_cast<PrimitiveDataType_Container<dtype>*>(this->oTreePrimitiveBranchesMap[branchName])->get();
}
