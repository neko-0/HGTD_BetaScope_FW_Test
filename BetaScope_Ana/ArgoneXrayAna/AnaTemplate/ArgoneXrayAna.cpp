#include "ArgoneXrayAna.h"

void ArgoneXrayAna::initialize( )
{
  this->beta_scope.fileIO_Open( this->ifile.c_str() );
  BetaScope_AnaFramework::initialize( "/home/yuzhan/HGTD_BetaScope_FW_Test/BetaScope_Ana/ArgoneXrayAna/AnaTemplate/myOwnTree.ini" );

  //do your own stuffs here
  for(int ch = 0; ch < 16; ch++)
  {
    auto br_check = simple_readBranch( this->beta_scope, "TTreeReaderArray<double>", Form("w%i",ch), Form("w%i",ch) );

    br_check = makeBranch<std::vector<double>>(this->beta_scope.oTree, Form("w%i", ch ), Form("w%i", ch ), &this->beta_scope.oTreeVecDoubleMap, this->beta_scope.oTreeVecDouble[this->beta_scope.newBranchCounterKeeper], this->beta_scope.newBranchCounterKeeper, &this->beta_scope.oTreeVecDoubleMapIndex, this->beta_scope.newBranchCounterKeeper );
    this->beta_scope.oTreeVecDouble[this->beta_scope.newBranchCounterKeeper-1]->reserve(1000000);

    if(ch==0){ br_check = simple_readBranch( this->beta_scope, "TTreeReaderArray<double>", "t", "t"); }
  }

}

void ArgoneXrayAna::loopEvents()
{
  while( this->beta_scope.treeReader->Next() )
  {
    for(int i = 0; i < this->beta_scope.iTreeDoubleArrayMap[Form("w%i",0)]->GetSize(); i++)
    {
      this->beta_scope.oTreeVecDoubleMap["w0"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w0"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w1"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w1"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w2"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w2"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w3"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w3"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w4"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w4"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w5"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w5"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w6"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w6"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w7"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w7"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w8"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w8"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w9"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w9"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w10"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w10"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w11"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w11"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w12"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w12"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w13"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w13"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w14"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w14"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w15"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w15"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["w16"]->push_back( this->beta_scope.iTreeDoubleArrayMap["w16"]->At(i) );
      this->beta_scope.oTreeVecDoubleMap["Time"]->push_back( this->beta_scope.iTreeDoubleArrayMap["t"]->At(i) );
    }


    BetaScope_AnaFramework::filldata();
  }
}

void ArgoneXrayAna::finalize()
{
  //do your own stuffs here


  BetaScope_AnaFramework::finalize();
}
