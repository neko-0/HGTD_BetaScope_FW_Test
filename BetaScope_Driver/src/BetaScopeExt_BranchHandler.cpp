#include "BetaScope_Driver/include/BetaScopeExt_Class.h"

template <>
bool BetaScope_Ext::buildBranch<double>(std::string branchName ){ return BetaScope::buildPrimitiveBranch<double>(branchName); }

template <>
bool BetaScope_Ext::buildBranch<int>(std::string branchName ){ return BetaScope::buildPrimitiveBranch<int>(branchName); }

template <>
bool BetaScope_Ext::buildBranch<std::vector<double>>(std::string branchName ){ return BetaScope::buildPrimitiveBranch<std::vector<double>>(branchName); }

template <>
bool BetaScope_Ext::buildBranch<std::vector<int>>(std::string branchName ){ return BetaScope::buildPrimitiveBranch<std::vector<int>>(branchName); }

template <>
bool BetaScope_Ext::buildBranch<TH1I>( std::string branchName )
{
  try{
    this->oTree_TH1I[newBranchCounterKeeper] = new TH1I;
    this->oTree_TH1I_Map.insert( std::pair<std::string, TH1I*>(branchName, this->oTree_TH1I[newBranchCounterKeeper]) );
    oTree->Branch( branchName.c_str(), this->oTree_TH1I[newBranchCounterKeeper] );
    //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, newBranchCounterKeeper) );
    //this->reserved_vec_i.push_back( this->newBranchCounterKeeper );
    newBranchCounterKeeper++;
    return true;
  }
  catch(...){
    return false;
  }
}

template <>
bool BetaScope_Ext::buildBranch<TH1F>( std::string branchName )
{
  try{
    this->oTree_TH1F[newBranchCounterKeeper] = new TH1F;
    this->oTree_TH1F_Map.insert( std::pair<std::string, TH1F*>(branchName, this->oTree_TH1F[newBranchCounterKeeper]) );
    oTree->Branch( branchName.c_str(), this->oTree_TH1F[newBranchCounterKeeper] );
    //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, newBranchCounterKeeper) );
    //this->reserved_vec_i.push_back( this->newBranchCounterKeeper );
    newBranchCounterKeeper++;
    return true;
  }
  catch(...){
    return false;
  }
}

template <>
bool BetaScope_Ext::buildBranch<TH1D>( std::string branchName )
{
  try{
    this->oTree_TH1D[newBranchCounterKeeper] = new TH1D;
    this->oTree_TH1D_Map.insert( std::pair<std::string, TH1D*>(branchName, this->oTree_TH1D[newBranchCounterKeeper]) );
    oTree->Branch( branchName.c_str(), this->oTree_TH1D[newBranchCounterKeeper] );
    //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, newBranchCounterKeeper) );
    //this->reserved_vec_i.push_back( this->newBranchCounterKeeper );
    newBranchCounterKeeper++;
    return true;
  }
  catch(...){
    return false;
  }
}

template <>
bool BetaScope_Ext::buildBranch<std::vector<TH1I>>( std::string branchName )
{
  try{
    this->oTree_TH1I_Array[newBranchCounterKeeper] = new std::vector<TH1I>;
    this->oTree_TH1I_Array_Map.insert( std::pair<std::string, std::vector<TH1I>*>(branchName, this->oTree_TH1I_Array[newBranchCounterKeeper]) );
    oTree->Branch( branchName.c_str(), this->oTree_TH1I_Array[newBranchCounterKeeper] );
    //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, newBranchCounterKeeper) );
    //this->reserved_vec_i.push_back( this->newBranchCounterKeeper );
    newBranchCounterKeeper++;
    return true;
  }
  catch(...){
    return false;
  }
}

template <>
bool BetaScope_Ext::buildBranch<std::vector<TH1F>>( std::string branchName )
{
  try{
    this->oTree_TH1F_Array[newBranchCounterKeeper] = new std::vector<TH1F>;
    this->oTree_TH1F_Array_Map.insert( std::pair<std::string, std::vector<TH1F>*>(branchName, this->oTree_TH1F_Array[newBranchCounterKeeper]) );
    oTree->Branch( branchName.c_str(), this->oTree_TH1F_Array[newBranchCounterKeeper] );
    //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, newBranchCounterKeeper) );
    //this->reserved_vec_i.push_back( this->newBranchCounterKeeper );
    newBranchCounterKeeper++;
    return true;
  }
  catch(...){
    return false;
  }
}

template <>
bool BetaScope_Ext::buildBranch<std::vector<TH1D>>( std::string branchName )
{
  try{
    this->oTree_TH1D_Array[newBranchCounterKeeper] = new std::vector<TH1D>;
    this->oTree_TH1D_Array_Map.insert( std::pair<std::string, std::vector<TH1D>*>(branchName, this->oTree_TH1D_Array[newBranchCounterKeeper]) );
    oTree->Branch( branchName.c_str(), this->oTree_TH1D_Array[newBranchCounterKeeper] );
    //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, newBranchCounterKeeper) );
    //this->reserved_vec_i.push_back( this->newBranchCounterKeeper );
    newBranchCounterKeeper++;
    return true;
  }
  catch(...){
    return false;
  }
}


bool BetaScope_Ext::setBranch( std::string typeName, std::string key, std::string branchName)
{
  if( typeName.compare("TTreeReaderArray<double>") ==0 ){ return BetaScope::set_iBranch<TTreeReaderArray,double>( branchName.c_str(), key.c_str()); }
  else if( typeName.compare("TTreeReaderArray<int>") ==0 ){ return BetaScope::set_iBranch<TTreeReaderArray,int>( branchName.c_str(), key.c_str()); }
  else if( typeName.compare("TTreeReaderValue<double>") ==0 ){ return BetaScope::set_iBranch<TTreeReaderValue,double>( branchName.c_str(), key.c_str()); }
  else if( typeName.compare("TTreeReaderValue<int>") ==0 ){ return BetaScope::set_iBranch<TTreeReaderValue,int>( branchName.c_str(), key.c_str()); }
  else if( typeName.compare("TTreeReaderArray<TH1I>") == 0 )
  {
    this->iTree_TH1I_Array[iTreeBranchCounter] = new TTreeReaderArray<TH1I>( *this->treeReader, branchName.c_str() );
    this->iTree_TH1I_Array_Map.insert( std::pair<std::string, TTreeReaderArray<TH1I>*>(key, this->iTree_TH1I_Array[iTreeBranchCounter]) );
    //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->iTreeBranchCounter) );
    iTreeBranchCounter+=1;
    return true;
  }
  else if( typeName.compare("TTreeReaderArray<TH1F>") == 0 )
  {
    this->iTree_TH1F_Array[iTreeBranchCounter] = new TTreeReaderArray<TH1F>( *this->treeReader, branchName.c_str() );
    this->iTree_TH1F_Array_Map.insert( std::pair<std::string, TTreeReaderArray<TH1F>*>(key, this->iTree_TH1F_Array[iTreeBranchCounter]) );
    //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->iTreeBranchCounter) );
    iTreeBranchCounter+=1;
    return true;
  }
  else if( typeName.compare("TTreeReaderArray<TH1D>") == 0 )
  {
    this->iTree_TH1D_Array[iTreeBranchCounter] = new TTreeReaderArray<TH1D>( *this->treeReader, branchName.c_str() );
    this->iTree_TH1D_Array_Map.insert( std::pair<std::string, TTreeReaderArray<TH1D>*>(key, this->iTree_TH1D_Array[iTreeBranchCounter]) );
    //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->iTreeBranchCounter) );
    iTreeBranchCounter+=1;
    return true;
  }
  else if( typeName.compare("TTreeReaderValue<TH1I>") == 0 )
  {
    this->iTree_TH1I[iTreeBranchCounter] = new TTreeReaderValue<TH1I>( *this->treeReader, branchName.c_str() );
    this->iTree_TH1I_Map.insert( std::pair<std::string, TTreeReaderValue<TH1I>*>(key, this->iTree_TH1I[iTreeBranchCounter]) );
    //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->iTreeBranchCounter) );
    iTreeBranchCounter+=1;
    return true;
  }
  else if( typeName.compare("TTreeReaderValue<TH1F>") == 0 )
  {
    this->iTree_TH1F[iTreeBranchCounter] = new TTreeReaderValue<TH1F>( *this->treeReader, branchName.c_str() );
    this->iTree_TH1F_Map.insert( std::pair<std::string, TTreeReaderValue<TH1F>*>(key, this->iTree_TH1F[iTreeBranchCounter]) );
    //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->iTreeBranchCounter) );
    iTreeBranchCounter+=1;
    return true;
  }
  else if( typeName.compare("TTreeReaderValue<TH1D>") == 0 )
  {
    this->iTree_TH1D[iTreeBranchCounter] = new TTreeReaderValue<TH1D>( *this->treeReader, branchName.c_str() );
    this->iTree_TH1D_Map.insert( std::pair<std::string, TTreeReaderValue<TH1D>*>(key, this->iTree_TH1D[iTreeBranchCounter]) );
    //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->iTreeBranchCounter) );
    iTreeBranchCounter+=1;
    return true;
  }
  else{return false;}
}
