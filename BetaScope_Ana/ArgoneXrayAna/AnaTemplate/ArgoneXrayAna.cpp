#include "ArgoneXrayAna.h"

void ArgoneXrayAna::initialize( )
{
  this->beta_scope.fileIO_Open( this->ifile.c_str() );
  BetaScope_AnaFramework::initialize( "/home/yuzhan/HGTD_BetaScope_FW_Test/BetaScope_Ana/ArgoneXrayAna/AnaTemplate/myOwnTree.ini" );

  //do your own stuffs here
  for(int ch = 0; ch < 16; ch++)
  {
    auto br_check = this->beta_scope.setBranch( "TTreeReaderArray<double>", Form("w%i",ch), Form("w%i",ch) );

    br_check = this->beta_scope.buildBranch< std::vector<double> >(Form("w%i", ch ));
    //this->beta_scope.oTreeVecDouble[this->beta_scope.newBranchCounterKeeper-1]->reserve(1000000);

    if(ch==0){ br_check = this->beta_scope.setBranch( "TTreeReaderArray<double>", "t", "t"); }
  }
  //auto br_check = makeBranch<std::vector<double>>(this->beta_scope.oTree, "t", "t", &this->beta_scope.oTreeVecDoubleMap, this->beta_scope.oTreeVecDouble[this->beta_scope.newBranchCounterKeeper], this->beta_scope.newBranchCounterKeeper, &this->beta_scope.oTreeVecDoubleMapIndex, this->beta_scope.newBranchCounterKeeper );
  //this->beta_scope.oTreeVecDouble[this->beta_scope.newBranchCounterKeeper-1]->reserve(1000000);
  auto br_check = this->beta_scope.buildBranch< std::vector<double> >("t");
  br_check = this->beta_scope.buildBranch<int>("counter");



  w[0] = this->beta_scope.getDV("w0");
  w[1] = this->beta_scope.getDV("w1");
  w[2] = this->beta_scope.getDV("w2");
  w[3] = this->beta_scope.getDV("w3");
  w[4] = this->beta_scope.getDV("w4");
  w[5] = this->beta_scope.getDV("w5");
  w[6] = this->beta_scope.getDV("w6");
  w[7] = this->beta_scope.getDV("w7");
  w[8] = this->beta_scope.getDV("w8");
  w[9] = this->beta_scope.getDV("w9");
  w[10] = this->beta_scope.getDV("w10");
  w[11] = this->beta_scope.getDV("w11");
  w[12] = this->beta_scope.getDV("w12");
  w[13] = this->beta_scope.getDV("w13");
  w[14] = this->beta_scope.getDV("w14");
  w[15] = this->beta_scope.getDV("w15");
  t = this->beta_scope.getDV("t");

}

void ArgoneXrayAna::loopEvents()
{
  //double *d = this->beta_scope.get<double>("ws0", "vector<double?");
  int count = 0;

  while( this->beta_scope.treeReader->Next() )
  {
    for(int i = 0; i < this->beta_scope.iTreeDoubleArrayMap[Form("w0")]->GetSize(); i++)
    {
      w[0]->push_back( this->beta_scope.iTreeDoubleArrayMap["w0"]->At(i) );
      w[1]->push_back( this->beta_scope.iTreeDoubleArrayMap["w1"]->At(i) );
      w[2]->push_back( this->beta_scope.iTreeDoubleArrayMap["w2"]->At(i) );
      w[3]->push_back( this->beta_scope.iTreeDoubleArrayMap["w3"]->At(i) );
      w[4]->push_back( this->beta_scope.iTreeDoubleArrayMap["w4"]->At(i) );
      w[5]->push_back( this->beta_scope.iTreeDoubleArrayMap["w5"]->At(i) );
      w[6]->push_back( this->beta_scope.iTreeDoubleArrayMap["w6"]->At(i) );
      w[7]->push_back( this->beta_scope.iTreeDoubleArrayMap["w7"]->At(i) );
      w[8]->push_back( this->beta_scope.iTreeDoubleArrayMap["w8"]->At(i) );
      w[9]->push_back( this->beta_scope.iTreeDoubleArrayMap["w9"]->At(i) );
      w[10]->push_back( this->beta_scope.iTreeDoubleArrayMap["w10"]->At(i) );
      w[11]->push_back( this->beta_scope.iTreeDoubleArrayMap["w11"]->At(i) );
      w[12]->push_back( this->beta_scope.iTreeDoubleArrayMap["w12"]->At(i) );
      w[13]->push_back( this->beta_scope.iTreeDoubleArrayMap["w13"]->At(i) );
      w[14]->push_back( this->beta_scope.iTreeDoubleArrayMap["w14"]->At(i) );
      w[15]->push_back( this->beta_scope.iTreeDoubleArrayMap["w15"]->At(i) );
      t->push_back( this->beta_scope.iTreeDoubleArrayMap["t"]->At(i) );
    }

    *this->beta_scope.oTreeIntMap["counter"] = count;
    count++;


    BetaScope_AnaFramework::filldata();
  }
}

void ArgoneXrayAna::finalize()
{
  //do your own stuffs here


  BetaScope_AnaFramework::finalize();
}
