///////////////////////////////
//                           //
// Argument class header     //
//                           //
///////////////////////////////

#ifndef BETACOPE_EXT_H
#define BETACOPE_EXT_H

// struct TH1_BaseContainer;
// template <typename th1_type> struct TH1_Container;

#include "BetaScope_Class.h"
#include "BetaScope_Templates.h"

#include <TH1.h>
#include <TH2.h>

/*==============================================================================
TH Base container for TH1 and TH2 histograms from ROOT.
==============================================================================*/
struct TH_BaseContainer {
  TH_BaseContainer(){};
  virtual ~TH_BaseContainer();
};

template <typename TH_Type> struct TH_Container : public TH_BaseContainer {
private:
  TH_Type *th = new TH_Type;

public:
  TH_Container(){};
  ~TH_Container(){ if (this->th){ delete th; } };
  TH_Type *get(){ return this->th; };
};

/*==============================================================================

==============================================================================*/

class BetaScope_Ext : public BetaScope {
private:
  std::string class_name = "BetaScope_Ext";
  int new_th_branch_counter_ = 0;

  int input_branch_counter_ = 0;
  int new_branch_counter_keeper_ = 0;

  // TH1 for input
  TTreeReaderArray<TH1I> *iTree_TH1I_Array[kNumChannels] = {};
  TTreeReaderArray<TH1F> *iTree_TH1F_Array[kNumChannels] = {};
  TTreeReaderArray<TH1D> *iTree_TH1D_Array[kNumChannels] = {};

  std::map<std::string, TTreeReaderArray<TH1I> *> iTree_TH1I_Array_Map;
  std::map<std::string, TTreeReaderArray<TH1F> *> iTree_TH1F_Array_Map;
  std::map<std::string, TTreeReaderArray<TH1D> *> iTree_TH1D_Array_Map;

  TTreeReaderValue<TH1I> *iTree_TH1I[kNumChannels] = {};
  TTreeReaderValue<TH1F> *iTree_TH1F[kNumChannels] = {};
  TTreeReaderValue<TH1D> *iTree_TH1D[kNumChannels] = {};

  std::map<std::string, TTreeReaderValue<TH1I> *> iTree_TH1I_Map;
  std::map<std::string, TTreeReaderValue<TH1F> *> iTree_TH1F_Map;
  std::map<std::string, TTreeReaderValue<TH1D> *> iTree_TH1D_Map;

  // TH1 for output
  std::vector<TH1I> *oTree_TH1I_Array[kNumChannels] = {};
  std::vector<TH1F> *oTree_TH1F_Array[kNumChannels] = {};
  std::vector<TH1D> *oTree_TH1D_Array[kNumChannels] = {};

  std::map<std::string, std::vector<TH1I> *> oTree_TH1I_Array_Map;
  std::map<std::string, std::vector<TH1F> *> oTree_TH1F_Array_Map;
  std::map<std::string, std::vector<TH1D> *> oTree_TH1D_Array_Map;

  TH1I *oTree_TH1I[kNumChannels] = {};
  TH1F *oTree_TH1F[kNumChannels] = {};
  TH1D *oTree_TH1D[kNumChannels] = {};

  TH_BaseContainer *oTree_TH[kNumChannels] = {};
  std::map<std::string, TH_BaseContainer *> oTree_TH_Map;

  std::map<std::string, TH1I *> oTree_TH1I_Map;
  std::map<std::string, TH1F *> oTree_TH1F_Map;
  std::map<std::string, TH1D *> oTree_TH1D_Map;

public:
  BetaScope_Ext() { };
  ~BetaScope_Ext(){
    for(auto &ptr : this->oTree_TH )
    {
      if(ptr){ delete ptr; }
    }
  };

  void FillEvent();

  template <typename dtype> bool BuildBranch(std::string branchName);

  template <typename TH1_Type> bool BuildTH1Branch(std::string branchName);

  bool SetBranch(std::string typeName, std::string key, std::string branchName);

  template <typename TH1_Type>
  typename DataType<TH1_Type>::type *GetOutTH1(std::string branchName);

  template <typename th1_type> int FillOutTH1(double value);

  // void test();
  // oTree_TH1[0] = new TH1_Container<TH1D>;
};

#endif // BETACOPE_EXT_H
