#include "ArgoneXrayAna.h"

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


void ArgoneXrayAna::initialize( )
{
  this->beta_scope.fileIO_Open( this->ifile.c_str() );

  char *check_path = getenv("BETASCOPE_SCRIPTS");
  if(check_path!=NULL)
  {
    std::string beta_scope_path(getenv("BETASCOPE_SCRIPTS"));
    BetaScope_AnaFramework::initialize( beta_scope_path + "/../BetaScope_Ana/ArgoneXrayAna/AnaTemplate/myOwnTree.ini" );
  }
  else
  {
    BetaScope_AnaFramework::initialize("");
  }

  //do your own stuffs here

  hitpos = new TH1F("hitpos", "hitpos", 16,0,16);

  for(int ch = 0; ch < 16; ch++)
  {
    this->beta_scope.setBranch( "TTreeReaderArray<double>", Form("w%i",ch), Form("w%i",ch) );
    //this->beta_scope.oTreeVecDouble[this->beta_scope.newBranchCounterKeeper-1]->reserve(1000000);
    if(ch==0){ this->beta_scope.setBranch( "TTreeReaderArray<double>", "t", "t"); }
  }

  //this->beta_scope.oTreeVecDouble[this->beta_scope.newBranchCounterKeeper-1]->reserve(1000000);
  this->beta_scope.buildBranch< std::vector<double> >("t");
  this->beta_scope.buildBranch<int>("counter");
  t = this->beta_scope.get_oTreeBranch<std::vector<double>>("t");

  for( int i =0; i < 16; i++)
  {
    this->beta_scope.buildBranch< std::vector<double> >(Form("w%i", i ));
    w[i] = this->beta_scope.get_oTreeBranch<std::vector<double>>(Form("w%i",i));

    this->beta_scope.buildBranch<std::vector<double>>(Form("tmax%i",i));
    this->beta_scope.buildBranch<std::vector<double>>(Form("pmax%i",i));
    this->beta_scope.buildBranch<std::vector<double>>(Form("rms%i",i));

    this->pmax[i] = this->beta_scope.get_oTreeBranch<std::vector<double>>(Form("pmax%i",i));
    this->tmax[i] = this->beta_scope.get_oTreeBranch<std::vector<double>>(Form("tmax%i",i));
    this->rms[i] = this->beta_scope.get_oTreeBranch<std::vector<double>>(Form("rms%i",i));

    this->beta_scope.buildBranch<std::vector<int>>(Form("max_indexing%i",i));
    this->max_indexing[i] = this->beta_scope.get_oTreeBranch<std::vector<int>>(Form("max_indexing%i",i));

    this->beta_scope.buildBranch<std::vector<double>>(Form("pulseArea%i",i));
    this->pulseArea[i] = this->beta_scope.get_oTreeBranch<std::vector<double>>(Form("pulseArea%i",i));

    this->beta_scope.buildBranch<std::vector<int>>(Form("found_peaks%i",i));
    this->found_peaks[i] = this->beta_scope.get_oTreeBranch<std::vector<int>>(Form("found_peaks%i",i));

    this->beta_scope.buildBranch<std::vector<int>>(Form("separated%i",i));
    this->separated[i] = this->beta_scope.get_oTreeBranch<std::vector<int>>(Form("separated%i",i));

    //this->beta_scope.buildBranch<double>(Form("first_threshold%i",i));
    //this->first_threshold[i] = this->beta_scope.get_oTreeBranch<double>(Form("first_threshold%i",i));
  }

  //this->beta_scope.buildBranch<TH1D>("counter_histo");
  //this->beta_scope.buildTH1Branch<TH1D>("counter");
  //this->standAloneHisto_ptr = new TH1D("standAloneHisto_ptr", "standAloneHisto_ptr", 100, 1, 1);

}

//==============================================================================
//==============================================================================
//==============================================================================

void ArgoneXrayAna::loopEvents()
{
  //double *d = this->beta_scope.get<double>("ws0", "vector<double?");
  int count = 0;

  double verScaler = -1.0;
  double horiScaler = 1.0;

  WaveformAnalysis WaveAna;
  double assistThreshold = 100.0;

  while( this->beta_scope.treeReader->Next() )
  {
    ///*
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
      t->push_back( this->beta_scope.iTreeDoubleArrayMap["t"]->At(i) * horiScaler );
    }

    //Analysis:

    for( int i =0; i < 16; i++)
    {
      WaveAna.Correct_Baseline2(*this->w[i], 0.30);
      double this_rms = WaveAna.Find_Noise(*this->w[i], 80);
      rms[i]->push_back(this_rms);
      //std::cout<<"\nEvent "<<count<<" Channel "<<i<<std::endl;
      this->found_peaks[i]->push_back(WaveAna.Get_PmaxTmax_Of_Multiple_Signal(5*this_rms, *this->w[i], *this->t, *this->pmax[i], *this->tmax[i], *this->max_indexing[i], 1.0 ));
      //if(*found_peaks[i] > 0) std::cout<<*found_peaks[i]<<" pmax1 "<<this->pmax[i]->at(0)<<" tmax1 "<<this->tmax[i]->at(0)<<std::endl;
      //*found_peaks[i] = (WaveAna.Get_PmaxTmax_Of_Multiple_Signal_Argonne_Fixed(5*this_rms, *this->w[i], *this->t, *this->pmax[i], *this->tmax[i],
      //                                                                        *this->max_indexing[i], 1.0, 0.029, 0.003, 8));
      //*first_threshold[i] = 0.;
      //*first_threshold[i] = (WaveAna.Get_First_Threshold(assistThreshold, *this->w[i], *this->t));

      this->separated[i]->push_back(1);
      for(std::size_t vSize=0, maxSize=this->pmax[i]->size(); vSize<maxSize; vSize++)
      {
        std::pair<double, unsigned int> pmax_tmax = std::make_pair( this->pmax[i]->at(vSize), this->max_indexing[i]->at(vSize) );
        pulseArea[i]->push_back( WaveAna.Find_Pulse_Area(*this->w[i], *this->t, pmax_tmax) );

        //Check if the event has well separated peaks
        for(std::size_t vSize2=0, maxSize2=this->pmax[i]->size(); vSize2<maxSize2; vSize2++){
          if( vSize != vSize2 && this->found_peaks[i]->at(0) > 1 && fabs(this->tmax[i]->at(vSize) - this->tmax[i]->at(vSize2)) < 0.005) {
            this->separated[i]->at(0) = 0;
            //std::cout<<"separation "<<this->found_peaks[i]->at(0)<<" "<<fabs(this->tmax[i]->at(vSize) - this->tmax[i]->at(vSize2))<<std::endl;
          }
        }
        //hitpos->Fill(i);
      }
      bool baseline_corrected = WaveAna.Correct_Baseline4( *this->w[i], *this->t, *this->pmax[i], *this->tmax[i] );
      hitpos->SetBinContent(i+1, hitpos->GetBinContent(i+1) + this->pmax[i]->size());
    }

    //t->push_back( this->beta_scope.iTreeDoubleArrayMap["t"]->At(i) * horiScaler );

    //TH1D tmp(Form("tmp%i",count), "", 100, 1, 1);
    //tmp.Fill(count);
    //*this->beta_scope.oTree_TH1D_Map["counter_histo"] = tmp;

    //TH1D tmp2(Form("tmp2%i",count), "", 100, 1, 1);
    //tmp2.Fill(count);
    //*static_cast<TH1_Container<TH1D>*>(this->beta_scope.oTree_TH1_Map["counter"])->get() = tmp2;
    //*this->beta_scope.get_oHisto1D<TH1D>("counter") = tmp2;

    //this->standAloneHisto.Fill(count*2.0);
    //this->standAloneHisto_ptr->Fill(count*2.0);

    *this->beta_scope.oTreeIntMap["counter"] = count;
    count++;

    BetaScope_AnaFramework::filldata();

    if(count > 1000) break;
    //this->beta_scope.oTree_TH1D_Map["counter_histo"]->Reset();

    // some extra cleaning
    for( int i =0; i < 16; i++){
      this->found_peaks[i]->clear();
      this->separated[i]->clear();
    }
  }

  for( int i =0; i < 16; i++) hitpos->SetBinContent(i+1, hitpos->GetBinContent(i+1)/count);
}

//==============================================================================
//==============================================================================
//==============================================================================

void ArgoneXrayAna::finalize()
{
  //do your own stuffs here
  /*
  TFile f("test_out.root", "RECREATE");
  f.cd();
  this->standAloneHisto.Write( );
  this->standAloneHisto_ptr->Write( );
  hitpos->Write();
  f.Close();
  */
  BetaScope_AnaFramework::finalize();
}
