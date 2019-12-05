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

void loopHelper(WaveformAnalysis *WaveAna,
  std::vector<double> *w,
  std::vector<double> *t,
  std::vector<double> *pmax,
  std::vector<double> *tmax,
  std::vector<int> *max_indexing,
  std::vector<double> *negPmax,
  std::vector<double> *negTmax,
  std::vector<double> *pulseArea,
  double assistThreshold
)
{
  WaveAna->Correct_Baseline2(*w, 0.30);
  WaveAna->Get_PmaxTmax_Of_Multiple_Singal(assistThreshold, *w, *t, *pmax, *tmax, *max_indexing, 1.0 );
  for(std::size_t vSize=0, maxSize=pmax->size(); vSize<maxSize; vSize++)
  {
    std::pair<double, unsigned int> my_pmax = std::make_pair( pmax->at(vSize), max_indexing->at(vSize) );
    pulseArea->push_back( WaveAna->Find_Pulse_Area(*w, *t, my_pmax) );
  }
  bool baseline_corrected = WaveAna->Correct_Baseline4( *w, *t, *pmax, *tmax );

  if( !baseline_corrected )return;

  WaveAna->Find_Bunch_Negative_Signal_Maximum( *w, *t, *pmax, *tmax, *negPmax, *negTmax );
}

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
  br_check = this->beta_scope.buildPrimitiveBranch< std::vector<double> >("w2");
  w[0] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>("w2");

  br_check = this->beta_scope.setBranch( "TTreeReaderArray<double>", "w3", "w3" );
  br_check = this->beta_scope.buildPrimitiveBranch< std::vector<double> >("w3");
  w[1] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>("w3");

  br_check = this->beta_scope.setBranch( "TTreeReaderArray<double>", "t2", "t2");
  br_check = this->beta_scope.buildPrimitiveBranch< std::vector<double> >("t2");
  t = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>("t2");

  /*
  for( int i =0; i < 16; i++)
  {
    w[i] = this->beta_scope.get_oTreeBranch<std::vector<double>>(Form("w%i",i));
    br_check = this->beta_scope.buildBranch<std::vector<double>>(Form("tmax%i",i));
  }
  t = this->beta_scope.get_oTreeBranch<std::vector<double>>("t");

  for( int i =0; i < 16; i++)
  {
    auto br_check = this->beta_scope.buildBranch<std::vector<double>>(Form("pmax%i",i));
    this->pmax[i] = this->beta_scope.get_oTreeBranch<std::vector<double>>(Form("pmax%i",i));
    this->tmax[i] = this->beta_scope.get_oTreeBranch<std::vector<double>>(Form("tmax%i",i));
    br_check = this->beta_scope.buildBranch<std::vector<int>>(Form("max_indexing%i",i));
    this->max_indexing[i] = this->beta_scope.get_oTreeBranch<std::vector<int>>(Form("max_indexing%i",i));

    br_check = this->beta_scope.buildBranch<std::vector<double>>(Form("pulseArea%i",i));
    this->pulseArea[i] = this->beta_scope.get_oTreeBranch<std::vector<double>>(Form("pulseArea%i",i));
  }
  */
  for( int i =0; i < 10; i++)
  {
    br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>(Form("pmax%i",i));
    br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>(Form("tmax%i",i));
    br_check = this->beta_scope.buildPrimitiveBranch<std::vector<int>>(Form("max_indexing%i",i));
    this->pmax[i] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>(Form("pmax%i",i));
    this->tmax[i] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>(Form("tmax%i",i));
    this->max_indexing[i] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<int>>(Form("max_indexing%i",i));
  }

  br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>("rate");
  br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>("npulses");
  br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>("thresholds");

  this->rate = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>("rate");
  this->npulses = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>("npulses");
  this->threshold = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>("thresholds");
  /*
  for( int i = 2; i <= 3; i++)
  {
    br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>( Form("pmax%i",i) );
    br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>( Form("tmax%i",i) );
    br_check = this->beta_scope.buildPrimitiveBranch<std::vector<int>>( Form("max_indexing%i",i) );
    br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>(Form("pulseArea%i",i) );
    br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>( Form("negPmax%i",i) );
    br_check = this->beta_scope.buildPrimitiveBranch<std::vector<double>>( Form("negTmax%i",i) );


    this->pmax[i] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( Form("pmax%i",i));
    this->tmax[i] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>(Form("tmax%i",i));
    this->max_indexing[i] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<int>>(Form("max_indexing%i",i));
    this->pulseArea[i] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>(Form("pulseArea%i",i));
    this->negPmax[i] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( Form("negPmax%i",i));
    this->negTmax[i] = this->beta_scope.get_oTree_PrimitiveBranch<std::vector<double>>( Form("negTmax%i",i));
  }
  */
  br_check = this->beta_scope.buildPrimitiveBranch<int>("counter");

  this->beta_scope.buildTH1Branch<TH1D>("counter_histo");

  this->beta_scope.buildTH1Branch<TH1D>("counter");
  this->beta_scope.buildTH1Branch<TH2D>("counter2D");

  this->standAloneHisto_ptr = new TH1D("standAloneHisto_ptr", "standAloneHisto_ptr", 100, 1, 1);

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

  while( this->beta_scope.get_treeReader()->Next() )
  {
    nevents += 1;
    if(nevents > 10) break;

    for(int i = 0, max = this->beta_scope.get_iBranch<TTreeReaderArray, double>(Form("w2"))->GetSize(); i<max; i++)
    {
      w[0]->push_back( this->beta_scope.get_iBranch<TTreeReaderArray, double>("w2")->At(i) * verScaler );
      w[1]->push_back( this->beta_scope.get_iBranch<TTreeReaderArray, double>("w3")->At(i) * verScaler );
      t->push_back( this->beta_scope.get_iBranch<TTreeReaderArray, double>("t2")->At(i) * horiScaler );
      //std::cout<<this->beta_scope.get_iBranch<TTreeReaderArray, double>("t2")->At(i) * horiScaler<<" "<<\
      //        this->beta_scope.get_iBranch<TTreeReaderArray, double>("w2")->At(i) * verScaler<<std::endl;
    }


    //std::mutex mu;
    //mu.lock();
    /*
    int entry = this->beta_scope.treeReader->GetCurrentEntry();
    std::vector<boost::thread*> workers;
    for( int i = 0; i < 15; i++)
    {
      std::string bName(Form("w%i",i));
      workers.push_back( new boost::thread( &BetaScope::copyTTreeReaderArrayToVector<std::vector<double>>, &this->beta_scope, bName, bName, entry) );
      //workers.push_back( new std::thread( SelfTrAna::copyTTreeReaderArrayToVector<std::vector<double>, double>, &ww[i], this->beta_scope.iTreeDoubleArrayMap[Form("w%i",i)] ) );

    }

    for(std::size_t id=0; id < workers.size(); id++ )
    {
      workers[id]->join();
      delete workers[id];
    }
    */
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


    //Analysis:================================================================

    //WaveAna.Correct_Baseline2(*this->w[0], 0.30);
    float thresholds[10] = {0.001, 0.003, 0.005, 0.07, 0.01, 0.02, 0.03, 0.04, 0.05, 0.1};
    for(int th = 0; th < 10; th++){
      float thres = thresholds[th];
      WaveAna.Get_PmaxTmax_Of_Multiple_Singal(thres, *this->w[0], *this->t, *this->pmax[th], *this->tmax[th], *this->max_indexing[th], 1.0 );
      std::cout<<"NPEAKS "<<thres<<": "<<this->pmax[th]->size()<<std::endl;

      threshold->push_back(thres);
      npulses->push_back(this->pmax[th]->size());
      rate->push_back((this->pmax[th]->size()-1)/1e-4);
      /*WaveAna.Get_PmaxTmax_Of_Multiple_Singal(0.01, *this->w[0], *this->t, *this->pmax[1], *this->tmax[1], *this->max_indexing[1], 1.0 );
      std::cout<<"NPEAKS 10mV: "<<this->pmax->size()<<std::endl;
      WaveAna.Get_PmaxTmax_Of_Multiple_Singal(0.03, *this->w[0], *this->t, *this->pmax, *this->tmax, *this->max_indexing, 1.0 );
      std::cout<<"NPEAKS 30mV: "<<this->pmax->size()<<std::endl;
      WaveAna.Get_PmaxTmax_Of_Multiple_Singal(0.05, *this->w[0], *this->t, *this->pmax, *this->tmax, *this->max_indexing, 1.0 );
      std::cout<<"NPEAKS 50mV: "<<this->pmax->size()<<std::endl;
      WaveAna.Get_PmaxTmax_Of_Multiple_Singal(0.1, *this->w[0], *this->t, *this->pmax, *this->tmax, *this->max_indexing, 1.0 );
      std::cout<<"NPEAKS 100mV: "<<this->pmax->size()<<std::endl;*/
    }
    //std::cout<<"POS[0]: "<<this->pmax->at(0)<<" "<<this->tmax->at(0)<<std::endl;
    std::cout<<"+++++++"<<std::endl;

    /*
    for( int i = 0; i < 2; i++)
    {
      WaveAna.Correct_Baseline2(*this->w[i], 0.30);
      WaveAna.Get_PmaxTmax_Of_Multiple_Singal(assistThreshold, *this->w[i], *this->t, *this->pmax[i], *this->tmax[i], *this->max_indexing[i], 1.0 );
      for(std::size_t vSize=0, maxSize=this->pmax[i]->size(); vSize<maxSize; vSize++)
      {
        std::pair<double, unsigned int> my_pmax = std::make_pair( this->pmax[i]->at(vSize), this->max_indexing[i]->at(vSize) );
        pulseArea[i]->push_back( WaveAna.Find_Pulse_Area(*this->w[i], *this->t, my_pmax) );
      }
      bool baseline_corrected = WaveAna.Correct_Baseline4( *this->w[i], *this->t, *this->pmax[i], *this->tmax[i] );

      WaveAna.Find_Bunch_Negative_Signal_Maximum( *this->w[i], *this->t, *this->pmax[i], *this->tmax[i], *this->negPmax[i], *this->negTmax[i] );
    }
    */
    /*
    WaveformAnalysis *WaveAna_ptr = &WaveAna;
    std::vector<std::thread*> workers;
    for( int i = 0; i < 16; i++)
    {
      workers.push_back( new std::thread( loopHelper, WaveAna_ptr, this->w[i], this->t, this->pmax[i], this->tmax[i], this->max_indexing[i], this->negPmax[i], this->negTmax[i], this->pulseArea[i], assistThreshold) );
    }

    for(std::size_t id=0; id < workers.size(); id++ )
    {
      workers[id]->join();
      delete workers[id];
    }
    */



    //t->push_back( this->beta_scope.iTreeDoubleArrayMap["t"]->At(i) * horiScaler );
    /*
    TH1D tmp(Form("tmp%i",count), "", 100, 1, 1);
    tmp.Fill(count);
    *this->beta_scope.get_oHisto1D<TH1D>("counter_histo") = tmp;

    TH1D tmp2(Form("tmp2%i",count), "", 100, 1, 1);
    tmp2.Fill(count);
    //*static_cast<TH1_Container<TH1D>*>(this->beta_scope.oTree_TH1_Map["counter"])->get() = tmp2;
    *this->beta_scope.get_oHisto1D<TH1D>("counter") = tmp2;

    TH2D tmp3(Form("tmp3%i",count), "", 100, 0, 5000, 100, 0, 5000);
    tmp3.Fill(count, count);
    *this->beta_scope.get_oHisto1D<TH2D>("counter2D") = tmp3;

    this->standAloneHisto.Fill(count*2.0);
    this->standAloneHisto_ptr->Fill(count*2.0);


    *this->beta_scope.get_oTree_PrimitiveBranch<int>("counter") = count;
    */
    count++;

    BetaScope_AnaFramework::filldata();

    //this->beta_scope.oTree_TH1D_Map["counter_histo"]->Reset();
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
