#include "ArgoneXrayAna.h"

//user include files
#include "General/WaveformAna/include/Waveform_Analysis.hpp"

#include <boost/thread.hpp>
#include <thread>
#include <TThread.h>
#include <TROOT.h>
#include <mutex>
#include <ROOT/TProcessExecutor.hxx>

//---------------


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



  w[0] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w0");
  w[1] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w1");
  w[2] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w2");
  w[3] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w3");
  w[4] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w4");
  w[5] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w5");
  w[6] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w6");
  w[7] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w7");
  w[8] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w8");
  w[9] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w9");
  w[10] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w10");
  w[11] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w11");
  w[12] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w12");
  w[13] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w13");
  w[14] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w14");
  w[15] = this->beta_scope.get_oTreeBranch<std::vector<double>>("w15");
  t = this->beta_scope.get_oTreeBranch<std::vector<double>>("t");

}

void ArgoneXrayAna::loopEvents()
{
  //double *d = this->beta_scope.get<double>("ws0", "vector<double?");
  int count = 0;

  double verScaler = 1.0;
  double horiScaler = 1.0;

  while( this->beta_scope.treeReader->Next() )
  {
    /*
    for(int i = 0, max = this->beta_scope.iTreeDoubleArrayMap[Form("w0")]->GetSize(); i<max; i++)
    {
      w[0]->push_back( this->beta_scope.iTreeDoubleArrayMap["w0"]->At(i) * verScaler );
      w[1]->push_back( this->beta_scope.iTreeDoubleArrayMap["w1"]->At(i) * verScaler );
      w[2]->push_back( this->beta_scope.iTreeDoubleArrayMap["w2"]->At(i) * verScaler );
      w[3]->push_back( this->beta_scope.iTreeDoubleArrayMap["w3"]->At(i) * verScaler );
      w[4]->push_back( this->beta_scope.iTreeDoubleArrayMap["w4"]->At(i) * verScaler );
      w[5]->push_back( this->beta_scope.iTreeDoubleArrayMap["w5"]->At(i) * verScaler );
      w[6]->push_back( this->beta_scope.iTreeDoubleArrayMap["w6"]->At(i) * verScaler );
      w[7]->push_back( this->beta_scope.iTreeDoubleArrayMap["w7"]->At(i) * verScaler );
      w[8]->push_back( this->beta_scope.iTreeDoubleArrayMap["w8"]->At(i) * verScaler );
      w[9]->push_back( this->beta_scope.iTreeDoubleArrayMap["w9"]->At(i) * verScaler );
      w[10]->push_back( this->beta_scope.iTreeDoubleArrayMap["w10"]->At(i) * verScaler );
      w[11]->push_back( this->beta_scope.iTreeDoubleArrayMap["w11"]->At(i) * verScaler );
      w[12]->push_back( this->beta_scope.iTreeDoubleArrayMap["w12"]->At(i) * verScaler );
      w[13]->push_back( this->beta_scope.iTreeDoubleArrayMap["w13"]->At(i) * verScaler );
      w[14]->push_back( this->beta_scope.iTreeDoubleArrayMap["w14"]->At(i) * verScaler );
      w[15]->push_back( this->beta_scope.iTreeDoubleArrayMap["w15"]->At(i) * verScaler );
      //t->push_back( this->beta_scope.iTreeDoubleArrayMap["t"]->At(i) * horiScaler );
    }
    */

    //std::mutex mu;
    //mu.lock();
    ///*
    int entry = this->beta_scope.treeReader->GetCurrentEntry();
    std::vector<boost::thread*> workers;
    for( int i = 0; i < 15; i++)
    {
      std::string bName(Form("w%i",i));
      workers.push_back( new boost::thread( &BetaScope::copyTTreeReaderArrayToVector<std::vector<double>>, &this->beta_scope, bName, bName, entry) );
      //workers.push_back( new std::thread( ArgoneXrayAna::copyTTreeReaderArrayToVector<std::vector<double>, double>, &ww[i], this->beta_scope.iTreeDoubleArrayMap[Form("w%i",i)] ) );

    }

    for(std::size_t id=0; id < workers.size(); id++ )
    {
      workers[id]->join();
      delete workers[id];
    }
    //*/
    //mu.unlock();

    /*
    TThread::Lock();
    ROOT::TProcessExecutor pool(16);
    // define our method: it will simply return the square of each element of a vector
    auto squares = pool.Map(
      [&]( int k )
      {
        std::string bName = Form("w%i",k);
        for(int i = 0, max = this->beta_scope.iTreeDoubleArrayMap[bName]->GetSize(); i < max; i++)
        {
          this->beta_scope.oTreeVecDoubleMap[bName]->push_back( this->beta_scope.iTreeDoubleArrayMap[bName]->At(i) );
        }
        return 1;
      },
      {0,1,2,3,4,5}
    );
    TThread::Lock();
    */




    //t->push_back( this->beta_scope.iTreeDoubleArrayMap["t"]->At(i) * horiScaler );

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
