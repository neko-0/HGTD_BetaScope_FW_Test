#include "SelfTrAna.h"

//user include files
#include "General/WaveformAna/include/Waveform_Analysis.hpp"
#include "General/WaveformAna/include/general.hpp"

#include <stdlib.h>
#include <boost/thread.hpp>
#include <thread>
#include <TThread.h>
#include <TROOT.h>
#include <mutex>
#include <ROOT/TProcessExecutor.hxx>

//---------------

void SelfTrAna::initialize( )
{
  this->beta_scope.fileIO_Open( this->ifile.c_str() );

  char *check_path = getenv("BETASCOPE_SCRIPTS");
  if(check_path!=NULL)
  {
    std::string beta_scope_path(getenv("BETASCOPE_SCRIPTS"));
    BetaScope_AnaFramework::initialize( beta_scope_path + "/../BetaScope_Ana/SelfTrAna/AnaTemplate/myOwnTree.ini" );
  }
  else
  {
    BetaScope_AnaFramework::initialize("");
  }

  auto br_check = this->beta_scope.setBranch( "TTreeReaderArray<double>", "w2", "w2" );
  //std::cout<<"BR CHECK: "<<br_check<<std::endl;
  br_check = this->beta_scope.buildPrimitiveBranch< std::vector<double> >("w2");
  //std::cout<<"BR CHECK: "<<br_check<<std::endl;
  w2 = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>("w2");

  br_check = this->beta_scope.setBranch( "TTreeReaderArray<double>", "t2", "t2");
  //std::cout<<"BR CHECK: "<<br_check<<std::endl;
  br_check = this->beta_scope.buildPrimitiveBranch< std::vector<double> >("t2");
  //std::cout<<"BR CHECK: "<<br_check<<std::endl;
  t2 = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>("t2");

  br_check = this->beta_scope.setBranch( "TTreeReader<double>", "i_timestamp", "i_timestamp");
  //std::cout<<"BR CHECK: "<<br_check<<std::endl;
  br_check = this->beta_scope.buildPrimitiveBranch<double>("i_timestamp");
  //std::cout<<"BR CHECK: "<<br_check<<std::endl;
  this->timestamp = this->beta_scope.get_oTree_PrimitiveBranch<double>("i_timestamp");

  br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>("rate");
  this->rate = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>("rate");
  //std::cout<<"BR CHECK: "<<br_check<<std::endl;
  br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>("npulses");
  this->npulses = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>("npulses");
  //std::cout<<"BR CHECK: "<<br_check<<std::endl;
  br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>("thresholds");
  //std::cout<<"BR CHECK: "<<br_check<<std::endl;
  this->threshold = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>("thresholds");

  br_check = this->beta_scope.buildPrimitiveBranch<int>("counter");
  std::cout<<"BR CHECK: "<<br_check<<std::endl;
  //br_check = this->beta_scope.buildTH1Branch<TH1D>("counter_histo");

  //this->standAloneHisto_ptr = new TH1D("standAloneHisto_ptr", "standAloneHisto_ptr", 100, 1, 1);

  //std::cout << "END INITIALIZE";

}

//==============================================================================
//==============================================================================
//==============================================================================

void SelfTrAna::loopEvents()
{
  //double *d = this->beta_scope.get<double>("ws0", "vector<double?");
  int count = 0;

  double verScaler = -1.0;
  double horiScaler = 1.0;

  WaveformAnalysis WaveAna;

  int nevents = 0;
  float thresholds[10] = {0.001, 0.002, 0.003, 0.005, 0.007, 0.01, 0.02, 0.03, 0.05, 0.1};

  while( this->beta_scope.get_treeReader()->Next() )
  {
    nevents += 1;
    std::cout<<"EVENT: "<<nevents<<std::endl;
    if(nevents > 2) break;

    std::vector<double> w;
    std::vector<double> t;

    for(int i = 0, max = this->beta_scope.get_iBranch<TTreeReaderArray, double>(Form("w2"))->GetSize(); i<max; i++)
    {
      w.push_back( this->beta_scope.get_iBranch<TTreeReaderArray, double>("w2")->At(i) * verScaler );
      t.push_back( this->beta_scope.get_iBranch<TTreeReaderArray, double>("t2")->At(i) * horiScaler );
      w2->push_back( this->beta_scope.get_iBranch<TTreeReaderArray, double>("w2")->At(i) * verScaler );
      t2->push_back( this->beta_scope.get_iBranch<TTreeReaderArray, double>("t2")->At(i) * horiScaler );
    }

    //timestamp = this->beta_scope.get_iBranch<TTreeReaderValue,double>("i_timestamp");

    float time_window = t[t.size()-1] - t[0];
    //std::cout<< t->size()<<" "<< t->at(0)<<" "<<t->at(1)<<" "<<(t->at(0)-t->at(1))*t->size()<<" "<<t->at(0)+(t->at(0)-t->at(1))*t->size()<<std::endl;
    //Analysis:================================================================

    //WaveAna.Correct_Baseline2(*this->w[0], 0.30);
    for(int th = 0; th < 10; th++){
      float thres = thresholds[th];
      int num_pulses = WaveAna.Get_Number_Of_Multiple_Signals(thres, w, t);
      //std::cout<<"NPEAKS "<<thres<<": "<<num_pulses<<std::endl;
      //std::cout<<"  TIME W "<<time_window<<": "<<num_pulses/time_window<<std::endl;

      threshold->push_back(thres);
      npulses->push_back(num_pulses);
      rate->push_back((num_pulses)/time_window);
    }
    //std::cout<<"POS[0]: "<<this->pmax->at(0)<<" "<<this->tmax->at(0)<<std::endl;
    //std::cout<<"+++++++"<<std::endl;

    *this->beta_scope.get_oTree_PrimitiveBranch<int>("counter") = count;
    count++;

    w.clear();
    t.clear();

    BetaScope_AnaFramework::filldata();

  }
}

//==============================================================================
//==============================================================================
//==============================================================================

void SelfTrAna::_finalize()
{
  //do your own stuffs here
  //this->standAloneHisto.Write( );
  //this->standAloneHisto_ptr->Write();

  BetaScope_AnaFramework::finalize();
}
